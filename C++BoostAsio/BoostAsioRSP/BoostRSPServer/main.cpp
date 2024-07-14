#include "GameServer.h"

int main()
{
	boost::asio::io_context io_context;

	GameServer gameServer(io_context);
	
	gameServer.Init();
	gameServer.Start();

	// �񵿱� �۾��� ����ϴ� io_context ���� 
	io_context.run();

	return 0;
}