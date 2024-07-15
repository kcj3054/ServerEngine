#pragma once

#include <boost/bind/bind.hpp>

#include "Session.h"
#include "Packet.h"

class GameServer
{
public:

	//re_use addr = true
	/*
	* �Ϲ������� ������ ���� ��, �ü���� �ش� ��Ʈ ��ȣ�� ���� �ð� ���� �������� ���ϰ� �Ѵ�.
	�̸� TIME_WAIT ���¶�� �ϴµ�.
	SO_REUSEADDR �ɼ��� ����ϸ�,
	TIME_WAIT ���¿� �ִ� ��Ʈ ��ȣ�� ��� ������ �� �ִ�. 
	*/
	GameServer(boost::asio::io_context& io_context)
		: acceptor(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 
			1111), true) 
	{
		isAccepting = false;
	};

	~GameServer() = default;

	void Init()
	{
		//SessionPool �̸� ���� 
		for (int i = 0; i < 100; ++i)
		{
			//new session �ٸ� ������� ����, �׸��� ���ڳѱ�� ���� ã�ƺ��� 
			/*auto session = new Session(i, (boost::asio::io_context&)acceptor.get_executor().context()
				);*/

			//unique_ptr�� ���� 
			auto session = std::make_unique<Session>(i, (boost::asio::io_context&)acceptor.get_executor().context());
			sessionList.push_back(std::move(session));

			sessionQueue.push_back(i);
		}
	}

	void Start()
	{
		//server start
		
		PostAccept();
	}

	void ProcessPacket(const int sessionID, const char* data)
	{
		// packet header ..
		auto packetHeader = reinterpret_cast<const PacketHeader*>(data);

		//�̸� dictionary ���� ���� packet���� ����س��� �κ����� ���� 
		//�̷��� switch case�� �����Ͱ� �������� �ڵ尡 ���������� (todo ����)
		switch (packetHeader->packetID)
		{
		default:
			break;
		}
		
		//�ٽ� session -> receive �ɾ� �� 

		//�񵿱⿡�� Post�� ���� ���̱� �� -> Async�� �����ϸ� ��� ���
		// sessionList[sessionID]->PostSend();
	}

	void CloseSession(const int sessionID)
	{
		sessionList[sessionID]->GetSocket().close();

		//�̷��� �ص� �����ƴѰ�? sessionList�� �����ϴ°ŵ��� unique_ptr�� �����Ǵ�..
		sessionQueue.push_back(sessionID); //session pool�� �ٽ� �ݳ� 

		//�˻� 
		if (isAccepting == false)
		{
			PostAccept();
		}
	}
private:
	bool PostAccept()
	{
		isAccepting = true;
		
		int sessionID = sessionQueue.front(); // �̸� �����ص� SESSION POOL 
		sessionQueue.pop_front();

		acceptor.async_accept(sessionList[sessionID]->GetSocket(),
			//  �Լ��� �񵿱������� ������ �����Ǿ��� �� ȣ��˴ϴ�. 
			//  &GameServer::handle_accept ȣ���� �Լ� 
			//  �ɹ��Լ� ȣ���� ��ü 
			//  &GameServer::handle_accept�� ������ ���� 1
			// &GameServer::handle_accept�� ������ ���� 2

			boost::bind(&GameServer::handle_accept, this, sessionList[sessionID].get(),
				boost::asio::placeholders::error));

		return true; // ����? 
	}

	//session�� �������� ������ sessionList�� �����Ѵ� 
	void handle_accept(Session* session, const boost::system::error_code& error)
	{
		if (error)
		{
			//Error
			return;
		}

		//message client ���� ���� 
		session->PostRecv();
		PostAccept();
	}
private:

	bool isAccepting;
	/*
	*     acceptor(boost::asio::io_context& io_context, const endpoint& endpoint, bool reuse_addr);
	*/
	boost::asio::ip::tcp::acceptor acceptor; //boost asio�� acceptor 

	// std::vector<Session*> sessionList;
	std::vector<std::unique_ptr<Session>> sessionList;
	std::deque<int> sessionQueue;
};

