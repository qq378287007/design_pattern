#include <iostream>
#include <memory>
using namespace std;

// 通过组装方式增强类的功能（动态增加新功能）
// 4种角色
// Control（抽象构件），定义必需接口
// ListCtrl（具体构件），实现接口且用装饰器可给该构件增加额外方法
// Decorator（抽象装饰器类），扩展接口，用于装饰
// BorderDec、VerScrollBarDec、HorScrollBarDec（具体装饰器类），增加新方法扩充构件能力

// 组合复用原则
// 组合优于继承

namespace ns1
{
    class Control
    {
    public:
        virtual void draw() const = 0;

    public:
        virtual ~Control() {}
    };

    class ListCtrl : public Control
    {
    public:
        void draw() const override
        {
            cout << "Draw Common List Controls!" << endl;
        }
    };

    class Decorator : public Control
    {
    public:
        Decorator(const shared_ptr<Control> &tmpctrl) : m_control(tmpctrl) {}
        void draw() const override
        {
            m_control->draw();
        }

    private:
        shared_ptr<Control> m_control;
    };

    class BorderDec : public Decorator
    {
    public:
        BorderDec(const shared_ptr<Control> &tmpctrl) : Decorator(tmpctrl) {}
        void draw() const override
        {
            Decorator::draw();
            drawBorder();
        }

    private:
        void drawBorder() const
        {
            cout << "bound box!" << endl;
        }
    };

    class VerScrollBarDec : public Decorator
    {
    public:
        VerScrollBarDec(const shared_ptr<Control> &tmpctrl) : Decorator(tmpctrl) {}
        void draw() const override
        {
            Decorator::draw();
            drawVerScrollBar();
        }

    private:
        void drawVerScrollBar() const
        {
            cout << "Draw vertical scroll bar!" << endl;
        }
    };

    class HorScrollBarDec : public Decorator
    {
    public:
        HorScrollBarDec(const shared_ptr<Control> &tmpctrl) : Decorator(tmpctrl) {}
        void draw() const override
        {
            Decorator::draw();
            drawHorScrollBar();
        }

    private:
        void drawHorScrollBar() const
        {
            cout << "Draw horizontal scroll bar!" << endl;
        }
    };
}

namespace ns2
{
    class Beverage // 抽象的饮料类
    {
    public:
        virtual ~Beverage() {}
        virtual int getprice() const = 0; // 获取价格
    };

    class FruitBeverage : public Beverage // 水果饮料类
    {
    public:
        int getprice() const override { return 10; }
    };

    class Decorator : public Beverage // 抽象的装饰器类
    {
    public:
        Decorator(const shared_ptr<Beverage> &tmpbvg) : m_pbvg(tmpbvg) {}
        int getprice() const override { return m_pbvg->getprice(); }

    private:
        shared_ptr<Beverage> m_pbvg;
    };

    class SugarDec : public Decorator // 具体的“砂糖”装饰器类
    {
    public:
        SugarDec(const shared_ptr<Beverage> &tmpbvg) : Decorator(tmpbvg) {}
        int getprice() const override { return Decorator::getprice() + 1; }
    };

    class MilkDec : public Decorator // 具体的“牛奶”装饰器类
    {
    public:
        MilkDec(const shared_ptr<Beverage> &tmpbvg) : Decorator(tmpbvg) {}
        int getprice() const override { return Decorator::getprice() + 2; }
    };

    class BubbleDec : public Decorator // 具体的“珍珠”装饰器类
    {
    public:
        BubbleDec(const shared_ptr<Beverage> &tmpbvg) : Decorator(tmpbvg) {} // 构造函数
        int getprice() const override { return Decorator::getprice() + 2; }
    };
}

namespace ns3
{

    class Shape
    {
    public:
        virtual ~Shape() = default;
        virtual void draw() const = 0;
    };

    class Rectangle : public Shape
    {
    public:
        void draw() const override
        {
            cout << "Shape: Rectangle" << endl;
        }
    };

    class Circle : public Shape
    {
    public:
        void draw() const override
        {
            cout << "Shape: Circle" << endl;
        }
    };

    class ShapeDecorator : public Shape
    {
    protected:
        shared_ptr<Shape> decoratedShape;

    public:
        ShapeDecorator(const shared_ptr<Shape> &s)
        {
            decoratedShape = s;
        }
        void draw() const override
        {
            decoratedShape->draw();
        }
    };

    class RedShapeDecorator : public ShapeDecorator
    {
    public:
        RedShapeDecorator(const shared_ptr<Shape> &s) : ShapeDecorator(s) {}

        void draw() const override
        {
            ShapeDecorator::draw(); // decoratedShape->draw();
            setRedBorder();
        }

    private:
        void setRedBorder() const
        {
            cout << "Border Color: Red" << endl;
        }
    };

}

int main()
{
#if 0
    using namespace ns1;
    //(1)创建一个又带边框，又带垂直滚动条的列表控件
    shared_ptr<Control> plistctrl = make_shared<ListCtrl>();                       // 普通列表控件
    shared_ptr<Decorator> plistctrl_b = make_shared<BorderDec>(plistctrl);         // 带边框的列表控件
    shared_ptr<Control> plistctrl_b_v = make_shared<VerScrollBarDec>(plistctrl_b); // 带垂直滚动条又带边框的列表控件
    plistctrl_b_v->draw();

    cout << "-------------------------------" << endl;

    //(2)创建一个只带水平滚动条的列表控件
    shared_ptr<Control> plistctrl2 = make_shared<ListCtrl>();                      // 普通列表控件
    shared_ptr<Decorator> plistctrl2_h = make_shared<HorScrollBarDec>(plistctrl2); // 带水平滚动条的列表控件
    plistctrl2_h->draw();
#endif

#if 1
    using namespace ns2;
    shared_ptr<Beverage> pfruit = make_shared<FruitBeverage>();                            // 单纯水果饮料，10元
    shared_ptr<Decorator> pfruit_addbubb = make_shared<BubbleDec>(pfruit);                 // 增加珍珠，增加2元
    shared_ptr<Decorator> pfruit_addbubb_addsugar = make_shared<SugarDec>(pfruit_addbubb); // 增加砂糖，增加1元
    cout << "last price: " << pfruit_addbubb_addsugar->getprice() << endl;                 // 最终价格
#endif

#if 1
    using namespace ns3;
    auto circle = make_shared<Circle>();
    cout << "Circle with normal border" << endl;
    circle->draw();

    auto redCircle = make_shared<RedShapeDecorator>(circle);
    cout << "\nCircle of red border" << endl;
    redCircle->draw();

    auto rectange = make_shared<Rectangle>();
    auto redRectangle = make_shared<RedShapeDecorator>(rectange);
    cout << "\nRectangle of red border" << endl;
    redRectangle->draw();
#endif
    cout << "Over!\n";
    return 0;
}
