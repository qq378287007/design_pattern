#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <ctime>
using namespace std;

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
    static unordered_map<string, shared_ptr<Shape>> colorShapeMap;

    static shared_ptr<Shape> getColorShape(const string &color)
    {
        if (colorShapeMap.count(color) > 0)
            return colorShapeMap[color];

        cout << "Creating ColorShape of color : " << color << endl;
        auto shape = make_shared<ColorShape>(color);
        colorShapeMap[color] = shape;
        return shape;
    }
};
unordered_map<string, shared_ptr<Shape>> ShapeFactory::colorShapeMap;

double random()
{
    return (rand() % 10) * 0.1;
}
string colors[] = {"Red", "Green", "Blue", "White", "Black"};
string getRandomColor()
{
    return colors[(int)(random() * sizeof(colors) / sizeof(colors[0]))];
}

int main()
{
    srand(time(0));
    for (int i = 0; i < 20; ++i)
    {
        auto colorShape = ShapeFactory::getColorShape(getRandomColor());
        colorShape->draw();
    }
    return 0;
}
