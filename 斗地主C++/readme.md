# 斗地主C++

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

