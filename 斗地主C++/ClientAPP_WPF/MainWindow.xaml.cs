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
            this.CardButtons = new List<Button>(){
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
            this.PointButtons = new List<Button>(){
                this.Point0Button,
                this.Point1Button,
                this.Point2Button,
                this.Point3Button,
            };
            this.CardSelectedFlag = new bool[this.CardButtons.Count];
            for (int i = 0; i < this.CardSelectedFlag.Length; i++) 
            {
                this.CardSelectedFlag[i] = false;
            }
            foreach(var cardButton in this.CardButtons)
            {
                // cardButton.Visibility = Visibility.Hidden;
                cardButton.IsEnabled = false;
                cardButton.Background = Brushes.LightGray;
            }
            this.HideComponents();
            

            this.UserNameButton.IsEnabled = false;
#if DEBUG
            core.DebugEvent += this.WriteToDebugTextBox;
            core.ServerNotFound += () => { this.WriteToDebugTextBox("ServerNotFound"); };
            core.ServerFound += () => { this.WriteToDebugTextBox("ServerFound"); };
            //core.SceneReceived += (Scene se) => { this.WriteToDebugTextBox(se.ParseIPAdress().ToString()); };
#endif

            core.ServerFound += this.ServerFoundHandler;
            core.ServerNotFound += this.ServerNotFoundHandler;
            core.NoHearingFromServer += this.NoHearingFromServerHandler;
            core.JoinGameFailed += this.JoinGameFailHandler;
            core.SceneReceived += this.RenderCurrentScene;
            core.CardInformationGot += this.RenderSelfCards;

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
        private List<Button> PointButtons;
        
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

        private unsafe void RenderCurrentScene(Scene se)
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
                        foreach (Button pointButton in this.PointButtons)
                        {
                            pointButton.Visibility = Visibility.Hidden;
                        }
                        if (this.IsPlayerPrepared(this.PlayerIndex))
                        {
                            this.PrepareButton.IsEnabled = false;
                            this.PrepareButton.Content = "已准备";
                            this.SelfStatusLabel.Content = "已准备";
                        }
                        else
                        {
                            this.PrepareButton.IsEnabled = true;
                            this.PrepareButton.Content = "准备";
                            this.SelfStatusLabel.Content = "未准备";
                        }
                        if (this.IsPlayerPrepared(this.LastPlayerIndex))
                        {
                            this.Player1StatusLabel.Content = "已准备";
                        }
                        else
                        {
                            this.Player1StatusLabel.Content = "未准备";
                        }
                        if (this.IsPlayerPrepared(this.SecondLastPlayerIndex))
                        {
                            this.Player2StatusLabel.Content = "已准备";
                        }
                        else
                        {
                            this.Player2StatusLabel.Content = "未准备";
                        }
                        this.EreaseLordCars();
                        this.CardPostButton.Visibility = Visibility.Hidden;
                        this.CardPassButton.Visibility = Visibility.Hidden;
                        break;
                    }
                case GAME_STAGE_DETERMINE_LANDLORD:
                    {
                        this.core.InfoGetCards();
                        this.PrepareButton.Visibility = Visibility.Hidden;
                        if (this.CurrentScene.ActiveIndex == this.PlayerIndex)
                        {
                            foreach (Button pointButton in this.PointButtons)
                            {
                                pointButton.Visibility = Visibility.Visible;
                            }
                            this.Point0Button.IsEnabled = true;
                            int leastPoint = (int)this.CurrentScene.ActiveParam;
                            for (int i = 1; i < this.PointButtons.Count; i++) 
                            {
                                this.PointButtons[i].IsEnabled = i >= leastPoint;
                            }
                        }
                        else
                        {
                            foreach (Button pointButton in this.PointButtons)
                            {
                                pointButton.Visibility = Visibility.Hidden;
                            }
                        }
                        this.EreaseLordCars();
                        this.CardPostButton.Visibility = Visibility.Hidden;
                        this.CardPassButton.Visibility = Visibility.Hidden;
                        this.Player1StatusLabel.Content = $"{this.CurrentScene.LandlordWillingness[this.LastPlayerIndex].ToString()}分";
                        this.Player2StatusLabel.Content = $"{this.CurrentScene.LandlordWillingness[this.SecondLastPlayerIndex].ToString()}分";
                        this.SelfStatusLabel.Content = $"{this.CurrentScene.LandlordWillingness[this.PlayerIndex].ToString()}分";
                        break;
                    }
                case GAME_STAGE_MAINLOOP_ONGOING:
                    {
                        this.core.InfoGetCards();
                        foreach (Button pointButton in this.PointButtons)
                        {
                            pointButton.Visibility = Visibility.Hidden;
                        }
                        int activeIndex = (int)this.CurrentScene.ActiveIndex;  
                        List<Poker> lordCards = PokerSerializer.Deserialize(this.CurrentScene.GetLordCards());
                        this.RenderLordCards(lordCards);
                        if (activeIndex == this.PlayerIndex)
                        {
                            this.CardPostButton.Visibility = Visibility.Visible;
                            this.CardPassButton.Visibility = Visibility.Visible;
                        }
                        else
                        {
                            // this.SelfStatusLabel.Content = "等待其他玩家出牌";
                            this.CardPostButton.Visibility = Visibility.Hidden;
                            this.CardPassButton.Visibility = Visibility.Hidden;
                        }
                        if (this.CurrentScene.LastActiveIndex == this.LastPlayerIndex)
                        {
                            if (this.CurrentScene.LastActType == ACTIVE_TYPE_PASS)
                            {
                                this.Player1ActionContentLabel.Content = "跳过";
                            }
                            else
                            {
                                List<Poker> lastCards = PokerSerializer.Deserialize(this.CurrentScene.GetLastAct());
                                this.Player1ActionContentLabel.Content = PokerSerializer.DecodePokersToString(lastCards);
                            }
                            if (this.CurrentScene.SecondLastActType == ACTIVE_TYPE_PASS)
                            {
                                this.Player2ActionContentLabel.Content = "跳过";
                            }
                            else
                            {
                                List<Poker> secondLastCards = PokerSerializer.Deserialize(this.CurrentScene.GetSecondLastAct());
                                this.Player2ActionContentLabel.Content = PokerSerializer.DecodePokersToString(secondLastCards);
                            }
                            this.SelfStatusLabel.Content = "轮到自己出牌";
                        }
                        else if(this.CurrentScene.LastActiveIndex == this.SecondLastPlayerIndex)
                        {
                            if (this.CurrentScene.LastActType == ACTIVE_TYPE_PASS)
                            {
                                this.Player2ActionContentLabel.Content = "跳过";
                            }
                            else
                            {
                                List<Poker> lastCards = PokerSerializer.Deserialize(this.CurrentScene.GetLastAct());
                                this.Player2ActionContentLabel.Content = PokerSerializer.DecodePokersToString(lastCards);
                            }
                            if (this.CurrentScene.SecondLastActType == ACTIVE_TYPE_PASS)
                            {
                                this.SelfStatusLabel.Content = "跳过";
                            }
                            else
                            {
                                List<Poker> secondLastCards = PokerSerializer.Deserialize(this.CurrentScene.GetSecondLastAct());
                                this.SelfStatusLabel.Content = PokerSerializer.DecodePokersToString(secondLastCards, 10);
                            }
                            this.Player1ActionContentLabel.Content = "出牌中";
                        }
                        else if(this.CurrentScene.LastActiveIndex == this.PlayerIndex)
                        {
                            if (this.CurrentScene.LastActType == ACTIVE_TYPE_PASS)
                            {
                                this.SelfStatusLabel.Content = "已跳过";
                            }
                            else
                            {
                                List<Poker> lastCards = PokerSerializer.Deserialize(this.CurrentScene.GetLastAct());
                                this.SelfStatusLabel.Content = PokerSerializer.DecodePokersToString(lastCards, 10);
                                // this.SelfStatusLabel.Content = "已出牌";
                            }
                            if (this.CurrentScene.SecondLastActType == ACTIVE_TYPE_PASS)
                            {
                                this.Player1ActionContentLabel.Content = "跳过";
                            }
                            else
                            {
                                List<Poker> secondLastCards = PokerSerializer.Deserialize(this.CurrentScene.GetSecondLastAct());
                                this.Player1ActionContentLabel.Content = PokerSerializer.DecodePokersToString(secondLastCards);
                            }
                            this.Player2ActionContentLabel.Content = "出牌中";
                        }
                        this.Player1StatusLabel.Content = $"剩余牌数:{this.CurrentScene.CardLeftCount[this.LastPlayerIndex]}";
                        this.Player2StatusLabel.Content = $"剩余牌数:{this.CurrentScene.CardLeftCount[this.SecondLastPlayerIndex]}";
                        break;
                    }
                case GAME_STAGE_END:
                    {
                        this.CardPostButton.Visibility = Visibility.Hidden;
                        this.CardPassButton.Visibility = Visibility.Hidden;
                        int winnerFlag = (int)this.CurrentScene.WinnerFlag;
                        if ((winnerFlag & (1 << this.PlayerIndex)) != 0) 
                        {
                            this.SelfStatusLabel.Content = "Victory";
                        }
                        else
                        {
                            this.SelfStatusLabel.Content = "Defeat";
                        }
                        if ((winnerFlag & (1 << this.SecondLastPlayerIndex)) != 0)
                        {
                            this.Player1StatusLabel.Content = "Victory";
                        }
                        else
                        {
                            this.Player1StatusLabel.Content = "Defeat";
                        }
                        if ((winnerFlag & (1 << this.LastPlayerIndex)) != 0)
                        {
                            this.Player2StatusLabel.Content = "Victory";
                        }
                        else
                        {
                            this.Player2StatusLabel.Content = "Defeat";
                        }
                        break;
                    }
                default:

                    break;
            }

        }

        private void RenderSelfCards(List<Poker> cards)
        {
            int cardCount = cards.Count;
            int totalPos = this.CardButtons.Count;
            for (int i = 0; i < totalPos; i++) 
            {
                if (i < cardCount) 
                {
                    this.CardButtons[i].Content = cards[i].ToString();
                    this.CardButtons[i].IsEnabled = true;
                }
                else 
                {
                    this.CardButtons[i].Content = "Card";
                    this.CardButtons[i].IsEnabled = false;
                }
            }
        }

        private void RenderLordCards(List<Poker> pokers)
        {
            this.LordCard1.Content = pokers[0].ToString();
            this.LordCard2.Content = pokers[1].ToString();
            this.LordCard3.Content = pokers[2].ToString();
        }
        private void EreaseLordCars()
        {
            this.LordCard1.Content = "Cards";
            this.LordCard2.Content = "Cards";
            this.LordCard3.Content = "Cards";
        }

        private void HideComponents()
        {
            this.PrepareButton.Visibility = Visibility.Hidden;
            foreach (Button pointButton in this.PointButtons)
            {
                pointButton.Visibility = Visibility.Hidden;
            }
            this.CardPostButton.Visibility = Visibility.Hidden;
            this.CardPassButton.Visibility = Visibility.Hidden;
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
            MessageBox.Show("加入游戏失败，可能是服务器未响应、或者游戏已经满员。你或许仍可以尝试继续游戏。", "尝试加入失败");
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

        private void Point1Button_Click(object sender, RoutedEventArgs e)
        {
            this.core.PostPoint(1);
            foreach (Button pointButton in this.PointButtons)
            {
                pointButton.IsEnabled = false;
            }
            this.SelfStatusLabel.Content = "提交叫分中";
        }

        private void Point2Button_Click(object sender, RoutedEventArgs e)
        {
            this.core.PostPoint(2);
            foreach (Button pointButton in this.PointButtons)
            {
                pointButton.IsEnabled = false;
            }
            this.SelfStatusLabel.Content = "提交叫分中";
        }

        private void Point3Button_Click(object sender, RoutedEventArgs e)
        {
            this.core.PostPoint(3);
            foreach (Button pointButton in this.PointButtons)
            {
                pointButton.IsEnabled = false;
            }
            this.SelfStatusLabel.Content = "提交叫分中";
        }

        private void Point0Button_Click(object sender, RoutedEventArgs e)
        {
            this.core.PostPoint(0);
            foreach (Button pointButton in this.PointButtons)
            {
                pointButton.IsEnabled = false;
            }
            this.SelfStatusLabel.Content = "提交叫分中";
        }

        private bool[] CardSelectedFlag;
        private void PlayerCard_Click(object sender, RoutedEventArgs e)
        {
            int count = this.CardButtons.Count;
            for (int i = 0; i < count; i++)
            {
                if (e.Source == this.CardButtons[i])
                {
                    if (!this.CardSelectedFlag[i])
                    {
                        this.CardButtons[i].Background = Brushes.AliceBlue;
                        this.CardSelectedFlag[i] = true;
                    }
                    else
                    {
                        this.CardButtons[i].Background = Brushes.LightGray;
                        this.CardSelectedFlag[i] = false;
                    }

                }
            }
        }

        private int SelectedCardCount { get
            {
                int c = 0;
                foreach(bool isCardSelected in this.CardSelectedFlag)
                {
                    if (isCardSelected) { c++; }
                }
                return c;
            } }

        private void CardPassButton_Click(object sender, RoutedEventArgs e)
        {
            this.ResetCardSelectStatus();
            this.core.PostCardAction(null);
        }

        private void CardPostButton_Click(object sender, RoutedEventArgs e)
        {
            int selectedCount = this.SelectedCardCount;
            if(selectedCount == 0)
            {
                return;
            }
            byte[] cardIndexes = new byte[selectedCount];
            int c = 0;
            for (int cardIndex = 0; cardIndex < 20; cardIndex++) 
            {
                if(this.CardSelectedFlag[cardIndex])
                {
                    cardIndexes[c] = (byte)cardIndex;
                    c++;
                }
            }
            this.core.PostCardAction(cardIndexes);
            this.ResetCardSelectStatus();
        }

        private void ResetCardSelectStatus()
        {
            foreach (Button cardButton in this.CardButtons)
            {
                cardButton.Background = Brushes.LightGray;
            }
            for (int i = 0; i < 20; i++) 
            {
                this.CardSelectedFlag[i] = false;
            }
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
