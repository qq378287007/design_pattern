#include <iostream>
#include <memory>
using namespace std;

// if或switch分支不稳定，经常改动时，考虑引入算法独立到策略类中去实现

// 依赖倒置原则
// 高层组件不应该依赖于底层组件（具体实现类），两者都应该依赖于抽象层
// 面向接口（抽象层）编程，而不是针对实现（实现类）编程

namespace ns0
{
    enum ItemAddlife // 增加补充生命值道具
    {
        LF_BXD, // 补血丹
        LF_DHD, // 大还丹
        LF_SHD, // 守护丹
    };

    class Fighter
    {
    protected:
        int m_life; // 人物生命值
        int m_magic;
        int m_attack;

    public:
        Fighter(int life, int magic, int attack) : m_life(life), m_magic(magic), m_attack(attack) {}
        virtual ~Fighter() {}

    public:
        void UseItem(ItemAddlife djtype) // 吃药补充生命值
        {
            switch (djtype) // 道具类型
            {
            case LF_BXD:       // 补血丹
                m_life += 200; // 补充200点生命值
                // if (主角中毒了)
                //{
                //     停止中毒状态，也就是主角吃药后就不再中毒
                // }
                // if (主角处于狂暴状态)
                {
                    m_life += 400;  // 额外再补充400点生命值
                    m_magic += 200; // 魔法值也再补充200点
                }
                break;
            case LF_DHD:       // 大还丹
                m_life += 300; // 补充300点生命值
                break;
            case LF_SHD:       // 守护丹
                m_life += 500; // 补充500点生命
                break;
            }
        }
    };

    class F_Warrior : public Fighter // “战士”类
    {
    public:
        F_Warrior(int life, int magic, int attack) : Fighter(life, magic, attack) {}
    };

    class F_Mage : public Fighter // “法师”类
    {
    public:
        F_Mage(int life, int magic, int attack) : Fighter(life, magic, attack) {}
    };
}

namespace ns1
{
    class Fighter;
    class ItemStrategy // 道具策略类的父类
    {
    public:
        virtual ~ItemStrategy() {}
        virtual void UseItem(Fighter *mainobj) = 0;
    };

    // 战斗者父类
    class Fighter
    {
        shared_ptr<ItemStrategy> itemstrategy;

    protected:
        int m_life; // 人物生命值
        int m_magic;
        int m_attack;

    public:
        Fighter(int life, int magic, int attack) : m_life(life), m_magic(magic), m_attack(attack) {}
        virtual ~Fighter() {}

    public:
        void SetItemStrategy(const shared_ptr<ItemStrategy> &strategy) // 设置道具使用的策略
        {
            itemstrategy = strategy;
        }
        void UseItem() // 使用道具（吃药）
        {
            itemstrategy->UseItem(this);
        }

        int GetLife() const { return m_life; }

        void SetLife(int life) { m_life = life; }
    };

    class F_Warrior : public Fighter // “战士”类
    {
    public:
        F_Warrior(int life, int magic, int attack) : Fighter(life, magic, attack) {}
    };

    class F_Mage : public Fighter // “法师”类
    {
    public:
        F_Mage(int life, int magic, int attack) : Fighter(life, magic, attack) {}
    };

    class ItemStrategy_BXD : public ItemStrategy // 补血丹策略类
    {
    public:
        void UseItem(Fighter *mainobj) override
        {
            mainobj->SetLife(mainobj->GetLife() + 200); // 补充200点生命值
        }
    };

    class ItemStrategy_DHD : public ItemStrategy // 大还丹策略类
    {
    public:
        void UseItem(Fighter *mainobj) override
        {
            mainobj->SetLife(mainobj->GetLife() + 300); // 补充300点生命值
        }
    };

    class ItemStrategy_SHD : public ItemStrategy // 守护丹策略类
    {
    public:
        void UseItem(Fighter *mainobj) override
        {
            mainobj->SetLife(mainobj->GetLife() + 500); // 补充500点生命值
        }
    };
}

namespace ns2
{
    class M_Undead // 亡灵类怪物
    {
    public:
        void getinfo() const { cout << "M_Undead" << endl; }
    };
    class M_Element // 元素类怪物
    {
    public:
        void getinfo() const { cout << "M_Element" << endl; }
    };
    class M_Mechanic // 机械类怪物
    {
    public:
        void getinfo() const { cout << "M_Mechanic" << endl; }
    };

    class F_Warrior // 战士主角
    {
    public:
        void attack_enemy_undead(const shared_ptr<M_Undead> &pobj) // 攻击亡灵类怪物
        {
            pobj->getinfo(); // 可以调用亡灵类怪物相关的成员函数
        }

    public:
        void attack_enemy_element(const shared_ptr<M_Element> &pobj) // 攻击元素类怪物
        {
            pobj->getinfo(); // 可以调用元素类怪物相关的成员函数
        }
    };
}

namespace ns3
{
    class Monster // 作为所有怪物类（抽象层）
    {
    public:
        virtual ~Monster() {}
        virtual void getinfo() const = 0; // 纯虚函数
    };

    class M_Undead : public Monster // 亡灵类怪物
    {
    public:
        void getinfo() const override { cout << "M_Undead" << endl; }
    };
    class M_Element : public Monster // 元素类怪物
    {
    public:
        void getinfo() const override { cout << "M_Element" << endl; }
    };
    class M_Mechanic : public Monster // 机械类怪物
    {
    public:
        void getinfo() const override { cout << "M_Mechanic" << endl; }
    };

    class F_Warrior // 战士主角
    {
    public:
        void attack_enemy(const shared_ptr<Monster> &pobj) // 击杀怪物
        {
            pobj->getinfo(); // 可以调用怪物相关的成员函数
        }
    };
}

int main()
{
#if 1
    using namespace ns0;
    shared_ptr<Fighter> prole_war(new F_Warrior(1000, 0, 200)); // 这没有采用工厂模式，如果主角很多，可以考虑采用工厂模式创建对象
    prole_war->UseItem(LF_DHD);
#endif

#if 0
    using namespace ns1;
    // 创建主角
    shared_ptr<Fighter> prole_war(new F_Warrior(1000, 0, 200));

    // 吃一颗大还丹
    prole_war->SetItemStrategy(make_shared<ItemStrategy_DHD>()); // 主角设置大还丹策略，准备吃大还丹
    prole_war->UseItem();                                        // 主角吃大还丹

    // 再吃一颗补血丹
    shared_ptr<ItemStrategy> strateby2(new ItemStrategy_BXD()); // 创建补血丹策略
    prole_war->SetItemStrategy(strateby2);                      // 主角设置补血丹策略，准备吃补血丹
    prole_war->UseItem();                                       // 主角吃补血丹
#endif

#if 0
    using namespace ns2;
    shared_ptr<F_Warrior> pobjwar(new F_Warrior());

    pobjwar->attack_enemy_undead(make_shared<M_Undead>()); // 攻击一只亡灵类怪物

    shared_ptr<M_Element> pobjelm(new M_Element());
    pobjwar->attack_enemy_element(pobjelm); // 攻击一只元素类怪物
#endif

#if 0
    using namespace ns3;
    shared_ptr<F_Warrior> pobjwar(new F_Warrior());

    pobjwar->attack_enemy(make_shared<M_Undead>()); // 攻击一只亡灵类怪物

    shared_ptr<Monster> pobjelm(new M_Element());
    pobjwar->attack_enemy(pobjelm); // 攻击一只元素类怪物

#endif

    return 0;
}
