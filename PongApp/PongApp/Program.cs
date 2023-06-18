using System;
using System.IO;
using System.IO.Pipes;
using System.Text;
using System.Threading;

class PongProgram
{
    static void Main()
    {
        int counter = 0;
        using (var pipeClient = new NamedPipeClientStream(".", "PingPongPipe"))
        using (var reader = new StreamReader(pipeClient, Encoding.UTF8))
        using (var writer = new StreamWriter(pipeClient, Encoding.UTF8))
        {
            Console.WriteLine("Waiting for connection from Ping...");

            pipeClient.Connect();
            Console.WriteLine("Connected to Ping.");

            while (counter < 15)
            {
                string message = reader.ReadLine();
                Console.WriteLine($"Received: {message}");

                Thread.Sleep(500); // Wait for 0.5 second 

                writer.WriteLine("Pong!");
                writer.Flush();

                ++counter;
            }

            Console.WriteLine("Closing connection...");
        }
    }
}
