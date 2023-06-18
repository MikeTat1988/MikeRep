using System;
using System.IO.Pipes;
using System.Text;

class PingProgram
{
    static void Main()
    {
        Console.WriteLine("Ping application started. Press any key to send a Ping. Press 'Q' to quit.");

        while (true)
        {
            using (var pipeServer = new NamedPipeServerStream("PingPongPipe"))
            {
                pipeServer.WaitForConnection();
                ConsoleKeyInfo keyInfo = Console.ReadKey();

                if (keyInfo.Key == ConsoleKey.Q)
                    break;

                using (var writer = new StreamWriter(pipeServer, Encoding.UTF8, 4096, leaveOpen: true))
                {
                    writer.WriteLine("Ping");
                }

                Console.WriteLine("Pong");
            }
        }
    }
}
