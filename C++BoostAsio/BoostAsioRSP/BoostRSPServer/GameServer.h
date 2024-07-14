#pragma once

#include <boost/bind/bind.hpp>

#include "Session.h"

class GameServer
{
public:

	//re_use addr = true
	/*
	* 일반적으로 소켓이 닫힐 때, 운영체제는 해당 포트 번호를 일정 시간 동안 재사용하지 못하게 한다.
	이를 TIME_WAIT 상태라고 하는데.
	SO_REUSEADDR 옵션을 사용하면,
	TIME_WAIT 상태에 있는 포트 번호를 즉시 재사용할 수 있다. 
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
		//SessionPool 미리 생성 
		for (int i = 0; i < 100; ++i)
		{
			//new session 다른 방법으로 보기, 그리고 인자넘기는 이유 찾아보기 
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

		//packet id에 따른 invoke

		//미리 dictionary 같은 곳에 packet들을 등록해놓는 부분으로 수정 

		
		//비동기에서 Post를 많이 붙이긴 함 -> Async로 변경하면 어떨까 고민
		// sessionList[sessionID]->PostSend();
	}

	void CloseSession(const int sessionID)
	{
		// COUT << SESSION ID... 

		// sessionList[sessionID]->socket

		sessionQueue.push_back(sessionID); //session pool에 다시 반납 

		//검색 
		if (isAccepting == false)
		{
			PostAccept();
		}
	}
private:
	bool PostAccept()
	{
		isAccepting = true;
		
		int sessionID = sessionQueue.front(); // 미리 생성해둔 SESSION POOL 
		sessionQueue.pop_front();

		acceptor.async_accept(sessionList[sessionID]->GetSocket(),
			//  함수는 비동기적으로 연결이 수락되었을 때 호출됩니다. 
			//  &GameServer::handle_accept 호출할 함수 
			//  맴버함수 호출할 객체 
			//  &GameServer::handle_accept에 전달할 인자 1
			// &GameServer::handle_accept에 전달할 인자 2

			boost::bind(&GameServer::handle_accept, this, sessionList[sessionID],
				boost::asio::placeholders::error));

		return true; // 굳이? 
	}

	void handle_accept(Session* session, const boost::system::error_code& error)
	{
		if (error)
		{
			//Error
			return;
		}

		//message client 접속 성공 
		session->PostRecv();
		PostAccept();
	}
private:

	bool isAccepting;
	/*
	*     acceptor(boost::asio::io_context& io_context, const endpoint& endpoint, bool reuse_addr);
	*/
	boost::asio::ip::tcp::acceptor acceptor; //boost asio의 acceptor 

	std::vector<Session*> sessionList;
	std::deque<int> sessionQueue;
};

