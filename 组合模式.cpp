#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <algorithm>
#include <list>
using namespace std;

// 主要用来处理树形结构的数据
// 将一组对象（文件和目录）组织成树形结构以表示“部分-整体”的层次结构（目录中包含文件和子目录）
// 使得用户对单个对象（文件）和组合对象（目录）的操作/使用/处理（递归遍历并执行ShowName等）具有一致性

// 3种角色
// Component（抽象组件），为树枝和树叶定义接口（增加，删除，获取子节点等）
// Leaf（叶子组件），表示树叶节点对象，没有子节点
// Composite（树枝组件），表示容器（树枝）节点对象，可以包含子节点树枝或树叶

// 透明组合模式
// 所有组件都有相同接口，即使叶节点中无意义（Add，Remove）

// 安全组合模式
// 不够透明，管理和访问子节点的成员函数（接口）不在抽象组件中定义

namespace ns1
{
    class File // 文件相关类
    {
        string m_sname; // 文件名
    public:
        File(const string &name) : m_sname(name) {}
        void ShowName(const string &lvlstr) const // 显示文件名
        {
            cout << lvlstr << "-" << m_sname << endl; // 显示"-"代表是一个文件，属末端节点（不会再有子节点）
        }
    };

    class Dir // 目录相关类
    {
        string m_sname;                     // 目录名
        list<shared_ptr<File>> m_childFile; // 文件列表
        list<shared_ptr<Dir>> m_childDir;   // 子目录列表

    public:
        Dir(const string &name) : m_sname(name) {}

    public:
        void AddFile(const shared_ptr<File> &pfile) { m_childFile.push_back(pfile); }
        void AddDir(const shared_ptr<Dir> &pdir) { m_childDir.push_back(pdir); }

        // 显示目录名，同时也要负责其下面的文件和目录名的显示工作
        void ShowName(const string &lvlstr) const // lvlstr：为了显示层次关系的缩进字符串内容
        {
            // （1）输出本目录名
            cout << lvlstr << "+" << m_sname << endl; // 显示"+"代表是一个目录，其中会包含其他内容
            // （2）输出所包含的文件名
            for (auto iter = m_childFile.cbegin(); iter != m_childFile.cend(); ++iter)
                (*iter)->ShowName(lvlstr + "    "); // 显示文件名
            // （3）输出所包含的目录名
            for (auto iter = m_childDir.cbegin(); iter != m_childDir.cend(); ++iter)
                (*iter)->ShowName(lvlstr + "    "); // 显示目录名，这里涉及到了递归调用
        }
    };
}

namespace ns2
{
    class FileSystem // 抽象接口
    {
    public:
        virtual ~FileSystem() {}
        virtual void ShowName(int level) const = 0;                     // 显示名字，参数level用于表示显示的层级，用于显示对齐
        virtual int Add(const shared_ptr<FileSystem> &pfilesys) = 0;    // 向当前目录中增加文件或子目录
        virtual int Remove(const shared_ptr<FileSystem> &pfilesys) = 0; // 从当前目录中移除文件或子目录
    };

    class File : public FileSystem // 文件相关类
    {
        string m_sname; // 文件名
    public:
        File(const string &name) : m_sname(name) {}
        void ShowName(int level) const
        {
            for (int i = 0; i < level; ++i) // 显示若干个空格用与对齐
                cout << "    ";
            cout << "-" << m_sname << endl;
        }
        int Add(const shared_ptr<FileSystem> &pfilesys) override { return -1; }
        int Remove(const shared_ptr<FileSystem> &pfilesys) override { return -1; }
    };

    class Dir : public FileSystem // 目录相关类
    {
        string m_sname;                       // 文件名
        list<shared_ptr<FileSystem>> m_child; // 目录中包含的文件或其他目录列表
    public:
        Dir(const string &name) : m_sname(name) {}

        void ShowName(int level) const
        {
            // （1）显示若干个空格用与对齐
            for (int i = 0; i < level; ++i)
                cout << "    ";
            // （2）输出本目录名
            cout << "+" << m_sname << endl;
            // （3）输出所包含的子内容（可能是文件，也可能是子目录）
            for (auto iter = m_child.cbegin(); iter != m_child.cend(); ++iter)
                (*iter)->ShowName(level + 1);
        }

        int Add(const shared_ptr<FileSystem> &pfilesys) override
        {
            m_child.push_back(pfilesys);
            return 0;
        }

        int Remove(const shared_ptr<FileSystem> &pfilesys) override
        {
            m_child.remove(pfilesys);
            return 0;
        }
    };
}

namespace ns3
{
    class Dir;
    class FileSystem // 抽象接口
    {
    public:
        virtual void ShowName(int level) const = 0; // 显示名字，参数level用于表示显示的层级，用于显示对齐
        virtual ~FileSystem() {}

    public:
        virtual Dir *ifCompositeObj() { return nullptr; }
        virtual int countNumOfFiles() const = 0; // 统计目录下包含的文件个数
    };

    class File : public FileSystem // 文件相关类
    {
        string m_sname; // 文件名
    public:
        File(const string &name) : m_sname(name) {}
        void ShowName(int level) const override
        {
            for (int i = 0; i < level; ++i)
                cout << "    ";
            cout << "-" << m_sname << endl;
        }

    public:
        int countNumOfFiles() const override { return 1; }
    };

    class Dir : public FileSystem
    {
        string m_sname;                       // 文件名
        list<shared_ptr<FileSystem>> m_child; // 目录中包含的文件或其他目录列表
    public:
        Dir(const string &name) : m_sname(name) {}
        void ShowName(int level) const override
        {
            // （1）显示若干个空格用与对齐
            for (int i = 0; i < level; ++i)
                cout << "    ";
            // （2）输出本目录名
            cout << "+" << m_sname << endl;
            // （3）输出所包含的子内容（可能是文件，也可能是子目录）
            for (auto iter = m_child.cbegin(); iter != m_child.cend(); ++iter)
                (*iter)->ShowName(level + 1);
        }
        int Add(const shared_ptr<FileSystem> &pfilesys)
        {
            m_child.push_back(pfilesys);
            return 0;
        }
        int Remove(const shared_ptr<FileSystem> &pfilesys)
        {
            m_child.remove(pfilesys);
            return 0;
        }

    public:
        Dir *ifCompositeObj() override { return this; }

    public:
        int countNumOfFiles() const override
        {
            int iNumOfFiles = 0;
            for (auto iter = m_child.cbegin(); iter != m_child.cend(); ++iter)
                iNumOfFiles += (*iter)->countNumOfFiles(); // 递归调用
            return iNumOfFiles;
        }
    };
}

namespace ns4
{
    class Employee
    {
        string name;
        string dept;
        int salary;
        vector<shared_ptr<Employee>> subordinates;

    public:
        Employee(const string &m_name, const string &m_dept, int m_salary) : name(m_name), dept(m_dept), salary(m_salary) {}
        void add(const shared_ptr<Employee> &e) { subordinates.push_back(e); }
        void remove(const shared_ptr<Employee> &e)
        {
            auto it = find(subordinates.cbegin(), subordinates.cend(), e);
            if (it != subordinates.cend())
                subordinates.erase(it);
        }
        const vector<shared_ptr<Employee>> &getSubordinates() const { return subordinates; }

        string toString()
        {
            return ("Employee :[ Name : " + name + ", dept : " + dept + ", salary :" + to_string(salary) + " ]");
        }
    };
}

int main()
{
#if 0
    using namespace ns1;
    // （1）创建各种目录，文件对象
    shared_ptr<Dir> pdir1(new Dir("root"));
    //-------
    shared_ptr<File> pfile1(new File("common.mk"));
    shared_ptr<File> pfile2(new File("config.mk"));
    shared_ptr<File> pfile3(new File("makefile"));
    //-------
    shared_ptr<Dir> pdir2(new Dir("app"));
    shared_ptr<File> pfile4(new File("nginx.c"));
    shared_ptr<File> pfile5(new File("ngx_conf.c"));
    //-------
    shared_ptr<Dir> pdir3(new Dir("signal"));
    shared_ptr<File> pfile6(new File("ngx_signal.c"));
    //-------
    shared_ptr<Dir> pdir4(new Dir("_include"));
    shared_ptr<File> pfile7(new File("ngx_func.h"));
    shared_ptr<File> pfile8(new File("ngx_signal.h"));

    // （2）构造树形目录结构
    pdir1->AddFile(pfile1);
    pdir1->AddFile(pfile2);
    pdir1->AddFile(pfile3);
    //-------
    pdir1->AddDir(pdir2);
    pdir2->AddFile(pfile4);
    pdir2->AddFile(pfile5);
    //-------
    pdir1->AddDir(pdir3);
    pdir3->AddFile(pfile6);
    //-------
    pdir1->AddDir(pdir4);
    pdir4->AddFile(pfile7);
    pdir4->AddFile(pfile8);

    // （3）输出整个目录结构，只要调用根目录的ShowName方法即可，每个子目录都有自己的ShowName方法负责自己旗下的文件和目录的显示
    pdir1->ShowName(""); // 缩进字符刚开始可以为空
#endif

#if 0
    using namespace ns2;
    // （1）创建各种目录，文件对象
    shared_ptr<FileSystem> pdir1(new Dir("root"));
    //-------
    shared_ptr<FileSystem> pfile1(new File("common.mk"));
    shared_ptr<FileSystem> pfile2(new File("config.mk"));
    shared_ptr<FileSystem> pfile3(new File("makefile"));
    //-------
    shared_ptr<FileSystem> pdir2(new Dir("app"));
    shared_ptr<FileSystem> pfile4(new File("nginx.c"));
    shared_ptr<FileSystem> pfile5(new File("ngx_conf.c"));
    //-------
    shared_ptr<FileSystem> pdir3(new Dir("signal"));
    shared_ptr<FileSystem> pfile6(new File("ngx_signal.c"));
    //-------
    shared_ptr<FileSystem> pdir4(new Dir("_include"));
    shared_ptr<FileSystem> pfile7(new File("ngx_func.h"));
    shared_ptr<FileSystem> pfile8(new File("ngx_signal.h"));

    // （2）构造树形目录结构
    pdir1->Add(pfile1);
    pdir1->Add(pfile2);
    pdir1->Add(pfile3);
    //-------
    pdir1->Add(pdir2);
    pdir2->Add(pfile4);
    pdir2->Add(pfile5);
    //-------
    pdir1->Add(pdir3);
    pdir3->Add(pfile6);
    //-------
    pdir1->Add(pdir4);
    pdir4->Add(pfile7);
    pdir4->Add(pfile8);

    // （3）输出整个目录结构，只要调用根目录的ShowName方法即可，每个子目录都有自己的ShowName方法负责自己旗下的文件和目录的显示
    pdir1->ShowName(0);
#endif

#if 0
    using namespace ns3;
    // （1）创建各种目录，文件对象
    shared_ptr<Dir> pdir1(new Dir("root"));
    //-------
    shared_ptr<FileSystem> pfile1(new File("common.mk"));
    shared_ptr<FileSystem> pfile2(new File("config.mk"));
    shared_ptr<FileSystem> pfile3(new File("makefile"));
    //-------
    shared_ptr<Dir> pdir2(new Dir("app"));
    if (pdir2->ifCompositeObj() != nullptr)
    {
        // 是个组合对象，可以向其中增加单个对象和其它组合对象
    }
    if (dynamic_pointer_cast<Dir>(pdir2) != nullptr)
    {
        // 是个组合对象，可以向其中增加单个对象和其它组合对象
    }

    shared_ptr<FileSystem> pfile4(new File("nginx.c"));
    shared_ptr<FileSystem> pfile5(new File("ngx_conf.c"));
    //-------
    shared_ptr<Dir> pdir3(new Dir("signal"));
    shared_ptr<FileSystem> pfile6(new File("ngx_signal.c"));
    //-------
    shared_ptr<Dir> pdir4(new Dir("_include"));
    shared_ptr<FileSystem> pfile7(new File("ngx_func.h"));
    shared_ptr<FileSystem> pfile8(new File("ngx_signal.h"));

    // （2）构造树形目录结构
    pdir1->Add(pfile1);
    pdir1->Add(pfile2);
    pdir1->Add(pfile3);
    //-------
    pdir1->Add(pdir2);
    pdir2->Add(pfile4);
    pdir2->Add(pfile5);
    //-------
    pdir1->Add(pdir3);
    pdir3->Add(pfile6);
    //-------
    pdir1->Add(pdir4);
    pdir4->Add(pfile7);
    pdir4->Add(pfile8);

    // （3）输出整个目录结构，只要调用根目录的ShowName方法即可，每个子目录都有自己的ShowName方法负责自己旗下的文件和目录的显示
    pdir1->ShowName(0);
    cout << "number of files: " << pdir1->countNumOfFiles() << endl;
#endif

#if 1
    using namespace ns4;

    shared_ptr<Employee> clerk1(new Employee("Laura", "Marketing", 10000));
    shared_ptr<Employee> clerk2(new Employee("Bob", "Marketing", 10000));
    shared_ptr<Employee> headMarketing(new Employee("Michel", "Head Marketing", 20000));
    headMarketing->add(clerk1);
    headMarketing->add(clerk2);

    shared_ptr<Employee> salesExecutive1(new Employee("Richard", "Sales", 10000));
    shared_ptr<Employee> salesExecutive2(new Employee("Rob", "Sales", 10000));
    shared_ptr<Employee> headSales(new Employee("Robert", "Head Sales", 20000));
    headSales->add(salesExecutive1);
    headSales->add(salesExecutive2);

    shared_ptr<Employee> CEO(new Employee("John", "CEO", 30000));
    CEO->add(headSales);
    CEO->add(headMarketing);
    cout << CEO->toString() << endl;
    cout << "------------------------------------------------------------" << endl;
    for (const shared_ptr<Employee> &e : CEO->getSubordinates())
    {
        cout << e->toString() << endl;
        for (const shared_ptr<Employee> &ee : e->getSubordinates())
            cout << ee->toString() << endl;
    }
#endif

    cout << "Over!\n";
    return 0;
}
