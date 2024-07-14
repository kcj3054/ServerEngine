#include "GameServer.h"

int main()
{
	boost::asio::io_context io_context;

	GameServer gameServer(io_context);
	
	gameServer.Init();
	gameServer.Start();

	// 비동기 작업을 담당하는 io_context 시작 
	io_context.run();

	return 0;
}