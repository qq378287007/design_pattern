#include <iostream>
#include <map>
#include <memory>
using namespace std;

// 中介者封装一系列的对象交互
// 4种角色
// Mediator（抽象中介者类），定义接口，各个同事类通过该接口与中介者通信
// ConcreteMediator（具体中介者类），实现接口，保存同事类指针用于与同事对象通信实现协作行为
// Colleague（抽象同事类），定义同事共有方法和抽象方法，同时维持中介者对象指针用于通信
// ConcreteColleague（具体同事类），实现方法
// 可结合观察者模式，即中介者实现为观察者，各个同事类实现为观察目标

namespace ns1
{
    class CtlParent // UI控件类
    {
    protected:
        string m_caption; // 控件上面显示的文字内容，本范例假设每个UI控件上的文字都不同
    public:
        virtual ~CtlParent() {}
        CtlParent(const string &caption) : m_caption(caption) {}

    public:                                                                         // 当UI控件发生变化时该成员函数会被调用
        virtual void Changed(map<string, shared_ptr<CtlParent>> &tmpuictllist) = 0; // 形参所代表的map容器中包含着所有对话框中涉及到的UI控件
        virtual void Enable(bool sign) const = 0;                                   // 设置UI控件启用或禁用
    };

    class Button : public CtlParent // 普通按钮相关类
    {
    public:
        Button(const string &caption) : CtlParent(caption) {}

    public:
        void Enable(bool sign) const override // 设置按钮的启用或禁用
        {
            if (sign)
                cout << "button \"" << m_caption << "\" enable" << endl;
            else
                cout << "button \"" << m_caption << "\" disable" << endl;
        }

        // 按钮被按下时该成员函数会被调用
        void Changed(map<string, shared_ptr<CtlParent>> &tmpuictllist) override;
    };

    class RadioBtn : public CtlParent // 单选按钮相关类
    {
    public:
        RadioBtn(const string &caption) : CtlParent(caption) {}

    public:
        void Enable(bool sign) const override {} // 设置单选按钮的启用或禁用
        // 设置单选按钮为被选中或者被取消选中，被选中的单选按钮中间有个黑色实心圆点
        void Selected(bool sign) const
        {
            if (sign)
                cout << "radio button \"" << m_caption << "\" selected" << endl;
            else
                cout << "radio button \"" << m_caption << "\" not selected" << endl;
        }
        // 单选按钮被单击时该成员函数会被调用
        void Changed(map<string, shared_ptr<CtlParent>> &tmpuictllist) override;
    };

    class EditCtl : public CtlParent // 编辑框相关类
    {
        string m_content = ""; // 编辑框中的内容
    public:
        EditCtl(const string &caption) : CtlParent(caption) {}

    public:
        void Enable(bool sign) const override // 设置编辑框的启用或禁用
        {
            if (sign)
                cout << "edit box \"" << m_caption << "\" enable" << endl;
            else
                cout << "edit box \"" << m_caption << "\" disable" << endl;
        }
        // 是否编辑框中的内容为空
        bool isContentEmpty() const { return m_content.empty(); }
        // 编辑框内容发生变化时该成员函数会被调用
        void Changed(map<string, shared_ptr<CtlParent>> &tmpuictllist) override;
    };

    // 按钮被按下时该成员函数会被调用
    void Button::Changed(map<string, shared_ptr<CtlParent>> &tmpuictllist)
    {
        if (m_caption == "sign in") // 按下的是登录按钮
            cout << "Start the game login verification, and decide whether to enter the game or give a prompt according to the verification result!" << endl;
        else if (m_caption == "exit") // 按下的是退出按钮，则退出整个游戏
            cout << "Game exit, goodbye!" << endl;
    }

    // 单选按钮被单击时该成员函数会被调用
    void RadioBtn::Changed(map<string, shared_ptr<CtlParent>> &tmpuictllist)
    {
        if (m_caption == "visitor login")
        {
            static_pointer_cast<RadioBtn>(tmpuictllist["visitor login"])->Selected(true);  // “游客登录”单选按钮设置为选中
            static_pointer_cast<RadioBtn>(tmpuictllist["account login"])->Selected(false); // “账号登录”单选按钮设置为取消选中

            tmpuictllist["account"]->Enable(false);  // “账号”编辑框设置为禁用
            tmpuictllist["password"]->Enable(false); // “密码”编辑框设置为禁用

            tmpuictllist["sign in"]->Enable(true); // “登录”按钮设置为启用
        }
        else if (m_caption == "account login")
        {
            static_pointer_cast<RadioBtn>(tmpuictllist["visitor login"])->Selected(false); // “游客登录”单选按钮设置为取消选中
            static_pointer_cast<RadioBtn>(tmpuictllist["account login"])->Selected(true);  // “账号登录”单选按钮设置为选中

            tmpuictllist["account"]->Enable(true);  // “账号”编辑框设置为启用
            tmpuictllist["password"]->Enable(true); // “密码”编辑框设置为启用

            // 如果“账号”编辑框或者“密码”编辑框有一个为空，则不允许登录
            if (static_pointer_cast<EditCtl>(tmpuictllist["account"])->isContentEmpty() || static_pointer_cast<EditCtl>(tmpuictllist["password"])->isContentEmpty())
                tmpuictllist["sign in"]->Enable(false); // “登录”按钮设置为禁用
            else
                tmpuictllist["sign in"]->Enable(true); // “登录”按钮设置为启用
        }
    }

    // 编辑框内容发生变化时该成员函数会被调用
    void EditCtl::Changed(map<string, shared_ptr<CtlParent>> &tmpuictllist)
    {
        // 如果“账号”编辑框或者“密码”编辑框有一个为空，则不允许登录
        if (static_pointer_cast<EditCtl>(tmpuictllist["account"])->isContentEmpty() || static_pointer_cast<EditCtl>(tmpuictllist["password"])->isContentEmpty())
            tmpuictllist["sign in"]->Enable(false); // “登录”按钮设置为禁用
        else
            tmpuictllist["sign in"]->Enable(true); // “登录”按钮设置为启用
    }
}

namespace ns2
{
    class CtlParent; // 类前向声明
    class Mediator   // 中介者父类
    {
    public:
        virtual ~Mediator() {}

    public:
        virtual void createCtrl() = 0;                 // 创建所有需要用到的UI控件
        virtual void ctlChanged(CtlParent *const) = 0; // 当某个UI控件发生变化时调用中介者对象的该成员函数来通知中介者
    };

    class CtlParent // UI控件类的父类
    {
    protected:
        Mediator *m_pmediator; // 指向中介者对象的指针
        string m_caption;      // 控件上面显示的文字内容，可能并不是所有控件都需要但这里为显示方便依旧引入
    public:
        virtual ~CtlParent() {}
        CtlParent(Mediator *const ptmpm, const string &caption) : m_pmediator(ptmpm), m_caption(caption) {}

    public:                                                       // 当UI控件发生变化时该成员函数会被调用
        virtual void Changed() { m_pmediator->ctlChanged(this); } // 通知中介者对象，所有事情让中介者对象去做
        virtual void Enable(bool sign) = 0;                       // 设置UI控件启用或禁用
    };

    class Button : public CtlParent // 普通按钮相关类
    {
    public:
        Button(Mediator *const ptmpm, const string &caption) : CtlParent(ptmpm, caption) {}
        void Enable(bool sign) override // 设置按钮的启用或禁用
        {
            if (sign == true)
                cout << "button \"" << m_caption << "\" enable" << endl;
            else
                cout << "button \"" << m_caption << "\" disable" << endl;
        }
    };

    class RadioBtn : public CtlParent // 单选按钮相关类
    {
    public:
        RadioBtn(Mediator *const ptmpm, const string &caption) : CtlParent(ptmpm, caption) {}
        void Enable(bool sign) override {} // 设置单选按钮的启用或禁用
        void Selected(bool sign)           // 设置单选按钮为被选中或者被取消选中，被选中的单选按钮中间有个黑色实心圆点
        {
            if (sign == true)
                cout << "radio button \"" << m_caption << "\" selected" << endl;
            else
                cout << "radio button \"" << m_caption << "\" not selected" << endl;
        }
    };

    class EditCtl : public CtlParent // 编辑框相关类
    {
        string m_content = ""; // 编辑框中的内容
    public:
        EditCtl(Mediator *const ptmpm, const string &caption) : CtlParent(ptmpm, caption) {}
        void Enable(bool sign) override // 设置编辑框的启用或禁用
        {
            if (sign == true)
                cout << "edit box \"" << m_caption << "\" enable" << endl;
            else
                cout << "edit box \"" << m_caption << "\" disable" << endl;
        }
        bool isContentEmpty() const { return m_content.empty(); } // 是否编辑框中的内容为空
    };

    class concrMediator : public Mediator // 具体中介者类
    {
    public:                           // 为方便外界使用，这里以public修饰，实际项目中可以写一个成员函数来return这些指针
        shared_ptr<Button> mp_login;  // 登录按钮
        shared_ptr<Button> mp_logout; // 退出按钮

        shared_ptr<RadioBtn> mp_rbtn1; // 游客登录单选按钮
        shared_ptr<RadioBtn> mp_rbtn2; // 账号登录单选按钮

        shared_ptr<EditCtl> mp_edtctl1; // 账号编辑框
        shared_ptr<EditCtl> mp_edtctl2; // 密码编辑框
    public:
        void createCtrl() override // 创建各种UI控件
        {
            // 当然，各种UI控件对象在外面创建，然后把地址传递进来也可以
            mp_login = make_shared<Button>(this, "sign in");
            mp_logout = make_shared<Button>(this, "exit");

            mp_rbtn1 = make_shared<RadioBtn>(this, "visitor login");
            mp_rbtn2 = make_shared<RadioBtn>(this, "account login");

            mp_edtctl1 = make_shared<EditCtl>(this, "account editing box");
            mp_edtctl2 = make_shared<EditCtl>(this, "password editing box");

            // 设置一下缺省的UI控件状态
            mp_rbtn1->Selected(true);  // “游客登录”单选按钮设置为选中
            mp_rbtn2->Selected(false); // “账号登录”单选按钮设置为取消选中

            mp_edtctl1->Enable(false); // “账号”编辑框设置为禁用
            mp_edtctl2->Enable(false); // “密码”编辑框设置为禁用

            mp_login->Enable(true);  // “登录”按钮设置为启用
            mp_logout->Enable(true); // “退出”按钮设置为启用
        }

        // 当某个UI控件发生变化时调用中介者对象的该成员函数来通知中介者
        virtual void ctlChanged(CtlParent *const p_ctrl)
        {
            if (p_ctrl == mp_login.get()) // 登录按钮被单击
                cout << "Start the game login verification, and decide whether to enter the game or give a prompt according to the verification result!" << endl;
            else if (p_ctrl == mp_logout.get())        // 退出按钮被单击
                cout << "Game exit, goodbye!" << endl; // 退出整个游戏

            if (p_ctrl == mp_rbtn1.get()) // 游客登录单选按钮被单击
            {
                mp_rbtn1->Selected(true);  // “游客登录”单选按钮设置为选中
                mp_rbtn2->Selected(false); // “账号登录”单选按钮设置为取消选中

                mp_edtctl1->Enable(false); // “账号”编辑框设置为禁用
                mp_edtctl2->Enable(false); // “密码”编辑框设置为禁用

                mp_login->Enable(true); // “登录”按钮设置为启用
            }
            else if (p_ctrl == mp_rbtn2.get()) // 账号登录单选按钮被单击
            {
                mp_rbtn1->Selected(false); // “游客登录”单选按钮设置为取消选中
                mp_rbtn2->Selected(true);  // “账号登录”单选按钮设置为选中

                mp_edtctl1->Enable(true); // “账号”编辑框设置为启用
                mp_edtctl2->Enable(true); // “密码”编辑框设置为启用

                if (mp_edtctl1->isContentEmpty() || mp_edtctl2->isContentEmpty()) // 如果“账号”编辑框或者“密码”编辑框有一个为空，则不允许登录
                    mp_login->Enable(false);                                      // “登录”按钮设置为禁用
                else
                    mp_login->Enable(true); // “登录”按钮设置为启用
            }

            if (p_ctrl == mp_edtctl1.get() || p_ctrl == mp_edtctl2.get())         // 账号或密码编辑框内容发生改变
                if (mp_edtctl1->isContentEmpty() || mp_edtctl2->isContentEmpty()) // 如果“账号”编辑框或者“密码”编辑框有一个为空，则不允许登录
                    mp_login->Enable(false);                                      // “登录”按钮设置为禁用
                else
                    mp_login->Enable(true); // “登录”按钮设置为启用
        }
    };
}

int main()
{
#if 0
    using namespace ns1;

    // 创建各种UI控件
    map<string, shared_ptr<CtlParent>> uictllist; // 将所有创建的UI控件保存到map容器中，方便进行参数传递
    uictllist["sign in"] = make_shared<Button>("sign in");
    uictllist["exit"] = make_shared<Button>("exit");

    uictllist["visitor login"] = make_shared<RadioBtn>("visitor login");
    uictllist["account login"] = make_shared<RadioBtn>("account login");

    uictllist["account"] = make_shared<EditCtl>("account");
    uictllist["password"] = make_shared<EditCtl>("password");

    // 设置一下缺省的UI控件状态
    // 因为只有子类型才有Selected成员函数，所以这里需要用到强制类型转换
    static_pointer_cast<RadioBtn>(uictllist["visitor login"])->Selected(true);  // “游客登录”单选按钮设置为选中
    static_pointer_cast<RadioBtn>(uictllist["account login"])->Selected(false); // “账号登录”单选按钮设置为取消选中

    uictllist["account"]->Enable(false);  // “账号”编辑框设置为禁用
    uictllist["password"]->Enable(false); // “密码”编辑框设置为禁用

    uictllist["sign in"]->Enable(true); // “登录”按钮设置为启用
    uictllist["exit"]->Enable(true);    // “退出”按钮设置为启用

    cout << "--------------------------" << endl;
    uictllist["account login"]->Changed(uictllist); // 模拟“账号登录”单选按钮被单击选中
#endif

#if 1
    using namespace ns2;
    concrMediator mymedia;
    mymedia.createCtrl();
    cout << "-------------when \"account login\" radio button pressed:-------------" << endl;
    // 模拟“账号”按钮被按下，则去通知中介者，由中介者实现具体的逻辑处理
    mymedia.mp_rbtn2->Changed();
#endif

    cout << "Over!\n";
    return 0;
}