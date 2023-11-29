#include <iostream>
#include <memory>
using namespace std;

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
        cout << "Rectangle::draw()" << endl;
    }
};

class Square : public Shape
{
public:
    void draw() const override
    {
        cout << "Square::draw()" << endl;
    }
};

class Circle : public Shape
{
public:
    void draw() const override
    {
        cout << "Circle::draw()" << endl;
    }
};

class ShapeMaker
{
    shared_ptr<Shape> circle;
    shared_ptr<Shape> rectangle;
    shared_ptr<Shape> square;
public:
    ShapeMaker()
    {
        circle = make_shared<Circle>();
        rectangle = make_shared<Rectangle>();
        square = make_shared<Square>();
    }
    void drawCircle() const
    {
        circle->draw();
    }
    void drawRectangle() const
    {
        rectangle->draw();
    }
    void drawSquare() const
    {
        square->draw();
    }
};

int main()
{
    auto shapeMaker = make_shared<ShapeMaker>();
    shapeMaker->drawCircle();
    shapeMaker->drawRectangle();
    shapeMaker->drawSquare();
    return 0;
}
