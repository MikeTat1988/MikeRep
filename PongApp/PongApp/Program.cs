using System;
using System.IO.Pipes;
using System.Text;

class PongProgram
{
    static void Main()
    {
        Console.WriteLine("Pong application started. Waiting for Ping...");

        while (true)
        {
            using (var pipeClient = new NamedPipeClientStream(".", "PingPongPipe"))
            {
                pipeClient.Connect();

                using (var reader = new StreamReader(pipeClient, Encoding.UTF8))
                {
                    string message = reader.ReadLine();
                    Console.WriteLine($"Received: {message}");
                }
            }
        }
    }
}
