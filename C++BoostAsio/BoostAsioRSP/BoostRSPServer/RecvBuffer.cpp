#include "RecvBuffer.h"

RecvBuffer::RecvBuffer(int bufferSize) : bufferSize(bufferSize)
{
	capacity = bufferSize * BUFFER_COUNT;
}

void RecvBuffer::clean()
{
	int dataSize = CanReadSize();

	//read, write ������ ��ġ ���� clean ���� 
	if (dataSize == 0)
	{
		readPos = writePos = 0;
	}
	//���� ������ ����1�� ũ�� �̸��̶�� ������ ���� (ȿ���� ����.. �˳��ϰ� �̸� ��� ���´�)
	// �̷��� ���� ����� ������ capacity = bufferSize * BUFFER_COUNT;
	else if(FreeSzie() < bufferSize)
	{
		::memcpy(&buffer[0], &buffer[readPos], dataSize);
		readPos = 0;
		writePos = dataSize;
	}
}

bool RecvBuffer::OnRead(int numOfBytes)
{
	// ���� �� �ִ� �����ͺ��� �� ���� ���� �������� �Ѵٸ�
	if (numOfBytes > CanReadSize())
	{
		return false;
	}
	return true;
}

bool RecvBuffer::OnWrite(int numOfBytes)
{
	//��ü �� �� �� �ִ� ���� ���� �� ū���� �������Ѵٸ� 
	if (FreeSzie() < numOfBytes)
	{
		return false;
	}
	return true;
}


