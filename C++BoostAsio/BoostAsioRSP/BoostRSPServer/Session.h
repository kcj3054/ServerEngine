#pragma once

#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>

#include <queue>

// �����ϴ� �������� ���� PacketManger�� ���� �ΰ� 
class GameServer;

class Session
{
public:
	Session(int sessionID, boost::asio::io_context& io_context);
	~Session();

	void PostSend(const int size, char* data);

	void PostRecv();

	boost::asio::ip::tcp::socket& GetSocket() { return socket; }
private:
	/*Private �ɹ� �Լ�*/
	
	void HandleWrite(const boost::system::error_code& error, int byteTransferred);

	void HandleReceive(const boost::system::error_code& error, int byteTransferred);
private:
	/*Private �ɹ� ���� */
	
	int sessionID = 0; //session ID

	//RECVBUFF 
	int packetBufferMakr = 0;  // packetBufferMakr�� ���� ������ ��ġ�� ������ 
	std::array<char, 65536> packetBuffer{}; // ���ŵ� packet �����͸� �����ϰ� ó��
	std::array<char, 65536> reeiveBuff{};

	// char packetBuffer[65536]{0, }; 
	//char receiveBuff[65536]{0, }; // packet buffer�� receive buffer ���� 

	//char�� �Ȼ���ϰ� ���� std::arry ����ϴ� ����??

	boost::asio::ip::tcp::socket socket; //���� session�� socket
	std::queue<char*> sendQueue;//SendQueue 

};

