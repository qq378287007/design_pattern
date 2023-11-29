#include <iostream>
#include <memory>
using namespace std;

namespace ns1
{
    class Warrior // “战士”类
    {
    public:
        Warrior(int life, int magic, int attack) : m_life(life), m_magic(magic), m_attack(attack) {}

    public:
        void JN_Burn() // 技能“燃烧”
        {
            m_life -= 300;
            cout << "Lose 500 HP points for each enemy" << endl;
            cout << "The protagonist loses 300 HP" << endl;
            cout << "Play the special effect of the \"burning\" skill to the players" << endl;
        }

    private:
        int m_life;   // 生命值
        int m_magic;  // 魔法值
        int m_attack; // 攻击力
    };
}

namespace ns2
{
    class Fighter // 战斗者父类
    {
    public:
        Fighter(int life, int magic, int attack) : m_life(life), m_magic(magic), m_attack(attack) {}
        virtual ~Fighter() {} // 做父类时析构函数应该为虚函数

        // 模板方法模式
        // 动态绑定，多态
        // 稳定+变化
        // 稳定，内部函数调用顺序固定
        // 变化
        // 内部具体函数调用由各子类决定
        void JN_Burn() // 技能“燃烧”
        {
            if (canUseJN() == false) // 如果不能使用该技能，则直接返回
                return;

            effect_enemy(); // 对敌人产生的影响
            effect_self();  // 对主角自身产生的影响
            play_effect();  // 播放技能“燃烧”的技能特效
        }

    private:
        virtual bool canUseJN() const = 0; // 判断是否能使用技能“燃烧”，纯虚函数声明，子类中必须实现

    private:
        virtual void effect_enemy() const {}
        virtual void effect_self(){};
        void play_effect() const
        {
            cout << "Play the special effect of the \"burning\" skill to the players" << endl;
        }

    protected:
        int m_life;   // 生命值
        int m_magic;  // 魔法值
        int m_attack; // 攻击力
    };

    class F_Warrior : public Fighter // “战士”类
    {
    public:
        F_Warrior(int life, int magic, int attack) : Fighter(life, magic, attack) {}

    private:
        bool canUseJN() const override
        {
            return m_life >= 300;
        }

    private:
        void effect_enemy() const override
        {
            cout << "Lose 500 HP points for each enemy" << endl;
        }
        void effect_self() override
        {
            cout << "The protagonist loses 300 HP" << endl;
            m_life -= 300;
        }
    };

    class F_Mage : public Fighter // “法师”类
    {
    public:
        F_Mage(int life, int magic, int attack) : Fighter(life, magic, attack) {}

    private:
        bool canUseJN() const override
        {
            return m_magic >= 100;
        }

    private:
        void effect_enemy() const override
        {
            cout << "Lose 650 HP points for each enemy" << endl;
        }
        void effect_self() override
        {
            cout << "The protagonist loses 100 MV" << endl;
            m_magic -= 100;
        }
    };
}

int main()
{
#if 0
    using namespace ns1;
    auto mroleobj = make_shared<Warrior>(1000, 0, 200);
    mroleobj->JN_Burn();
#endif

#if 1
    using namespace ns2;
    //shared_ptr<Fighter> fighter = make_shared<F_Warrior>(1000, 0, 200);
    shared_ptr<Fighter> fighter(new F_Warrior(1000, 0, 200));
    fighter->JN_Burn();
    
    cout << "-------------------------" << endl;

    //fighter = make_shared<F_Mage>(800, 200, 300);
    fighter.reset(new F_Mage(800, 200, 300));
    fighter->JN_Burn();
#endif

    cout << "Over!\n";
    return 0;
}