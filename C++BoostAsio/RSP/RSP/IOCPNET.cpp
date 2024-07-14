#include "IOCPNET.h"

void IOCPNET::Init()
{
	WSADATA	wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		//Error
	}

	// WSA_FLAG_OVERLAPPED -> 
	listenSocket = WSASocketA(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, NULL, WSA_FLAG_OVERLAPPED);

	if (listenSocket != 0)
	{
		//Error
	}
}

void IOCPNET::Listen()
{
	if (listen(listenSocket, 10) != 0)
	{
		//Error
	}
}

void IOCPNET::Bind(int port)
{
	//socket 주소 정함 
	SOCKADDR_IN	serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);

	if (bind(listenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) != 0)
	{
		//Error, AssertCrash 적용 
	}
}

void IOCPNET::StartServer(int clientSessionCount)
{
	//SetSocketOpriont, nagle, 등등 socket option 설정 


	//client session pool
	if (CreateClientSession(clientSessionCount) == false)
	{
		//Error
	}

	//마지막 0이면 core 수 만큼 자동으로 생성 됌 
	iocpHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);

	if (iocpHandle == nullptr)
	{
		//Error
	}

	if (CreateWorkerThread() == false)
	{
		//Error
	}

	if (CreateAcceptWorkerThread == false)
	{
		//ERROR
	}


}

void IOCPNET::StopServer(ClientSession& clientSession)
{
	closesocket(clientSession.clientSocket);
	clientSession.clientSocket = INVALID_SOCKET;
}


//WaitingThread Queue에 대기할 쓰레드들을 생성 
bool IOCPNET::CreateWorkerThread(const int count)
{
	for (int i = 0; i < count; ++i)
	{
		// this 왜 ? 확인 
		ioWorkerThread.emplace_back([this]() {WorkerThread(); });

	}

	return true;
}

bool IOCPNET::CreateAcceptWorkerThread(const int count)
{
	accepterThread = std::thread([this]() {AccepterThread(); });

	return true;
}

bool IOCPNET::CreateClientSession(const int clientSessionCount)
{
	for (int i = 0; i < clientSessionCount; ++i)
	{
		clientSessions.emplace_back();
	}

	return true;
}

void IOCPNET::WorkerThread()
{
	ClientSession* clinetsession = nullptr;
	
	DWORD dw = 0; //overlapped io 작업에서 전송된 데이터 크기 

	LPOVERLAPPED lpOverlapped = nullptr; // io 작업을 위해 요청한 overlapped 구조체를 받을 포인터 
 	
	while (isWorker)
	{
		// GetQueuedCompletionStatus, GetQueuedCompletionStatusEx(한번에 여러 io를 처리 가능)
		::GetQueuedCompletionStatus(iocpHandle, &dw, (PULONG_PTR)&clinetsession, &lpOverlapped, INFINITE);

		auto overlappedEx = (OverlappedEx*)lpOverlapped;

		switch (overlappedEx->operation)
		{
		case IOOPerations::RECV:
			//LOG
			
			//SEND
			
			//BIND RECV
			break;
		case IOOPerations::SEND:
			//LOG
		default:
			break;
		}
	}
}

//wsasend overlapped io 작업.
bool Send()
{

}

void IOCPNET::AccepterThread()
{

}
