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

	// 1 - send ������� ���� 
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
	* //socket.async_read_some , socket.async_receive ���� ���� 
	* 
	* async_read_some stream�ʿ��� ���� ��� 
	* async_receive UDP �ʿ��� ���� ��� 
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
	//error �� client�� ������ ������� ��� return ;
	// eof end of file 
	if (error == boost::asio::error::eof)
	{
		//Server->CloseSession ȣ�� !! 
		
		//client�� ������ ����ƽ��ϴ� 
		return;
	}

	//���ŵ� ������ ó�� 
	// memcpy(&packetBuffer[dataEndPos], reeiveBuff.data(), byteTransferred);
	recvBuffer.OnWrite(byteTransferred); // recvBuffer�� ����ϸ� �� ���� ��ü ���� 

	int packetData = dataEndPos + static_cast<int>(byteTransferred);
	int readData = 0;

	//data�� �޾Ҵ� 
	while (packetData > 0)
	{
		//packet ���� �Ұ� 
		if (packetData < sizeof(PacketHeader))
		{
			return;
		}

		// auto packetHeader = (PacketHeader*)&packetBuffer[readData];
		auto packetHeader = reinterpret_cast<PacketHeader*>(& packetBuffer[readData]);

		//�ϳ��� ��Ŷ���� ���� �Ұ� 
		if (packetHeader->packetSize > packetData)
		{
			return;
		}

		if (packetHeader->packetSize <= packetData)
		{ 
			//���� �� 
			pServer->ProcessPacket(sessionID, &packetBuffer[readData]);

			packetData -= packetHeader->packetSize; 
			readData += packetHeader->packetSize;
		}
	}

	//RecvBuffer�� �� ��ü ���� �� 
	//Data�� ���� ��� (DATA�� �������� �ϳ��� ��Ŷ�� �����ϱ⿡ ������ ���
	if (packetData > 0)
	{
		// todo data ���� 
	}

	PostRecv();
}
