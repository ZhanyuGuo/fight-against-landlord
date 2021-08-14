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
                cardButton.Visibility = Visibility.Hidden;
            }
            core.DebugEvent += this.WriteToDebugTextBox;
            core.NoHearingFromServer += this.NoHearingFromServerHandler;
            core.SceneReceived += (Scene se) => { this.WriteToDebugTextBox(se.ParseIPAdress().ToString()); };
            core.InitAndRun();
        }

        private ClientCore core;
        private bool IsNameSet;
        private List<Button> CardButtons;
        private int UserNameHashCode { get => this.core.UserNameHashCode; set => this.core.UserNameHashCode = value; }
        private void UserNameButton_Click(object sender, RoutedEventArgs e)
        {
            if (!this.IsNameSet)
            {
                string name = this.UserNameTextBox.Text;
                if (!string.IsNullOrWhiteSpace(name) && name != "NULL")
                {
                    this.IsNameSet = true;
                    this.UserNameTextBox.IsEnabled = false;
                    this.UserNameButton.Content = "修改";
                    this.UserNameHashCode = name.GetHashCode();
                    if (name == "Debug")
                    {
                        this.EasterEgg();
                    }
                }
                else
                {
                    var _ = MessageBox.Show("名称不能为空或者\"NULL\"", "名称无效", MessageBoxButton.OK);
                }
            }
            else
            {
                // this.UserNameTextBox.Text = string.Empty;
                this.IsNameSet = false;
                this.UserNameTextBox.IsEnabled = true;
                this.UserNameButton.Content = "确定";
                this.UserNameHashCode = 1478206855;
            }
        }

        private void NoHearingFromServerHandler()
        {
            this.ServerIPTextBox.Text = "event invoked";
        }

        private void WriteToDebugTextBox(string str)
        {
            this.DebugTextBox.Text = str;
        }

        private void JoinServerButton_Click(object sender, RoutedEventArgs e)
        {
            //System.Diagnostics.Process.Start("explorer.exe", @"https://gitee.com/iLanceHe");
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
