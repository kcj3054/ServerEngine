#include "IOCPEnum.h"

#include <vector>
#include <thread>
#include <atomic>


#include "ClientSession.h"

class IOCPNET
{
public:
	IOCPNET() = default;
	~IOCPNET() { WSACleanup(); }

	void Init();
	
	void Listen();
	
	void Bind(const int port);

	void StartServer(const int clientSessionCount);

	void StopServer(ClientSession& clientSession);

private:
	//worker Thread ����
	
	bool CreateWorkerThread(const int count);
	
	bool CreateAcceptWorkerThread(const int count);

	bool CreateClientSession(const int count);

	//worker Thread �۵�
	void WorkerThread();

	void AccepterThread();

private:

	HANDLE iocpHandle;

	SOCKET listenSocket = INVALID_SOCKET;
	
	std::vector<ClientSession*> clientSessions;

	//SessionManager���� session ī��Ʈ�� ��Ƹ���������

	std::vector<std::thread> ioWorkerThread; //ioThread
	
	std::thread accepterThread;

	//flag
	std::atomic<bool> isWorker = false;
	std::atomic<bool> isAcceptor = false;

	//socket buffer
	char socketBuf[1024] = { 0, };
};