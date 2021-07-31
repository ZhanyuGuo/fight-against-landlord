# 斗地主C++

2021.7.31

改动：

+ 将牌型相关的类型声明和定义单独移到了新的头/源文件中
+ 新增了TypedCardEvent类，继承自CardEvent，携带牌型信息，并且将Player类的CardResponse方法的参数改为此类型，相关的地方也做了相应调整
+ 源文件中加入了一些#pragma region块

需要完成的：

+ CardResponse方法目前的返回值仍是基本的CardEvent类型，但是逻辑上也需要携带牌型信息；实际程序中，返回的也都是Typed的类型，并在调用方进行了向下造型；如果返回的不是Typed的类型，可能会出错。后续应考虑修改返回值类型，或者考虑增加其他的接口。

---

2021.7.30

+ 一些使用裸指针的地方已经改成了智能指针
+ 重载了PokerCard类的ToString方法，可以返回图形风格的字符串表示，但是在本机的VS2019的调试控制台（GBK编码）下，特殊字符无法显示
+ PokerCardCollection类新增ToString方法，并在IdBasedCardCollection里给出了一个简单的实现。后续不同牌型预计要覆写此方法
+ 将GameProcess的主流程拆成了多个函数，并额外打印了一些提示信息
+ 对Player类的一些方法补充了noexcept声明

---

2021.7.28初次提交

已经实现了一些基本的类型和过程，并且实现了一个简单的测试案例。

接下来需要完成的：

+ 游戏规则方面

需要补完斗地主牌型方面的规则，大体思路是每一种牌型都继承自抽象类TypedCardCollection并且实现所有的抽象方法，

```C++
enum class GeneralCardType
{
    Dan, Dui, San, Zha, Shun, LianDui, FeiJi, LianZha, WangZha
};

class TypedCardCollection : public IdBasedCardCollection
{
    public:
    virtual GeneralCardType GetGeneralType() = 0;
    virtual bool IsSameType(TypedCardCollection& other) = 0;
    virtual bool IsLargerThan(TypedCardCollection& other) = 0;
    virtual ~TypedCardCollection(){}
};
```

这是一个用于专门表示某一种牌型的PokerCardCollection类，每一个具体实现对应一种牌型，可以判断是否与另一个牌集属于同一牌型，以及是否大于该牌集。

另外，构造函数也需要专门实现，以保证该类符合对应的牌型。

+ 平台方面

初步设想是联网功能的实现全部写在OnlinePlayer类里：

```C++
class OnlinePlayer : public Player
```

目前的流程是基于抽象类Player的，只要实现了相应的抽象方法就能运行。

在此基础上，OnlinePlayer类需要负责作为服务器，与客户端进行网络通信。另外，出牌的合法性暂时依靠Player类自身来保证。

+ 其他
  + 将一些使用裸指针的部分改为智能指针，防止内存泄漏
  + PokerCardCollection提供的接口可能需要调整（尤其是PickOut相关功能）
  + 目前把整个流程写在了一个大函数里，应当拆分成多个函数
  + 暂时没有考虑没人抢叫地主的情况，后续需要补充
  + 考虑将主流程改成由事件驱动，以便扩充更多内容

