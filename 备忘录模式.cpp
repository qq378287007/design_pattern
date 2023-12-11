#include <iostream>
#include <vector>
#include <memory>
using namespace std;

// 保存对象内部状态，必要时恢复
// 在不破坏封装性的前提下，捕获对象的内部状态并保存，之后可将对象恢复到原先保存的状态
// 3种角色
// Originator（原发器），普通业务类，可以创建备忘录用于保存其当前的内部状态，之后用备忘录恢复内部状态
// Memento（备忘录），存储原发器对象某个瞬间的内部状态
// CareTaker(负责人/管理者)，保存和传递备忘录，不知道备忘录细节，不能检查或操作备忘录

namespace ns1
{
    class FighterMemento // 玩家主角相关的备忘录类
    {
        friend class Fighter; // 友元类Fighter可以访问本类的私有成员函数

        // 玩家主角类中要保存起来的数据，就放到这里来
        int m_life;   // 生命值
        int m_magic;  // 魔法值
        int m_attack; // 攻击力

    private: // 构造函数，用private修饰以防止在外部被随意创建
        FighterMemento(int life, int magic, int attack) : m_life(life), m_magic(magic), m_attack(attack) {}

    private: // 提供一些供Fighter类来访问的接口，用private修饰防止被任意类访问
        int getLife() const { return m_life; }
        void setLife(int life) { m_life = life; }
        int getMagic() const { return m_magic; }
        void setMagic(int magic) { m_magic = magic; }
        int getAttack() const { return m_attack; }
        void setAttack(int attack) { m_attack = attack; }
    };

    class Fighter     // 玩家主角类
    {                 // 角色属性
        int m_life;   // 生命值
        int m_magic;  // 魔法值
        int m_attack; // 攻击力
    public:
        Fighter(int life, int magic, int attack) : m_life(life), m_magic(magic), m_attack(attack) {}

    public: // 将玩家数据写入备忘录（创建备忘录，并在其中存储了当前状态）
        shared_ptr<FighterMemento> createMomento() const
        {
            shared_ptr<FighterMemento> back(new FighterMemento(m_life, m_magic, m_attack));
            return back;
            // return make_shared<FighterMemento>(m_life, m_magic, m_attack);
        }
        void restoreMomento(const shared_ptr<FighterMemento> &pfm) // 从备忘录中恢复玩家数据
        {
            m_life = pfm->m_life; // 友元类
            m_magic = pfm->getMagic();
            m_attack = pfm->getAttack();
        }
        void setToDead() { m_life = 0; } // 为测试目的引入的接口，设置玩家的生命值为0（玩家死亡）
        void displayInfo() const         // 用于输出一些信息
        {
            cout << "The player's current HP, magic and attack power are respectively " << m_life << ", " << m_magic << ", " << m_attack << endl;
        }
    };

    class FCareTaker // 管理者（负责人）类
    {
        shared_ptr<FighterMemento> m_pfm; // 指向备忘录对象的指针
    public:
        FCareTaker(const shared_ptr<FighterMemento> &ptmpfm = nullptr) : m_pfm(ptmpfm) {} // 形参是指向备忘录对象的指针
        shared_ptr<FighterMemento> getMemento() const { return m_pfm; }                   // 获取指向备忘录对象的指针
        void setMemento(const shared_ptr<FighterMemento> &ptmpfm) { m_pfm = ptmpfm; }     // 保存指向备忘录对象的指针
    };

    class FCareTaker2 // 支持多个快照的负责人（管理者）类
    {
        vector<shared_ptr<FighterMemento>> m_pfmContainer; // 存储备忘录对象指针的容器

    public:
        void push(const shared_ptr<FighterMemento> &ptmpfm) { m_pfmContainer.emplace_back(ptmpfm); } // 保存指向备忘录对象的指针
        shared_ptr<FighterMemento> pop()
        {
            if (m_pfmContainer.empty())
                return nullptr;
            return m_pfmContainer.back();
        }
        shared_ptr<FighterMemento> getMemento(int index) const // 获取指向备忘录对象的指针
        {
            if (index >= 0 && index < m_pfmContainer.size())
                return m_pfmContainer[index];
            return nullptr;
        }
    };
}

int main()
{
#if 0
    using namespace ns1;
    shared_ptr<Fighter> p_fighter(new Fighter(800, 200, 300));
    
    // （1）显示玩家主角在与BOSS战斗之前的信息
    p_fighter->displayInfo();

    // （2）为玩家主角类对象创建一个备忘录对象（其中保存了当前主角类对象中的必要信息）
    shared_ptr<FighterMemento> p_fighterMemo = p_fighter->createMomento();

    // （3）玩家与BOSS开始战斗
    cout << "The protagonist of the player and BOSS began to fight fiercely------" << endl;
    p_fighter->setToDead();   // 玩家主角在与BOSS战斗中，生命值最终变成0而死亡（被BOSS击败）
    p_fighter->displayInfo(); // 显示玩家主角在与BOSS战斗之后的信息

    // （4）因为在与BOSS战斗之前已经通过NPC保存了游戏进度，这里模拟载入游戏进度，恢复玩家主角类对象的数据，让其可以与BOSS再次战斗
    cout << "Players recover their information through memos------" << endl;
    p_fighter->restoreMomento(p_fighterMemo);
    p_fighter->displayInfo(); // 显示玩家主角通过备忘录恢复到战斗之前的信息
#endif

#if 0
    using namespace ns1;
    shared_ptr<Fighter> p_fighter(new Fighter(800, 200, 300));

    // （1）显示玩家主角在与BOSS战斗之前的信息
    p_fighter->displayInfo();

    // （2）为玩家主角类对象创建一个备忘录对象（其中保存了当前主角类对象中的必要信息）
    shared_ptr<FCareTaker> pfcaretaker(new FCareTaker(p_fighter->createMomento()));

    // （3）玩家与BOSS开始战斗
    cout << "The protagonist of the player and BOSS began to fight fiercely------" << endl;
    p_fighter->setToDead();   // 玩家主角在与BOSS战斗中，生命值最终变成0而死亡（被BOSS击败）
    p_fighter->displayInfo(); // 显示玩家主角在与BOSS战斗之后的信息

    // （4）因为在与BOSS战斗之前已经通过NPC保存了游戏进度，这里模拟载入游戏进度，恢复玩家主角类对象的数据，让其可以与BOSS再次战斗
    cout << "Players recover their information through memos------" << endl;
    p_fighter->restoreMomento(pfcaretaker->getMemento());
    p_fighter->displayInfo(); // 显示玩家主角通过备忘录恢复到战斗之前的信息
#endif

#if 1
    using namespace ns1;
    shared_ptr<Fighter> p_fighter2(new Fighter(800, 200, 300));
    shared_ptr<FCareTaker2> pfcaretaker2(new FCareTaker2());
    pfcaretaker2->push(p_fighter2->createMomento()); // 做第一次快照吗，此快照玩家生命值为800
    p_fighter2->setToDead();                         // 改变玩家主角的生命值
    pfcaretaker2->push(p_fighter2->createMomento()); // 做第二次快照，此快照玩家生命值为0
    p_fighter2->displayInfo();                       // 玩家主角生命值应该为0
    cout << "------------------" << endl;
    // 当前玩家生命值为0，恢复第一次快照，也就是恢复玩家生命值为800
    p_fighter2->restoreMomento(pfcaretaker2->getMemento(0));
    p_fighter2->displayInfo(); // 玩家主角生命值应该恢复为800
#endif

    cout << "Over!\n";
    return 0;
}