using System.Text;
using System.Timers;
using NetMQ;
using NetMQ.Monitoring;
using NetMQ.Sockets;
using Timer = System.Timers.Timer;

namespace EchoClient;

public class DealerNetwork : IDisposable
{
    private NetMQPoller _poller = new();
    private DealerSocket _dealerSocket = new();
    private NetMQQueue<NetMQMessage> _dealerQueue = new();
    private NetMQMonitor _monitor;
    private bool _isConnected = false;
    private string _identity;
    private int _targetPort;
    private Timer timer = new();
    public bool alive = false;


    public DealerNetwork(string identity, int port)
    {
        alive = true;
        _identity = identity;
        _targetPort = port;
        timer.Interval = 500;
        timer.Elapsed += EchoProcess;
        var idbyte = Encoding.UTF8.GetBytes(_identity);
        _dealerSocket.Options.Identity = idbyte;
        _monitor = new(_dealerSocket, $"inproc://req.inproc{identity}", SocketEvents.All);
        _poller.RunAsync();
    }
    

    public void Start()
    {
        _dealerSocket.Options.HeartbeatInterval = TimeSpan.FromMilliseconds(100);
        _dealerSocket.Options.HeartbeatTimeout = TimeSpan.FromMilliseconds(200);
        _dealerSocket.Options.ReconnectInterval = TimeSpan.FromDays(1);
        _monitor.AttachToPoller(_poller);
        _poller.Add(_dealerSocket);
        _poller.Add(_dealerQueue);
        _monitor.Connected += Connect;
        _monitor.Disconnected += Disconnected;
        _dealerSocket.ReceiveReady += Receive;
        _dealerQueue.ReceiveReady += Send;
        _dealerSocket.Connect($"tcp://127.0.0.1:{_targetPort}");
        timer.Enabled = true;
    }
    
    void Connect(object? sender, NetMQMonitorSocketEventArgs args)
    {
        _isConnected = true;
    }

    void Receive(object? sender, NetMQSocketEventArgs args)
    {
        if (_isConnected)
        {
            var msg = args.Socket.ReceiveMultipartMessage();
            var serverMsg = msg[1].ConvertToString();
            Console.WriteLine($"Echo Message From Server : {serverMsg}");
        }
        else
        {
            Console.WriteLine("Invalid receive packet. close socket");
            args.Socket.Close();
        }
    }
    public void SendMessage(NetMQMessage msg)
    {
        if (!_isConnected)
        {
            Console.WriteLine("Please connect router before send message.");
            
        }

        _dealerQueue.Enqueue(msg);
    }

    void Send(object? sender, NetMQQueueEventArgs<NetMQMessage> args)
    {
        while (_dealerQueue.TryDequeue(out var message, TimeSpan.Zero))
        {
            _dealerSocket.TrySendMultipartMessage(TimeSpan.FromMilliseconds(100),message );
        }
    }



    void Disconnected(object? sender, NetMQMonitorSocketEventArgs args)
    {
        _isConnected = false;
        Console.WriteLine("Router disconnected");
        _dealerSocket.Disconnect($"tcp://127.0.0.1:{_targetPort}");
        var selection = Console.ReadLine();
        if (selection == "1")
        {
            _dealerSocket.Connect($"tcp://127.0.0.1:{_targetPort}");
        }
        else
        {
            Dispose();
        }

    }
    
    public void Dispose()
    {
        alive = false;
        NetMQConfig.Cleanup();
        _poller.StopAsync();
        _poller.Dispose();
        _dealerSocket.Close();
        _monitor.DetachFromPoller();
        _dealerSocket.Dispose();
        _dealerQueue.Dispose();
        _monitor.Dispose();

        GC.SuppressFinalize(this);
    }
    
    private void EchoProcess(object? sender, ElapsedEventArgs args)
    {
        if (_isConnected)
        {
            var echoMessage = new NetMQMessage();
            echoMessage.AppendEmptyFrame();
            echoMessage.Append("echo");
            SendMessage(echoMessage);
        }
    }
}