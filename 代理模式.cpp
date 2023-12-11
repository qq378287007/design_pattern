#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <fstream>
using namespace std;

// 客户端和访问对象间引入代理对象（间接层），中介或桥梁
// 增加额外功能、约束或屏蔽复杂细节
// 为其他对象提供一种代理，以控制对这个对象的访问
// 3种角色
// Subject（抽象主题），定义接口
// Proxy（代理主题），内部包含真实主题的引用
// RealSubject（真实主题），代理主题所代表的真实主题

// 远程代理
// 虚拟代理
// 保护代理
// 缓存/缓冲代理
// 智能引用代理
// 写时复制优化代理

namespace ns1
{
    class WebAddr
    {
    public:
        virtual ~WebAddr() {}
        virtual void visit() = 0; // 访问网站
    };

    class WebAddr_Shopping : public WebAddr // 某购物网站
    {
    public: // 访问该购物网站，可能涉及到复杂的网络通信
        void visit() override { cout << "visit WebAddr_Shopping!" << endl; }
    };

    class WebAddr_Video : public WebAddr // 某视频网站
    {
    public: // 访问该视频网站，可能涉及到复杂的网络通信
        void visit() override { cout << "visit WebAddr_Video!" << endl; }
    };

    class WebAddrProxy : public WebAddr // 网站代理类
    {
        shared_ptr<WebAddr> mp_webaddr; // 代表要访问的具体网站
    public:                             // 引入的目的是传递进来要访问的具体网站
        WebAddrProxy(const shared_ptr<WebAddr> &pwebaddr) : mp_webaddr(pwebaddr) {}

    public:
        void visit() override
        {
            // 在这里进行访问的合法性检查、日志记录或者流量限制......
            if (mp_webaddr != nullptr)
                mp_webaddr->visit();
            // 在这里可以进行针对返回数据的过滤......
        }
    };

    class WebAddr_Shopping_Proxy : public WebAddr // 专门针对某购物网站WebAddr_Shopping的代理
    {
    public:
        void visit() override
        {
            // 在这里进行访问的合法性检查、日志记录或者流量限制......
            shared_ptr<WebAddr> p_webaddr(new WebAddr_Shopping());
            p_webaddr->visit();
            // 在这里可以进行针对返回数据的过滤......
        }
    };
}

namespace ns2
{
    static vector<string> g_fileItemList;

    class ReadInfo // 抽象主题
    {
    public:
        virtual ~ReadInfo() {}
        virtual void read() = 0;
    };

    class ReadInfoFromFile : public ReadInfo // 真实主题
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

            g_fileItemList.clear();
            string linebuf;
            while (getline(fin, linebuf)) // 系统函数，从文件中逐行读入内容
            {
                if (!linebuf.empty())                  // 读入的不是空行
                    g_fileItemList.push_back(linebuf); // 将文件中的每一行都保存到vector容器中
            }
            fin.close(); // 关闭文件输入流
        }
    };

    class ReadInfoProxy : public ReadInfo // 代理主题
    {
        bool m_loaded = false; // false表示还没有从文件中读出数据到内存
    public:
        void read() override
        {
            if (!m_loaded)
            {
                // 没有从文件中载入信息，则载入
                m_loaded = true; // 标记信息已经从文件中被载入进来，这样下次再获取这些数据时就不需要再去读文件了

                shared_ptr<ReadInfo> rf(new ReadInfoFromFile());
                rf->read(); // 将文件中的数据读入全局容器g_fileItemList中

                cout << "read from file: " << endl;
            }
            else
            {
                cout << "read from cache: " << endl;
            }

            // 现在数据一定在g_fileItemList中，开始显示
            for (auto iter = g_fileItemList.cbegin(); iter != g_fileItemList.cend(); ++iter)
                cout << *iter << endl;
        }
    };
}

int main()
{
#if 0
    using namespace ns1;

    shared_ptr<WebAddr> wba1(new WebAddr_Shopping());
    wba1->visit(); // 访问该购物网站

    shared_ptr<WebAddr> wba2(new WebAddr_Video());
    wba2->visit(); // 访问该视频网站
#endif

#if 0
    using namespace ns1;

    shared_ptr<WebAddr> wba1(new WebAddr_Shopping());
    shared_ptr<WebAddr> wbaproxy1(new WebAddrProxy(wba1));
    wbaproxy1->visit(); // 通过代理去访问WebAddr_Shopping购物网站

    shared_ptr<WebAddr> wba2(new WebAddr_Video());
    shared_ptr<WebAddr> wbaproxy2(new WebAddrProxy(wba2));
    wbaproxy2->visit(); // 通过代理去访问WebAddr_Video视频网站
#endif

#if 0
    using namespace ns1;
    shared_ptr<WebAddr> wbasproxy(new WebAddr_Shopping_Proxy());
    wbasproxy->visit(); // 访问的实际是某购物网站
#endif

#if 1
    using namespace ns2;

    shared_ptr<ReadInfo> preadinfoproxy(new ReadInfoProxy());
    preadinfoproxy->read(); // 第一次调用read是借助代理使用真实的主题到文件中拿数据
    preadinfoproxy->read(); // 后续调用read都是直接从缓存中拿数据
    preadinfoproxy->read(); // 从缓存中拿数据
#endif

    return 0;
}
