#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <ctime>
#include <memory>
#include <list>
#include <vector>
using namespace std;

// 被共享的单元或对象
// 尽量共用对象，节省内存，提升效率
// 运用共享技术有效地支持大量细粒度的对象（的复用）

// 3种角色
// Flyweight（抽象亨元类），接口或抽象类，外部状态作为接口方法的参数
// ConcreteFlyweight（具体亨元类），创建亨元对象
// FlyWeightFactory（亨元工厂类），创建并管理亨元对象，内部存在亨元池（map等容器）

namespace ns1
{
    enum EnumColor // 棋子颜色
    {
        Black, // 黑
        White  // 白
    };

    struct Position // 棋子位置
    {
        int m_x;
        int m_y;
        Position(int tmpx, int tmpy) : m_x(tmpx), m_y(tmpy) {}
    };

    class Piece // 棋子
    {
        EnumColor m_color; // 棋子颜色
        Position m_pos;    // 棋子位置
    public:
        Piece(EnumColor tmpcolor, Position tmppos) : m_color(tmpcolor), m_pos(tmppos) {}

        void draw() const // 棋子的绘制
        {
            switch (m_color)
            {
            case Black:
                cout << "position: (" << m_pos.m_x << ", " << m_pos.m_y << ") draw a black piece!" << endl;
                break;
            case White:
                cout << "position: (" << m_pos.m_x << ", " << m_pos.m_y << ") draw a white piece!" << endl;
                break;
            }
        }
    };
}

namespace ns2
{
    enum EnumColor // 棋子颜色
    {
        Black, // 黑
        White  // 白
    };

    struct Position // 棋子位置
    {
        int m_x;
        int m_y;
        Position(int tmpx, int tmpy) : m_x(tmpx), m_y(tmpy) {}
    };

    class Piece // 棋子抽象类
    {
    public:
        virtual ~Piece() {}
        virtual void draw(Position tmppos) const = 0;
    };

    class BlackPiece : public Piece // 黑色棋子
    {
    public:
        void draw(Position tmppos) const override
        {
            cout << "position: (" << tmppos.m_x << ", " << tmppos.m_y << ") draw a black piece!" << endl;
        }
    };

    class WhitePiece : public Piece // 白色棋子
    {
    public:
        void draw(Position tmppos) const override
        {
            cout << "position: (" << tmppos.m_x << ", " << tmppos.m_y << ") draw a white piece!" << endl;
        }
    };

    class pieceFactory // 创建棋子的工厂
    {
        unordered_map<EnumColor, shared_ptr<Piece>> m_FlyWeihgtMap; // 两个享元对象（黑色棋子，白色棋子）

    public: // 获取享元对象(被共享的棋子对象)
        shared_ptr<Piece> getFlyWeight(EnumColor tmpcolor)
        {
            auto iter = m_FlyWeihgtMap.find(tmpcolor);
            if (iter == m_FlyWeihgtMap.end()) // 创建享元对象
            {
                shared_ptr<Piece> tmpfw;
                switch (tmpcolor)
                {
                case Black: // 黑子
                    tmpfw.reset(new BlackPiece());
                    break;
                case White: // 白子
                    tmpfw.reset(new WhitePiece());
                    break;
                }
                m_FlyWeihgtMap.insert(make_pair(tmpcolor, tmpfw));
                return tmpfw;
            }
            else
            {
                return iter->second;
            }
        }
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

    class ColorShape : public Shape
    {
        string color;

    public:
        ColorShape(const string &m_color) : color(m_color) {}
        void draw() const override
        {
            cout << "ColorShape: Draw() [Color : " << color << "]" << endl;
        }
    };

    class ShapeFactory
    {
    public:
        static shared_ptr<Shape> getColorShape(const string &color)
        {
            static unordered_map<string, shared_ptr<Shape>> colorShapeMap;
            if (colorShapeMap.count(color) > 0)
                return colorShapeMap[color];

            cout << "Creating ColorShape of color : " << color << endl;
            shared_ptr<Shape> shape = make_shared<ColorShape>(color);
            colorShapeMap[color] = shape;
            return shape;
        }
    };

    static double random()
    {
        return (rand() % 10) * 0.1;
    }
    static string colors[] = {"Red", "Green", "Blue", "White", "Black"};
    static string getRandomColor()
    {
        return colors[(int)(random() * sizeof(colors) / sizeof(colors[0]))];
    }
}

int main()
{
#if 0
    using namespace ns1;
    shared_ptr<Piece> p_piece1(new Piece(Black, Position(3, 3))); // 黑子落子到3,3位置
    shared_ptr<Piece> p_piece2(new Piece(White, Position(5, 5))); // 白子落子到5,5位置
    shared_ptr<Piece> p_piece3(new Piece(Black, Position(4, 6))); // 黑子落子到4,6位置
    shared_ptr<Piece> p_piece4(new Piece(White, Position(5, 7))); // 白子落子到5,7位置

    list<shared_ptr<Piece>> piecelist;
    piecelist.push_back(p_piece1);
    piecelist.push_back(p_piece2);
    piecelist.push_back(p_piece3);
    piecelist.push_back(p_piece4);

    for (const shared_ptr<Piece> &piece : piecelist)
        piece->draw();
#endif

#if 0
    using namespace ns2;
    shared_ptr<pieceFactory> pfactory(new pieceFactory());

    shared_ptr<Piece> p_piece1 = pfactory->getFlyWeight(Black);
    p_piece1->draw(Position(3, 3)); // 黑子落子到3,3位置

    shared_ptr<Piece> p_piece2 = pfactory->getFlyWeight(White);
    p_piece2->draw(Position(5, 5)); // 白子落子到5,5位置

    shared_ptr<Piece> p_piece3 = pfactory->getFlyWeight(Black);
    p_piece3->draw(Position(4, 6)); // 黑子落子到4,6位置

    shared_ptr<Piece> p_piece4 = pfactory->getFlyWeight(White);
    p_piece4->draw(Position(5, 7)); // 白子落子到5,7位置

    vector<pair<EnumColor, Position>> piecevector;
    piecevector.push_back(pair<EnumColor, Position>(Black, Position(3, 3)));
    piecevector.push_back(make_pair(White, Position(5, 5)));
    piecevector.push_back(make_pair(Black, Position(4, 6)));
    piecevector.push_back(make_pair(White, Position(5, 7)));

    for (const auto &piece : piecevector)
        pfactory->getFlyWeight(piece.first)->draw(piece.second);
#endif

#if 1
    using namespace ns3;
    srand(time(0));
    for (int i = 0; i < 20; ++i)
    {
        shared_ptr<Shape> colorShape = ShapeFactory::getColorShape(getRandomColor());
        colorShape->draw();
    }
#endif

    cout << "Over!\n";
    return 0;
}
