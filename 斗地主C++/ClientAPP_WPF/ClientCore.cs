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

        public IPAddress BroadcastIPAddress { get; set; }
        public int BroadcastPort { get; set; }
        public IPAddress ServiceIPAddress { get; set; }
        public int ServicePort { get; set; }
        public int UserNameHashCode { get; set; }

        public event SceneEventHandler SceneReceived;
        public ClientCore()
        {
            this.PrivateSocket = new UdpClient(AddressFamily.InterNetwork);
            this.BroadcastReceiver = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
            this.BroadcastIPAddress = IPAddress.Parse("239.0.1.10");
            this.BroadcastPort = 6666;
            this.ServicePort = 6665;
            this.UserNameHashCode = 1478206855;
            // this.UserNameHashCode = "NULL".GetHashCode();
        }

        private void KeepReceiveScene()
        {

        }
      
        

        protected void OnSceneReceived(Scene se)
        {
            this.SceneReceived?.Invoke(se);
        }
    }
}
