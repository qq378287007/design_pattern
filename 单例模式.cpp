#include <iostream>
#include <memory>
#include <mutex>
#include <vector>
#include <atomic>
using namespace std;

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
            Deleter(){};
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
        static CSingleton4 *GetInstance()
        {
            return instance;
        }
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
        static void destoryInstance(Singleton3 *x)
        {
            delete x;
        }

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
            Deleter(){};
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
namespace ns1
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

/*
namespace ns2
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
                {
                    volatile shared_ptr<Singleton> temp(new Singleton());
                    singleton = temp;
                }
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
*/

/*
namespace ns3
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
*/

// 模板实现
namespace ns4
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
    SingleInstance *s = SingleInstance::GetInstance();
    s->Print();
    SingleInstance::GetInstance()->Print();
    SingleInstance::deleteInstance();
#endif

#if 1
    using namespace ns4;
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

    return 0;
}
