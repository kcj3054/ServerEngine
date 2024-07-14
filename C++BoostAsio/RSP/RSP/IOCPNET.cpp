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
	//socket �ּ� ���� 
	SOCKADDR_IN	serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);

	if (bind(listenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) != 0)
	{
		//Error, AssertCrash ���� 
	}
}

void IOCPNET::StartServer(int clientSessionCount)
{
	//SetSocketOpriont, nagle, ��� socket option ���� 


	//client session pool
	if (CreateClientSession(clientSessionCount) == false)
	{
		//Error
	}

	//������ 0�̸� core �� ��ŭ �ڵ����� ���� �� 
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


//WaitingThread Queue�� ����� ��������� ���� 
bool IOCPNET::CreateWorkerThread(const int count)
{
	for (int i = 0; i < count; ++i)
	{
		// this �� ? Ȯ�� 
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
	
	DWORD dw = 0; //overlapped io �۾����� ���۵� ������ ũ�� 

	LPOVERLAPPED lpOverlapped = nullptr; // io �۾��� ���� ��û�� overlapped ����ü�� ���� ������ 
 	
	while (isWorker)
	{
		// GetQueuedCompletionStatus, GetQueuedCompletionStatusEx(�ѹ��� ���� io�� ó�� ����)
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

//wsasend overlapped io �۾�.
bool Send()
{

}

void IOCPNET::AccepterThread()
{

}
