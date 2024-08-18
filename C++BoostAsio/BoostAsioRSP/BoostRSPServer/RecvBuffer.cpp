#include "RecvBuffer.h"

RecvBuffer::RecvBuffer(int bufferSize) : bufferSize(bufferSize)
{
	capacity = bufferSize * BUFFER_COUNT;
}

void RecvBuffer::clean()
{
	int dataSize = CanReadSize();

	//read, write 동일한 위치 쉽게 clean 가능 
	if (dataSize == 0)
	{
		readPos = writePos = 0;
	}
	//여유 공간이 버퍼1개 크기 미만이라면 데이터 당기기 (효율이 좋네.. 넉넉하게 미리 당겨 놓는다)
	// 이러한 것을 사용한 이유가 capacity = bufferSize * BUFFER_COUNT;
	else if(FreeSzie() < bufferSize)
	{
		::memcpy(&buffer[0], &buffer[readPos], dataSize);
		readPos = 0;
		writePos = dataSize;
	}
}

bool RecvBuffer::OnRead(int numOfBytes)
{
	// 읽을 수 있는 데이터보다 더 많은 양을 읽을려고 한다면
	if (numOfBytes > CanReadSize())
	{
		return false;
	}
	return true;
}

bool RecvBuffer::OnWrite(int numOfBytes)
{
	//전체 중 쓸 수 있는 공간 보다 더 큰양을 쓰려고한다면 
	if (FreeSzie() < numOfBytes)
	{
		return false;
	}
	return true;
}


