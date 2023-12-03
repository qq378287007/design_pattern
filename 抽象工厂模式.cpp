#include <iostream>
#include <memory>
using namespace std;

// 一个工厂，多个产品
// 产品类别稳定（元素、亡灵、机械类怪物固定），场景变换（沼泽、山脉、城镇等增加）

namespace ns1
{
    class Monster // 怪物父类
    {
    public:
        Monster(int life, int magic, int attack) : m_life(life), m_magic(magic), m_attack(attack) {}
        virtual ~Monster() {} // 做父类时析构函数应该为虚函数

    protected:
        // 怪物属性
        int m_life;   // 生命值
        int m_magic;  // 魔法值
        int m_attack; // 攻击力
    };

    class M_Undead_Swamp : public Monster // 沼泽亡灵类怪物
    {
    public:
        M_Undead_Swamp(int life, int magic, int attack) : Monster(life, magic, attack)
        {
            cout << "A dead monster from the swamp came to this world" << endl;
        }
    };

    class M_Element_Swamp : public Monster // 沼泽元素类怪物
    {
    public:
        M_Element_Swamp(int life, int magic, int attack) : Monster(life, magic, attack)
        {
            cout << "A swamp elemental monster came to this world" << endl;
        }
    };

    class M_Mechanic_Swamp : public Monster // 沼泽机械类怪物
    {
    public:
        M_Mechanic_Swamp(int life, int magic, int attack) : Monster(life, magic, attack)
        {
            cout << "A mechanical monster of swamp came to this world" << endl;
        }
    };

    class M_Undead_Mountain : public Monster // 山脉亡灵类怪物
    {
    public:
        M_Undead_Mountain(int life, int magic, int attack) : Monster(life, magic, attack)
        {
            cout << "A mountain necromancer came to this world" << endl;
        }
    };

    class M_Element_Mountain : public Monster // 山脉元素类怪物
    {
    public:
        M_Element_Mountain(int life, int magic, int attack) : Monster(life, magic, attack)
        {
            cout << "A mountain elemental monster came to this world" << endl;
        }
    };

    class M_Mechanic_Mountain : public Monster // 山脉机械类怪物
    {
    public:
        M_Mechanic_Mountain(int life, int magic, int attack) : Monster(life, magic, attack)
        {
            cout << "A mechanical monster from the mountains came to this world" << endl;
        }
    };

    class M_Undead_Town : public Monster // 城镇亡灵类怪物
    {
    public:
        M_Undead_Town(int life, int magic, int attack) : Monster(life, magic, attack)
        {
            cout << "A town's undead monster came to this world" << endl;
        }
    };

    class M_Element_Town : public Monster // 城镇元素类怪物
    {
    public:
        M_Element_Town(int life, int magic, int attack) : Monster(life, magic, attack)
        {
            cout << "A town's elemental monster came to this world" << endl;
        }
    };

    class M_Mechanic_Town : public Monster // 城镇机械类怪物
    {
    public:
        M_Mechanic_Town(int life, int magic, int attack) : Monster(life, magic, attack)
        {
            cout << "A town's mechanical monster came to this world" << endl;
        }
    };

    class M_ParFactory // 所有工厂类的父类
    {
    public:
        virtual Monster *createMonster_Undead() = 0;   // 创建亡灵类怪物
        virtual Monster *createMonster_Element() = 0;  // 创建元素类怪物
        virtual Monster *createMonster_Mechanic() = 0; // 创建机械类怪物
        virtual ~M_ParFactory() {}                     // 做父类时析构函数应该为虚函数
    };

    class M_Factory_Swamp : public M_ParFactory // 沼泽地区的工厂
    {
    public:
        virtual Monster *createMonster_Undead()
        {
            return new M_Undead_Swamp(300, 50, 120); // 创建沼泽亡灵类怪物
        }
        virtual Monster *createMonster_Element()
        {
            return new M_Element_Swamp(200, 80, 110); // 创建沼泽元素类怪物
        }
        virtual Monster *createMonster_Mechanic()
        {
            return new M_Mechanic_Swamp(400, 0, 90); // 创建沼泽机械类怪物
        }
    };

    class M_Factory_Mountain : public M_ParFactory // 山脉地区的工厂
    {
    public:
        virtual Monster *createMonster_Undead()
        {
            return new M_Undead_Mountain(300, 50, 80); // 创建山脉亡灵类怪物
        }
        virtual Monster *createMonster_Element()
        {
            return new M_Element_Mountain(200, 80, 100); // 创建山脉元素类怪物
        }
        virtual Monster *createMonster_Mechanic()
        {
            return new M_Mechanic_Mountain(600, 0, 110); // 创建山脉机械类怪物
        }
    };

    class M_Factory_Town : public M_ParFactory // 城镇的工厂
    {
    public:
        virtual Monster *createMonster_Undead()
        {
            return new M_Undead_Town(300, 50, 80); // 创建城镇亡灵类怪物
        }
        virtual Monster *createMonster_Element()
        {
            return new M_Element_Town(200, 80, 100); // 创建城镇元素类怪物
        }
        virtual Monster *createMonster_Mechanic()
        {
            return new M_Mechanic_Town(400, 0, 110); // 创建城镇机械类怪物
        }
    };
}

namespace _nmsp7
{
    // 抽象工厂模式
    class Monster // 怪物父类
    {
    public:
        Monster(int life, int magic, int attack) : m_life(life), m_magic(magic), m_attack(attack) {}
        virtual ~Monster() {} // 做父类时析构函数应该为虚函数

    protected:
        // 怪物属性
        int m_life;   // 生命值
        int m_magic;  // 魔法值
        int m_attack; // 攻击力
    };

    class M_Undead_Swamp : public Monster // 沼泽亡灵类怪物
    {
    public:
        M_Undead_Swamp(int life, int magic, int attack) : Monster(life, magic, attack)
        {
            cout << "A dead monster from the swamp came to this world" << endl;
        }
    };

    class M_Element_Swamp : public Monster // 沼泽元素类怪物
    {
    public:
        M_Element_Swamp(int life, int magic, int attack) : Monster(life, magic, attack)
        {
            cout << "A swamp elemental monster came to this world" << endl;
        }
    };

    class M_Mechanic_Swamp : public Monster // 沼泽机械类怪物
    {
    public:
        M_Mechanic_Swamp(int life, int magic, int attack) : Monster(life, magic, attack)
        {
            cout << "A mechanical monster of swamp came to this world" << endl;
        }
    };

    class M_Undead_Mountain : public Monster // 山脉亡灵类怪物
    {
    public:
        M_Undead_Mountain(int life, int magic, int attack) : Monster(life, magic, attack)
        {
            cout << "A mountain necromancer came to this world" << endl;
        }
    };

    class M_Element_Mountain : public Monster // 山脉元素类怪物
    {
    public:
        M_Element_Mountain(int life, int magic, int attack) : Monster(life, magic, attack)
        {
            cout << "A mountain elemental monster came to this world" << endl;
        }
    };

    class M_Mechanic_Mountain : public Monster // 山脉机械类怪物
    {
    public:
        M_Mechanic_Mountain(int life, int magic, int attack) : Monster(life, magic, attack)
        {
            cout << "A mechanical monster from the mountains came to this world" << endl;
        }
    };

    class M_Undead_Town : public Monster // 城镇亡灵类怪物
    {
    public:
        M_Undead_Town(int life = 30, int magic = 50, int attack = 80) : Monster(life, magic, attack)
        {
            cout << "A town's undead monster came to this world" << endl;
        }
    };

    class M_Element_Town : public Monster // 城镇元素类怪物
    {
    public:
        M_Element_Town(int life = 100, int magic = 0, int attack = 44) : Monster(life, magic, attack)
        {
            cout << "A town's elemental monster came to this world" << endl;
        }
    };

    class M_Mechanic_Town : public Monster // 城镇机械类怪物
    {
    public:
        M_Mechanic_Town(int life = 400, int magic = 0, int attack = 110) : Monster(life, magic, attack)
        {
            cout << "A town's mechanical monster came to this world" << endl;
        }
    };

    // 创建怪物工厂类模板
    template <typename T>
    class M_Factory
    {
    public:
        static Monster *createMonster()
        {
            return new T();
        }
        static Monster *createMonster(int life, int magic, int attack)
        {
            return new T(life, magic, attack);
        }
    };
}

namespace _nmsp8
{
    // 抽象工厂模式
    class Body // 身体抽象类
    {
    public:
        virtual void getName() = 0;
        virtual ~Body() {}
    };
    // 衣服抽象类
    class Clothes
    {
    public:
        virtual void getName() = 0;
        virtual ~Clothes() {}
    };
    // 鞋子抽象类
    class Shoes
    {
    public:
        virtual void getName() = 0;
        virtual ~Shoes() {}
    };

    // 抽象工厂类
    class AbstractFactory
    {
    public:
        // 所创建的部件应该稳定的保持这三个部件，才适合抽象工厂模式
        virtual Body *createBody() = 0;       // 创建身体
        virtual Clothes *createClothes() = 0; // 创建衣服
        virtual Shoes *createShoes() = 0;     // 创建鞋子
        virtual ~AbstractFactory() {}
    };

    // 芭比娃娃类
    class BarbieDoll
    {
    public:
        // 构造函数
        BarbieDoll(Body *tmpbody, Clothes *tmpclothes, Shoes *tmpshoes)
        {
            body = tmpbody;
            clothes = tmpclothes;
            shoes = tmpshoes;
        }
        void Assemble() // 组装芭比娃娃
        {
            cout << "Successfully assembled a Barbie doll: " << endl;
            body->getName();
            clothes->getName();
            shoes->getName();
        }

    private:
        Body *body;
        Clothes *clothes;
        Shoes *shoes;
    };

    // 中国厂商实现的三个部件
    class China_Body : public Body
    {
    public:
        virtual void getName()
        {
            cout << "Made by Chinese manufacturers_Body parts" << endl;
        }
    };
    class China_Clothes : public Clothes
    {
    public:
        virtual void getName()
        {
            cout << "Made by Chinese manufacturers_Clothes parts" << endl;
        }
    };
    class China_Shoes : public Shoes
    {
    public:
        virtual void getName()
        {
            cout << "Made by Chinese manufacturers_Shoes parts" << endl;
        }
    };
    // 创建一个中国工厂
    class ChinaFactory : public AbstractFactory
    {
    public:
        virtual Body *createBody()
        {
            return new China_Body;
        }
        virtual Clothes *createClothes()
        {
            return new China_Clothes;
        }
        virtual Shoes *createShoes()
        {
            return new China_Shoes;
        }
    };

    // 日本厂商实现的三个部件
    class Japan_Body : public Body
    {
    public:
        virtual void getName()
        {
            cout << "Made by Japanese manufacturers_Body parts" << endl;
        }
    };
    class Japan_Clothes : public Clothes
    {
    public:
        virtual void getName()
        {
            cout << "Made by Japanese manufacturers_Clothes parts" << endl;
        }
    };
    class Japan_Shoes : public Shoes
    {
    public:
        virtual void getName()
        {
            cout << "Made by Japanese manufacturers_Shoes parts" << endl;
        }
    };
    // 创建一个日本工厂
    class JapanFactory : public AbstractFactory
    {
    public:
        virtual Body *createBody()
        {
            return new Japan_Body;
        }
        virtual Clothes *createClothes()
        {
            return new Japan_Clothes;
        }
        virtual Shoes *createShoes()
        {
            return new Japan_Shoes;
        }
    };

    // 美国厂商实现的三个部件
    class America_Body : public Body
    {
    public:
        virtual void getName()
        {
            cout << "Made by American manufacturers_Body parts" << endl;
        }
    };
    class America_Clothes : public Clothes
    {
    public:
        virtual void getName()
        {
            cout << "Made by American manufacturers_Clothes parts" << endl;
        }
    };
    class America_Shoes : public Shoes
    {
    public:
        virtual void getName()
        {
            cout << "Made by American manufacturers_Shoes parts" << endl;
        }
    };
    // 创建一个美国工厂
    class AmericaFactory : public AbstractFactory
    {
    public:
        virtual Body *createBody()
        {
            return new America_Body;
        }
        virtual Clothes *createClothes()
        {
            return new America_Clothes;
        }
        virtual Shoes *createShoes()
        {
            return new America_Shoes;
        }
    };
}

int main()
{
#if 1
    using namespace ns1;
    M_ParFactory *p_mou_fy = new M_Factory_Mountain(); // 多态工厂，山脉地区的工厂
    Monster *pM1 = p_mou_fy->createMonster_Element();  // 创建山脉地区的元素类怪物

    M_ParFactory *p_twn_fy = new M_Factory_Town();     // 多态工厂，城镇的工厂
    Monster *pM2 = p_twn_fy->createMonster_Undead();   // 创建城镇的亡灵类怪物
    Monster *pM3 = p_twn_fy->createMonster_Mechanic(); // 创建城镇的机械类怪物
#endif

    // 抽象工厂，类模板实现
#if 0
        _nmsp7::Monster *pM1 = _nmsp7::M_Factory<_nmsp7::M_Element_Town>::createMonster();             // 创建山脉地区的元素类怪物
        _nmsp7::Monster *pM2 = _nmsp7::M_Factory<_nmsp7::M_Undead_Town>::createMonster();              // 创建城镇的亡灵类怪物
        _nmsp7::Monster *pM3 = _nmsp7::M_Factory<_nmsp7::M_Mechanic_Town>::createMonster(400, 0, 110); // 创建城镇的机械类怪物

        // 释放怪物
        delete pM1;
        delete pM2;
        delete pM3;
#endif

    // 抽象工厂模式2
#if 0
        // 创建第一个芭比娃娃------------------------------------
        //(1)创建一个中国工厂
        _nmsp8::AbstractFactory *pChinaFactory = new _nmsp8::ChinaFactory();
        //(2)创建中国产的各种部件
        _nmsp8::Body *pChinaBody = pChinaFactory->createBody();
        _nmsp8::Clothes *pChinaClothes = pChinaFactory->createClothes();
        _nmsp8::Shoes *pChinaShoes = pChinaFactory->createShoes();
        //(3)创建芭比娃娃
        _nmsp8::BarbieDoll *pbd1obj = new _nmsp8::BarbieDoll(pChinaBody, pChinaClothes, pChinaShoes);
        pbd1obj->Assemble(); // 组装芭比娃娃

        // 创建第二个芭比娃娃------------------------------------
        //(1)创建另外两个工厂：日本工厂，美国工厂
        _nmsp8::AbstractFactory *pJapanFactory = new _nmsp8::JapanFactory();
        _nmsp8::AbstractFactory *pAmericaFactory = new _nmsp8::AmericaFactory();
        //(2)创建中国产的身体部件，日本产的衣服部件，美国产的鞋子部件
        _nmsp8::Body *pChinaBody2 = pChinaFactory->createBody();
        _nmsp8::Clothes *pJapanClothes = pJapanFactory->createClothes();
        _nmsp8::Shoes *pAmericaShoes = pAmericaFactory->createShoes();
        //(3)创建芭比娃娃
        _nmsp8::BarbieDoll *pbd2obj = new _nmsp8::BarbieDoll(pChinaBody2, pJapanClothes, pAmericaShoes);
        pbd2obj->Assemble(); // 组装芭比娃娃

        delete pbd1obj;
        delete pChinaShoes;
        delete pChinaClothes;
        delete pChinaBody;
        delete pChinaFactory;
        //-----------
        delete pbd2obj;
        delete pAmericaShoes;
        delete pJapanClothes;
        delete pChinaBody2;
        delete pAmericaFactory;
        delete pJapanFactory;
#endif

    cout << "Over!\n";
    return 0;
}
