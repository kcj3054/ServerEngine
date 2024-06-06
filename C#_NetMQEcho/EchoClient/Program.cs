using EchoClient;

var processRun = true;
var identity = Console.ReadLine();
var dealer = new DealerNetwork(identity, 15999);
dealer.Start();
while (processRun)
{
    if (!dealer.alive)
    {
        processRun = false;
    }
}
Console.WriteLine("Terminated");