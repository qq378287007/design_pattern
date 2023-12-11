#include <iostream>
#include <list>
#include <memory>
using namespace std;

// 5种角色
// Receiver（接收者），Cook，提供请求业务的处理接口
// Invoker（调用者），Waiter，发送请求，通过命令对象执行请求
// Command（抽象命令），声明执行操作的接口
// ConcreteCommand（具体命令），通过调用Receiver相关操作执行请求
// Client（客户端），创建具体命令并设定接收者，创建调用者并驱动执行命令动作

// 将请求或命令封装为对象，通过参数进行传递，对象化的对象可以排队执行或者撤销

namespace ns1
{
    class Cook // 厨师类
    {
    public:
        void cook_fish() { cout << "fish" << endl; } // 做红烧鱼
        void cook_meat() { cout << "meat" << endl; } // 做锅包肉
    };

    class Command // 厨师做的每样菜品对应的抽象类
    {
    protected:
        shared_ptr<Cook> m_pcook;

    public:
        Command(const shared_ptr<Cook> &pcook) : m_pcook(pcook) {}
        virtual ~Command() {}
        virtual void Execute() = 0;
    };

    class CommandFish : public Command // 做红烧鱼菜品命令（顾客下的红烧鱼菜品便签）
    {
    public:
        CommandFish(const shared_ptr<Cook> &pcook) : Command(pcook) {}
        void Execute() override { m_pcook->cook_fish(); }
    };

    class CommandMeat : public Command // 做锅包肉菜品命令（顾客下的锅包肉菜品便签）
    {
    public:
        CommandMeat(const shared_ptr<Cook> &pcook) : Command(pcook) {}
        void Execute() override { m_pcook->cook_meat(); }
    };

    class Waiter0 // 服务员类
    {
        shared_ptr<Command> m_pcommand; // 服务员手中握着顾客书写的菜品便签
    public:
        void SetCommand(const shared_ptr<Command> &pcommand) { m_pcommand = pcommand; } // 顾客将便签交给服务员
        void Notify() { m_pcommand->Execute(); }                                        // 服务员将便签交到厨师手里让厨师开始做菜
    };

    class Waiter
    {
    private:
        // 一个便签中可以包含多个菜品甚至可以一次收集多个顾客的便签，达到一次性通知厨师做多道菜的效果
        list<shared_ptr<Command>> m_commlist; // 菜品列表，每道菜品作为一项，如果一个便签中有多个菜品，则这里将包含多项
    public:
        // 将顾客的便签增加到便签列表中，即便一个便签中包含多道菜品，这也相当于一道一道菜品加入到列表中
        void AddCommand(const shared_ptr<Command> &pcommand)
        {
            m_commlist.push_back(pcommand);
        }
        void DelCommand(const shared_ptr<Command> &pcommand) // 如果顾客想撤单则将便签从列表中删除
        {
            m_commlist.remove(pcommand);
        }
        void Notify() // 服务员将所有便签一次性交到厨师手里让厨师开始按顺序做菜
        {
            for (auto iter = m_commlist.cbegin(); iter != m_commlist.cend(); ++iter) // 依次让厨师做每一道菜品
                (*iter)->Execute();
        }
    };
}

namespace ns2
{
    class Cook // 厨师类
    {
    public:
        void cook_fish() { cout << "fish" << endl; } // 做红烧鱼
        void cook_meat() { cout << "meat" << endl; } // 做锅包肉
    };

    class Command // 厨师做的每样菜品对应的抽象类
    {
    protected:
        shared_ptr<Cook> m_pcook;

    public:
        Command(const shared_ptr<Cook> &pcook) : m_pcook(pcook) {}
        virtual ~Command() {}
        virtual void Execute() = 0;
    };

    class CommandFish : public Command // 做红烧鱼菜品命令（顾客下的红烧鱼菜品便签）
    {
    public:
        CommandFish(const shared_ptr<Cook> &pcook) : Command(pcook) {}
        void Execute() override { m_pcook->cook_fish(); }
    };

    class CommandMeat : public Command // 做锅包肉菜品命令（顾客下的锅包肉菜品便签）
    {
    public:
        CommandMeat(const shared_ptr<Cook> &pcook) : Command(pcook) {}
        void Execute() override { m_pcook->cook_meat(); }
    };

    class Traineewaiter // 实习服务员类
    {
        shared_ptr<Command> m_pcommand; // 实习服务员手中握着顾客书写的菜品便签
    public:
        Traineewaiter(const shared_ptr<Command> &pcommand) : m_pcommand(pcommand) {}
        void Notify() // 实习服务员将便签交到厨师手里让厨师开始做菜
        {
            if (m_pcommand != nullptr)
                m_pcommand->Execute();
        }
    };
}

namespace ns3
{
    class Cook // 厨师类
    {
    public:
        void cook_fish() { cout << "fish" << endl; } // 做红烧鱼
        void cook_meat() { cout << "meat" << endl; } // 做锅包肉
    };

    class Command // 厨师做的每样菜品对应的抽象类
    {
    protected:
        shared_ptr<Cook> m_pcook;

    public:
        Command(const shared_ptr<Cook> &pcook = nullptr) : m_pcook(pcook) {}
        virtual ~Command() {}
        virtual void Execute() = 0;
    };

    class CommandFish : public Command // 做红烧鱼菜品命令（顾客下的红烧鱼菜品便签
    {
    public:
        CommandFish(const shared_ptr<Cook> &pcook = nullptr) : Command(pcook) {}
        void Execute() override
        {
            if (m_pcook != nullptr)
                m_pcook->cook_fish();
        }
    };

    class CommandMeat : public Command // 做锅包肉菜品命令（顾客下的锅包肉菜品便签）
    {
    public:
        CommandMeat(const shared_ptr<Cook> &pcook = nullptr) : Command(pcook) {}
        void Execute() override
        {
            if (m_pcook != nullptr)
                m_pcook->cook_meat();
        }
    };

    class CommandMacro : public Command // 宏命令：把多个命令组合
    {
        list<shared_ptr<Command>> m_commlist;

    public:
        void AddCommand(const shared_ptr<Command> &pcommand)
        {
            m_commlist.push_back(pcommand);
        }
        virtual void Execute()
        {
            for (auto iter = m_commlist.cbegin(); iter != m_commlist.cend(); ++iter)
                (*iter)->Execute();
        }
    };

    class Traineewaiter // 实习服务员类
    {
        shared_ptr<Command> m_pcommand; // 实习服务员手中握着顾客书写的菜品便签
    public:
        Traineewaiter(const shared_ptr<Command> &pcommand = nullptr) : m_pcommand(pcommand) {}
        void Notify() // 实习服务员将便签交到厨师手里让厨师开始做菜
        {
            if (m_pcommand != nullptr)
                m_pcommand->Execute();
        }
    };
}

namespace ns4
{
    class TC
    {
    public:
        void operator()(int tv)
        {
            cout << "TC::operator(int tv), tv=" << tv << endl;
        }

    public:
        int operator()(int tv1, int tv2)
        {
            cout << "TC::operator(int tv1, int tv2), tv1=" << tv1 << ", tv2=" << tv2 << endl;
            return tv1 + tv2;
        }
    };

    template <class T>          // T代表可调用对象的类型
    void functmptest(T callobj) // callobj是个可调用对象
    {
        callobj(100);
    }
}

int main()
{
#if 0
    using namespace ns1;
    shared_ptr<Cook> pcook(new Cook());
    pcook->cook_fish();
    pcook->cook_meat();
#endif

#if 0
    using namespace ns1;
    shared_ptr<Cook> cook(new Cook());

    shared_ptr<Command> pcmd1(new CommandFish(cook));
    pcmd1->Execute(); // 做红烧鱼

    shared_ptr<Command> pcmd2(new CommandMeat(cook));
    pcmd2->Execute(); // 做锅包肉
#endif

#if 0
    using namespace ns1;
    shared_ptr<Cook> cook(new Cook());
    shared_ptr<Command> pcmd1(new CommandFish(cook));
    shared_ptr<Command> pcmd2(new CommandMeat(cook));
    shared_ptr<Waiter0> pwaiter(new Waiter0());

    pwaiter->SetCommand(pcmd1);
    pwaiter->Notify(); // 做红烧鱼

    pwaiter->SetCommand(pcmd2);
    pwaiter->Notify(); // 做锅包肉
#endif

#if 0
    using namespace ns1;
    shared_ptr<Cook> cook(new Cook());
    shared_ptr<Command> pcmd1(new CommandFish(cook));
    shared_ptr<Command> pcmd2(new CommandMeat(cook));
    shared_ptr<Waiter> pwaiter(new Waiter());
    // 把多道菜品分别加入到菜品列表中
    pwaiter->AddCommand(pcmd1);
    pwaiter->AddCommand(pcmd2);
    // 服务员一次性通知厨师做多道菜
    pwaiter->Notify();
#endif

#if 0
    using namespace ns2;
    shared_ptr<Cook> cook(new Cook());
    shared_ptr<Command> pcmd1(new CommandFish(cook));
    shared_ptr<Command> pcmd2(new CommandMeat(cook));

    shared_ptr<Traineewaiter> pwaitersx1(new Traineewaiter(pcmd1));
    pwaitersx1->Notify(); // 做红烧鱼
    shared_ptr<Traineewaiter> pwaitersx2(new Traineewaiter(pcmd2));
    pwaitersx2->Notify(); // 做锅包肉
#endif

#if 0
    using namespace ns3;
    shared_ptr<Cook> cook(new Cook());
    shared_ptr<Command> pcmd1(new CommandFish(cook));
    shared_ptr<Command> pcmd2(new CommandMeat(cook));
    shared_ptr<CommandMacro> pcmd(new CommandMacro());
    shared_ptr<Traineewaiter> pwaiter(new Traineewaiter(pcmd));

    pcmd->AddCommand(pcmd1);
    pcmd->AddCommand(pcmd2);
    pwaiter->Notify();
#endif

#if 1
    using namespace ns4;
    TC tc;
    tc(20); // 调用的是()操作符，这就是个可调用对象。等价于tc.operator()(20);
    cout << tc(30, 50) << endl;
    functmptest(tc);
#endif

    cout << "Over!\n";
    return 0;
}