#pragma once
#include <vector>

class RecvBuffer
{
public:
	RecvBuffer(int bufferSize);
	~RecvBuffer();

	//read write pos 위치 옮기기 
	void	clean();
	void	OnRead();
	void	OnWrite();

	char* ReadPos() { return &buffer[readPos]; }
	char* WritePos() { return &buffer[writePos]; }

	//read할 수 있는 공간 todo 이름을 CanReadSize로 변경 
	int	  DataSize() { return writePos - readPos; }

	//남은 공간 
	int	  FreeSzie() { return bufferSize - writePos; }
private:

	int bufferSize = 0;
	int readPos = 0;
	int writePos = 0;
	std::vector<char> buffer;
};

