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

int main()
{
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
    return 0;
}
