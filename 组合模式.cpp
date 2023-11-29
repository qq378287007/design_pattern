#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

class Employee
{
    string name;
    string dept;
    int salary;
    vector<shared_ptr<Employee>> subordinates;

public:
    Employee(const string &m_name, const string &m_dept, int m_salary) : name(m_name), dept(m_dept), salary(m_salary) {}
    void add(const shared_ptr<Employee> &e) { subordinates.push_back(e); }
    void remove(const shared_ptr<Employee> &e)
    {
        auto it = find(subordinates.cbegin(), subordinates.cend(), e);
        if (it != subordinates.cend())
            subordinates.erase(it);
    }
    const vector<shared_ptr<Employee>> &getSubordinates() const { return subordinates; }

    string toString()
    {
        return ("Employee :[ Name : " + name + ", dept : " + dept + ", salary :" + to_string(salary) + " ]");
    }
};

int main()
{
    auto clerk1 = make_shared<Employee>("Laura", "Marketing", 10000);
    auto clerk2 = make_shared<Employee>("Bob", "Marketing", 10000);
    auto headMarketing = make_shared<Employee>("Michel", "Head Marketing", 20000);
    headMarketing->add(clerk1);
    headMarketing->add(clerk2);

    auto salesExecutive1 = make_shared<Employee>("Richard", "Sales", 10000);
    auto salesExecutive2 = make_shared<Employee>("Rob", "Sales", 10000);
    auto headSales = make_shared<Employee>("Robert", "Head Sales", 20000);
    headSales->add(salesExecutive1);
    headSales->add(salesExecutive2);

    auto CEO = make_shared<Employee>("John", "CEO", 30000);
    CEO->add(headSales);
    CEO->add(headMarketing);
    cout << CEO->toString() << endl
         << endl;

    for (const shared_ptr<Employee> &e : CEO->getSubordinates())
    {
        cout << e->toString() << endl;
        for (const shared_ptr<Employee> &ee : e->getSubordinates())
            cout << ee->toString() << endl;
    }

    return 0;
}
