#pragma once
#include <vector>

class RecvBuffer
{
	enum {BUFFER_COUNT = 10};
public:
	RecvBuffer(int bufferSize);
	~RecvBuffer() = default;

	//read write pos ��ġ �ű�� 
	void	clean();
	bool    OnRead(int numOfBytes);
	bool	OnWrite(int numOfBytes);

	char* ReadPos() { return &buffer[readPos]; }
	char* WritePos() { return &buffer[writePos]; }

	//read�� �� �ִ� ���� todo �̸��� CanReadSize�� ���� 
	int	  DataSize() { return writePos - readPos; }

	//���� ���� 
	int	  FreeSzie() { return bufferSize - writePos; }
private:

	int capacity = 0;
	int bufferSize = 0;
	int readPos = 0;
	int writePos = 0;
	std::vector<char> buffer;
};

