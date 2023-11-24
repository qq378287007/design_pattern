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
        cout << "Inside Rectangle::draw() method." << endl;
    }
};

class Square : public Shape
{
public:
    void draw() const override
    {
        cout << "Inside Square::draw() method." << endl;
    }
};

class Circle : public Shape
{
public:
    void draw() const override
    {
        cout << "Inside Circle::draw() method." << endl;
    }
};

class ShapeFactory
{
public:
    static shared_ptr<Shape> getShape(const string &shapeType)
    {
        if (shapeType == "CIRCLE")
            return make_shared<Circle>();
        else if (shapeType == "RECTANGLE")
            return make_shared<Rectangle>();
        else if (shapeType == "SQUARE")
            return make_shared<Square>();
        else
            return nullptr;
    }
};

int main()
{
    auto shape1 = ShapeFactory::getShape("CIRCLE");
    shape1->draw();

    auto shape2 = ShapeFactory::getShape("RECTANGLE");
    shape2->draw();

    auto shape3 = ShapeFactory::getShape("SQUARE");
    shape3->draw();

    return 0;
}
