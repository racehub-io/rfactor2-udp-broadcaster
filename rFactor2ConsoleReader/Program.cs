using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using RFactor2UdpBroadcaster;
using Google.Protobuf;
using System.Linq;

namespace rFactor2ConsoleReader
{
    class Program
    {
        static void Main(string[] args)
        {
            int port = 20888;

            using (UdpClient udpClient = new UdpClient(port))
            {
                while (true)
                {
                    try
                    {
                        //IPEndPoint object will allow us to read datagrams sent from any source.
                        IPEndPoint RemoteIpEndPoint = new IPEndPoint(IPAddress.Any, port);

                        // Blocks until a message returns on this socket from a remote host.

                        Byte[] receiveBytes = udpClient.Receive(ref RemoteIpEndPoint);

                        DataPacket data = null;
                        try
                        {
                            data = DataPacket.Parser.ParseFrom(receiveBytes);
                        }
                        catch(Exception ex)
                        {
                            Console.WriteLine(ex.ToString());
                        }

                        var header = data.PacketHeader;

                        //Console.WriteLine($"Epoch: {header.EpochTime}, Contract Version: {header.ContractVersion}");

                        Console.Write($"Time: {header.EpochTime} ");

                        if (header.PacketType == PacketType.Telemetry)
                        {
                            Console.WriteLine($"mID: {data.Telemetry.MID}, ElapsedTime: {data.Telemetry.MElapsedTime}");
                        }
                        else if (header.PacketType == PacketType.Scoring)
                        {
                            Console.WriteLine($"LapDistance: {data.Scoring.MLapDist}, Driver: {data.Scoring.MVehicle.First().MDriverName}");
                        }
                        else if (header.PacketType == PacketType.Graphics)
                        {
                            Console.WriteLine($"CamPos: {data.Graphics.MCamPos.X},{data.Graphics.MCamPos.Y},{data.Graphics.MCamPos.Z}");
                        }
                        else if (header.PacketType == PacketType.Physics)
                        {
                            Console.WriteLine($"TractionControl?: {data.Physics.MTractionControl}");
                        }


                    }
                    catch (Exception e)
                    {
                        Console.WriteLine(e.ToString());
                    }
                }

            }
        }
    }
}
