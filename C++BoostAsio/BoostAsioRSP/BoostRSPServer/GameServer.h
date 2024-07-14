#pragma once

#include <boost/bind/bind.hpp>

#include "Session.h"

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
			auto session = new Session(i, (boost::asio::io_context&)acceptor.get_executor().context()
				);
			sessionList.push_back(session);
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

		//packet id�� ���� invoke

		//�̸� dictionary ���� ���� packet���� ����س��� �κ����� ���� 

		
		//�񵿱⿡�� Post�� ���� ���̱� �� -> Async�� �����ϸ� ��� ���
		// sessionList[sessionID]->PostSend();
	}

	void CloseSession(const int sessionID)
	{
		// COUT << SESSION ID... 

		// sessionList[sessionID]->socket

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

			boost::bind(&GameServer::handle_accept, this, sessionList[sessionID],
				boost::asio::placeholders::error));

		return true; // ����? 
	}

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

	std::vector<Session*> sessionList;
	std::deque<int> sessionQueue;
};

