#include <iostream>
#include <memory>
using namespace std;

// 开闭原则（Open Close Principle，OCP）
// 对扩展开放，对修改关闭（封闭）
// 增加新功能时，不修改已有代码，而应该扩展代码（增加新类、新成员函数）。

// 一个工厂，一个产品
// 增加新子类时，添加子类工厂，已有代码不改变

namespace ns1
{
    class Monster // 怪物父类
    {
    protected:        // 怪物属性
        int m_life;   // 生命值
        int m_magic;  // 魔法值
        int m_attack; // 攻击力

    public:
        Monster(int life, int magic, int attack) : m_life(life), m_magic(magic), m_attack(attack) {}
        virtual ~Monster() {}
    };

    class M_Undead : public Monster // 亡灵类怪物
    {
    public:
        M_Undead(int life, int magic, int attack) : Monster(life, magic, attack) { cout << "A ghost monster came to this world" << endl; }
    };

    class M_Element : public Monster // 元素类怪物
    {
    public:
        M_Element(int life, int magic, int attack) : Monster(life, magic, attack) { cout << "An elemental monster came to this world" << endl; }
    };

    class M_Mechanic : public Monster // 机械类怪物
    {
    public:
        M_Mechanic(int life, int magic, int attack) : Monster(life, magic, attack) { cout << "A mechanical monster came to this world" << endl; }
    };

    class M_ParFactory // 所有工厂类的父类
    {
    public:
        virtual ~M_ParFactory() {}
        virtual shared_ptr<Monster> createMonster() const = 0;
    };

    class M_UndeadFactory : public M_ParFactory // M_Undead怪物类型的工厂，生产M_Undead类型怪物
    {
    public:
        shared_ptr<Monster> createMonster() const override { return make_shared<M_Undead>(300, 50, 80); } // 创建亡灵类怪物
    };

    class M_ElementFactory : public M_ParFactory // M_Element怪物类型的工厂，生产M_Element类型怪物
    {
    public:
        shared_ptr<Monster> createMonster() const override { return make_shared<M_Element>(200, 80, 100); } // 创建元素类怪物
    };

    class M_MechanicFactory : public M_ParFactory // M_Mechanic怪物类型的工厂，生产M_Mechanic类型怪物
    {
    public:
        shared_ptr<Monster> createMonster() const override { return make_shared<M_Mechanic>(400, 0, 110); } // 创建机械类怪物
    };
}

namespace ns2
{
    class Monster // 怪物父类
    {
    protected:        // 怪物属性
        int m_life;   // 生命值
        int m_magic;  // 魔法值
        int m_attack; // 攻击力

    public:
        Monster(int life, int magic, int attack) : m_life(life), m_magic(magic), m_attack(attack) {}
        virtual ~Monster() {}
    };

    class M_Undead : public Monster // 亡灵类怪物
    {
    public:
        M_Undead(int life, int magic, int attack) : Monster(life, magic, attack) { cout << "A ghost monster came to this world" << endl; }
    };

    class M_Element : public Monster // 元素类怪物
    {
    public:
        M_Element(int life, int magic, int attack) : Monster(life, magic, attack) { cout << "An elemental monster came to this world" << endl; }
    };

    class M_Mechanic : public Monster // 机械类怪物
    {
    public:
        M_Mechanic(int life, int magic, int attack) : Monster(life, magic, attack) { cout << "A mechanical monster came to this world" << endl; }
    };

    class M_ParFactory // 所有工厂类的父类
    {
    public:
        virtual ~M_ParFactory() {}
        virtual shared_ptr<Monster> createMonster() const = 0;
    };

    class M_UndeadFactory : public M_ParFactory // M_Undead怪物类型的工厂，生产M_Undead类型怪物
    {
    public:
        shared_ptr<Monster> createMonster() const override { return make_shared<M_Undead>(300, 50, 80); } // 创建亡灵类怪物
    };

    class M_ElementFactory : public M_ParFactory // M_Element怪物类型的工厂，生产M_Element类型怪物
    {
    public:
        shared_ptr<Monster> createMonster() const override { return make_shared<M_Element>(200, 80, 100); } // 创建元素类怪物
    };

    class M_MechanicFactory : public M_ParFactory // M_Mechanic怪物类型的工厂，生产M_Mechanic类型怪物
    {
    public:
        shared_ptr<Monster> createMonster() const override { return make_shared<M_Mechanic>(400, 0, 110); } // 创建机械类怪物
    };

    // 模板实现
    template <typename T>
    class M_ChildFactory1 : public M_ParFactory
    {
    public:
        shared_ptr<Monster> createMonster() const { return make_shared<T>(300, 50, 80); }
    };

    template <typename T>
    class M_ChildFactory2
    {
    public:
        static shared_ptr<Monster> createMonster(int life, int magic, int attack) { return make_shared<T>(life, magic, attack); }
    };

    class M_ChildFactory3
    {
    public:
        template <typename T>
        static shared_ptr<Monster> createMonster(int life, int magic, int attack) { return make_shared<T>(life, magic, attack); }
    };

    template <typename T>
    shared_ptr<T> getMonster(int life, int magic, int attack) { return make_shared<T>(life, magic, attack); }
}

namespace ns3
{
    class Monster // 怪物父类
    {
    protected:        // 怪物属性
        int m_life;   // 生命值
        int m_magic;  // 魔法值
        int m_attack; // 攻击力

    public:
        Monster(int life, int magic, int attack) : m_life(life), m_magic(magic), m_attack(attack) {}
        virtual ~Monster() {}
    };

    class M_Undead : public Monster // 亡灵类怪物
    {
    public:
        M_Undead(int life, int magic, int attack) : Monster(life, magic, attack) { cout << "A ghost monster came to this world" << endl; }
    };

    class M_Element : public Monster // 元素类怪物
    {
    public:
        M_Element(int life, int magic, int attack) : Monster(life, magic, attack) { cout << "An elemental monster came to this world" << endl; }
    };

    class M_Mechanic : public Monster // 机械类怪物
    {
    public:
        M_Mechanic(int life, int magic, int attack) : Monster(life, magic, attack) { cout << "A mechanical monster came to this world" << endl; }
    };

    class M_ParFactory // 所有工厂类的父类
    {
    public:
        virtual ~M_ParFactory() {}
        virtual shared_ptr<Monster> createMonster() const = 0;
    };

    class M_UndeadFactory : public M_ParFactory // M_Undead怪物类型的工厂，生产M_Undead类型怪物
    {
    public:
        shared_ptr<Monster> createMonster() const override { return make_shared<M_Undead>(300, 50, 80); } // 创建亡灵类怪物
    };

    class M_ElementFactory : public M_ParFactory // M_Element怪物类型的工厂，生产M_Element类型怪物
    {
    public:
        shared_ptr<Monster> createMonster() const override { return make_shared<M_Element>(200, 80, 100); } // 创建元素类怪物
    };

    class M_MechanicFactory : public M_ParFactory // M_Mechanic怪物类型的工厂，生产M_Mechanic类型怪物
    {
    public:
        shared_ptr<Monster> createMonster() const override { return make_shared<M_Mechanic>(400, 0, 110); } // 创建机械类怪物
    };

    // 全局函数实现
    shared_ptr<Monster> Gbl_CreateMonster(shared_ptr<M_ParFactory> &factory)
    {
        return factory->createMonster(); // createMonster虚函数扮演了多态new的行为，factory指向的具体怪物工厂类不同，创建的怪物对象也不同
    }
}

int main()
{
#if 0
    using namespace ns1;
    shared_ptr<M_ParFactory> p_ud_fy(new M_UndeadFactory()); // 多态工厂，注意指针类型
    shared_ptr<Monster> pM1 = p_ud_fy->createMonster();

    shared_ptr<M_ParFactory> p_elm_fy(new M_ElementFactory());
    shared_ptr<Monster> pM2 = p_elm_fy->createMonster(); // 产生了一只元素类怪物

    shared_ptr<M_ParFactory> p_mec_fy(new M_MechanicFactory());
    shared_ptr<Monster> pM3 = p_mec_fy->createMonster(); // 产生了一只机械类怪物
#endif

#if 0
    using namespace ns2;
    M_ChildFactory1<M_Undead> myFactory;
    shared_ptr<Monster> pM1 = myFactory.createMonster();
    shared_ptr<Monster> pM2 = M_ChildFactory2<M_Element>::createMonster(400, 0, 100);
    shared_ptr<Monster> pM3 = M_ChildFactory3::createMonster<M_Mechanic>(400, 0, 100);
    shared_ptr<Monster> pM = getMonster<M_Mechanic>(400, 0, 100);
#endif

#if 1
    using namespace ns3;
    shared_ptr<M_ParFactory> p_ud_fy(new M_UndeadFactory()); // 多态工厂，注意指针类型
    shared_ptr<Monster> pM1 = Gbl_CreateMonster(p_ud_fy);    // 产生了一只亡灵类怪物，也是多态，注意返回类型

    shared_ptr<M_ParFactory> p_elm_fy(new M_ElementFactory());
    shared_ptr<Monster> pM2 = Gbl_CreateMonster(p_elm_fy); // 产生了一只元素类怪物

    shared_ptr<M_ParFactory> p_mec_fy(new M_MechanicFactory());
    shared_ptr<Monster> pM3 = Gbl_CreateMonster(p_mec_fy); // 产生了一只机械类怪物
#endif

    cout << "Over!\n";
    return 0;
}
