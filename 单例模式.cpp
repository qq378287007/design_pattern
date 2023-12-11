#include <iostream>
#include <memory>
#include <mutex>
#include <vector>
#include <atomic>
using namespace std;

// 懒汉式，未释放
namespace ns1
{
    class GameConfig
    {
        static GameConfig *m_instance1; // 指向本类对象的指针
        // static mutex m_mutex;
        // static shared_ptr<GameConfig> GameConfig::m_instance3;

    private:
        GameConfig(){};
        GameConfig(const GameConfig &tmpobj);
        GameConfig &operator=(const GameConfig &tmpobj);
        ~GameConfig(){};

    public:
        static GameConfig *getInstance1()
        {
            // lock_guard<mutex> l(m_mutex);
            if (m_instance1 == nullptr)
                m_instance1 = new GameConfig();
            return m_instance1;
        }
        static GameConfig *getInstance2()
        {
            static GameConfig m_instance2;
            return &m_instance2;
        }
        /*
        static shared_ptr<GameConfig> getInstance3()
        {
            if (m_instance3 == nullptr)
                m_instance3 = make_shared<GameConfig>();
            return m_instance3;
        }
        */
        /*
        static shared_ptr<GameConfig> getInstance4()
        {
            static shared_ptr<GameConfig> m_instance4(new GameConfig());
            return m_instance4;
        }
        */
    };
    GameConfig *GameConfig::m_instance1 = nullptr;
    // mutex GameConfig::m_mutex;
    // shared_ptr<GameConfig> GameConfig::m_instance3 = nullptr;
}

// 懒汉式，嵌套类释放
namespace ns2
{
    class GameConfig
    {
        static GameConfig *m_instance;
        // static mutex m_mutex;
    private:
        GameConfig(){};
        GameConfig(const GameConfig &tmpobj);
        GameConfig &operator=(const GameConfig &tmpobj);
        ~GameConfig(){};

    public:
        static GameConfig *getInstance()
        {
            // lock_guard<mutex> gcguard(m_mutex);
            if (m_instance == nullptr)
            {
                // 这里再加锁
                // lock_guard<mutex> gcguard(m_mutex);
                // if (m_instance == nullptr)
                //{
                m_instance = new GameConfig();
                static Garbo garboobj;
                //}
            }
            return m_instance;
        }

    public:
        static void freeInstance() // 要手工调用才能释放内存
        {
            if (GameConfig::m_instance != nullptr)
            {
                delete GameConfig::m_instance;
                GameConfig::m_instance = nullptr;
            }
        }

    private:
        class Garbo // 手工释放单件类对象引入的GameConfig类中的嵌套类（垃圾回收）
        {
        public:
            ~Garbo() { GameConfig::freeInstance(); }
        };
    };
    GameConfig *GameConfig::m_instance = nullptr;
    // mutex GameConfig::m_mutex;
}

// 懒汉式，加锁+嵌套类释放
namespace ns3
{
    class GameConfig
    {
        static atomic<GameConfig *> m_instance;
        static mutex m_mutex;

    private:
        GameConfig(){};
        GameConfig(const GameConfig &tmpobj);
        GameConfig &operator=(const GameConfig &tmpobj);
        ~GameConfig(){};

    public:
        static GameConfig *getInstance()
        {
            GameConfig *tmp = m_instance.load(memory_order_relaxed);
            atomic_thread_fence(memory_order_acquire);
            if (tmp == nullptr)
            {
                lock_guard<mutex> lock(m_mutex);
                tmp = m_instance.load(memory_order_relaxed);
                if (tmp == nullptr)
                {
                    tmp = new GameConfig();
                    static Garbo garboobj;
                    atomic_thread_fence(memory_order_release);
                    m_instance.store(tmp, memory_order_relaxed);
                }
            }
            return tmp;
        }

    private:
        class Garbo // 手工释放单件类对象引入的GameConfig类中的嵌套类（垃圾回收）
        {
        public:
            ~Garbo()
            {
                if (GameConfig::m_instance != nullptr)
                {
                    delete GameConfig::m_instance;
                    GameConfig::m_instance = nullptr;
                }
            }
        };
    };
    atomic<GameConfig *> GameConfig::m_instance;
    mutex GameConfig::m_mutex;
}

// 饿汉式，嵌套类释放
namespace ns4
{
    class GameConfig
    {
        static GameConfig *m_instance;

    private:
        GameConfig(){};
        GameConfig(const GameConfig &tmpobj);
        GameConfig &operator=(const GameConfig &tmpobj);
        ~GameConfig(){};

    public:
        static GameConfig *getInstance() { return m_instance; }

    private:
        class Garbo // 手工释放单件类对象引入的GameConfig类中的嵌套类（垃圾回收）
        {
        public:
            ~Garbo()
            {
                if (GameConfig::m_instance != nullptr)
                {
                    delete GameConfig::m_instance;
                    GameConfig::m_instance = nullptr;
                }
            }
        };

    private:
        static Garbo garboobj;
    };
    GameConfig *GameConfig::m_instance = new GameConfig(); // 趁静态成员变量定义的时机直接初始化是被允许的，即便GameConfig构造函数用private修饰
    GameConfig::Garbo GameConfig::garboobj;
}

// 饿汉式
namespace ns5
{
    class GameConfig2
    {
        static GameConfig2 m_instance;

    private:
        GameConfig2(){};
        GameConfig2(const GameConfig2 &tmpobj);
        GameConfig2 &operator=(const GameConfig2 &tmpobj);
        ~GameConfig2(){};

    public:
        static GameConfig2 *getInstance() { return &m_instance; }
    };
    GameConfig2 GameConfig2::m_instance;

    class GameConfig
    {
    private:
        GameConfig(){};
        GameConfig(const GameConfig &tmpobj);
        GameConfig &operator=(const GameConfig &tmpobj);
        ~GameConfig(){};

    public:
        static GameConfig &getInstance()
        {
            static GameConfig instance;
            return instance;
        }
    };

    int myfunc()
    {
        static int stcs = 100; // 不需要调用myfunc函数，stcs就已经等于100了
        stcs += 180;
        return stcs;
    }
}

// 饿汉式
namespace ns111
{
    // 饿汉模式
    class CSingleton
    {
        CSingleton() {}
        ~CSingleton() {}

    public:
        static CSingleton *GetInstance()
        {
            static CSingleton instance;
            return &instance;
        }
    };

    // 饿汉模式2
    class CSingleton2
    {
        static CSingleton2 *instance;
        CSingleton2() {}
        ~CSingleton2() {}

    public:
        static CSingleton2 *GetInstance()
        {
            return instance;
        }
    };
    CSingleton2 *CSingleton2::instance = new CSingleton2();

    // 饿汉模式3，智能指针
    class CSingleton3
    {
        static shared_ptr<CSingleton3> instance;
        CSingleton3() {}
        ~CSingleton3() {}

    public:
        static shared_ptr<CSingleton3> GetInstance()
        {
            return instance;
        }
        static void destoryInstance(CSingleton3 *x)
        {
            delete x;
        }
    };
    shared_ptr<CSingleton3> CSingleton3::instance(new CSingleton3(), destoryInstance);

    // 饿汉模式4，静态嵌套类
    class CSingleton4
    {
        static CSingleton4 *instance;
        class Deleter
        {
        public:
            ~Deleter()
            {
                if (instance != nullptr)
                {
                    delete instance;
                    instance = nullptr;
                }
            }
        };
        static Deleter m_deleter;

        CSingleton4() {}
        ~CSingleton4() {}

    public:
        static CSingleton4 *GetInstance() { return instance; }
    };
    CSingleton4 *CSingleton4::instance = new CSingleton4();
    CSingleton4::Deleter CSingleton4::m_deleter;
}

namespace ns222
{
    // 懒汉模式1
    class Singleton1
    {
        static Singleton1 *instance;

        Singleton1() {}
        ~Singleton1() {}

    public:
        static Singleton1 *getInstance()
        {
            if (instance == nullptr)
                instance = new Singleton1();
            return instance;
        }
    };
    Singleton1 *Singleton1::instance = nullptr;

    // 懒汉模式2
    class Singleton2
    {
        static Singleton2 *m_instance;
        static mutex mtx;
        Singleton2() {}
        ~Singleton2() {}

    public:
        Singleton2(const Singleton2 &) = delete;
        Singleton2 &operator=(const Singleton2 &) = delete;

    public:
        static Singleton2 *getInstance()
        {
            if (NULL == m_instance)
            {
                unique_lock<mutex> lock(mtx);
                if (NULL == m_instance)
                    m_instance = new Singleton2();
            }
            return m_instance;
        }
    };
    Singleton2 *Singleton2::m_instance = nullptr;
    mutex Singleton2::mtx;

    // 懒汉模式3，智能指针
    class Singleton3
    {
        static shared_ptr<Singleton3> m_instance;
        static mutex mtx;
        Singleton3() {}
        ~Singleton3() {}

    public:
        Singleton3(const Singleton3 &) = delete;
        Singleton3(Singleton3 &&) = delete;
        Singleton3 &operator=(const Singleton3 &) = delete;
        Singleton3 &operator=(Singleton3 &&) = delete;

    public:
        static void destoryInstance(Singleton3 *x) { delete x; }

        static shared_ptr<Singleton3> getInstance()
        {
            if (NULL == m_instance)
            {
                lock_guard<mutex> lock(mtx);
                if (NULL == m_instance)
                    m_instance.reset(new Singleton3(), destoryInstance);
            }
            return m_instance;
        }
    };
    shared_ptr<Singleton3> Singleton3::m_instance = nullptr;
    mutex Singleton3::mtx;

    // 懒汉模式4，静态嵌套类
    class Singleton4
    {
        static Singleton4 *m_instance;
        static mutex mtx;
        Singleton4() {}
        ~Singleton4() {}

        class Deleter
        {
        public:
            ~Deleter()
            {
                if (m_instance != nullptr)
                {
                    delete m_instance;
                    m_instance = nullptr;
                }
            }
        };
        static Deleter m_deleter;

    public:
        Singleton4(const Singleton4 &) = delete;
        Singleton4 &operator=(const Singleton4 &) = delete;

    public:
        static Singleton4 *getInstance()
        {
            if (NULL == m_instance)
            {
                lock_guard<mutex> lock(mtx);
                if (NULL == m_instance)
                    m_instance = new Singleton4();
            }
            return m_instance;
        }
    };
    Singleton4 *Singleton4::m_instance = nullptr;
    mutex Singleton4::mtx;
    Singleton4::Deleter Singleton4::m_deleter;

    // 懒汉模式5
    class Singleton5
    {
        static atomic<Singleton5 *> m_instance;
        static mutex mtx;
        Singleton5() {}
        ~Singleton5() {}

    public:
        Singleton5(const Singleton5 &) = delete;
        Singleton5 &operator=(const Singleton5 &) = delete;

    public:
        static Singleton5 *getInstance()
        {
            Singleton5 *p = m_instance;
            if (p == nullptr)
            {
                lock_guard<mutex> ll{mtx};
                if ((p = m_instance) == nullptr)
                    m_instance = p = new Singleton5();
            }
            return m_instance;
        }
    };
    atomic<Singleton5 *> Singleton5::m_instance{nullptr};
    mutex Singleton5::mtx;
}

//  加锁的懒汉式
namespace ns2_1
{
    class SingleInstance
    {
    private:
        static SingleInstance *m_SingleInstance;
        static mutex m_Mutex;

    private:
        SingleInstance() {}
        ~SingleInstance() {}

        SingleInstance(const SingleInstance &signal) {}
        const SingleInstance &operator=(const SingleInstance &signal) {}

    public:
        static SingleInstance *GetInstance()
        {
            if (m_SingleInstance == nullptr)
            {
                unique_lock<mutex> lock(m_Mutex); // 加锁
                if (m_SingleInstance == nullptr)
                {
                    volatile auto temp = new (nothrow) SingleInstance();
                    m_SingleInstance = temp;
                }
            }
            return m_SingleInstance;
        }

        static void deleteInstance()
        {
            unique_lock<mutex> lock(m_Mutex); // 加锁
            if (m_SingleInstance)
            {
                delete m_SingleInstance;
                m_SingleInstance = nullptr;
            }
        }
        void Print()
        {
            cout << "实例内存地址: " << this << endl;
        }
    };
    SingleInstance *SingleInstance::m_SingleInstance = nullptr;
    mutex SingleInstance::m_Mutex;
}

namespace ns2_2
{
    class Singleton
    {
        static shared_ptr<Singleton> singleton;
        static mutex singletonMutex;

    public:
        static shared_ptr<Singleton> getSingleton()
        {
            if (singleton == nullptr)
            {
                unique_lock<mutex> lock(singletonMutex);
                if (singleton == nullptr)
                    singleton.reset(new Singleton());
            }
            return singleton;
        }

        void print()
        {
            cout << "Hello World." << endl;
        }

        ~Singleton()
        {
            cout << __PRETTY_FUNCTION__ << endl;
        }

    private:
        Singleton()
        {
            cout << __PRETTY_FUNCTION__ << endl;
        }
    };
    shared_ptr<Singleton> Singleton::singleton = nullptr;
    mutex Singleton::singletonMutex;
}

namespace ns2_3
{
    class Singleton
    {
        static shared_ptr<Singleton> singleton;
        static once_flag singletonFlag;

    private:
        Singleton() {}
        ~Singleton() {}

    public:
        void print()
        {
            cout << "Hello World." << endl;
        }

    public:
        static shared_ptr<Singleton> getSingleton()
        {
            call_once(singletonFlag, [&]
                      { singleton = make_shared<Singleton>(); });
            return singleton;
        }
    };

    shared_ptr<Singleton> Singleton::singleton = nullptr;
    once_flag Singleton::singletonFlag;
}

// 模板实现
namespace ns2_4
{
    template <typename T>
    shared_ptr<T> getInstance()
    {
        static shared_ptr<T> instance(new T());
        return instance;
    }

    class TestClass
    {
    public:
        TestClass()
        {
            cout << "TestClass constructed!" << endl;
        }

        void print()
        {
            cout << "TestClass Address: " << this << endl;
        }
    };
}

/*
namespace ns22222
{

    class Single
    {
    public:
        static Single &getInstance()
        {
            mutex mt;
            if (instance.get() == NULL)
            {
                mt.lock();
                // if (instance.get() == NULL)
                //     instance.reset(new Single());
                mt.unlock();
            }
            return *instance;
        }

    private:
        Single() {}
        ~Single() {}
        static shared_ptr<Single> instance;
        friend class shared_ptr<Single>;

        Single(const Single &);
        Single &operator=(const Single &);
    };

    shared_ptr<Single> Single::instance = make_shared<Single>();


    class SingleObject
    {
    private:
        static shared_ptr<SingleObject> instance;

        SingleObject() {}
        ~SingleObject() {}
        friend class shared_ptr<SingleObject>;

    public:
        static SingleObject &getInstance()
        {
            return *instance;
        }
        void showMessage()
        {
            cout << "Hello World!" << endl;
        }
    };
    shared_ptr<SingleObject> SingleObject::instance = make_shared<SingleObject>();

}
*/

int main()
{
#if 0
    using namespace ns1;
    GameConfig *g_gc1 = GameConfig::getInstance1();
    GameConfig *g_gc2 = GameConfig::getInstance2();
#endif

#if 0
    using namespace ns2;
    GameConfig *g_gc = GameConfig::getInstance();
    // g_gc->freeInstance(); // 手工释放内存演示
#endif

#if 0
    using namespace ns3;
    GameConfig *g_gc = GameConfig::getInstance();
    GameConfig *g_gc2 = GameConfig::getInstance();
#endif

#if 0
    using namespace ns4;
    GameConfig *g_gc = GameConfig::getInstance();
    GameConfig *g_gc2 = GameConfig::getInstance();
#endif

#if 1
    using namespace ns5;
    GameConfig2 *g_gc = GameConfig2::getInstance();
    GameConfig &g_gc2 = GameConfig::getInstance();
    myfunc();
#endif

#if 0
    using namespace ns2_1;
    SingleInstance *s = SingleInstance::GetInstance();
    s->Print();
    SingleInstance::GetInstance()->Print();
    SingleInstance::deleteInstance();
#endif

#if 0
    using namespace ns2_4;
    getInstance<TestClass>()->print();
    getInstance<TestClass>()->print();

    auto instance1 = getInstance<vector<int>>();
    auto instance2 = getInstance<vector<int>>();
    instance1->push_back(1);
    instance2->push_back(2);
    cout << "instance1->size(): " << instance1->size() << endl;
    cout << "instance2->size(): " << instance2->size() << endl;
    cout << "instance1 address: " << instance1.get() << endl;
    cout << "instance2 address: " << instance2.get() << endl;
#endif

    cout << "Over!\n";
    return 0;
}
