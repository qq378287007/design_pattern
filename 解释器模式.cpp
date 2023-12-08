#include <iostream>
#include <map>
#include <stack>
#include <vector>
#include <cstring>
#include <memory>
#include <set>
#include <sstream>
using namespace std;

// 定义一个语言的文法（语法规则），并建立一个解释器解释该语言中的句子
// 4种角色
// AbstractExpression（抽象表达式），声明抽象的解释操作
// TerminalExpression（终结符表达式），实现语言文法中与终结表达式相关的解释操作
// NonterminalExpression（非终结符表达式），实现语言文法中与非终结表达式相关的解释操作
// Context（环境类/上下文类），存储解释器之外的全局信息，变量名与值的映射关系、存储和访问表达式解释器的状态等，作为公共对象的参数传递到表达式的解释操作中

namespace ns1
{
    class Expression // 表达式（节点）父类
    {
    public:              // 以下两个成员变量是为程序跟踪调试时观察某些数据方便而引入
        int m_dbg_num;   // 创建该对象时的一个编号，用于记录本对象是第几个创建的
        char m_dbg_sign; // 标记本对象的类型，可能是个字符v代表变量（终结符表达式），也可能是个加减号（非终结符表达式）
    public:
        Expression(int num, char sign) : m_dbg_num(num), m_dbg_sign(sign) {}
        virtual ~Expression() {}

    public:                                                         // 解析语法树中的当前节点
        virtual int interpret(const map<char, int> &var) const = 0; // 变量名及对应的值
    };

    class VarExpression : public Expression // 变量表达式（终结符表达式）
    {
        char m_key; // 变量名，本范例中诸如a、b、c、d都是变量名
    public:
        VarExpression(const char &key, int num, char sign) : Expression(num, sign), m_key(key) {}
        int interpret(const map<char, int> &var) const override { return var.at(m_key); } // 返回变量名对应的数值
    };

    class SymbolExpression : public Expression // 运算符表达式（非终结符表达式）父类
    {
    protected: // 左右各有一个操作数
        shared_ptr<Expression> m_left;
        shared_ptr<Expression> m_right;

    public:
        SymbolExpression(const shared_ptr<Expression> &left, const shared_ptr<Expression> &right, int num, char sign) : m_left(left), m_right(right), Expression(num, sign) {}
        shared_ptr<Expression> getLeft() const { return m_left; }
        shared_ptr<Expression> getRight() const { return m_right; }
    };

    class AddExpression : public SymbolExpression // 加法运算符表达式（非终结符表达式）
    {
    public:
        AddExpression(const shared_ptr<Expression> &left, const shared_ptr<Expression> &right, int num, char sign) : SymbolExpression(left, right, num, sign) {} // 构造函数

        int interpret(const map<char, int> &var) const override
        {
            // 分步骤拆开写，方便理解和观察
            int value1 = m_left->interpret(var);  // 递归调用左操作数的interpret方法
            int value2 = m_right->interpret(var); // 递归调用右操作数的interpret方法
            int result = value1 + value2;
            return result; // 返回两个变量相加的结果
        }
    };

    class SubExpression : public SymbolExpression // 减法运算符表达式（非终结符表达式）
    {
    public:
        SubExpression(const shared_ptr<Expression> &left, const shared_ptr<Expression> &right, int num, char sign) : SymbolExpression(left, right, num, sign) {} // 构造函数

        int interpret(const map<char, int> &var) const override
        {
            int value1 = m_left->interpret(var);
            int value2 = m_right->interpret(var);
            int result = value1 - value2;
            return result; // 返回两个变量相减的结果
        }
    };

    // 分析—创建语法树（表达式树）
    shared_ptr<Expression> analyse(const string &strExp) // strExp：要计算结果的表达式字符串，比如"a-b+c+d"
    {
        stack<shared_ptr<Expression>> expStack; // #include <stack>，这里用到了栈这种顺序容器
        shared_ptr<Expression> left;
        shared_ptr<Expression> right;
        int icount = 1;
        for (size_t i = 0; i < strExp.size(); ++i) // 循环遍历表达式字符串中的每个字符
        {
            switch (strExp[i])
            {
            case '+':                                                       // 加法运算符表达式（非终结符表达式）
                left = expStack.top();                                      // 返回栈顶元素（左操作数）
                right.reset(new VarExpression(strExp[++i], icount++, 'v')); // v代表是个变量节点
                // 在栈顶增加元素
                expStack.push(make_shared<AddExpression>(left, right, icount++, '+')); //'+'代表是个减法运算符节点
                break;
            case '-':                  // 减法运算符表达式（非终结符表达式）
                left = expStack.top(); // 返回栈顶元素
                right.reset(new VarExpression(strExp[++i], icount++, 'v'));
                expStack.push(make_shared<SubExpression>(left, right, icount++, '-')); //'-'代表是个减法运算符节点
                break;
            default: // 变量表达式（终结符表达式）
                expStack.push(make_shared<VarExpression>(strExp[i], icount++, 'v'));
                break;
            }
        }
        shared_ptr<Expression> expression = expStack.top(); // 返回栈顶元素
        return expression;
    }
}

namespace ns2
{
    class Expression // 表达式父类
    {
    public:
        virtual ~Expression() {}

    public: // 解析语法树中的当前节点
        virtual string interpret() const = 0;
    };

    class DirectionExpression : public Expression // 运动方向表达式（终结符表达式）
    {
        string m_direction; // 运动方向：up、down、left、right分别表示上、下、左、右
    public:
        DirectionExpression(const string &direction) : m_direction(direction) {}
        string interpret() const override
        {
            static set<string> directionSet = {"up", "down", "left", "right"};
            if (directionSet.find(m_direction) != directionSet.end())
                return m_direction;
            else
                return "direction error";
        }
    };

    class ActionExpression : public Expression // 运动方式表达式（终结符表达式）
    {
        string m_action; // 运动方式：walk、run分别表示行走、奔跑
    public:
        ActionExpression(const string &action) : m_action(action) {}
        string interpret() const override
        {
            static set<string> actionSet = {"walk", "run"};
            if (actionSet.find(m_action) != actionSet.end())
                return m_action;
            else
                return "action error";
        }
    };

    class DistanceExpression : public Expression // 运动距离表达式（终结符表达式）
    {
        string m_distance; // 运动距离，用字符串表示即可
    public:
        DistanceExpression(const string &distance) : m_distance(distance) {}
        string interpret() const override
        {
            return m_distance + "m";
        }
    };

    class SentenceExpression : public Expression // “句子”表达式（非终结符表达式）,“运动方向 运动方式 运动距离”构成
    {
        shared_ptr<Expression> m_direction; // 运动方向
        shared_ptr<Expression> m_action;    // 运动方式
        shared_ptr<Expression> m_distance;  // 运动距离
    public:
        SentenceExpression(const shared_ptr<Expression> &direction, const shared_ptr<Expression> &action, const shared_ptr<Expression> &distance)
            : m_direction(direction), m_action(action), m_distance(distance) {}
        shared_ptr<Expression> getDirection() const { return m_direction; }
        shared_ptr<Expression> getAction() const { return m_action; }
        shared_ptr<Expression> getDistance() const { return m_distance; }

        string interpret() const override
        {
            return m_direction->interpret() + " " + m_action->interpret() + " " + m_distance->interpret();
        }
    };

    class AndExpression : public Expression // “和”表达式（非终结符表达式）
    {
        shared_ptr<Expression> m_left;
        shared_ptr<Expression> m_right;

    public:
        AndExpression(const shared_ptr<Expression> &left, const shared_ptr<Expression> &right) : m_left(left), m_right(right) {}
        shared_ptr<Expression> getLeft() const { return m_left; }
        shared_ptr<Expression> getRight() const { return m_right; }

        string interpret() const override
        {
            return m_left->interpret() + " and " + m_right->interpret();
        }
    };

    // 分析—创建语法树（表达式树）
    shared_ptr<Expression> analyse(const string &strExp) // strExp：要计算结果的表达式字符串，比如"left walk 15 and down run 20"
    {
        stack<shared_ptr<Expression>> expStack;
        shared_ptr<Expression> direction;
        shared_ptr<Expression> action;
        shared_ptr<Expression> distance;
        shared_ptr<Expression> left;
        shared_ptr<Expression> right;

        // 机器人运动控制命令之间是用空格来分隔的，所以用空格作为分隔字符来对整个字符串进行拆分
        vector<string> resultVec;
        /*
        char *strc = new char[strlen(strExp.c_str()) + 1];
        strcpy(strc, strExp.c_str()); // 若本行编译报错提醒使用strcpy_s，则可以在文件头增加代码行：#pragma warning(disable : 4996)
        char *tmpStr = strtok(strc, " "); // 按空格来切割字符串
        while (tmpStr != nullptr)
        {
            resultVec.push_back(string(tmpStr));
            tmpStr = strtok(NULL, " ");
        }
        delete[] strc;
        */
        stringstream iss(strExp);        // 输入流
        string token;                    // 接收缓冲区
        while (getline(iss, token, ' ')) // 以' '为分隔符
            resultVec.push_back(token);

        for (auto iter = resultVec.begin(); iter != resultVec.end(); ++iter)
        {
            if ((*iter) == "and") // 和
            {
                left = expStack.top(); // 返回栈顶元素（左操作数）
                ++iter;

                direction.reset(new DirectionExpression(*iter)); // 运动方向
                ++iter;
                action.reset(new ActionExpression(*iter)); // 运动方式
                ++iter;
                distance.reset(new DistanceExpression(*iter)); // 运动距离
                right.reset(new SentenceExpression(direction, action, distance));
                expStack.push(make_shared<AndExpression>(left, right));
            }
            else
            {
                direction.reset(new DirectionExpression(*iter)); // 运动方向
                ++iter;
                action.reset(new ActionExpression(*iter)); // 运动方式
                ++iter;
                distance.reset(new DistanceExpression(*iter)); // 运动距离
                expStack.push(make_shared<SentenceExpression>(direction, action, distance));
            }
        }
        shared_ptr<Expression> expression = expStack.top(); // 返回栈顶元素
        return expression;
    }
}

int main()
{
#if 0
    using namespace ns1;
    map<char, int> varmap;
    // 下面是给字符串表达式中所有参与运算的变量一个对应的数值
    varmap.insert(make_pair('a', 7)); // 类似于赋值语句a = 7
    varmap.insert(make_pair('b', 9)); // 类似于赋值语句b = 9
    varmap.insert(make_pair('c', 3)); // 类似于赋值语句c = 3
    varmap.insert(make_pair('d', 2)); // 类似于赋值语句d = 2

    string strExp = "a-b+c+d";                           // 将要求值的字符串表达式
    shared_ptr<Expression> expression = analyse(strExp); // 调用analyse函数创建语法树
    int result = expression->interpret(varmap);          // 调用interpret接口求解字符串表达式的结果
    cout << strExp << " = " << result << endl;           // 输出字符串表达式结果
#endif

#if 1
    using namespace ns2;
    string strExp = "left walk 15 and down run 20";
    shared_ptr<Expression> expression = analyse(strExp); // 调用analyse函数创建语法树
    cout << expression->interpret() << endl;
#endif

    cout << "Over!\n";
    return 0;
}