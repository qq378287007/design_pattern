#include <iostream>
#include <memory>
#include <string>
using namespace std;

// 请求传递给链中的若干对象，哪个对象适合处理就自行处理
// 使多个对象都有机会处理请求，从而避免请求的发送者和接收者之间的耦合关系
// 将这些对象构成对象链，并沿着链传递请求，直到有对象处理为止

// 3种角色
// Handler（处理者），定义处理请求的接口，记录下一个处理者
// ConcreteHandler（具体处理者），实现针对具体请求的处理，自身无法处理会将请求传递给后继者
// Client（请求者/客户端），创建责任链，向责任链的具体处理者提交处理请求

// 单纯的责令链模式，请求得到处理后不向下传递
// 非单纯的责令链模式（功能链），请求得到处理后继续向下传递

namespace ns1
{
    class SalaryHandler // 薪水处理类
    {
        void depManagerSP(const string &sname, int salfigure) const // 部门经理审批加薪请求
        {
            cout << sname << " ask for a raise: " << salfigure << ", manager agree!" << endl;
        }

        void CTOSP(const string &sname, int salfigure) const // 技术总监审批加薪请求
        {
            cout << sname << " ask for a raise: " << salfigure << ", Technical Director agree!" << endl;
        }

        void genManagerSP(const string &sname, int salfigure) const // 总经理审批加薪请求
        {
            cout << sname << " ask for a raise: " << salfigure << ", general manager agree!" << endl;
        }

    public:                                                         // 处理加薪请求
        void raiseRequest(const string &sname, int salfigure) const // 参数1代表要加薪的员工名字，参数2代表求要加薪多少
        {
            if (salfigure <= 1000) // 加薪要求不超过1000，部门经理可以批准
                depManagerSP(sname, salfigure);
            else if (salfigure <= 5000) // 加薪要求在1000元之上但不超过5000，技术总监才能批准
                CTOSP(sname, salfigure);
            else // 加薪要求超过5000元，总经理才能批准
                genManagerSP(sname, salfigure);
        }
    };
}

namespace ns2
{
    class RaiseRequest // 加薪请求类
    {
        string m_sname;   // 请求加薪的人员名字
        int m_isalfigure; // 请求加薪的数字
    public:
        RaiseRequest(const string &sname, int salfigure) : m_sname(sname), m_isalfigure(salfigure) {}
        const string &getName() const { return m_sname; } // 获取请求加薪的人员名字
        int getSalFigure() const { return m_isalfigure; } // 获取请求加薪的数字
    };

    class ParSalApprover // 薪水审批者父类
    {
        shared_ptr<ParSalApprover> m_nextChain; // 指向下一个审批者（对象）的多态指针（指向自身类型），每个都指向下一个，就会构成一个职责链（链表）

    protected:
        void sendRequestToNextHandler(const RaiseRequest &req) const // 找链中的下个对象并把请求投递给下个链中的对象
        {
            if (m_nextChain != nullptr)           // 找链中的下个对象
                m_nextChain->processRequest(req); // 把请求投递给链中的下个对象
            else                                  // 没找到链中的下个对象，程序流程执行这里似乎不应该
                cout << req.getName() << " ask for a raise: " << req.getSalFigure() << ", nobody agree!" << endl;
        }

    public:
        ParSalApprover(const shared_ptr<ParSalApprover> &next = nullptr) : m_nextChain(next) {}
        virtual ~ParSalApprover() {}
        void setNextChain(const shared_ptr<ParSalApprover> &next) { m_nextChain = next; } // 设置指向的职责链中的下个审批者
        virtual void processRequest(const RaiseRequest &req) const = 0;                   // 处理加薪请求
    };

    class depManager_SA : public ParSalApprover // 部门经理子类
    {
    public:
        depManager_SA(const shared_ptr<ParSalApprover> &next = nullptr) : ParSalApprover(next) {}
        void processRequest(const RaiseRequest &req) const override
        {
            int salfigure = req.getSalFigure();
            if (salfigure <= 1000) // 如果自己能处理，则自己处理
                cout << req.getName() << " ask for a raise: " << salfigure << ", manager agree!" << endl;
            else // 自己不能处理，尝试找链中的下个对象来处理
                sendRequestToNextHandler(req);
        }
    };

    class CTO_SA : public ParSalApprover // 技术总监子类
    {
    public:
        CTO_SA(const shared_ptr<ParSalApprover> &next = nullptr) : ParSalApprover(next) {}
        void processRequest(const RaiseRequest &req) const override
        {
            int salfigure = req.getSalFigure();
            if (salfigure > 1000 && salfigure <= 5000) // 如果自己能处理，则自己处理
                cout << req.getName() << " ask for a raise: " << salfigure << ", CTO_SA agree!" << endl;
            else
                sendRequestToNextHandler(req); // 自己不能处理，尝试找链中的下个对象来处理
        }
    };

    class genManager_SA : public ParSalApprover // 总经理子类
    {
    public:
        genManager_SA(const shared_ptr<ParSalApprover> &next = nullptr) : ParSalApprover(next) {}
        void processRequest(const RaiseRequest &req) const override
        {
            int salfigure = req.getSalFigure();
            if (salfigure > 5000) // 如果自己能处理，则自己处理
                cout << req.getName() << " ask for a raise: " << salfigure << ", genManager_SA agree!" << endl;
            else
                sendRequestToNextHandler(req); // 自己不能处理，尝试找链中的下个对象来处理
        }
    };
}

namespace ns3
{
    class ParWordFilter // 敏感词过滤器父类
    {
        shared_ptr<ParWordFilter> m_nextChain{nullptr};

    protected: // 找链中的下个对象并把请求投递给下个链中的对象
        string sendRequestToNextHandler(const string &strWord) const
        {
            if (m_nextChain != nullptr)                      // 找链中的下个对象
                return m_nextChain->processRequest(strWord); // 把请求投递给链中的下个对象
            return strWord;
        }

    public:
        virtual ~ParWordFilter() {}
        void setNextChain(const shared_ptr<ParWordFilter> &next) { m_nextChain = next; } // 设置指向的职责链中的下个过滤器
        virtual string processRequest(const string &strWord) const = 0;                  // 处理敏感词过滤请求
    };

    class SexyWordFilter : public ParWordFilter // 性敏感词过滤器子类
    {
    public:
        string processRequest(const string &strWord) const override
        {
            cout << "replace sex with XXX!" << endl;
            return sendRequestToNextHandler(strWord + "XXX");
        }
    };

    class DirtyWordFilter : public ParWordFilter // 脏话词过滤器子类
    {
    public:
        string processRequest(const string &strWord) const override
        {
            cout << "replace obscenities with YYY!" << endl;
            return sendRequestToNextHandler(strWord + "YYY");
        }
    };

    class PoliticsWordFilter : public ParWordFilter // 政治敏感词过滤器子类
    {
    public:
        string processRequest(const string &strWord) const override
        {
            cout << "replace politices with ZZZ!" << endl;
            return sendRequestToNextHandler(strWord + "ZZZ");
        }
    };
}

namespace ns4
{
    class AbstractLogger
    {
    public:
        static int INFO;
        static int DEBUG;
        static int ERROR;

    public:
        virtual ~AbstractLogger() = default;
        AbstractLogger(int m_level) : level(m_level) {}
        void setNextLogger(const shared_ptr<AbstractLogger> &m_nextLogger) { nextLogger = m_nextLogger; }

        void logMessage(int m_level, const string &message)
        {
            if (level <= m_level)
                write(message);

            if (nextLogger != nullptr)
                nextLogger->logMessage(m_level, message);
        }

    protected:
        int level;
        shared_ptr<AbstractLogger> nextLogger;
        virtual void write(const string &message) const = 0;
    };
    int AbstractLogger::INFO = 1;
    int AbstractLogger::DEBUG = 2;
    int AbstractLogger::ERROR = 3;

    class ConsoleLogger : public AbstractLogger
    {
    public:
        ConsoleLogger(int level) : AbstractLogger(level) {}

    protected:
        void write(const string &message) const override
        {
            cout << "Standard Console::Logger: " + message << endl;
        }
    };

    class ErrorLogger : public AbstractLogger
    {
    public:
        ErrorLogger(int level) : AbstractLogger(level) {}

    protected:
        void write(const string &message) const override
        {
            cout << "ErrorLogger Console::Logger: " + message << endl;
        }
    };

    class FileLogger : public AbstractLogger
    {
    public:
        FileLogger(int level) : AbstractLogger(level) {}

    protected:
        void write(const string &message) const override
        {
            cout << "FileLogger Console::Logger: " + message << endl;
        }
    };

    shared_ptr<AbstractLogger> getChainOfLoggers()
    {
        shared_ptr<AbstractLogger> errorLogger = make_shared<ErrorLogger>(AbstractLogger::ERROR);
        shared_ptr<AbstractLogger> fileLogger = make_shared<FileLogger>(AbstractLogger::DEBUG);
        shared_ptr<AbstractLogger> consoleLogger = make_shared<ConsoleLogger>(AbstractLogger::INFO);

        errorLogger->setNextLogger(fileLogger);
        fileLogger->setNextLogger(consoleLogger);

        return errorLogger;
    }
}

int main()
{
#if 0
    ns1::SalaryHandler sh;
    sh.raiseRequest("zs", 15000); // 张三要求加薪1.5万
    sh.raiseRequest("ls", 3500);  // 李四要求加薪3千5
    sh.raiseRequest("we", 800);   // 王二要求加薪8百
#endif

#if 0
    using namespace ns2;
    //(1)创建出职责链中包含的各个对象（部门经理、技术总监、总经理）
    shared_ptr<ParSalApprover> pzzlinkobj3(new genManager_SA());
    shared_ptr<ParSalApprover> pzzlinkobj2(new CTO_SA(pzzlinkobj3));
    shared_ptr<ParSalApprover> pzzlinkobj1(new depManager_SA(pzzlinkobj2));

    //(2)将这些对象串在一起构成职责链（链表），现在职责链中pzzlinkobj1排在最前面，pzzlinkobj3排在最后面
    // pzzlinkobj1->setNextChain(pzzlinkobj2);
    // pzzlinkobj2->setNextChain(pzzlinkobj3);
    // pzzlinkobj3->setNextChain(nullptr); //可以不写此行，因为ParSalApprover构造函数中设置了m_nextChain为nullptr

    //(3)创建几位员工关于加薪的请求（对象）
    RaiseRequest emp1Req("zs", 15000); // 张三要求加薪1.5万
    RaiseRequest emp2Req("ls", 3500);  // 李四要求加薪3千5
    RaiseRequest emp3Req("we", 800);   // 王二要求加薪8百

    // 看看每位员工的加薪请求由职责链中的哪个对象（部门经理、技术总监、总经理）来处理，从职责链中排在最前面的接收者（pzzlinkobj1）开始
    pzzlinkobj1->processRequest(emp1Req);
    pzzlinkobj1->processRequest(emp2Req);
    pzzlinkobj1->processRequest(emp3Req);
#endif

#if 0
    using namespace ns3;
    //(1)创建出职责链中包含的各个对象（性敏感词过滤器、脏话词过滤器、政治敏感词过滤器）
    shared_ptr<ParWordFilter> pwflinkobj1(new SexyWordFilter());
    shared_ptr<ParWordFilter> pwflinkobj2(new DirtyWordFilter());
    shared_ptr<ParWordFilter> pwflinkobj3(new PoliticsWordFilter());

    //(2)将这些对象串在一起构成职责链（链表），现在职责链中pwflinkobj1排在最前面，pwflinkobj3排在最后面
    pwflinkobj1->setNextChain(pwflinkobj2);
    pwflinkobj2->setNextChain(pwflinkobj3);
    pwflinkobj3->setNextChain(nullptr);

    // 从职责链中排在最前面的接收者（pwflinkobj1）开始，processRequest的参数代表的是聊天内容
    string strWordFilterResult = pwflinkobj1->processRequest("Hello, here is an example of filtering sensitive words test");
    cout << "The result of filtering sensitive words is: " << strWordFilterResult << endl;
#endif

#if 1
    using namespace ns4;
    shared_ptr<AbstractLogger> loggerChain = getChainOfLoggers();
    loggerChain->logMessage(AbstractLogger::INFO, "This is an information.");
    loggerChain->logMessage(AbstractLogger::DEBUG, "This is a debug level information.");
    loggerChain->logMessage(AbstractLogger::ERROR, "This is an error information.");
#endif

    cout << "Over!\n";
    return 0;
}
