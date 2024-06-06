using Server;
using ServerCore;
using System;
using System.Net;
using System.Threading;
using System.Threading.Tasks;

string host = Dns.GetHostName();
IPHostEntry ipHost = Dns.GetHostEntry(host);
IPAddress ipAddr = ipHost.AddressList[0];
IPEndPoint endPoint = new (ipAddr, 7777);

Listener _listener = new();
_listener.Init(endPoint, () => { return new ClientSession(); });
Console.WriteLine("Listening...");

//await Task.Delay(Timeout.Infinite);  // 비동기로 막는 방식 더 cpu가 덜 소모된다. 
// Console.ReadLine();  // 동기로 막는 방식.. 