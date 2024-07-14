#include <WinSock2.h>

enum class IOOPerations
{
	CONNECT,
	RECV,
	SEND
};

struct OverlappedEx
{
	WSAOVERLAPPED		wsaOverlapped;
	SOCKET				socket;
	WSABUF				wsaBuf;		//overlapped io buf
	char				buf[65536];	//databuf
	IOOPerations		operation;
};

