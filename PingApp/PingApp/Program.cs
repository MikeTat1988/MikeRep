using System;
using System.IO;
using System.IO.Pipes;
using System.Text;
using System.Threading;

class PingProgram
{
    static void Main()
    {
        using (var pipeServer = new NamedPipeServerStream("PingPongPipe"))
        using (var reader = new StreamReader(pipeServer, Encoding.UTF8))
        using (var writer = new StreamWriter(pipeServer, Encoding.UTF8))
        {
            pipeServer.WaitForConnection();
            Console.WriteLine("Connected to Pong.");

            while (true)
            {
                writer.WriteLine("Ping!");
                writer.Flush();

                Thread.Sleep(500); // Wait for 0.5 second 

                string response = reader.ReadLine();
                Console.WriteLine($"Received: {response}");
            }

            Console.WriteLine("Closing connection...");
        }
    }
}
