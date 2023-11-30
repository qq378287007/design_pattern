#include <iostream>
#include <atomic>
#include <mutex>
#include <memory>
using namespace std;

namespace ns1
{
    class GameConfig
    {
    private:
        GameConfig(){};
        GameConfig(const GameConfig &tmpobj);
        GameConfig &operator=(const GameConfig &tmpobj);
        ~GameConfig(){};

    public:
        static GameConfig *getInstance()
        {
            if (m_instance == nullptr)
                m_instance = new GameConfig();
            return m_instance;
        } /*
         static shared_ptr<GameConfig> getInstance2()
         {
             if (m_instance2 == nullptr)
                 m_instance2 = make_shared<GameConfig>();
             return m_instance2;
         }*/

    private:
        static GameConfig *m_instance;
        // static shared_ptr<GameConfig> m_instance2;
    };
    GameConfig *GameConfig::m_instance = nullptr;
    // shared_ptr<GameConfig> GameConfig::m_instance2 = nullptr;
}

// 懒汉式
namespace ns2
{
    class GameConfig
    {
    private:
        GameConfig(){};
        GameConfig(const GameConfig &tmpobj);
        GameConfig &operator=(const GameConfig &tmpobj);
        ~GameConfig(){};

    public:
        static GameConfig *getInstance()
        {
            // std::lock_guard<std::mutex> gcguard(my_mutex);
            if (m_instance == nullptr)
            {
                // 这里再加锁
                // std::lock_guard<std::mutex> gcguard(my_mutex);
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
            if (m_instance != nullptr)
            {
                delete GameConfig::m_instance;
                GameConfig::m_instance = nullptr;
            }
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

    private:
        static GameConfig *m_instance;
    };

    GameConfig *GameConfig::m_instance = nullptr;
}

namespace ns3
{
    class GameConfig
    {
    private:
        GameConfig(){};
        GameConfig(const GameConfig &tmpobj);
        GameConfig &operator=(const GameConfig &tmpobj);
        ~GameConfig(){};

    public:
        static GameConfig *getInstance()
        {
            GameConfig *tmp = m_instance.load(std::memory_order_relaxed);
            std::atomic_thread_fence(std::memory_order_acquire);
            if (tmp == nullptr)
            {
                std::lock_guard<std::mutex> lock(m_mutex);
                tmp = m_instance.load(std::memory_order_relaxed);
                if (tmp == nullptr)
                {
                    tmp = new GameConfig();
                    static Garbo garboobj;
                    std::atomic_thread_fence(std::memory_order_release);
                    m_instance.store(tmp, std::memory_order_relaxed);
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

    private:
        static atomic<GameConfig *> m_instance;
        static std::mutex m_mutex;
    };
    std::atomic<GameConfig *> GameConfig::m_instance;
    std::mutex GameConfig::m_mutex;
}

// 饿汉式
namespace ns4
{
    class GameConfig
    {
    private:
        GameConfig(){};
        GameConfig(const GameConfig &tmpobj);
        GameConfig &operator=(const GameConfig &tmpobj);
        ~GameConfig(){};

    public:
        static GameConfig *getInstance()
        {
            return m_instance;
        }

    private:
        static GameConfig *m_instance;

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

namespace ns5
{
    class GameConfig2
    {
    private:
        GameConfig2(){};
        GameConfig2(const GameConfig2 &tmpobj);
        GameConfig2 &operator=(const GameConfig2 &tmpobj);
        ~GameConfig2(){};

    public:
        static GameConfig2 *getInstance()
        {
            return &m_instance;
        }

    private:
        static GameConfig2 m_instance;
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

int main()
{
#if 0
    using namespace ns1;
    GameConfig *g_gc = GameConfig::getInstance();
    GameConfig *g_gc2 = GameConfig::getInstance();
#endif

#if 1
    using namespace ns2;
    // GameConfig g_config;//error
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

#if 0
    using namespace ns5;
    GameConfig2 *g_gc = GameConfig2::getInstance();
    GameConfig &g_gc2 = GameConfig::getInstance();
    myfunc();
#endif

    cout << "Over!\n";
    return 0;
}