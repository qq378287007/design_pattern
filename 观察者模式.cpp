#include <iostream>
#include <list>
#include <map>
#include <memory>
using namespace std;

// 观察者模式
// 对象状态变化时，通过公告或广播方式通知一系列相关对象，实现对象之间的联动
// 4种角色
// Subject（主题），观察目标，被观察对象，Notifier类，addToList、removeFromList
// ConcreteSubject（具体主题），维护观察者列表，状态改变时，notify通知各个观察者，TalkNotifier
// Observer（观察者），被观察对象状态变化时，观察者自身收到通知，Fighter
// ConcreteObserver（具体观察者），F_Warrior、F_Mage

namespace ns1
{
    class Fighter;
    list<shared_ptr<Fighter>> g_playerList;

    class Fighter // 战斗者玩家
    {
    public:
        Fighter(int tmpID, const string &tmpName) : m_iPlayerID(tmpID), m_sPlayerName(tmpName) {}
        virtual ~Fighter() {}

    public:
        void SetFamilyID(int tmpID) { m_iFamilyID = tmpID; } // 加入家族时设置家族ID

    public:
        void SayWords(const string &tmpContent) // 玩家说了某句话
        {
            if (m_iFamilyID != -1)
                // 该玩家属于某个家族，应该把聊天内容信息传送给该家族的其他玩家
                for (auto iter = g_playerList.cbegin(); iter != g_playerList.cend(); ++iter)
                    if (m_iFamilyID == (*iter)->m_iFamilyID)
                        NotifyWords((*iter), tmpContent); // 同一个家族的其他玩家也应该收到聊天信息
        }

    private:
        void ReceiveMessage(const Fighter *const otherPlayer, const string &tmpContent) const
        {
            cout << "receive message: " << tmpContent << ", from " << otherPlayer->m_sPlayerName << ", by " << m_sPlayerName << endl;
        }
        void NotifyWords(const shared_ptr<Fighter> &otherPlayer, const string &tmpContent) // //其他玩家收到了当前玩家的聊天信息
        {
            if (otherPlayer.get() != this)
                otherPlayer->ReceiveMessage(this, tmpContent); // 同一个家族的其他玩家也应该收到聊天信息
        }

    private:
        int m_iPlayerID;      // 玩家ID，全局唯一
        string m_sPlayerName; // 玩家名字

        int m_iFamilyID{-1}; // 家族ID, -1表示没加入任何家族
    };

    class F_Warrior : public Fighter // “战士”类玩家
    {
    public:
        F_Warrior(int tmpID, const string &tmpName) : Fighter(tmpID, tmpName) {}
    };

    class F_Mage : public Fighter // “法师”类玩家
    {
    public:
        F_Mage(int tmpID, const string &tmpName) : Fighter(tmpID, tmpName) {}
    };
}

namespace ns2
{
    class Fighter; // 类前向声明
    class Notifier // 通知器父类
    {
    public:
        virtual void addToList(Fighter *player) = 0;                 // 把要被通知的玩家加到列表中
        virtual void removeFromList(Fighter *player) = 0;            // 把不想被通知的玩家从列表中去除
        virtual void notify(Fighter *talker, string tmpContent) = 0; // 通知的一些细节信息
        virtual ~Notifier() {}
    };

    class Fighter
    {
    public:
        Fighter(int tmpID, string tmpName) : m_iPlayerID(tmpID), m_sPlayerName(tmpName) // 构造函数
        {
            m_iFamilyID = -1; //-1表示没加入任何家族
        }
        virtual ~Fighter() {} // 析构函数

    public:
        void SetFamilyID(int tmpID) // 加入家族时设置家族ID
        {
            m_iFamilyID = tmpID;
        }
        int GetFamilyID() // 获取家族ID
        {
            return m_iFamilyID;
        }

        void SayWords(string tmpContent, Notifier *notifier) // 玩家说了某句话
        {
            notifier->notify(this, tmpContent);
        }

        // 通知该玩家接收到其他玩家发送来的聊天信息，虚函数，子类可以覆盖以实现不同的动作
        virtual void NotifyWords(Fighter *talker, string tmpContent)
        {
            // 显示信息
            cout << "player: " << m_sPlayerName << " receive player: " << talker->m_sPlayerName << " send chat message: " << tmpContent << endl;
        }

    private:
        int m_iPlayerID;      // 玩家ID，全局唯一
        string m_sPlayerName; // 玩家名字

        int m_iFamilyID; // 家族ID
    };

    // “战士”类玩家，父类为Fighter
    class F_Warrior : public Fighter
    {
    public:
        F_Warrior(int tmpID, string tmpName) : Fighter(tmpID, tmpName) {} // 构造函数
    };

    // “法师”类玩家，父类为Fighter
    class F_Mage : public Fighter
    {
    public:
        F_Mage(int tmpID, string tmpName) : Fighter(tmpID, tmpName) {} // 构造函数
    };

    class TalkNotifier : public Notifier // 聊天信息通知器
    {
    public:
        // 将玩家增加到家族列表中来
        virtual void addToList(Fighter *player)
        {
            int tmpfamilyid = player->GetFamilyID();
            if (tmpfamilyid != -1) // 加入了某个家族
            {
                auto iter = m_familyList.find(tmpfamilyid);
                if (iter != m_familyList.end())
                {
                    // 该家族id在map中已经存在
                    iter->second.push_back(player); // 直接把该玩家加入到该家族
                }
                else
                {
                    // 该家族id在map中不存在
                    list<Fighter *> tmpplayerlist;
                    m_familyList.insert(make_pair(tmpfamilyid, tmpplayerlist)); // 以该家族id为key，增加条目到map中
                    m_familyList[tmpfamilyid].push_back(player);                // 向该家族中增加第一个玩家
                }
            }
        }

        // 将玩家从家族列表中删除
        virtual void removeFromList(Fighter *player)
        {
            int tmpfamilyid = player->GetFamilyID();
            if (tmpfamilyid != -1) // 加入了某个家族
            {
                auto iter = m_familyList.find(tmpfamilyid);
                if (iter != m_familyList.end())
                    m_familyList[tmpfamilyid].remove(player);
            }
        }

        // 家族中某玩家说了句话，调用该函数来通知家族中所有人
        virtual void notify(Fighter *talker, string tmpContent) // talker是讲话的玩家
        {
            int tmpfamilyid = talker->GetFamilyID();
            if (tmpfamilyid != -1)
            {
                auto itermap = m_familyList.find(tmpfamilyid);
                if (itermap != m_familyList.end())
                    // 遍历该玩家所属家族的所有成员
                    for (auto iterlist = itermap->second.begin(); iterlist != itermap->second.end(); ++iterlist)
                        (*iterlist)->NotifyWords(talker, tmpContent);
            }
        }

    private:
        // map中的key表示家族id，value代表该家族中所有玩家列表
        map<int, list<Fighter *>> m_familyList;
    };
}

int main()
{
#if 1
    using namespace ns1;
    // 创建游戏玩家
    shared_ptr<Fighter> pplayerobj1 = make_shared<F_Warrior>(10, "1"); // 实际游戏中很多数据取自数据库
    pplayerobj1->SetFamilyID(100);                                     // 假设该玩家所在的家族的家族ID是100
    g_playerList.push_back(pplayerobj1);                               // 加入到全局玩家列表中

    shared_ptr<Fighter> pplayerobj2 = make_shared<F_Warrior>(20, "2");
    pplayerobj2->SetFamilyID(100);
    g_playerList.push_back(pplayerobj2);

    shared_ptr<Fighter> pplayerobj3 = make_shared<F_Mage>(30, "3");
    pplayerobj3->SetFamilyID(100);
    g_playerList.push_back(pplayerobj3);

    shared_ptr<Fighter> pplayerobj4 = make_shared<F_Mage>(40, "4");
    pplayerobj4->SetFamilyID(200); // pplayerobj4和前面三人属于两个不同的家族
    g_playerList.push_back(pplayerobj4);

    // 某游戏玩家聊天，同族人都应该收到该信息
    pplayerobj1->SayWords("come on, attact!");
#endif

#if 0
using namespace ns2;
        // 创建游戏玩家
        Fighter *pplayerobj1 = new F_Warrior(10, "3");
        pplayerobj1->SetFamilyID(100);

        Fighter *pplayerobj2 = new F_Warrior(20, "4");
        pplayerobj2->SetFamilyID(100);

        Fighter *pplayerobj3 = new F_Mage(30, "5");
        pplayerobj3->SetFamilyID(100);

        Fighter *pplayerobj4 = new F_Mage(50, "6");
        pplayerobj4->SetFamilyID(200);

        // 创建通知器
        Notifier *ptalknotify = new TalkNotifier();

        // 玩家增加到家族列表中来，这样才能收到家族聊天信息
        ptalknotify->addToList(pplayerobj1);
        ptalknotify->addToList(pplayerobj2);
        ptalknotify->addToList(pplayerobj3);
        ptalknotify->addToList(pplayerobj4);

        // 某游戏玩家聊天，同族人都应该收到该信息
        pplayerobj1->SayWords("come on, attack!", ptalknotify);

        cout << "leave---" << endl;
        ptalknotify->removeFromList(pplayerobj3); // 将王五从家族列表中删除

        pplayerobj2->SayWords("listen, go together!", ptalknotify);

        // 释放资源
        delete pplayerobj1;
        delete pplayerobj2;
        delete pplayerobj3;
        delete pplayerobj4;
        delete ptalknotify;
#endif

    cout << "Over!\n";
    return 0;
}