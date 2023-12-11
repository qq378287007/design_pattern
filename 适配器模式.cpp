#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <memory>
using namespace std;

// 解决两个类之间的兼容性问题
// 转换接口调用
// 3种角色
// Target（目标抽象类），定义需要暴露的接口
// Adaptee（适配者类），被适配角色，多个老接口
// Adapter（适配器类），包装类，转换器角色

// 既有东西增减内容就成为了一个适配器
// 容器适配器，双端队列deque减少东西就能实现stack和queue
// 算法适配器，std::bind(绑定器)
// 迭代器适配器，reverse_iterator(反向迭代器)，对迭代器iterator的简单封装

namespace ns1
{
    class LogToFile // 日志文件操作相关类
    {
    public:
        void initfile() {}                        // 做日志文件初始化工作，比如打开文件等等
        void writetofile(const char *pcontent) {} // 将日志内容写入文件
        void readfromfile() {}                    // 从日志中读取一些信息
        void closefile() {}                       // 关闭日志文件
    };
}

namespace ns2
{
    class LogToDatabase // 日志操作相关类（数据库版本）
    {
    public:
        void initdb() {}                        // 连接数据库，做一些基本的数据库连接设置等
        void writetodb(const char *pcontent) {} // 将日志内容写入数据库
        void readfromdb() {}                    // 从数据库中读取一些日志信息
        void closedb() {}                       // 关闭到数据库的连接
    };
}

namespace ns3
{
    class LogToFile // 日志文件操作相关类
    {
    public:
        void initfile() {}                        // 做日志文件初始化工作，比如打开文件等等
        void writetofile(const char *pcontent) {} // 将日志内容写入文件
        void readfromfile() {}                    // 从日志中读取一些信息
        void closefile() {}                       // 关闭日志文件
    };

    class LogToDatabase
    {
    public:
        virtual ~LogToDatabase() {}

        virtual void initdb() = 0; // 不一定非是纯虚函数
        virtual void writetodb(const char *pcontent) = 0;
        virtual void readfromdb() = 0;
        virtual void closedb() = 0;
    };

    class LogAdapter : public LogToDatabase // 对象适配器，通过组合，委托实现
    {
        shared_ptr<LogToFile> m_pfile;

    public:
        LogAdapter(const shared_ptr<LogToFile> &pfile = make_shared<LogToFile>()) : m_pfile(pfile) {} // 形参是老接口所属类
        void initdb() override
        {
            cout << "LogAdapter::initdb() Adapter LogToFile::initfile()" << endl;
            m_pfile->initfile();
        }
        void writetodb(const char *pcontent) override
        {
            cout << "LogAdapter::writetodb() Adapter LogToFile::writetofile()" << endl;
            m_pfile->writetofile(pcontent);
        }
        void readfromdb() override
        {
            cout << "LogAdapter::readfromdb() Adapter LogToFile::readfromdb()" << endl;
            m_pfile->readfromfile();
        }
        void closedb() override
        {
            cout << "LogAdapter::closedb() Adapter LogToFile::closedb()" << endl;
            m_pfile->closefile();
        }
    };
}

namespace ns4
{
    class LogToFile // 日志文件操作相关类
    {
    public:
        void initfile() {}                        // 做日志文件初始化工作，比如打开文件等等
        void writetofile(const char *pcontent) {} // 将日志内容写入文件
        void readfromfile() {}                    // 从日志中读取一些信息
        void closefile() {}                       // 关闭日志文件
    };

    class LogToDatabase
    {
    public:
        virtual ~LogToDatabase() {}

        virtual void initdb() = 0; // 不一定非是纯虚函数
        virtual void writetodb(const char *pcontent) = 0;
        virtual void readfromdb() = 0;
        virtual void closedb() = 0;
    };

    class LogAdapter : public LogToDatabase, private LogToFile // 类适配器，通过继承实现接口的适配
    {
    public:
        void initdb() override
        {
            cout << "LogAdapter::initdb() Adapter LogToFile::initfile()" << endl;
            initfile();
        }
        void writetodb(const char *pcontent) override
        {
            cout << "LogAdapter::writetodb() Adapter LogToFile::writetofile()" << endl;
            writetofile(pcontent);
        }
        void readfromdb() override
        {
            cout << "LogAdapter::readfromdb() Adapter LogToFile::readfromdb()" << endl;
            readfromfile();
        }
        void closedb() override
        {
            cout << "LogAdapter::closedb() Adapter LogToFile::closedb()" << endl;
            closefile();
        }
    };
}

int main()
{
#if 0
    using namespace ns1;
    shared_ptr<LogToFile> plog(new LogToFile());
    plog->initfile();
    plog->writetofile("Write a log to the log file"); // 写一条日志到日志文件中
    plog->readfromfile();
    plog->closefile();
#endif

#if 0
    using namespace ns2;
    shared_ptr<LogToDatabase> plogdb(new LogToDatabase());
    plogdb->initdb();
    plogdb->writetodb("Write a log to the database"); // 写一条日志到数据库中
    plogdb->closedb();
    plogdb->readfromdb();
#endif

#if 0
    using namespace ns3;
    //shared_ptr<LogToFile> plog2(new LogToFile());
    //shared_ptr<LogToDatabase> plogdb2(new LogAdapter(plog2));
    shared_ptr<LogToDatabase> plogdb2(new LogAdapter());
    plogdb2->initdb();
    plogdb2->writetodb("Writing a log to the database is actually writing a log to the log file");
    plogdb2->readfromdb();
    plogdb2->closedb();
#endif

#if 1
    using namespace ns4;
    shared_ptr<LogToDatabase> plogdb3(new LogAdapter());
    plogdb3->initdb();
    plogdb3->writetodb("Writing a log to the database is actually writing a log to the log file");
    plogdb3->readfromdb();
    plogdb3->closedb();
#endif

    cout << "Over!\n";
    return 0;
}