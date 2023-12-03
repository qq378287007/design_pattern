#include <iostream>
#include <memory>
using namespace std;

// 对象克隆

namespace ns1
{
    class Monster // 怪物
    {
    protected:
        int m_life;   // 生命值
        int m_magic;  // 魔法值
        int m_attack; // 攻击力
    public:
        Monster(int life, int magic, int attack) : m_life(life), m_magic(magic), m_attack(attack) {}
        virtual ~Monster() {}
        virtual shared_ptr<Monster> clone() const = 0; // 对象克隆
    };

    class M_Undead : public Monster // 亡灵类
    {
    public:
        M_Undead(int life, int magic, int attack) : Monster(life, magic, attack)
        {
            cout << "M_Undead(int life, int magic, int attack)" << endl;
        }
        M_Undead(const M_Undead &tmpobj) : Monster(tmpobj)
        {
            cout << "M_Undead(const M_Undead &tmpobj)" << endl;
        }

    public:
        shared_ptr<Monster> clone() const override
        {
            return make_shared<M_Undead>(*this);
        }
    };

    class M_Element : public Monster // 元素类
    {
    public:
        M_Element(int life, int magic, int attack) : Monster(life, magic, attack)
        {
            cout << "M_Element(int life, int magic, int attack)" << endl;
        }
        M_Element(const M_Element &tmpobj) : Monster(tmpobj)
        {
            cout << "M_Element(const M_Element &tmpobj)" << endl;
        }

    public:
        shared_ptr<Monster> clone() const override
        {
            return make_shared<M_Element>(*this);
        }
    };

    class M_Mechanic : public Monster // 机械类
    {
    public:
        M_Mechanic(int life, int magic, int attack) : Monster(life, magic, attack)
        {
            cout << "M_Mechanic(int life, int magic, int attack)" << endl;
        }
        M_Mechanic(const M_Mechanic &tmpobj) : Monster(tmpobj)
        {
            cout << "M_Mechanic(const M_Mechanic &tmpobj)" << endl;
        }

    public:
        shared_ptr<Monster> clone() const override
        {
            return make_shared<M_Mechanic>(*this);
        }
    };

    shared_ptr<Monster> CreateMonster(const shared_ptr<Monster> &pMonster)
    {
        /*
        if (dynamic_pointer_cast<M_Undead>(pMonster) != nullptr)
            return make_shared<M_Undead>(*dynamic_pointer_cast<M_Undead>(pMonster).get()); // 亡灵类
        else if (dynamic_pointer_cast<M_Element>(pMonster) != nullptr)
            return make_shared<M_Element>(*dynamic_pointer_cast<M_Element>(pMonster).get()); // 元素类
        else if (dynamic_pointer_cast<M_Mechanic>(pMonster) != nullptr)
            return make_shared<M_Mechanic>(*dynamic_pointer_cast<M_Mechanic>(pMonster).get()); // 机械类
        return nullptr;
        */
        return pMonster->clone();
    }
}

int main()
{
#if 0
    using namespace ns1;
    shared_ptr<Monster> myPropMecMonster = make_shared<M_Mechanic>(400, 0, 110); // 创建一只机械类怪物对象作为原型对象以用于克隆目的
    shared_ptr<Monster> p_CloneObj1 = myPropMecMonster->clone();                 // 使用原型对象克隆出新的机械类怪物对象

    shared_ptr<Monster> pmyPropEleMonster = make_shared<M_Element>(200, 80, 100);
    shared_ptr<Monster> p_CloneObj2 = pmyPropEleMonster->clone();
#endif

#if 1
    using namespace ns1;
    shared_ptr<Monster> pMonsterObj = make_shared<M_Element>(200, 80, 100);
    shared_ptr<Monster> copyObj = CreateMonster(pMonsterObj);
#endif

    cout << "Over!\n";
    return 0;
}
