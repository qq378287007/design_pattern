#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <fstream>
using namespace std;

namespace ns1
{
    class WebAddr
    {
    public:
        virtual ~WebAddr() {}     // 做父类时析构函数应该为虚函数
        virtual void visit() = 0; // 执行访问网站的动作，子类中重新实现
    };

    // 某购物网站
    class WebAddr_Shopping : public WebAddr
    {
    public:
        virtual void visit()
        {
            //......访问该购物网站，可能涉及到复杂的网络通信
            cout << "visit WebAddr_Shopping!" << endl;
        }
    };

    // 某视频网站
    class WebAddr_Video : public WebAddr
    {
    public:
        virtual void visit()
        {
            //......访问该视频网站，可能涉及到复杂的网络通信
            cout << "visit WebAddr_Video!" << endl;
        }
    };

    // 网站代理类
    class WebAddrProxy : public WebAddr
    {
    public:
        // 构造函数，引入的目的是传递进来要访问的具体网站
        WebAddrProxy(const shared_ptr<WebAddr> &pwebaddr) : mp_webaddr(pwebaddr) {}

    public:
        virtual void visit()
        {
            // 在这里进行访问的合法性检查、日志记录或者流量限制......
            if (mp_webaddr != nullptr)
                mp_webaddr->visit();
            // 在这里可以进行针对返回数据的过滤......
        }

    private:
        shared_ptr<WebAddr> mp_webaddr; // 代表要访问的具体网站
    };

    // 专门针对某购物网站WebAddr_Shopping的代理
    class WebAddr_Shopping_Proxy : public WebAddr
    {
    public:
        virtual void visit()
        {
            // 在这里进行访问的合法性检查、日志记录或者流量限制......
            auto p_webaddr = make_shared<WebAddr_Shopping>();
            p_webaddr->visit();
            // 在这里可以进行针对返回数据的过滤......
        }
    };
}

namespace ns2
{
    vector<string> g_fileItemList;

    // 抽象主题
    class ReadInfo
    {
    public:
        virtual ~ReadInfo() {}
        virtual void read() = 0;
    };

    // 真实主题
    class ReadInfoFromFile : public ReadInfo
    {
    public:
        void read() override
        {
            ifstream fin(".vscode/settings.json"); // 文件中读取信息
            if (!fin)
            {
                cout << "failure" << endl;
                return;
            }

            string linebuf;
            while (getline(fin, linebuf)) // 系统函数，从文件中逐行读入内容
            {
                if (!linebuf.empty())                  // 读入的不是空行
                    g_fileItemList.push_back(linebuf); // 将文件中的每一行都保存到vector容器中
            }
            fin.close(); // 关闭文件输入流
        }
    };

    // 代理主题
    class ReadInfoProxy : public ReadInfo
    {
        bool m_loaded = false; // false表示还没有从文件中读出数据到内存
    public:
        void read() override
        {
            if (!m_loaded)
            {
                // 没有从文件中载入信息，则载入
                m_loaded = true; // 标记信息已经从文件中被载入进来，这样下次再获取这些数据时就不需要再去读文件了

                auto rf = make_shared<ReadInfoFromFile>();
                rf->read(); // 将文件中的数据读入全局容器g_fileItemList中

                cout << "read from file: " << endl;
            }
            else
            {
                cout << "read from cache: " << endl;
            }

            // 现在数据一定在g_fileItemList中，开始显示
            for (auto iter = g_fileItemList.begin(); iter != g_fileItemList.end(); ++iter)
                cout << *iter << endl;
        }
    };
}

int main()
{
#if 0
    {
        using namespace ns1;

        auto wba1 = make_shared<WebAddr_Shopping>();
        wba1->visit(); // 访问该购物网站

        auto wba2 = make_shared<WebAddr_Video>();
        wba2->visit(); // 访问该视频网站
    }
#endif

#if 0
    {
        using namespace ns1;

        auto wba1 = make_shared<WebAddr_Shopping>();
        auto wbaproxy1 = make_shared<WebAddrProxy>(wba1);
        wbaproxy1->visit(); // 通过代理去访问WebAddr_Shopping购物网站

        auto wba2 = make_shared<WebAddr_Video>();
        auto wbaproxy2 = make_shared<WebAddrProxy>(wba2);
        wbaproxy2->visit(); // 通过代理去访问WebAddr_Video视频网站
    }
#endif

#if 0
    {
        using namespace ns1;

        shared_ptr<WebAddr> wbasproxy = make_shared<WebAddr_Shopping_Proxy>();
        //shared_ptr<WebAddr_Shopping_Proxy> wbasproxy = make_shared<WebAddr_Shopping_Proxy>();
        wbasproxy->visit(); // 访问的实际是某购物网站
    }
#endif

#if 1
    {
        using namespace ns2;

        ReadInfo *preadinfoproxy = new ReadInfoProxy();
        preadinfoproxy->read(); // 第一次调用read是借助代理使用真实的主题到文件中拿数据
        preadinfoproxy->read(); // 后续调用read都是直接从缓存中拿数据
        preadinfoproxy->read(); // 从缓存中拿数据

        // 资源释放
        delete preadinfoproxy;
    }
#endif

    return 0;
}
