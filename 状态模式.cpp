#include <iostream>
#include <memory>
using namespace std;

// 用类表示状态, 一般用于有限状态机
// 状态机3部分组成：状态（State）、事件（Event）、动作（Action）
// 事件（转移条件, 触发状态变化的行为或动作）发生时, 状态改变, 可能执行动作

// 允许一个对象在其内部状态改变时改变它的行为，对象看起来似乎修改了它的类
// 3种角色
// Context（环境/上下文类），该类对象拥有多种状态以便对这些状态进行维护
// State（抽象状态类），定义接口以封装与环境类的特征状态相关的行为，声明不同状态对应的方法
// ConcreteState（具体状态类），实现与环境类该状态相关的行为

namespace ns1
{
    class Monster // 怪物类
    {
    public:
        enum MonsterState // 怪物状态枚举值
        {
            MonS_Fer,  // 凶悍
            MonS_Worr, // 不安
            MonS_Fear, // 恐惧
            MonS_Dead  // 死亡
        };

    private:
        int m_life;            // 血量（生命值）
        MonsterState m_status; // 初始状态

    public:
        Monster(int life, MonsterState status = MonS_Fer) : m_life(life), m_status(status) {}

    public:
        void Attacked(int power) // 怪物被攻击。参数power表示主角对怪物的攻击力（即怪物丢失的血量）
        {
            m_life -= power; // 怪物剩余的血量
            switch (m_status)
            {
            case MonS_Fer:
                if (m_life > 400)
                {
                    cout << "monster " << power << " hurt, attact!" << endl;
                    // 处理其他动作逻辑比如反击
                }
                else if (m_life > 100)
                {
                    cout << "monster " << power << " hurt, attact, help!" << endl;
                    m_status = MonS_Worr;
                    // 处理其他动作逻辑比如反击和呼唤支援
                }
                else if (m_life > 0)
                {
                    cout << "monster " << power << " hurt, MonS_Fear, run!" << endl;
                    m_status = MonS_Fear;
                    // 处理其他动作逻辑比如逃跑
                }
                else
                {
                    cout << "monster " << power << " hurt, dead!" << endl;
                    m_status = MonS_Dead;
                    // 处理怪物死亡后事宜比如怪物尸体定时消失等
                }
                break;
            case MonS_Worr: // 目前怪物已经处于不安状态, 这说明怪物的血量 <=400 并且 >100
                if (m_life > 100)
                {
                    cout << "monster " << power << " hurt, attact, help!" << endl;
                    // 处理其他动作逻辑比如反击和呼唤支援
                }
                else if (m_life > 0)
                {
                    cout << "monster " << power << " hurt, MonS_Fear, run!" << endl;
                    m_status = MonS_Fear;
                    // 处理其他动作逻辑比如逃跑
                }
                else
                {
                    cout << "monster " << power << " hurt, dead!" << endl;
                    m_status = MonS_Dead;
                    // 处理怪物死亡后事宜比如怪物尸体定时消失等
                }
                break;
            case MonS_Fear:
                if (m_life > 0)
                {
                    cout << "monster " << power << " hurt, run!" << endl;
                    // 处理其他动作逻辑比如逃跑
                }
                else
                {
                    cout << "monster " << power << " hurt, dead!" << endl;
                    m_status = MonS_Dead;
                    // 处理怪物死亡后事宜比如怪物尸体定时消失等
                }
                break;
            default: // 怪物已经处于死亡状态
                cout << "monster dead, cannot be attacketed!" << endl;
                break;
            }
        }
    };
}

namespace ns2
{
    class Monster;      // 类前向声明
    class MonsterStatus // 怪物状态类
    {
    public:
        virtual ~MonsterStatus() {}
        virtual void Attacked(int power, Monster *mainobj) = 0;
    };

    class MonsterStatus_Feroc : public MonsterStatus // 凶悍状态类
    {
    public:
        // 传递进来的参数是否有必要使用, 开发者自行斟酌
        void Attacked(int power, Monster *mainobj) override
        {
            cout << "MonsterStatus_Feroc, attackt!" << endl;
            // 处理其他动作逻辑比如反击
        }
    };

    class MonsterStatus_Worr : public MonsterStatus // 不安状态类
    {
    public:
        void Attacked(int power, Monster *mainobj) override
        {
            cout << "MonsterStatus_Worr, attackt, help!" << endl;
            // 处理其他动作逻辑比如反击和不停的呼唤支援
        }
    };

    class MonsterStatus_Fear : public MonsterStatus // 恐惧状态类
    {
    public:
        void Attacked(int power, Monster *mainobj) override
        {
            cout << "MonsterStatus_Fear, run!" << endl;
            // 处理其他动作逻辑比如逃跑
        }
    };

    class MonsterStatus_Dead : public MonsterStatus // 死亡状态类
    {
    public:
        void Attacked(int power, Monster *mainobj) override
        {
            cout << "dead!" << endl;
            // 处理怪物死亡后事宜比如怪物尸体定时消失等
        }
    };

    class Monster // 怪物类
    {
        int m_life;                         // 血量（生命值）
        shared_ptr<MonsterStatus> m_pState; // 持有状态对象
    public:
        Monster(int life, const shared_ptr<MonsterStatus> &pState = make_shared<MonsterStatus_Feroc>()) : m_life(life), m_pState(pState) {}

    public:
        void Attacked(int power)
        {
            int orglife = m_life; // 暂存原来的怪物血量值用于后续比较
            m_life -= power;      // 怪物剩余的血量
            if (orglife > 400)    // 怪物原来处于凶悍状态
            {
                if (m_life > 400) // 状态未变
                {
                    // cout << "monster " << power << "点伤害并对主角进行疯狂的反击!" << endl;
                    m_pState->Attacked(power, this); // 其他的逻辑代码被本Monster类委托给了具体状态类来处理
                }
                else if (m_life > 100) // 状态从凶悍改变到不安
                {
                    // cout << "monster " << power << "点伤害并对主角进行反击, 怪物变得焦躁不安并开始呼唤支援!" << endl;
                    m_pState.reset(new MonsterStatus_Worr()); // 怪物转到不安状态
                    m_pState->Attacked(power, this);
                }
                else if (m_life > 0) // 状态从凶悍状态改变到恐惧状态, 主角的攻击太恐怖了
                {
                    // cout << "monster " << power << "点伤害, 怪物变得恐惧并开始逃跑!" << endl;
                    m_pState.reset(new MonsterStatus_Fear()); // 怪物转到恐惧状态
                    m_pState->Attacked(power, this);
                }
                else // 状态从凶悍改变到死亡
                {
                    // cout << "monster " << power << "点伤害, 已经死亡!" << endl;
                    m_pState.reset(new MonsterStatus_Dead()); // 怪物转到死亡状态
                    m_pState->Attacked(power, this);
                }
            }
            else if (orglife > 100) // 怪物原来处于不安状态
            {
                if (m_life > 100) // 状态未变
                {
                    // cout << "monster " << power << "点伤害并对主角进行反击, 并继续急促的呼唤支援!" << endl;
                    m_pState->Attacked(power, this);
                }
                else if (m_life > 0) // 状态从不安改变到恐惧
                {
                    // cout << "monster " << power << "点伤害, 怪物变得恐惧并开始逃跑!" << endl;
                    m_pState.reset(new MonsterStatus_Fear()); // 怪物转到恐惧状态
                    m_pState->Attacked(power, this);
                }
                else // 状态从不安改变到死亡
                {
                    // cout << "monster " << power << "点伤害, 已经死亡!" << endl;
                    m_pState.reset(new MonsterStatus_Dead()); // 怪物转到死亡状态
                    m_pState->Attacked(power, this);
                }
            }
            else if (orglife > 0) // 怪物原来处于恐惧状态
            {
                if (m_life > 0) // 状态未变
                {
                    // cout << "monster " << power << "点伤害, 怪物继续逃跑!" << endl;
                    m_pState->Attacked(power, this);
                }
                else // 状态从恐惧改变到死亡
                {
                    // cout << "monster " << power << "点伤害, 已经死亡!" << endl;
                    m_pState.reset(new MonsterStatus_Dead()); // 怪物转到死亡状态
                    m_pState->Attacked(power, this);
                }
            }
            else // 怪物已经死亡
            {
                // 已经死亡的怪物, 状态不会继续发生改变
                //  cout << "怪物已死亡, 不能再被攻击!" << endl;
                m_pState->Attacked(power, this);
            }
        }
    };
}

namespace ns3
{
    class Monster;      // 类前向声明
    class MonsterStatus // 怪物状态类
    {
    public:
        virtual ~MonsterStatus() {}
        virtual void Attacked(int power, Monster *mainobj) = 0;
    };

    class MonsterStatus_Feroc : public MonsterStatus
    {
    public:
        void Attacked(int power, Monster *mainobj) override;
    };

    class MonsterStatus_Worr : public MonsterStatus
    {
    public:
        void Attacked(int power, Monster *mainobj) override;
    };

    class MonsterStatus_Fear : public MonsterStatus
    {
    public:
        void Attacked(int power, Monster *mainobj) override;
    };

    class MonsterStatus_Dead : public MonsterStatus
    {
    public:
        void Attacked(int power, Monster *mainobj) override;
    };

    class Monster // 怪物类定义
    {
        int m_life;                         // 血量（生命值）
        shared_ptr<MonsterStatus> m_pState; // 持有状态对象

    public:
        Monster(int life, const shared_ptr<MonsterStatus> &pState = make_shared<MonsterStatus_Feroc>()) : m_life(life), m_pState(pState) {}

    public:
        void Attacked(int power) { m_pState->Attacked(power, this); } // 怪物被攻击

    public:
        int GetLife() const { return m_life; }                                               // 获取怪物血量
        void SetLife(int life) { m_life = life; }                                            // 设置怪物血量
        shared_ptr<MonsterStatus> getCurrentState() const { return m_pState; }               // 获取怪物当前状态
        void setCurrentState(const shared_ptr<MonsterStatus> &pState) { m_pState = pState; } // 设置怪物当前状态
    };

    void MonsterStatus_Feroc::Attacked(int power, Monster *mainobj) // 凶悍状态类
    {
        int orglife = mainobj->GetLife(); // 暂存原来的怪物血量值用于后续比较
        if ((orglife - power) > 400)      // 怪物原来处于凶悍状态, 现在依旧处于凶悍状态
        {
            // 状态未变
            mainobj->SetLife(orglife - power); // 怪物剩余的血量
            cout << "MonsterStatus_Feroc, attackt!" << endl;
            // 处理其他动作逻辑比如反击
        }
        else
        {
            // 不管下个状态是啥, 总之不会是凶悍状态, 只可能是不安、恐惧、死亡状态之一, 先无条件转到不安状态去（不安状态中会进行再次判断）
            mainobj->setCurrentState(make_shared<MonsterStatus_Worr>());
            mainobj->getCurrentState()->Attacked(power, mainobj);
        }
    }

    void MonsterStatus_Worr::Attacked(int power, Monster *mainobj) // 不安状态类
    {
        int orglife = mainobj->GetLife();
        if ((orglife - power) > 100) // 怪物原来处于不安状态, 现在依旧处于不安状态
        {
            // 状态未变
            mainobj->SetLife(orglife - power); // 怪物剩余的血量
            cout << "MonsterStatus_Worr, attackt, help!" << endl;
            // 处理其他动作逻辑比如反击和不停的呼唤支援
        }
        else
        {
            // 不管下个状态是啥, 总之不会是凶悍和不安状态, 只可能是恐惧、死亡状态之一, 先无条件转到恐惧状态去
            mainobj->setCurrentState(make_shared<MonsterStatus_Fear>());
            mainobj->getCurrentState()->Attacked(power, mainobj);
        }
    }

    void MonsterStatus_Fear::Attacked(int power, Monster *mainobj) // 恐惧状态类
    {
        int orglife = mainobj->GetLife();
        if ((orglife - power) > 0) // 怪物原来处于恐惧状态, 现在依旧处于恐惧状态
        {
            // 状态未变
            mainobj->SetLife(orglife - power); // 怪物剩余的血量
            cout << "MonsterStatus_Fear, run!" << endl;
            // 处理其他动作逻辑比如逃跑
        }
        else
        {
            // 不管下个状态是啥, 总之不会是凶悍、不安和恐惧状态, 只可能是死亡状态
            mainobj->setCurrentState(make_shared<MonsterStatus_Dead>());
            mainobj->getCurrentState()->Attacked(power, mainobj);
        }
    }

    void MonsterStatus_Dead::Attacked(int power, Monster *mainobj) // 死亡状态类
    {
        int orglife = mainobj->GetLife();
        if (orglife > 0)
        {
            // 还要把怪物生命值减掉
            mainobj->SetLife(orglife - power); // 怪物剩余的血量
            // 处理怪物死亡后事宜比如怪物尸体定时消失等
        }
        cout << "dead!" << endl;
    }
}

namespace ns4
{
    class Monster;      // 类前向声明
    class MonsterStatus // 怪物状态类
    {
    public:
        virtual ~MonsterStatus() {}
        virtual void Attacked(int power, Monster *mainobj) = 0;
    };

    class MonsterStatus_Feroc : public MonsterStatus
    {
    public:
        void Attacked(int power, Monster *mainobj) override;

    public:
        static shared_ptr<MonsterStatus> getInstance()
        {
            static shared_ptr<MonsterStatus> instance(new MonsterStatus_Feroc());
            return instance;
        }
    };

    class MonsterStatus_Worr : public MonsterStatus
    {
    public:
        void Attacked(int power, Monster *mainobj) override;

    public:
        static shared_ptr<MonsterStatus> getInstance()
        {
            static shared_ptr<MonsterStatus> instance(new MonsterStatus_Worr());
            return instance;
        }
    };

    class MonsterStatus_Fear : public MonsterStatus
    {
    public:
        void Attacked(int power, Monster *mainobj) override;

    public:
        static shared_ptr<MonsterStatus> getInstance()
        {
            static shared_ptr<MonsterStatus> instance(new MonsterStatus_Fear());
            return instance;
        }
    };

    class MonsterStatus_Dead : public MonsterStatus
    {
    public:
        void Attacked(int power, Monster *mainobj) override;

    public:
        static shared_ptr<MonsterStatus> getInstance()
        {
            static shared_ptr<MonsterStatus> instance(new MonsterStatus_Dead());
            return instance;
        }
    };

    class Monster // 怪物类定义
    {
        int m_life;                         // 血量（生命值）
        shared_ptr<MonsterStatus> m_pState; // 持有状态对象

    public:
        Monster(int life, const shared_ptr<MonsterStatus> &pState = MonsterStatus_Feroc::getInstance()) : m_life(life), m_pState(pState) {}

    public:
        void Attacked(int power) { m_pState->Attacked(power, this); } // 怪物被攻击

    public:
        int GetLife() const { return m_life; }                                               // 获取怪物血量
        void SetLife(int life) { m_life = life; }                                            // 设置怪物血量
        shared_ptr<MonsterStatus> getCurrentState() const { return m_pState; }               // 获取怪物当前状态
        void setCurrentState(const shared_ptr<MonsterStatus> &pState) { m_pState = pState; } // 设置怪物当前状态
    };

    void MonsterStatus_Feroc::Attacked(int power, Monster *mainobj) // 凶悍状态类
    {
        int orglife = mainobj->GetLife(); // 暂存原来的怪物血量值用于后续比较
        if ((orglife - power) > 400)      // 怪物原来处于凶悍状态, 现在依旧处于凶悍状态
        {
            // 状态未变
            mainobj->SetLife(orglife - power); // 怪物剩余的血量
            cout << "MonsterStatus_Feroc, attackt!" << endl;
            // 处理其他动作逻辑比如反击
        }
        else
        {
            // 不管下个状态是啥, 总之不会是凶悍状态, 只可能是不安、恐惧、死亡状态之一, 先无条件转到不安状态去（不安状态中会进行再次判断）
            mainobj->setCurrentState(MonsterStatus_Worr::getInstance());
            mainobj->getCurrentState()->Attacked(power, mainobj);
        }
    }

    void MonsterStatus_Worr::Attacked(int power, Monster *mainobj) // 不安状态类
    {
        int orglife = mainobj->GetLife();
        if ((orglife - power) > 100) // 怪物原来处于不安状态, 现在依旧处于不安状态
        {
            // 状态未变
            mainobj->SetLife(orglife - power); // 怪物剩余的血量
            cout << "MonsterStatus_Worr, attackt, help!" << endl;
            // 处理其他动作逻辑比如反击和不停的呼唤支援
        }
        else
        {
            // 不管下个状态是啥, 总之不会是凶悍和不安状态, 只可能是恐惧、死亡状态之一, 先无条件转到恐惧状态去
            mainobj->setCurrentState(MonsterStatus_Fear::getInstance());
            mainobj->getCurrentState()->Attacked(power, mainobj);
        }
    }

    void MonsterStatus_Fear::Attacked(int power, Monster *mainobj) // 恐惧状态类
    {
        int orglife = mainobj->GetLife();
        if ((orglife - power) > 0) // 怪物原来处于恐惧状态, 现在依旧处于恐惧状态
        {
            // 状态未变
            mainobj->SetLife(orglife - power); // 怪物剩余的血量
            cout << "MonsterStatus_Fear, run!" << endl;
            // 处理其他动作逻辑比如逃跑
        }
        else
        {
            // 不管下个状态是啥, 总之不会是凶悍、不安和恐惧状态, 只可能是死亡状态
            mainobj->setCurrentState(MonsterStatus_Dead::getInstance());
            mainobj->getCurrentState()->Attacked(power, mainobj);
        }
    }

    void MonsterStatus_Dead::Attacked(int power, Monster *mainobj) // 死亡状态类
    {
        int orglife = mainobj->GetLife();
        if (orglife > 0)
        {
            // 还要把怪物生命值减掉
            mainobj->SetLife(orglife - power); // 怪物剩余的血量
            // 处理怪物死亡后事宜比如怪物尸体定时消失等
        }
        cout << "dead!" << endl;
    }
}

int main()
{
#if 0
    using namespace ns1;
    Monster monster(500, Monster::MonS_Fer);
    cout << "Monster, MonS_Fer, 500 blood!" << endl;
    monster.Attacked(20);
    monster.Attacked(100);
    monster.Attacked(200);
    monster.Attacked(170);
    monster.Attacked(100);
    monster.Attacked(100);
#endif

#if 0
    using namespace ns2;
    Monster monster(500);
    cout << "Monster, MonS_Fer, 500 blood!" << endl;
    monster.Attacked(20);
    monster.Attacked(100);
    monster.Attacked(200);
    monster.Attacked(170);
    monster.Attacked(100);
    monster.Attacked(100);
#endif

#if 0
    using namespace ns3;
    Monster monster(500);
    cout << "Monster, MonS_Fer, 500 blood!" << endl;
    monster.Attacked(20);
    monster.Attacked(100);
    monster.Attacked(200);
    monster.Attacked(170);
    monster.Attacked(100);
    monster.Attacked(100);
#endif

#if 1
    using namespace ns4;
    Monster monster(500);
    cout << "Monster, MonS_Fer, 500 blood!" << endl;
    monster.Attacked(20);
    monster.Attacked(100);
    monster.Attacked(200);
    monster.Attacked(170);
    monster.Attacked(100);
    monster.Attacked(100);
#endif

    cout << "Over!\n";
    return 0;
}