#include <iostream>
#include <memory>
using namespace std;

// 一个工厂，多个产品
// 产品类别稳定（元素、亡灵、机械类怪物固定），场景变换（沼泽、山脉、城镇等增加）

namespace ns1
{
    class Monster // 怪物父类
    {
    protected:
        // 怪物属性
        int m_life;   // 生命值
        int m_magic;  // 魔法值
        int m_attack; // 攻击力
    public:
        Monster(int life, int magic, int attack) : m_life(life), m_magic(magic), m_attack(attack) {}
        virtual ~Monster() {}
    };

    class M_Undead_Swamp : public Monster // 沼泽亡灵类怪物
    {
    public:
        M_Undead_Swamp(int life, int magic, int attack) : Monster(life, magic, attack) { cout << "A dead monster from the swamp came to this world" << endl; }
    };

    class M_Element_Swamp : public Monster // 沼泽元素类怪物
    {
    public:
        M_Element_Swamp(int life, int magic, int attack) : Monster(life, magic, attack) { cout << "A swamp elemental monster came to this world" << endl; }
    };

    class M_Mechanic_Swamp : public Monster // 沼泽机械类怪物
    {
    public:
        M_Mechanic_Swamp(int life, int magic, int attack) : Monster(life, magic, attack) { cout << "A mechanical monster of swamp came to this world" << endl; }
    };

    class M_Undead_Mountain : public Monster // 山脉亡灵类怪物
    {
    public:
        M_Undead_Mountain(int life, int magic, int attack) : Monster(life, magic, attack) { cout << "A mountain necromancer came to this world" << endl; }
    };

    class M_Element_Mountain : public Monster // 山脉元素类怪物
    {
    public:
        M_Element_Mountain(int life, int magic, int attack) : Monster(life, magic, attack) { cout << "A mountain elemental monster came to this world" << endl; }
    };

    class M_Mechanic_Mountain : public Monster // 山脉机械类怪物
    {
    public:
        M_Mechanic_Mountain(int life, int magic, int attack) : Monster(life, magic, attack) { cout << "A mechanical monster from the mountains came to this world" << endl; }
    };

    class M_Undead_Town : public Monster // 城镇亡灵类怪物
    {
    public:
        M_Undead_Town(int life, int magic, int attack) : Monster(life, magic, attack) { cout << "A town's undead monster came to this world" << endl; }
    };

    class M_Element_Town : public Monster // 城镇元素类怪物
    {
    public:
        M_Element_Town(int life, int magic, int attack) : Monster(life, magic, attack) { cout << "A town's elemental monster came to this world" << endl; }
    };

    class M_Mechanic_Town : public Monster // 城镇机械类怪物
    {
    public:
        M_Mechanic_Town(int life, int magic, int attack) : Monster(life, magic, attack) { cout << "A town's mechanical monster came to this world" << endl; }
    };

    class M_ParFactory // 所有工厂类的父类
    {
    public:
        virtual ~M_ParFactory() {}
        virtual shared_ptr<Monster> createMonster_Undead() const = 0;   // 创建亡灵类怪物
        virtual shared_ptr<Monster> createMonster_Element() const = 0;  // 创建元素类怪物
        virtual shared_ptr<Monster> createMonster_Mechanic() const = 0; // 创建机械类怪物
    };

    class M_Factory_Swamp : public M_ParFactory // 沼泽地区的工厂
    {
    public:
        shared_ptr<Monster> createMonster_Undead() const override
        {
            return make_shared<M_Undead_Swamp>(300, 50, 120); // 创建沼泽亡灵类怪物
        }
        shared_ptr<Monster> createMonster_Element() const override
        {
            return make_shared<M_Element_Swamp>(200, 80, 110); // 创建沼泽元素类怪物
        }
        shared_ptr<Monster> createMonster_Mechanic() const override
        {
            return make_shared<M_Mechanic_Swamp>(400, 0, 90); // 创建沼泽机械类怪物
        }
    };

    class M_Factory_Mountain : public M_ParFactory // 山脉地区的工厂
    {
    public:
        shared_ptr<Monster> createMonster_Undead() const override
        {
            return make_shared<M_Undead_Mountain>(300, 50, 80); // 创建山脉亡灵类怪物
        }
        shared_ptr<Monster> createMonster_Element() const override
        {
            return make_shared<M_Element_Mountain>(200, 80, 100); // 创建山脉元素类怪物
        }
        shared_ptr<Monster> createMonster_Mechanic() const override
        {
            return make_shared<M_Mechanic_Mountain>(600, 0, 110); // 创建山脉机械类怪物
        }
    };

    class M_Factory_Town : public M_ParFactory // 城镇的工厂
    {
    public:
        shared_ptr<Monster> createMonster_Undead() const override
        {
            return make_shared<M_Undead_Town>(300, 50, 80); // 创建城镇亡灵类怪物
        }
        shared_ptr<Monster> createMonster_Element() const override
        {
            return make_shared<M_Element_Town>(200, 80, 100); // 创建城镇元素类怪物
        }
        shared_ptr<Monster> createMonster_Mechanic() const override
        {
            return make_shared<M_Mechanic_Town>(400, 0, 110); // 创建城镇机械类怪物
        }
    };
}

// 类模板实现
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

    class M_Undead_Swamp : public Monster // 沼泽亡灵类怪物
    {
    public:
        M_Undead_Swamp(int life, int magic, int attack) : Monster(life, magic, attack) { cout << "A dead monster from the swamp came to this world" << endl; }
    };

    class M_Element_Swamp : public Monster // 沼泽元素类怪物
    {
    public:
        M_Element_Swamp(int life, int magic, int attack) : Monster(life, magic, attack) { cout << "A swamp elemental monster came to this world" << endl; }
    };

    class M_Mechanic_Swamp : public Monster // 沼泽机械类怪物
    {
    public:
        M_Mechanic_Swamp(int life, int magic, int attack) : Monster(life, magic, attack) { cout << "A mechanical monster of swamp came to this world" << endl; }
    };

    class M_Undead_Mountain : public Monster // 山脉亡灵类怪物
    {
    public:
        M_Undead_Mountain(int life, int magic, int attack) : Monster(life, magic, attack) { cout << "A mountain necromancer came to this world" << endl; }
    };

    class M_Element_Mountain : public Monster // 山脉元素类怪物
    {
    public:
        M_Element_Mountain(int life, int magic, int attack) : Monster(life, magic, attack) { cout << "A mountain elemental monster came to this world" << endl; }
    };

    class M_Mechanic_Mountain : public Monster // 山脉机械类怪物
    {
    public:
        M_Mechanic_Mountain(int life, int magic, int attack) : Monster(life, magic, attack) { cout << "A mechanical monster from the mountains came to this world" << endl; }
    };

    class M_Undead_Town : public Monster // 城镇亡灵类怪物
    {
    public:
        M_Undead_Town(int life = 30, int magic = 50, int attack = 80) : Monster(life, magic, attack) { cout << "A town's undead monster came to this world" << endl; }
    };

    class M_Element_Town : public Monster // 城镇元素类怪物
    {
    public:
        M_Element_Town(int life = 100, int magic = 0, int attack = 44) : Monster(life, magic, attack) { cout << "A town's elemental monster came to this world" << endl; }
    };

    class M_Mechanic_Town : public Monster // 城镇机械类怪物
    {
    public:
        M_Mechanic_Town(int life = 400, int magic = 0, int attack = 110) : Monster(life, magic, attack) { cout << "A town's mechanical monster came to this world" << endl; }
    };

    // 创建怪物工厂类模板
    template <typename T>
    class M_Factory
    {
    public:
        static shared_ptr<Monster> createMonster() { return make_shared<T>(); }
        static shared_ptr<Monster> createMonster(int life, int magic, int attack) { return make_shared<T>(life, magic, attack); }
    };

    template <typename T>
    shared_ptr<T> getMonster()
    {
        return make_shared<T>();
    }

    template <typename T>
    shared_ptr<T> getMonster(int life, int magic, int attack)
    {
        return make_shared<T>(life, magic, attack);
    }
}

namespace ns3
{
    class Body // 身体抽象类
    {
    public:
        virtual void getName() const = 0;
        virtual ~Body() {}
    };

    class Clothes // 衣服抽象类
    {
    public:
        virtual void getName() const = 0;
        virtual ~Clothes() {}
    };

    class Shoes // 鞋子抽象类
    {
    public:
        virtual void getName() const = 0;
        virtual ~Shoes() {}
    };

    class AbstractFactory // 抽象工厂类
    {
    public:                                                    // 所创建的部件应该稳定的保持这三个部件，才适合抽象工厂模式
        virtual shared_ptr<Body> createBody() const = 0;       // 创建身体
        virtual shared_ptr<Clothes> createClothes() const = 0; // 创建衣服
        virtual shared_ptr<Shoes> createShoes() const = 0;     // 创建鞋子
        virtual ~AbstractFactory() {}
    };

    class BarbieDoll // 芭比娃娃类
    {
        shared_ptr<Body> body;
        shared_ptr<Clothes> clothes;
        shared_ptr<Shoes> shoes;

    public:
        BarbieDoll(const shared_ptr<Body> &tmpbody, const shared_ptr<Clothes> &tmpclothes, const shared_ptr<Shoes> &tmpshoes) : body(tmpbody), clothes(tmpclothes), shoes(tmpshoes) {}
        void Assemble() const // 组装芭比娃娃
        {
            cout << "Successfully assembled a Barbie doll: " << endl;
            body->getName();
            clothes->getName();
            shoes->getName();
        }
    };

    // 中国厂商实现的三个部件
    class China_Body : public Body
    {
    public:
        void getName() const override { cout << "Made by Chinese manufacturers_Body parts" << endl; }
    };
    class China_Clothes : public Clothes
    {
    public:
        void getName() const override { cout << "Made by Chinese manufacturers_Clothes parts" << endl; }
    };
    class China_Shoes : public Shoes
    {
    public:
        void getName() const override { cout << "Made by Chinese manufacturers_Shoes parts" << endl; }
    };
    class ChinaFactory : public AbstractFactory // 创建一个中国工厂
    {
    public:
        shared_ptr<Body> createBody() const override { return make_shared<China_Body>(); }
        shared_ptr<Clothes> createClothes() const override { return make_shared<China_Clothes>(); }
        shared_ptr<Shoes> createShoes() const override { return make_shared<China_Shoes>(); }
    };

    // 日本厂商实现的三个部件
    class Japan_Body : public Body
    {
    public:
        void getName() const override { cout << "Made by Japanese manufacturers_Body parts" << endl; }
    };
    class Japan_Clothes : public Clothes
    {
    public:
        void getName() const override { cout << "Made by Japanese manufacturers_Clothes parts" << endl; }
    };
    class Japan_Shoes : public Shoes
    {
    public:
        void getName() const override { cout << "Made by Japanese manufacturers_Shoes parts" << endl; }
    };
    class JapanFactory : public AbstractFactory // 创建一个日本工厂
    {
    public:
        shared_ptr<Body> createBody() const override { return make_shared<Japan_Body>(); }
        shared_ptr<Clothes> createClothes() const override { return make_shared<Japan_Clothes>(); }
        shared_ptr<Shoes> createShoes() const override { return make_shared<Japan_Shoes>(); }
    };

    // 美国厂商实现的三个部件
    class America_Body : public Body
    {
    public:
        void getName() const override { cout << "Made by American manufacturers_Body parts" << endl; }
    };
    class America_Clothes : public Clothes
    {
    public:
        void getName() const override { cout << "Made by American manufacturers_Clothes parts" << endl; }
    };
    class America_Shoes : public Shoes
    {
    public:
        void getName() const override { cout << "Made by American manufacturers_Shoes parts" << endl; }
    };
    class AmericaFactory : public AbstractFactory // 创建一个美国工厂
    {
    public:
        shared_ptr<Body> createBody() const override { return make_shared<America_Body>(); }
        shared_ptr<Clothes> createClothes() const override { return make_shared<America_Clothes>(); }
        shared_ptr<Shoes> createShoes() const override { return make_shared<America_Shoes>(); }
    };
}

namespace ns4
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
        void draw() const override { cout << "Inside Rectangle::draw() method." << endl; }
    };

    class Square : public Shape
    {
    public:
        void draw() const override { cout << "Inside Square::draw() method." << endl; }
    };

    class Circle : public Shape
    {
    public:
        void draw() const override { cout << "Inside Circle::draw() method." << endl; }
    };

    class Color
    {
    public:
        virtual ~Color() = default;
        virtual void fill() const = 0;
    };

    class Red : public Color
    {
    public:
        void fill() const override { cout << "Inside Red::fill() method." << endl; }
    };

    class Green : public Color
    {
    public:
        void fill() const override { cout << "Inside Green::fill() method." << endl; }
    };

    class Blue : public Color
    {
    public:
        void fill() const override { cout << "Inside Blue::fill() method." << endl; }
    };

    class AbstractFactory
    {
    public:
        virtual ~AbstractFactory() = default;
        virtual shared_ptr<Shape> getShape(const string &shape) const = 0;
        virtual shared_ptr<Color> getColor(const string &color) const = 0;
    };

    class ShapeFactory : public AbstractFactory
    {
    public:
        shared_ptr<Shape> getShape(const string &shapeType) const override
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
        shared_ptr<Color> getColor(const string &color) const override { return nullptr; }
    };

    class ColorFactory : public AbstractFactory
    {
    public:
        shared_ptr<Shape> getShape(const string &shapeType) const override { return nullptr; }
        shared_ptr<Color> getColor(const string &color) const override
        {
            if (color == "RED")
                return make_shared<Red>();
            else if (color == "GREEN")
                return make_shared<Green>();
            else if (color == "BLUE")
                return make_shared<Blue>();
            else
                return nullptr;
        }
    };

    shared_ptr<AbstractFactory> getFactory(const string &choice)
    {
        if (choice == "SHAPE")
            return make_shared<ShapeFactory>();
        else if (choice == "COLOR")
            return make_shared<ColorFactory>();
        else
            return nullptr;
    }
}

int main()
{
#if 0
    using namespace ns1;
    shared_ptr<M_ParFactory> p_mou_fy(new M_Factory_Mountain()); // 多态工厂，山脉地区的工厂
    shared_ptr<Monster> pM1 = p_mou_fy->createMonster_Element(); // 创建山脉地区的元素类怪物

    shared_ptr<M_ParFactory> p_twn_fy(new M_Factory_Town());      // 多态工厂，城镇的工厂
    shared_ptr<Monster> pM2 = p_twn_fy->createMonster_Undead();   // 创建城镇的亡灵类怪物
    shared_ptr<Monster> pM3 = p_twn_fy->createMonster_Mechanic(); // 创建城镇的机械类怪物
#endif

#if 0
    using namespace ns2;
    shared_ptr<Monster> pM1 = M_Factory<M_Element_Town>::createMonster();             // 创建山脉地区的元素类怪物
    shared_ptr<Monster> pM2 = M_Factory<M_Undead_Town>::createMonster();              // 创建城镇的亡灵类怪物
    shared_ptr<Monster> pM3 = M_Factory<M_Mechanic_Town>::createMonster(400, 0, 110); // 创建城镇的机械类怪物
    shared_ptr<Monster> pM = getMonster<M_Mechanic_Town>(400, 0, 110);
#endif

#if 1
    using namespace ns3;
    // 创建第一个芭比娃娃------------------------------------
    //(1)创建一个中国工厂
    shared_ptr<AbstractFactory> pChinaFactory(new ChinaFactory());
    //(2)创建中国产的各种部件
    shared_ptr<Body> pChinaBody = pChinaFactory->createBody();
    shared_ptr<Clothes> pChinaClothes = pChinaFactory->createClothes();
    shared_ptr<Shoes> pChinaShoes = pChinaFactory->createShoes();
    //(3)创建芭比娃娃
    shared_ptr<BarbieDoll> pbd1obj(new BarbieDoll(pChinaBody, pChinaClothes, pChinaShoes));
    pbd1obj->Assemble(); // 组装芭比娃娃

    // 创建第二个芭比娃娃------------------------------------
    //(1)创建另外两个工厂：日本工厂，美国工厂
    shared_ptr<AbstractFactory> pJapanFactory(new JapanFactory());
    shared_ptr<AbstractFactory> pAmericaFactory(new AmericaFactory());
    //(2)创建中国产的身体部件，日本产的衣服部件，美国产的鞋子部件
    shared_ptr<Body> pChinaBody2 = pChinaFactory->createBody();
    shared_ptr<Clothes> pJapanClothes = pJapanFactory->createClothes();
    shared_ptr<Shoes> pAmericaShoes = pAmericaFactory->createShoes();
    //(3)创建芭比娃娃
    shared_ptr<BarbieDoll> pbd2obj(new BarbieDoll(pChinaBody2, pJapanClothes, pAmericaShoes));
    pbd2obj->Assemble(); // 组装芭比娃娃
#endif

#if 0
    using namespace ns4;
    shared_ptr<AbstractFactory> shapeFactory = getFactory("SHAPE");
    shared_ptr<Shape> shape1 = shapeFactory->getShape("CIRCLE");
    shape1->draw();
    shared_ptr<Shape> shape2 = shapeFactory->getShape("RECTANGLE");
    shape2->draw();
    shared_ptr<Shape> shape3 = shapeFactory->getShape("SQUARE");
    shape3->draw();

    shared_ptr<AbstractFactory> colorFactory = getFactory("COLOR");
    shared_ptr<Color> color1 = colorFactory->getColor("RED");
    color1->fill();
    shared_ptr<Color> color2 = colorFactory->getColor("GREEN");
    color2->fill();
    shared_ptr<Color> color3 = colorFactory->getColor("BLUE");
    color3->fill();
#endif

    cout << "Over!\n";
    return 0;
}
