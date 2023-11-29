#include <iostream>
#include <memory>
#include <mutex>
using namespace std;

class Single
{
public:
    static Single &getInstance()
    {
        mutex mt;
        if (instance.get() == NULL)
        {
            mt.lock();
            //if (instance.get() == NULL)
            //    instance.reset(new Single());
            mt.unlock();
        }
        return *instance;
    }

private:
    Single() {}
    ~Single() {}
    static shared_ptr<Single> instance;
    friend class shared_ptr<Single>;

    Single(const Single &);
    Single &operator=(const Single &);
};

shared_ptr<Single> Single::instance = make_shared<Single>();

/*
class SingleObject
{
private:
    static shared_ptr<SingleObject> instance;

    SingleObject() {}
    ~SingleObject() {}
    friend class shared_ptr<SingleObject>;

public:
    static SingleObject &getInstance()
    {
        return *instance;
    }
    void showMessage()
    {
        cout << "Hello World!" << endl;
    }
};
shared_ptr<SingleObject> SingleObject::instance = make_shared<SingleObject>();
*/
int main()
{
    // auto object = SingleObject::getInstance();
    // object->showMessage();

    return 0;
}
