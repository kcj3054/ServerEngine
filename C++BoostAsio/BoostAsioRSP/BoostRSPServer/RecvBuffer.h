#pragma once
#include <vector>

class RecvBuffer
{
public:
	RecvBuffer(int bufferSize);
	~RecvBuffer();

	//read write pos ��ġ �ű�� 
	void	clean();
	void	OnRead();
	void	OnWrite();

	char* ReadPos() { return &buffer[readPos]; }
	char* WritePos() { return &buffer[writePos]; }

	//read�� �� �ִ� ���� todo �̸��� CanReadSize�� ���� 
	int	  DataSize() { return writePos - readPos; }

	//���� ���� 
	int	  FreeSzie() { return bufferSize - writePos; }
private:

	int bufferSize = 0;
	int readPos = 0;
	int writePos = 0;
	std::vector<char> buffer;
};

