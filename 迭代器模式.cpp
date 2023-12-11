#include <iostream>
#include <vector>
#include <list>
#include <memory>
using namespace std;

// 遍历容器中的元素
// 迭代器单独实现为类模板，与容器模板解耦
// 提供一种方法顺序访问一个聚合对象（容器）中各个元素，而又不暴露该对象的内部表示（实现代码）

// 4种角色
// Iterator（抽象迭代器），定义访问和遍历容器中元素的接口
// ConcreteIterator（具体迭代器），具体遍历聚合对象（容器）中元素，记录当前元素位置
// Aggregate（抽象聚合），容器，存储和管理元素对象，声明CreateIterator方法创建迭代器对象，充当创建迭代器的工厂角色
// ConcreteAggregate（具体聚合），返回ConcreteIterator实例

// 单一职责原则
// 一个类的职责应该单一，只做一件事情或者对外只提供一种功能

// 内聚性，功能角度度量模块或类内部组成部分彼此结合紧密程度
// 耦合性，模块间相互关联程度。
// 内容耦合，一个模块直接修改或操作另一个模块的数据
// 公共耦合，多个模块共同引用全局数据项
// 外部耦合，一组模块不是通过参数表访问同一全局简单变量
// 控制耦合，一个模块通过控制信号（传送开关、标志、名字等控制信息），明显地控制另一个模块的功能
// 标记耦合，两个模块间传递数据结构地址
// 数据耦合，模块间通过参数传递数据

// 高类聚、低耦合

namespace ns1
{
    template <typename T> // 抽象迭代器类模板
    class myIter
    {
    public:
        virtual ~myIter() {}
        virtual void First() = 0;     // 指向容器中第一个元素
        virtual void Next() = 0;      // 指向下一个元素
        virtual bool IsDone() = 0;    // 是否遍历完
        virtual T &CurrentItem() = 0; // 获取当前的元素
    };

    template <typename T> // 抽象容器类模板
    class myCotainer
    {
    public:
        virtual ~myCotainer() {}
        // virtual myIter<T> *CreateIterator() = 0; // 创建迭代器
        virtual shared_ptr<myIter<T>> CreateIterator() = 0; // 创建迭代器
        virtual T &getItem(int index) = 0;                  // 获取当前元素
        virtual int getSize() const = 0;                    // 容器中元素数量
    };

    template <typename T> // 具体迭代器类模板，针对10元素数组
    class myVectorIter : public myIter<T>
    {
        myCotainer<T> *myvector;
        int m_current; // 记录数组的当前下标（迭代器在当前容器中的位置）
    public:
        myVectorIter(myCotainer<T> *tmpc) : myvector(tmpc), m_current(0) {}
        void First() override { m_current = 0; } // 容器（数组）中的第一个元素下标为0
        void Next() override { m_current++; }    // 下标+1，意味着数组中的下一个元素
        bool IsDone() override { return m_current >= myvector->getSize(); }
        T &CurrentItem() override { return myvector->getItem(m_current); }
    };

    template <typename T> // 具体容器类模板
    class myVector : public myCotainer<T>
    {
    private:
        T m_elem[10]; // 固定10元素数组

    public:
        myVector()
        {
            for (int i = 0; i < 10; ++i)
                m_elem[i] = i;
        }
        // myIter<T> *CreateIterator() override { return new myVectorIter<T>(this); }
        shared_ptr<myIter<T>> CreateIterator() override { return make_shared<myVectorIter<T>>(this); }
        T &getItem(int index) override { return m_elem[index]; }
        int getSize() const override { return 10; }
    };
}

int main()
{
#if 0
    vector<int> msgVector;
    msgVector.push_back(1); // 末尾插入1，vector中内容：1
    msgVector.push_back(2); // 末尾插入2，vector中内容：1,2
    msgVector.push_back(3); // 末尾插入3，vector中内容：1,2,3
    for (vector<int>::const_iterator pos = msgVector.cbegin(); pos != msgVector.cend(); ++pos)
        cout << *pos << endl;
    cout << "-------------------" << endl;
    list<int> msgList;
    msgList.push_back(1);  // 末尾插入1，list中内容：1
    msgList.push_front(2); // 开头插入2，list中内容：2，1
    msgList.push_back(3);  // 末尾插入3，list中内容：2，1，3
    for (list<int>::const_iterator pos = msgList.cbegin(); pos != msgList.cend(); ++pos)
        cout << *pos << endl;
#endif

#if 0
    using namespace ns1;
    shared_ptr<myCotainer<int>> pcontainer(new myVector<int>());
    shared_ptr<myIter<int>> iter(pcontainer->CreateIterator());
    // 遍历容器中的元素
    for (iter->First(); !iter->IsDone(); iter->Next()) // 多态机制的遍历，效率上不好，尽量考虑栈机制
        cout << iter->CurrentItem() << endl;
#endif

#if 1
    using namespace ns1;
    shared_ptr<myCotainer<int>> pcontainer(new myVector<int>());
    myVectorIter<int> iter(pcontainer.get());
    // 遍历容器中的元素
    for (iter.First(); !iter.IsDone(); iter.Next()) // 非多态机制，可以明显改善程序性能
        cout << iter.CurrentItem() << endl;
#endif

    cout << "Over!\n";
    return 0;
}
