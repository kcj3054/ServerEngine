## C# 및 C++ SOCKET에서 학습한 socket 엔진

[Boost::Asio](./C++BoostAsio/BoostAsioRSP/)

- boost::asio를 이용하여 가위바위보 게임을 제작함
- socket option은 reuse true
- acceptor, io_context (boost::asio에서 지원되는 것을 사용 함)
- GameServer가 Init 할 때 미리 sessionPool을 특정 크기 만큼 할당 받는 방식을 사용함
- 해당 방식대로 하지 않고 client session이 연결될 때마다 session을 add하는 방식으로도 전환할 수 있음 
- 패킷을 처리하는 PacketProcess
    - 패킷 처리 부분을 담당 스레드를 돌릴 예정
    - ProcessPacket 부분에서 간단하게 패킷들을 처리할 예정이면 switch - case 문으로도 가능
        - 패킷 수가 많아지면 switch - case로는 코드가 어지러워질 수 있어 BaseHandle를 이용한 방식을 추가 함 packetID에 따른 Packet을 처리 함 
        - accept, send, receive 모두 async_xx를 사용하여, boost::bind로 호출할 함수들을 연동 함 
- 스레드 처리 방식 (예정)
    - 서버가 고도화된다면 채널링 방식을 사용할 수 있음
    - 하나의 서버에 여러 채널들을 생성하여 각 채널별 스레드를 따로 두거나, 채널에서 처리할 PacketProcess 들을 따로 두게된다면 관리의 효율이 높아짐 
    - Or 방 관리형 게임이라면 예시로 1 ~ 100번방, 101 ~ 200, 201 ~ 300으로 담당하는 스레드들을 따로두게된다면 더욱 효율적으로 스레드들을 처리할 수 있음   
[NetMQ](./C#_NetMQEcho/)


[C#_BASIC_SOCKET](./C#_Socket/)


[SuperSocket]() //예정 