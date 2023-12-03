#include <iostream>
#include <memory>
using namespace std;

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
        M_Undead(int life, int magic, int attack) : Monster(life, magic, attack)
        {
            cout << "A ghost monster came to this world" << endl;
        }
    };

    class M_Element : public Monster // 元素类怪物
    {
    public:
        M_Element(int life, int magic, int attack) : Monster(life, magic, attack)
        {
            cout << "An elemental monster came to this world" << endl;
        }
    };

    class M_Mechanic : public Monster // 机械类怪物
    {
    public:
        M_Mechanic(int life, int magic, int attack) : Monster(life, magic, attack)
        {
            cout << "A mechanical monster came to this world" << endl;
        }
    };

    shared_ptr<Monster> createMonster(const string &strmontype)
    {
        if (strmontype == "udd") // udd代表要创建亡灵类怪物
            return make_shared<M_Undead>(300, 50, 80);
        else if (strmontype == "elm") // ele代表要创建元素类怪物
            return make_shared<M_Element>(200, 80, 100);
        else if (strmontype == "mec") // mec代表要创建机械类怪物
            return make_shared<M_Mechanic>(400, 0, 110);
        else
            return nullptr;
    }

    class MonsterFactory // 怪物工厂类
    {
    public:
        static shared_ptr<Monster> createMonster(const string &strmontype)
        {
            if (strmontype == "udd") // udd代表要创建亡灵类怪物
                return make_shared<M_Undead>(300, 50, 80);
            else if (strmontype == "elm") // ele代表要创建元素类怪物
                return make_shared<M_Element>(200, 80, 100);
            else if (strmontype == "mec") // mec代表要创建机械类怪物
                return make_shared<M_Mechanic>(400, 0, 110);
            else
                return nullptr;
        }
    };
}

namespace ns2
{
    class Shape
    {
    public:
        virtual ~Shape() = default;
        virtual void draw() const = 0;
    };

    class Rectangle : public Shape
    {
    public:
        void draw() const override
        {
            cout << "Inside Rectangle::draw() method." << endl;
        }
    };

    class Square : public Shape
    {
    public:
        void draw() const override
        {
            cout << "Inside Square::draw() method." << endl;
        }
    };

    class Circle : public Shape
    {
    public:
        void draw() const override
        {
            cout << "Inside Circle::draw() method." << endl;
        }
    };

    class ShapeFactory
    {
    public:
        static shared_ptr<Shape> getShape(const string &shapeType)
        {
            if (shapeType == "CIRCLE")
                return make_shared<Circle>();
            else if (shapeType == "RECTANGLE")
                return make_shared<Rectangle>();
            else if (shapeType == "SQUARE")
                return make_shared<Square>();
            else
                return nullptr;
        }
    };

}

int main()
{
#if 0
    using namespace ns1;
    shared_ptr<Monster> pM1(new M_Undead(300, 50, 80));   // 产生了一只亡灵类怪物
    shared_ptr<Monster> pM2(new M_Element(200, 80, 100)); // 产生了一只元素类怪物
    shared_ptr<Monster> pM3(new M_Mechanic(400, 0, 110)); // 产生了一只机械类怪物
#endif

#if 0
    using namespace ns1;
    shared_ptr<Monster> pM1 = createMonster("udd"); // 产生了一只亡灵类怪物，当然这里必须知道"udd"代表的是创建亡灵类怪物
    shared_ptr<Monster> pM2 = createMonster("elm"); // 产生了一只元素类怪物
    shared_ptr<Monster> pM3 = createMonster("mec"); // 产生了一只机械类怪物
#endif

#if 0
    using namespace ns1;
    shared_ptr<Monster> pM1 = MonsterFactory::createMonster("udd"); // 产生了一只亡灵类怪物，当然这里必须知道"udd"代表的是创建亡灵类怪物
    shared_ptr<Monster> pM2 = MonsterFactory::createMonster("elm"); // 产生了一只元素类怪物
    shared_ptr<Monster> pM3 = MonsterFactory::createMonster("mec"); // 产生了一只机械类怪物
#endif

#if 1
    using namespace ns2;
    shared_ptr<Shape> shape1 = ShapeFactory::getShape("CIRCLE");
    shape1->draw();

    shared_ptr<Shape> shape2 = ShapeFactory::getShape("RECTANGLE");
    shape2->draw();

    shared_ptr<Shape> shape3 = ShapeFactory::getShape("SQUARE");
    shape3->draw();
#endif

    cout << "Over!\n";
    return 0;
}