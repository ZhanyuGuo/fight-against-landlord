using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;
using System.Net.Sockets;
using System.Runtime.InteropServices;
using System.Windows.Controls;

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
        private UdpClient PrivateSocket;
        private Socket BroadcastReceiver;
        private int BroadcastReceiveErrorCount;

        public IPAddress BroadcastIPAddress { get; set; }
        public int BroadcastPort { get; set; }
        public IPAddress ServiceIPAddress { get; set; }
        public int ServicePort { get; set; }
        public int UserNameHashCode { get; set; }

        public event SceneEventHandler SceneReceived;
        public event Action NoHearingFromServer;
        public event Action<string> DebugEvent;
        public ClientCore()
        {
            this.PrivateSocket = new UdpClient(AddressFamily.InterNetwork);
            this.BroadcastReceiver = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.IP);
            this.BroadcastIPAddress = IPAddress.Parse("239.0.1.10");
            this.BroadcastPort = 6666;
            this.ServicePort = 6665;
            this.UserNameHashCode = 1478206855;

            this.BroadcastReceiveErrorCount = 0;
            // this.UserNameHashCode = "NULL".GetHashCode();
        }

        public void InitAndRun()
        {
            
            this.BroadcastReceiver.ReceiveTimeout = 2000;
            this.BroadcastReceiver.Bind(new IPEndPoint(IPAddress.Any, 6666));
            MulticastOption option = new MulticastOption(this.BroadcastIPAddress, 0xa);
            this.BroadcastReceiver.SetSocketOption(SocketOptionLevel.IP, SocketOptionName.AddMembership, option);
            
            this.KeepReceiveScene();

        }

        private async void KeepReceiveScene()
        {
            while (true)
            {
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
                    this.DebugEvent?.Invoke(err.Message);
                }
                else
                {
                    this.DebugEvent?.Invoke(receivedLength.ToString());
                }
                this.OnRawBroadcastDatagramReceived(buffer, receivedLength);
                await Task.Delay(100);
            }
        }
      
        
        private unsafe void OnRawBroadcastDatagramReceived(byte[] buffer, int bufferLen)
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
                    this.OnNoHearingFromServer();
                }
            }
        }

        protected void OnNoHearingFromServer()
        {
            this.NoHearingFromServer?.Invoke();
        }

        protected void OnSceneReceived(Scene se)
        {
            this.SceneReceived?.Invoke(se);
        }


    }
}
