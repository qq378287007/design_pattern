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

class Color
{
public:
    virtual ~Color() = default;
    virtual void fill() const = 0;
};

class Red : public Color
{
public:
    void fill() const override
    {
        cout << "Inside Red::fill() method." << endl;
    }
};

class Green : public Color
{
public:
    void fill() const override
    {
        cout << "Inside Green::fill() method." << endl;
    }
};

class Blue : public Color
{
public:
    void fill() const override
    {
        cout << "Inside Blue::fill() method." << endl;
    }
};

class AbstractFactory
{
public:
    virtual ~AbstractFactory() = default;
    virtual shared_ptr<Shape> getShape(const string &shape) const = 0;
    virtual shared_ptr<Color> getColor(const string &color) const = 0;
};

class ShapeFactory : public AbstractFactory
{
public:
    shared_ptr<Shape> getShape(const string &shapeType) const override
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
    shared_ptr<Color> getColor(const string &color) const override
    {
        return nullptr;
    }
};

class ColorFactory : public AbstractFactory
{
public:
    shared_ptr<Shape> getShape(const string &shapeType) const override
    {
        return nullptr;
    }
    shared_ptr<Color> getColor(const string &color) const override
    {
        if (color == "RED")
            return make_shared<Red>();
        else if (color == "GREEN")
            return make_shared<Green>();
        else if (color == "BLUE")
            return make_shared<Blue>();
        else
            return nullptr;
    }
};

shared_ptr<AbstractFactory> getFactory(const string &choice)
{
    if (choice == "SHAPE")
        return make_shared<ShapeFactory>();
    else if (choice == "COLOR")
        return make_shared<ColorFactory>();
    else
        return nullptr;
}

int main()
{
    auto shapeFactory = getFactory("SHAPE");
    auto shape1 = shapeFactory->getShape("CIRCLE");
    shape1->draw();
    auto shape2 = shapeFactory->getShape("RECTANGLE");
    shape2->draw();
    auto shape3 = shapeFactory->getShape("SQUARE");
    shape3->draw();

    auto colorFactory = getFactory("COLOR");
    auto color1 = colorFactory->getColor("RED");
    color1->fill();
    auto color2 = colorFactory->getColor("GREEN");
    color2->fill();
    auto color3 = colorFactory->getColor("BLUE");
    color3->fill();

    return 0;
}
