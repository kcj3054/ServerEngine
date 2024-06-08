using ServerCore;
using System;
using System.Net;
using System.Threading;
using System.Threading.Tasks;
using Google.Protobuf;


class Packet
{
    public ushort size;
    public ushort packetId;
}

class PlayerInfoReq : Packet
{
    public long playerId;
}

class PlayerInfoOk : Packet
{
    public int hp;
    public int attack;
}

public enum PacketID
{
    PlayerInfoReq = 1,
    PlayerInfoOk = 2,
}

class ClientSession : PacketSession
{
    public override async void OnConnected(EndPoint endPoint)
    {
        Console.WriteLine($"OnConnected : {endPoint}");
        await Task.Delay(5000);
        Disconnect();
    }

    public override void OnRecvPacket(ArraySegment<byte> buffer)
    {
        int pos = 0;

        ushort id = BitConverter.ToUInt16(buffer.Array, buffer.Offset + pos);
        pos += 2;

        // 메시지 크기 가져오기
        int messageLength = BitConverter.ToInt32(buffer.Array, buffer.Offset + pos);
        pos += 4;

        // 메시지 데이터 가져오기
        byte[] messageData = new byte[messageLength];
        Array.Copy(buffer.Array, buffer.Offset + pos, messageData, 0, messageLength);

        // TODO
        switch ((PacketID)id)
        {
            case PacketID.PlayerInfoReq:
                {
                    try
                    {

                        var playerInfoReq = PlayerInfoReqMessage.Parser.ParseFrom(messageData);
                        long playerId = playerInfoReq.PlayerId;
                        // playerId를 사용한 로직 처리
                        Console.WriteLine($"Received PlayerInfoReq: PlayerId={playerId}");
                    }
                    catch (Exception ex)
                    {
                        Console.WriteLine($"Error parsing PlayerInfoReq: {ex.Message}");
                    }
                }
                break;
            case PacketID.PlayerInfoOk:
                {
                    try
                    {
                        var playerInfoOk = PlayerInfoOkMessage.Parser.ParseFrom(messageData);
                        int hp = playerInfoOk.Hp;
                        int attack = playerInfoOk.Attack;
                        // hp와 attack을 사용한 로직 처리
                        Console.WriteLine($"Received PlayerInfoOk: HP={hp}, Attack={attack}");
                    }
                    catch (Exception ex)
                    {
                        Console.WriteLine($"Error parsing PlayerInfoOk: {ex.Message}");
                    }
                }
                break;
            default:
                break;
        }
    }

    // TEMP
    public void Handle_PlayerInfoOk(ArraySegment<byte> buffer)
    {

    }

    public override void OnDisconnected(EndPoint endPoint)
    {
        Console.WriteLine($"OnDisconnected : {endPoint}");
    }

    public override void OnSend(int numOfBytes)
    {
        Console.WriteLine($"Transferred bytes: {numOfBytes}");
    }
}
