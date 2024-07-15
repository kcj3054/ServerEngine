#pragma once

#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>

#include <queue>

// 수정하는 방향으로 가자 PacketManger를 따로 두고 
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
	/*Private 맴버 함수*/
	
	void HandleWrite(const boost::system::error_code& error, int byteTransferred);

	void HandleReceive(const boost::system::error_code& error, int byteTransferred);
private:
	/*Private 맴버 변수 */
	
	int sessionID = 0; //session ID

	//RECVBUFF 
	int packetBufferMakr = 0;  // packetBufferMakr는 현재 버퍼의 위치를 추적함 
	std::array<char, 65536> packetBuffer{}; // 수신된 packet 데이터를 저장하고 처리
	std::array<char, 65536> reeiveBuff{};

	// char packetBuffer[65536]{0, }; 
	//char receiveBuff[65536]{0, }; // packet buffer와 receive buffer 차이 

	//char를 안사용하고 굳이 std::arry 사용하는 이유??

	boost::asio::ip::tcp::socket socket; //현재 session의 socket
	std::queue<char*> sendQueue;//SendQueue 

};

