using NetMQ;
using NetMQ.Sockets;

namespace EchoServer;

public class RouterNetwork
{
    private NetMQPoller _poller = new();
    private RouterSocket _routerSocket = new();
    private NetMQQueue<NetMQMessage> _routerQueue = new();
    private int _port;

    public RouterNetwork(int port)
    {
        _port = port;
        _routerSocket.Options.RouterHandover = true;
        _poller.RunAsync();
    }

    public void Start()
    {
        _routerSocket.Bind($"tcp://*:{_port}");
        _poller.Add(_routerSocket);
        _poller.Add(_routerQueue);
        _routerSocket.ReceiveReady += Receive;
        _routerQueue.ReceiveReady += Send;
    }
    private void Receive(object? sender, NetMQSocketEventArgs args)
    {
        var msg = args.Socket.ReceiveMultipartMessage();
        Relay(msg);
    }
    public void SendMessage(NetMQMessage msg)
    {
        _routerQueue.Enqueue(msg);
    }
    private void Send(object? sender, NetMQQueueEventArgs<NetMQMessage> args)
    {
        while (_routerQueue.TryDequeue(out var message, TimeSpan.Zero))
        {
            _routerSocket.TrySendMultipartMessage(TimeSpan.FromMilliseconds(100), message);
        }
    }
    public void Relay(NetMQMessage msg)
    {
        Console.WriteLine($"Meesage from identity : {msg[0].ConvertToString()}, msg : {msg[2].ConvertToString()}");
        SendMessage(msg);
    }

}