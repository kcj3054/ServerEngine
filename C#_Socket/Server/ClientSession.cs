using ServerCore;
using System;
using System.Net;
using System.Threading;
using System.Threading.Tasks;

namespace Server
{
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

            ushort size = BitConverter.ToUInt16(buffer.Array, buffer.Offset);
            pos += 2;
            ushort id = BitConverter.ToUInt16(buffer.Array, buffer.Offset + pos);
            pos += 2;

            // TODO
            switch ((PacketID)id)
            {
                case PacketID.PlayerInfoReq:
                    {
                        long playerId = BitConverter.ToInt64(buffer.Array, buffer.Offset + pos);
                        pos += 8;
                    }
                    break;
                case PacketID.PlayerInfoOk:
                    {
                        int hp = BitConverter.ToInt32(buffer.Array, buffer.Offset + pos);
                        pos += 4;
                        int attack = BitConverter.ToInt32(buffer.Array, buffer.Offset + pos);
                        pos += 4;
                    }
                    //Handle_PlayerInfoOk();
                    break;
                default:
                    break;
            }

            Console.WriteLine($"RecvPacketId: {id}, Size {size}");
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
}
