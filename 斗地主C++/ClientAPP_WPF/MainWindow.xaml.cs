using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace ClientAPP_WPF
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();

            this.IsServerOK = false;
            this.IsNameSet = false;

            this.core = new ClientCore();
            this.CardButtons = new List<Button> {
                this.PlayerCard1,
                this.PlayerCard2,
                this.PlayerCard3,
                this.PlayerCard4,
                this.PlayerCard5,                
                this.PlayerCard6,                             
                this.PlayerCard7,
                this.PlayerCard8,
                this.PlayerCard9,
                this.PlayerCard10,
                this.PlayerCard11,
                this.PlayerCard12,
                this.PlayerCard13,
                this.PlayerCard14,
                this.PlayerCard15,
                this.PlayerCard16,
                this.PlayerCard17,
                this.PlayerCard18,                
                this.PlayerCard19,                
                this.PlayerCard20,
            };

            foreach(var cardButton in this.CardButtons)
            {
                // cardButton.Visibility = Visibility.Hidden;
            }

            this.UserNameButton.IsEnabled = false;
#if DEBUG
            core.DebugEvent += this.WriteToDebugTextBox;
            core.ServerNotFound += () => { this.WriteToDebugTextBox("ServerNotFound"); };
            core.ServerFound += () => { this.WriteToDebugTextBox("ServerFound"); };
            core.SceneReceived += (Scene se) => { this.WriteToDebugTextBox(se.ParseIPAdress().ToString()); };
#endif

            core.ServerFound += this.ServerFoundHandler;
            core.ServerNotFound += this.ServerNotFoundHandler;
            core.NoHearingFromServer += this.NoHearingFromServerHandler;
            core.JoinGameFailed += this.JoinGameFailHandler;
            core.SceneReceived += this.RenderCurrentScene;
            

            core.Init();

            this.ServerStateLabel.Content = "正在扫描服务器...";
            this.FindingServerBar.Visibility = Visibility.Visible;
            core.ScanForServer();
        }

        private ClientCore core;
        private Scene CurrentScene;
        private bool IsNameSet;
        private bool IsServerOK;
        private List<Button> CardButtons;
        private byte[] UserNameHashCode { get => this.core.UserNameHashCode; set => this.core.UserNameHashCode = value; }
        private int PlayerIndex { get => this.core.PlayerIndex; set => this.core.PlayerIndex = value; }
        private int LastPlayerIndex { get => this.core.LastIndex; }
        private int SecondLastPlayerIndex { get => this.core.SecondLastIndex; }
  

        private void UserNameButton_Click(object sender, RoutedEventArgs e)
        {
            if (!this.IsNameSet)
            {
                string name = this.UserNameTextBox.Text;
                name = name.Trim();
                this.UserNameTextBox.Text = name;
                if (!string.IsNullOrWhiteSpace(name) && name != "NULL")
                {
                    this.IsNameSet = true;
                    this.UserNameTextBox.IsEnabled = false;
                    this.UserNameButton.Content = "注销";
                    this.UserNameHashCode = this.core.SHA256Generator.ComputeHash(Encoding.UTF8.GetBytes(name));
                    if (name == "Admin")
                    {
                        this.EasterEgg();
                    }
                    this.core.TryJoinGame();
                }
                else
                {
                    _ = MessageBox.Show("名称不能为空或者\"NULL\"", "名称无效", MessageBoxButton.OK);
                }
            }
            else
            {
                // this.UserNameTextBox.Text = string.Empty;
                this.IsNameSet = false;
                this.UserNameTextBox.IsEnabled = true;
                this.UserNameButton.Content = "登录";
                this.UserNameHashCode = this.core.SHA256Generator.ComputeHash(Encoding.UTF8.GetBytes("NULL"));
            }
        }


        private const sbyte GAME_STAGE_PREPARING = 0;
        private const sbyte GAME_STAGE_DETERMINE_LANDLORD = 1;
        private const sbyte GAME_STAGE_MAINLOOP_ONGOING = 2;
        private const sbyte GAME_STAGE_END = 3;
        private const sbyte ACTIVE_TYPE_ACTIVE = 0;
        private const sbyte ACTIVE_TYPE_FOLLOW = 1;
        private const sbyte ACTIVE_TYPE_PASS = 2;
        private const sbyte ACTIVE_PARAM_1 = 1;
        private const sbyte ACTIVE_PARAM_2 = 2;
        private const sbyte ACTIVE_PARAM_3 = 3;

        private void RenderCurrentScene(Scene se)
        {
            if (!this.core.IsPlayerIndexValid)
            {
                return;
            }
            this.CurrentScene = se;
            switch (se.CurrentStage)
            {
                case GAME_STAGE_PREPARING:
                    {
                        this.PrepareButton.Visibility = Visibility.Visible;
                        this.Player1ReadyLabel.Visibility = Visibility.Visible;
                        this.Player2ReadyLabel.Visibility = Visibility.Visible;
                        if (this.IsPlayerPrepared(this.PlayerIndex))
                        {
                            this.PrepareButton.IsEnabled = false;
                            this.PrepareButton.Content = "已准备";
                        }
                        else
                        {
                            this.PrepareButton.IsEnabled = true;
                            this.PrepareButton.Content = "准备";
                        }
                        if (this.IsPlayerPrepared(this.LastPlayerIndex))
                        {
                            this.Player1ReadyLabel.Content = "已准备";
                        }
                        else
                        {
                            this.Player1ReadyLabel.Content = "未准备";
                        }
                        if (this.IsPlayerPrepared(this.SecondLastPlayerIndex))
                        {
                            this.Player2ReadyLabel.Content = "已准备";
                        }
                        else
                        {
                            this.Player2ReadyLabel.Content = "未准备";
                        }
                        break;
                    }
                case GAME_STAGE_DETERMINE_LANDLORD:
                    {
                        
                        break;
                    }
                case GAME_STAGE_MAINLOOP_ONGOING:
                    {
                        
                        break;
                    }
                case GAME_STAGE_END:
                    {

                        break;
                    }
                default:

                    break;
            }

        }

        private void NoHearingFromServerHandler()
        {
            this.IsServerOK = false;
            this.ServerStateLabel.Content = "与服务器的连接断开, \n正在尝试重连";
            this.FindingServerBar.Visibility = Visibility.Visible;
            this.UserNameButton.IsEnabled = false;
            core.ScanForServer();
        }

        private void ServerFoundHandler()
        {
            this.IsServerOK = true;
            this.FindingServerBar.Visibility = Visibility.Hidden;
            this.ServerStateLabel.Content = "已经连接到服务器";
            this.UserNameButton.IsEnabled = true;
            this.core.KeepReceiveScene();
            if (this.IsNameSet)
            {
                this.core.TryJoinGame();
            }
        }

        private void ServerNotFoundHandler()
        {
            this.IsServerOK = false;
            this.FindingServerBar.Visibility = Visibility.Hidden;
            this.ServerStateLabel.Content = "未能找到服务器";
            this.UserNameButton.IsEnabled = false;
        }

        private void JoinGameFailHandler()
        {
            MessageBox.Show("加入游戏失败，可能是服务器未响应、或者游戏已经满员。", "加入失败");
        }

        private void WriteToDebugTextBox(string str)
        {
            this.DebugTextBox.Text = str;
        }

        private bool IsPlayerPrepared(int index)
        {
            sbyte readyFlag = this.CurrentScene.ReadyFlag;
            readyFlag &= (sbyte)(1 << index);
            return readyFlag != 0;
        }


        private void JoinServerButton_Click(object sender, RoutedEventArgs e)
        {
            //System.Diagnostics.Process.Start("explorer.exe", @"https://gitee.com/iLanceHe");
        }

        private void PrepareButton_Click(object sender, RoutedEventArgs e)
        {
            if (this.core.IsPlayerIndexValid)
            {
                this.PrepareButton.IsEnabled = false;
                this.PrepareButton.Content = "准备中";
                this.core.PostReady();
            }

            //this.core;
        }

        private void EasterEgg()
        {
            MessageBoxResult choice = MessageBox.Show(this.EasterEggText, "彩蛋", MessageBoxButton.OKCancel);
            if (choice == MessageBoxResult.OK)
            {
                System.Diagnostics.Process.Start("explorer.exe", this.MyGiteeURL);
            }
        }
        private readonly string MyGiteeURL = @"https://gitee.com/iLanceHe";
        private readonly string EasterEggText = @"    本程序是一个局域网联机的斗地主客户端，采用MIT开源许可，
本程序由立志成为某著名控制工程与科学系教授所定义的""卓越自动化工程师""的业余程序猿开发，仅限达到“能用”的水准，
在本程序及其源代码中您将看到：
    毫无设计的界面
    硬编码界面元素
    伪异步网络通信
欢迎提出建议与改进，本人Gitee主页https://gitee.com/iLanceHe ， 
点击""确认""可以立即前往";


    }
}
