#pragma once
#include <vector>

class RecvBuffer
{
	enum {BUFFER_COUNT = 10};
public:
	RecvBuffer(int bufferSize);
	~RecvBuffer() = default;

	//read write pos 위치 옮기기 
	void	clean();
	bool    OnRead(int numOfBytes);
	bool	OnWrite(int numOfBytes);

	char* ReadPos() { return &buffer[readPos]; }
	char* WritePos() { return &buffer[writePos]; }

	//read할 수 있는 공간 todo 이름을 CanReadSize로 변경 
	int	  DataSize() { return writePos - readPos; }

	//남은 공간 
	int	  FreeSzie() { return bufferSize - writePos; }
private:

	int capacity = 0;
	int bufferSize = 0;
	int readPos = 0;
	int writePos = 0;
	std::vector<char> buffer;
};

