using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;
using System.Net.NetworkInformation;
using System.Net.Sockets;
using System.Runtime.InteropServices;
using System.Windows.Controls;
using System.Security.Cryptography;

namespace ClientAPP_WPF
{
    [StructLayout(LayoutKind.Explicit)]
    public unsafe struct Scene
    {
        [FieldOffset(0)]
        public sbyte CurrentStage;
        [FieldOffset(1)]
        public fixed sbyte LandlordWillingness[3];
        [FieldOffset(4)]
        public sbyte LandlordIndex;
        [FieldOffset(5)]
        public sbyte ActiveIndex;
        [FieldOffset(6)]
        public sbyte ActiveParam;
        [FieldOffset(7)]
        public fixed sbyte CardLeftCount[3];
        [FieldOffset(10)]
        public fixed sbyte LandlordCards[3];
        [FieldOffset(13)]
        public sbyte LastActType;
        [FieldOffset(14)]
        public sbyte SecondLastActType;
        [FieldOffset(15)]
        public fixed sbyte LastCardDrop[20];
        [FieldOffset(35)]
        public fixed sbyte SecondLastCardDrop[20];
        [FieldOffset(55)]
        public sbyte WinnerFlag;
        [FieldOffset(56)]
        public sbyte ReadyFlag;
        [FieldOffset(57)]
        public fixed byte ServiceAddress[4];
        
        public IPAddress ParseIPAdress()
        {
            return new IPAddress(new byte[] {
                this.ServiceAddress[0],
                this.ServiceAddress[1],
                this.ServiceAddress[2],
                this.ServiceAddress[3]});
        }
    }

    public delegate void SceneEventHandler(Scene se);

    public class ClientCore
    {
        private UdpClient ProcessHandlerSocket;
        private UdpClient CardHandlerSocket;
        private Socket BroadcastReceiver;

        private int BroadcastReceiveErrorCount;
        private bool StopReceiveBroadcastFlag;
        private bool IsServiceAddressSet;

        public SHA256 SHA256Generator { get; }
        public IPAddress BroadcastIPAddress { get; set; }
        public int BroadcastPort { get; set; }
        public IPAddress ServiceIPAddress { get; set; }
        public int ServicePort { get; set; }
        public byte[] UserNameHashCode { get; set; }
        public int PlayerIndex { get; set; }
        public int LastIndex { get => ((this.PlayerIndex + 2) % 3); }
        public int SecondLastIndex { get => ((this.PlayerIndex + 1) % 3); }
        public bool IsPlayerIndexValid { get => (this.PlayerIndex != -1); }

        public event SceneEventHandler SceneReceived;
        public event Action NoHearingFromServer;
        public event Action ServerNotFound;
        public event Action ServerFound;
        public event Action<string> DebugEvent;
        public event Action JoinGameFailed;
        public event Action SelfTurnDetermineLandlord;
        public event Action SelfTurnCardAction;
        public event Action<List<Poker>> CardInformationGot;

        public ClientCore()
        {
            this.ProcessHandlerSocket = new UdpClient(AddressFamily.InterNetwork);
            this.ProcessHandlerSocket.Client.ReceiveTimeout = 3000;
            this.CardHandlerSocket = new UdpClient(AddressFamily.InterNetwork);
            this.CardHandlerSocket.Client.ReceiveTimeout = 3000;
            this.BroadcastReceiver = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.IP);
            this.BroadcastIPAddress = IPAddress.Parse("239.0.1.10");
            this.BroadcastPort = 6666;
            this.ServicePort = 6665;
            this.SHA256Generator = SHA256.Create();
            this.UserNameHashCode = this.SHA256Generator.ComputeHash(Encoding.UTF8.GetBytes("NULL"));

            this.PlayerIndex = -1;
            this.IsServiceAddressSet = false;
            this.BroadcastReceiveErrorCount = 0;
        }



        public void Init()
        {
            this.BroadcastReceiver.Bind(new IPEndPoint(IPAddress.Any, 6666));
        }

        public async void ScanForServer()
        {
            //MulticastOption option = new MulticastOption(this.BroadcastIPAddress, 0xa);
            //this.BroadcastReceiver.SetSocketOption(SocketOptionLevel.IP, SocketOptionName.AddMembership, option);
            this.OnDebugEvent("尝试收听服务器");
            await this.InnerScanForServer();
        }

        public async void KeepReceiveScene()
        {
            this.BroadcastReceiver.ReceiveTimeout = 2000;
            this.StopReceiveBroadcastFlag = false;
            while (true)
            {
                if (this.StopReceiveBroadcastFlag)
                {
                    break;
                }
                byte[] buffer = new byte[100];
                EndPoint ep = new IPEndPoint(IPAddress.Any, 6666);
                Exception err = null;
                int receivedLength = await Task.Run(() => {
                    try
                    {
                        //int len = this.BroadcastReceiver.ReceiveFrom(buffer, ref ep);
                        int len = this.BroadcastReceiver.Receive(buffer);
                        return len;
                    }
                    catch (Exception e)
                    {
                        err = e;
                        return -1;
                    }
                });
                if (receivedLength == -1)
                {
                    this.OnDebugEvent(err.Message);
                }
                else
                {
                    this.OnDebugEvent(receivedLength.ToString());
                }
                this.OnRawBroadcastDatagramReceived(buffer, receivedLength);
                await Task.Delay(100);
            }
        }

        public async void TryJoinGame()
        {
            if (!IsServiceAddressSet)
            {
                this.OnJoinGameFailed();
                return;
            }
            byte[] msgBytes = new byte[36];
            Array.Copy(this.UserNameHashCode, 0, msgBytes, 0, 32);
            Array.Copy(BitConverter.GetBytes(1), 0, msgBytes, 32, 4);
            bool succ = false;
            int retIndex = -1;
            await Task.Run(() => {
                try
                {
                    IPEndPoint IpEP = new IPEndPoint(this.ServiceIPAddress, this.ServicePort);
                    this.ProcessHandlerSocket.Send(msgBytes, msgBytes.Length, IpEP);
                    byte[] ret = this.ProcessHandlerSocket.Receive(ref IpEP);
                    string retStr = Encoding.UTF8.GetString(ret);
                    retIndex = int.Parse(retStr);
                    if (retIndex != -1)
                    {
                        succ = true;
                        this.PlayerIndex = retIndex;
                    }
                }
                catch
                {
                    succ = false;
                    this.PlayerIndex = -1;
                }      
            });
            if (!succ)
            {
                this.OnJoinGameFailed();
            }
        }

        public async void PostReady()
        {
            if (!IsServiceAddressSet)
            {
                return;
            }
            byte[] msgBytes = new byte[36];
            Array.Copy(this.UserNameHashCode, 0, msgBytes, 0, 32);
            Array.Copy(BitConverter.GetBytes(2), 0, msgBytes, 32, 4);
            await Task.Run(() => {
                try
                {
                    IPEndPoint IpEP = new IPEndPoint(this.ServiceIPAddress, this.ServicePort);
                    this.ProcessHandlerSocket.Send(msgBytes, msgBytes.Length, IpEP);                    
                }
                catch
                {

                }
            });
        }

        public async void PostPoint(int point)
        {
            if (!IsServiceAddressSet)
            {
                return;
            }
            byte[] msgBytes = new byte[40];
            Array.Copy(this.UserNameHashCode, 0, msgBytes, 0, 32);
            Array.Copy(BitConverter.GetBytes(4), 0, msgBytes, 32, 4);
            Array.Copy(BitConverter.GetBytes(point), 0, msgBytes, 36, 4);

            await Task.Run(() => {
                try
                {
                    IPEndPoint IpEP = new IPEndPoint(this.ServiceIPAddress, this.ServicePort);
                    this.ProcessHandlerSocket.Send(msgBytes, msgBytes.Length, IpEP);
                }
                catch
                {

                }
            });
        }

        public async void InfoGetCards()
        {
            if (!IsServiceAddressSet)
            {
                return;
            }
            byte[] msgBytes = new byte[36];
            Array.Copy(this.UserNameHashCode, 0, msgBytes, 0, 32);
            Array.Copy(BitConverter.GetBytes(3), 0, msgBytes, 32, 4);
            byte[] received = null;
            await Task.Run(() => {
                try
                {
                    IPEndPoint IpEP = new IPEndPoint(this.ServiceIPAddress, this.ServicePort);
                    this.CardHandlerSocket.Send(msgBytes, msgBytes.Length, IpEP);
                    received = this.CardHandlerSocket.Receive(ref IpEP);
                }
                catch
                {
                    received = null; 
                }
            });
            if (received == null)
            {

            }
            else
            {
                var pokers = PokerSerializer.Deserialize(received);
                this.OnCardInformationGot(pokers);
            }
        }


        private async Task InnerScanForServer()
        {
            this.SceneReceived += FirstSceneReceived;
            this.BroadcastReceiver.ReceiveTimeout = 3000;
            IEnumerable<int> ifIndexes = this.GetIfIndexes();
            bool foundServer = false;
            foreach (int ifIndex in ifIndexes)
            {
                MulticastOption option = new MulticastOption(this.BroadcastIPAddress, ifIndex);
                try
                {
                    this.BroadcastReceiver.SetSocketOption(SocketOptionLevel.IP, SocketOptionName.AddMembership, option);
                }
                catch (Exception e)
                {
                    this.OnDebugEvent($"尝试了一个无效的接口索引:{ifIndex}");
                    continue;
                }
                foundServer = await Task.Run(() => {
                    byte[] buffer = new byte[100];
                    try
                    {
                        int len = this.BroadcastReceiver.Receive(buffer);
                        return len > 0;
                    }
                    catch 
                    {
                        return false;
                    }
                });
                if (foundServer)
                {
                    break;
                }
                else
                {
                    this.OnDebugEvent($"尝试的接口索引超时仍未能收听到服务器:{ifIndex}");
                    this.BroadcastReceiver.SetSocketOption(SocketOptionLevel.IP, SocketOptionName.DropMembership, option);
                    await Task.Delay(500);
                }
            }
            if (!foundServer)
            {
                this.OnServerNotFound();
            }
            else
            {
                this.OnServerFound();
            }
            // this.BroadcastReceiver.ReceiveTimeout = -1;
        }
        
        private IEnumerable<int> GetIfIndexes()
        {
            NetworkInterface[] interfaces = NetworkInterface.GetAllNetworkInterfaces();
            foreach (NetworkInterface @if in interfaces)
            {
                IPv4InterfaceProperties prop = @if.GetIPProperties().GetIPv4Properties();
                if (prop != null)
                {
                    yield return prop.Index;
                }
            }
        }

        private void FirstSceneReceived(Scene se)
        {
            this.ServiceIPAddress = se.ParseIPAdress();
            this.IsServiceAddressSet = true;
            this.SceneReceived -= this.FirstSceneReceived;
        }

        protected unsafe void OnRawBroadcastDatagramReceived(byte[] buffer, int bufferLen)
        {
            int size = sizeof(Scene);
            if (bufferLen == size)
            {
                Scene s = new Scene();
                IntPtr ptrS = new IntPtr(&s);
                Marshal.Copy(buffer, 0, ptrS, size);
                this.BroadcastReceiveErrorCount = 0;
                this.OnSceneReceived(s);
            }
            else
            {
                this.BroadcastReceiveErrorCount += 1;
                if (this.BroadcastReceiveErrorCount >= 4)
                {
                    this.StopReceiveBroadcastFlag = true;
                    this.OnNoHearingFromServer();
                }
            }
        }

        protected void OnNoHearingFromServer()
        {
            this.NoHearingFromServer?.Invoke();
        }

        protected void OnServerNotFound()
        {
            this.ServerNotFound?.Invoke();
        }
        
        protected void OnServerFound()
        {
            this.ServerFound?.Invoke();
        }

        protected void OnJoinGameFailed()
        {
            this.JoinGameFailed?.Invoke();
        }

        protected void OnSceneReceived(Scene se)
        {
            this.SceneReceived?.Invoke(se);
        }

        protected void OnCardInformationGot(List<Poker> cards)
        {
            this.CardInformationGot?.Invoke(cards);
        }

        protected void OnDebugEvent(string s)
        {
            this.DebugEvent?.Invoke(s);
        }


    }
}
