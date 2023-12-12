#include <iostream>
#include <list>
#include <map>
#include <memory>
using namespace std;

// 对象状态变化时，通过公告或广播方式通知一系列相关对象，实现对象之间的联动
// 4种角色
// Subject（主题），观察目标，被观察对象，Notifier类，addToList、removeFromList
// ConcreteSubject（具体主题），维护观察者列表，状态改变时，notify通知各个观察者，TalkNotifier
// Observer（观察者），被观察对象状态变化时，观察者自身收到通知，Fighter
// ConcreteObserver（具体观察者），F_Warrior、F_Mage

namespace ns1
{
    class Fighter;
    static list<shared_ptr<Fighter>> g_playerList;

    class Fighter // 战斗者玩家
    {
        int m_iPlayerID;      // 玩家ID，全局唯一
        string m_sPlayerName; // 玩家名字
        int m_iFamilyID{-1};  // 家族ID, -1表示没加入任何家族
    private:
        void ReceiveMessage(const Fighter *const otherPlayer, const string &tmpContent) const
        {
            cout << "receive message: " << tmpContent << ", from " << otherPlayer->m_sPlayerName << ", by " << m_sPlayerName << endl;
        }
        void NotifyWords(const shared_ptr<Fighter> &otherPlayer, const string &tmpContent) const // //其他玩家收到了当前玩家的聊天信息
        {
            if (otherPlayer.get() != this)
                otherPlayer->ReceiveMessage(this, tmpContent); // 同一个家族的其他玩家也应该收到聊天信息
        }

    public:
        Fighter(int tmpID, const string &tmpName) : m_iPlayerID(tmpID), m_sPlayerName(tmpName) {}
        virtual ~Fighter() {}

    public:
        void SetFamilyID(int tmpID) { m_iFamilyID = tmpID; } // 加入家族时设置家族ID

    public:
        void SayWords(const string &tmpContent) const // 玩家说了某句话
        {
            if (m_iFamilyID != -1)
                // 该玩家属于某个家族，应该把聊天内容信息传送给该家族的其他玩家
                for (auto iter = g_playerList.cbegin(); iter != g_playerList.cend(); ++iter)
                    if (m_iFamilyID == (*iter)->m_iFamilyID)
                        NotifyWords((*iter), tmpContent); // 同一个家族的其他玩家也应该收到聊天信息
        }
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
        virtual ~Notifier() {}
        virtual void addToList(const shared_ptr<Fighter> &player) = 0;                        // 把要被通知的玩家加到列表中
        virtual void removeFromList(const shared_ptr<Fighter> &player) = 0;                   // 把不想被通知的玩家从列表中去除
        virtual void notify(const Fighter *const talker, const string &tmpContent) const = 0; // 通知的一些细节信息
    };

    class Fighter
    {
        int m_iPlayerID;      // 玩家ID，全局唯一
        string m_sPlayerName; // 玩家名字
        int m_iFamilyID{-1};  // 家族ID，-1表示没加入任何家族

    private:
        void ReceiveMessage(const Fighter *const otherPlayer, const string &tmpContent) const
        {
            cout << "receive message: " << tmpContent << ", from " << otherPlayer->m_sPlayerName << ", by " << m_sPlayerName << endl;
        }

    public:
        virtual ~Fighter() {}
        Fighter(int tmpID, const string &tmpName) : m_iPlayerID(tmpID), m_sPlayerName(tmpName) {}

    public:
        void SetFamilyID(int tmpID) { m_iFamilyID = tmpID; }
        int GetFamilyID() const { return m_iFamilyID; }

        void SayWords(const string &tmpContent, const shared_ptr<Notifier> &notifier) const // 玩家说了某句话
        {
            notifier->notify(this, tmpContent);
        }

    public:
        void NotifyWords(const Fighter *const talker, const string &tmpContent) const // 其他玩家收到了当前玩家的聊天信息
        {
            if (talker != this)
                ReceiveMessage(talker, tmpContent); // 同一个家族的其他玩家也应该收到聊天信息
        }
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

    class TalkNotifier : public Notifier // 聊天信息通知器
    {
        map<int, list<shared_ptr<Fighter>>> m_familyList; // key: 家族id，value: 玩家列表
    public:
        void addToList(const shared_ptr<Fighter> &player) override // 将玩家增加到家族列表中来
        {
            int tmpfamilyid = player->GetFamilyID();
            if (tmpfamilyid != -1) // 加入了某个家族
            {
                auto iter = m_familyList.find(tmpfamilyid);
                if (iter != m_familyList.end()) // 该家族id在map中已经存在
                {
                    iter->second.push_back(player); // 直接把该玩家加入到该家族
                }
                else // 该家族id在map中不存在
                {
                    list<shared_ptr<Fighter>> tmpplayerlist;
                    m_familyList.insert(make_pair(tmpfamilyid, tmpplayerlist)); // 以该家族id为key，增加条目到map中
                    m_familyList[tmpfamilyid].push_back(player);                // 向该家族中增加第一个玩家
                }
            }
        }

        void removeFromList(const shared_ptr<Fighter> &player) override // 将玩家从家族列表中删除
        {
            int tmpfamilyid = player->GetFamilyID();
            if (tmpfamilyid != -1) // 加入了某个家族
            {
                auto iter = m_familyList.find(tmpfamilyid);
                if (iter != m_familyList.end())
                {
                    m_familyList[tmpfamilyid].remove(player);
                    if (m_familyList[tmpfamilyid].empty()) // 该家族中没有玩家了，应该从map中删除该家族
                        m_familyList.erase(tmpfamilyid);
                }
            }
        }

        // 家族中某玩家说了句话，调用该函数来通知家族中所有人
        void notify(const Fighter *const talker, const string &tmpContent) const override // talker是讲话的玩家
        {
            int tmpfamilyid = talker->GetFamilyID();
            if (tmpfamilyid != -1)
            {
                auto itermap = m_familyList.find(tmpfamilyid);
                if (itermap != m_familyList.end())
                    // 遍历该玩家所属家族的所有成员
                    for (auto iterlist = itermap->second.cbegin(); iterlist != itermap->second.cend(); ++iterlist)
                        (*iterlist)->NotifyWords(talker, tmpContent);
            }
        }
    };
}

int main()
{
#if 0
    using namespace ns1;
    // 创建游戏玩家
    shared_ptr<Fighter> pplayerobj1(new F_Warrior(10, "1")); // 实际游戏中很多数据取自数据库
    pplayerobj1->SetFamilyID(100);                           // 假设该玩家所在的家族的家族ID是100
    g_playerList.push_back(pplayerobj1);                     // 加入到全局玩家列表中

    shared_ptr<Fighter> pplayerobj2(new F_Warrior(20, "2"));
    pplayerobj2->SetFamilyID(100);
    g_playerList.push_back(pplayerobj2);

    shared_ptr<Fighter> pplayerobj3(new F_Mage(30, "3"));
    pplayerobj3->SetFamilyID(100);
    g_playerList.push_back(pplayerobj3);

    shared_ptr<Fighter> pplayerobj4(new F_Mage(40, "4"));
    pplayerobj4->SetFamilyID(200); // pplayerobj4和前面三人属于两个不同的家族
    g_playerList.push_back(pplayerobj4);

    // 某游戏玩家聊天，同族人都应该收到该信息
    pplayerobj1->SayWords("come on, attact!");
#endif

#if 1
    using namespace ns2;
    // 创建游戏玩家
    shared_ptr<Fighter> pplayerobj1(new F_Warrior(10, "1"));
    pplayerobj1->SetFamilyID(100);

    shared_ptr<Fighter> pplayerobj2(new F_Warrior(20, "2"));
    pplayerobj2->SetFamilyID(100);

    shared_ptr<Fighter> pplayerobj3(new F_Mage(30, "3"));
    pplayerobj3->SetFamilyID(100);

    shared_ptr<Fighter> pplayerobj4(new F_Mage(40, "4"));
    pplayerobj4->SetFamilyID(200);

    // 创建通知器
    shared_ptr<Notifier> ptalknotify(new TalkNotifier());

    // 玩家增加到家族列表中来，这样才能收到家族聊天信息
    ptalknotify->addToList(pplayerobj1);
    ptalknotify->addToList(pplayerobj2);
    ptalknotify->addToList(pplayerobj3);
    ptalknotify->addToList(pplayerobj4);
    // 某游戏玩家聊天，同族人都应该收到该信息
    pplayerobj1->SayWords("come on, attack!", ptalknotify);

    cout << "leave---" << endl;

    ptalknotify->removeFromList(pplayerobj3); // 将pplayerobj3从家族列表中删除
    pplayerobj2->SayWords("listen, go together!", ptalknotify);
#endif

    cout << "Over!\n";
    return 0;
}