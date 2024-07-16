#include "Session.h"
#include "Packet.h"

#include "GameServer.h"

Session::Session(int sessionID, boost::asio::io_context& io_context, GameServer* pServer)
	: 
	socket(io_context.get_executor()),
	sessionID(sessionID),
	pServer(pServer)
{
}

Session::~Session()
{
	while (sendQueue.empty() == false)
	{
		delete[] sendQueue.front();
		sendQueue.pop();
	}
}

void Session::PostSend(const bool bImmediately, const int size, char* data)
{
	char* sendData = new char[size];
	memcpy(sendData, data, size);
	sendQueue.push(sendData);

	// 1 - send 방식으로 간다 
	if (sendQueue.size() > 1)
	{
		return;
	}

	/*boost::asio::async_write(m_Socket,
		boost::asio::buffer(pSendData, nSize),
		boost::bind(&Session::handle_write, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));*/
}

void Session::PostRecv()
{
	// Recv.. 
	/*
	* //socket.async_read_some , socket.async_receive 존재 차이 
	* 
	* async_read_some stream쪽에서 많이 사용 
	* async_receive UDP 쪽에서 많이 사용 
	*/
	socket.async_read_some(
		boost::asio::buffer(reeiveBuff),
		boost::bind(& Session::HandleReceive, this,
		boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void Session::HandleWrite(const boost::system::error_code& error, int byteTransferred)
{
	delete[] sendQueue.front();
}

void Session::HandleReceive(const boost::system::error_code& error, int byteTransferred)
{
	//error 중 client와 연결이 종료됐을 경우 return ;
	// eof end of file 
	if (error == boost::asio::error::eof)
	{
		//Server->CloseSession 호출 !! 
		
		//client와 연결이 종료됐습니다 
		return;
	}

	//수신된 데이터 처리 
	// memcpy(&packetBuffer[dataEndPos], reeiveBuff.data(), byteTransferred);
	recvBuffer.OnWrite(byteTransferred); // recvBuffer를 사용하면 위 구문 대체 가능 

	int packetData = dataEndPos + static_cast<int>(byteTransferred);
	int readData = 0;

	//data를 받았다 
	while (packetData > 0)
	{
		//packet 조립 불가 
		if (packetData < sizeof(PacketHeader))
		{
			return;
		}

		// auto packetHeader = (PacketHeader*)&packetBuffer[readData];
		auto packetHeader = reinterpret_cast<PacketHeader*>(& packetBuffer[readData]);

		//하나의 패킷으로 조립 불가 
		if (packetHeader->packetSize > packetData)
		{
			return;
		}

		if (packetHeader->packetSize <= packetData)
		{ 
			//로직 단 
			pServer->ProcessPacket(sessionID, &packetBuffer[readData]);

			packetData -= packetHeader->packetSize; 
			readData += packetHeader->packetSize;
		}
	}

	//RecvBuffer로 다 대체 가능 함 
	//Data가 남을 경우 (DATA는 남았으나 하나의 패킷을 조립하기에 부족한 경우
	if (packetData > 0)
	{
		// todo data 복사 
	}

	PostRecv();
}
