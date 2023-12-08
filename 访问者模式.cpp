#include <iostream>
#include <list>
#include <memory>
using namespace std;

// 提供一个作用于某对象结构中的各元素的操作表示，便可以在不改变各元素类的前提下定义（扩展）作用于这些元素的新操作
// 允许一个或多个操作应用于一组对象上，使对象本身和操作解耦
// 5种角色
// Visitor（抽象访问者），为对象结构中的元素声明访问接口
// ConcreteVisitor（具体访问者），实现接口
// Element（抽象元素），定义Accept方法，形参通常为Visitor指针
// ConcreteElement（具体元素），实现Accept方法，该方法中调用ConcreteVisitor的访问操作，实现元素操作
// ObjectStructure（对象结构），元素集合，存放元素对象并提供遍历其内部元素的接口，一般用于同一批元素的相同操作

namespace ns1
{
    class Medicine // 药品父类
    {
    public:
        virtual ~Medicine() {}
        virtual string getMdcName() const = 0; // 药品名称
        virtual float getPrice() const = 0;    // 药品总价格，单位：元
    };

    class M_asplcrp : public Medicine // 药品：阿司匹林肠溶片
    {
    public:
        string getMdcName() const override
        {
            return "Aspirin enteric-coated tablets";
        }
        float getPrice() const override
        {
            return 46.8f; // 为简化代码，直接给出药品总价而不单独强调药品数量了，比如该药品医生给开了两盒，一盒是23.4，那么这里直接返回两盒的价格
        }
    };

    class M_fftdnhsp : public Medicine // 药品：氟伐他汀钠缓释片
    {
    public:
        string getMdcName() const override
        {
            return "Fluvastatin sodium sustained-release tablets";
        }
        float getPrice() const override
        {
            return 111.3f; // 三盒的价格
        }
    };

    class M_dlx : public Medicine // 药品：黛力新
    {
    public:
        string getMdcName() const override
        {
            return "Deanxit";
        }
        float getPrice() const override
        {
            return 122.0f; // 两盒的价格
        }
    };

    class MedicineProc // 针对药品的处理相关类
    {
        list<shared_ptr<Medicine>> m_mdclist; // 药品列表，记录着药品单上的所有药品
    public:
        // 增加药品到药品列表中
        void addMedicine(const shared_ptr<Medicine> &p_mdc) { m_mdclist.push_back(p_mdc); }

        // 针对费用缴纳和取药所做的处理动作
        void procAction(const string &strvisitor) // strvisitor代表拿到了药品单的人，不同的人拿到药品单所要做的处理也不同
        {
            if (strvisitor == "Toll collector") // 收费人员要根据药品单向我（患者）收取费用
            {
                float totalcost = 0.0f; // 总费用
                for (auto iter = m_mdclist.begin(); iter != m_mdclist.end(); ++iter)
                {
                    float tmpprice = (*iter)->getPrice();
                    totalcost += tmpprice;

                    cout << "yao: " << (*iter)->getMdcName() << ", price: " << tmpprice << endl;
                }
                cout << "total price: " << totalcost << ", Toll collector charges a fee!" << endl;
            }
            else if (strvisitor == "Drug taking personnel") // 取药人员要根据药品单为我拿药
                for (auto iter = m_mdclist.begin(); iter != m_mdclist.end(); ++iter)
                    cout << "Drug taking personnel give " << (*iter)->getMdcName() << " to me!" << endl;
            else if (strvisitor == "dietitian")
                cout << "Nutritionist suggested: match coarse grains with staple foods and eat meat properly!" << endl;
            else if (strvisitor == "Fitness coach")
                cout << "The fitness coach suggested: do more aerobic exercises such as jogging (35 minutes), warm up before and after jogging, avoid anxiety, avoid staying up late, and take a bed rest before 22:30!" << endl;
        }
    };
}

namespace ns2
{
    class Visitor; // 类前向声明
    class Medicine // 药品父类
    {
    public:
        virtual ~Medicine() {}
        virtual void Accept(shared_ptr<Visitor> &pvisitor) = 0; // 这里的形参是访问者父类指针
    public:
        virtual string getMdcName() = 0; // 药品名称
        virtual float getPrice() = 0;    // 药品总价格，单位：元
    };

    class M_asplcrp : public Medicine // 药品：阿司匹林肠溶片
    {
    public:
        string getMdcName() override { return "Aspirin enteric-coated tablets"; }
        float getPrice() override { return 46.8f; }

    public:
        void Accept(shared_ptr<Visitor> &pvisitor) override;
    };

    class M_fftdnhsp : public Medicine // 药品：氟伐他汀钠缓释片
    {
    public:
        string getMdcName() override { return "Fluvastatin sodium sustained-release tablets"; }
        float getPrice() override { return 111.3f; }

    public:
        void Accept(shared_ptr<Visitor> &pvisitor) override;
    };

    class M_dlx : public Medicine // 药品：黛力新
    {
    public:
        string getMdcName() override { return "Deanxit"; }
        float getPrice() override { return 122.0f; }

    public:
        void Accept(shared_ptr<Visitor> &pvisitor) override;
    };

    class Visitor // 访问者父类
    {
    public:
        virtual ~Visitor() {}

        virtual void Visit_elm_asplcrp(M_asplcrp *pelem) = 0;   // 访问元素：阿司匹林肠溶片
        virtual void Visit_elm_fftdnhsp(M_fftdnhsp *pelem) = 0; // 访问元素：氟伐他汀钠缓释片
        virtual void Visit_elm_dlx(M_dlx *pelem) = 0;           // 访问元素：黛力新

        ////以下几个接口的名字都叫Visit（成员函数重载）
        // virtual void Visit(M_asplcrp* pelem) = 0;   //访问元素：阿司匹林肠溶片
        // virtual void Visit(M_fftdnhsp* pelem) = 0;  //访问元素：氟伐他汀钠缓释片
        // virtual void Visit(M_dlx* pelem) = 0;       //访问元素：黛力新
    };

    class Visitor_SFRY : public Visitor // 收费人员访问者子类
    {
        float m_totalcost = 0.0f; // 总费用
    public:
        void Visit_elm_asplcrp(M_asplcrp *pelem) override
        {
            float tmpprice = pelem->getPrice();
            cout << "yao: " << pelem->getMdcName() << ", price: " << tmpprice << endl;
            m_totalcost += tmpprice;
        }
        void Visit_elm_fftdnhsp(M_fftdnhsp *pelem) override
        {
            float tmpprice = pelem->getPrice();
            cout << "yao: " << pelem->getMdcName() << ", price: " << tmpprice << endl;
            m_totalcost += tmpprice;
        }
        void Visit_elm_dlx(M_dlx *pelem) override
        {
            float tmpprice = pelem->getPrice();
            cout << "yao: " << pelem->getMdcName() << ", price: " << tmpprice << endl;
            m_totalcost += tmpprice;
        }

        // 返回总费用
        float getTotalCost() const { return m_totalcost; }
    };

    class Visitor_QYRY : public Visitor // 取药人员访问者子类
    {
    public:
        void Visit_elm_asplcrp(M_asplcrp *pelem) override
        {
            cout << "Drug taking personnel give " << pelem->getMdcName() << " to me!" << endl;
        }
        void Visit_elm_fftdnhsp(M_fftdnhsp *pelem) override
        {
            cout << "Drug taking personnel give " << pelem->getMdcName() << " to me!" << endl;
        }
        void Visit_elm_dlx(M_dlx *pelem) override
        {
            cout << "Drug taking personnel give " << pelem->getMdcName() << " to me!" << endl;
        }
    };

    class Visitor_YYS : public Visitor // 营养师访问者子类
    {
    public:
        void Visit_elm_asplcrp(M_asplcrp *pelem) override
        {
            cout << "The nutritionist suggested: Eating more coarse grains and less oil can effectively prevent blood clots!" << endl;
        }
        void Visit_elm_fftdnhsp(M_fftdnhsp *pelem) override
        {
            cout << "The nutritionist suggested: Eating more mushrooms, onions, and kiwifruit can effectively reduce blood lipids!" << endl;
        }
        void Visit_elm_dlx(M_dlx *pelem) override
        {
            cout << "The nutritionist suggested: Go out more to breathe fresh air, get more sunshine, go to more crowded places, and keep an optimistic and cheerful mood!" << endl;
        }
    };

    // 各个药品子类Accept方法的实现体代码
    void M_asplcrp::Accept(shared_ptr<Visitor> &pvisitor)
    {
        pvisitor->Visit_elm_asplcrp(this);
    }
    void M_fftdnhsp::Accept(shared_ptr<Visitor> &pvisitor)
    {
        pvisitor->Visit_elm_fftdnhsp(this);
    }
    void M_dlx::Accept(shared_ptr<Visitor> &pvisitor)
    {
        pvisitor->Visit_elm_dlx(this);
    }

    class ObjectStructure // 对象结构
    {
        list<shared_ptr<Medicine>> m_mdclist; // 药品列表
    public:
        // 增加药品到药品列表中
        void addMedicine(const shared_ptr<Medicine> &p_mdc)
        {
            m_mdclist.push_back(p_mdc);
        }

        void procAction(shared_ptr<Visitor> &pvisitor)
        {
            for (auto iter = m_mdclist.begin(); iter != m_mdclist.end(); ++iter)
                (*iter)->Accept(pvisitor);
        }
    };
}

namespace ns3
{
    class Medicine; // 类前向声明
    class Visitor   // 访问者父类
    {
    public:
        virtual ~Visitor() {}
        virtual void Visit(Medicine *pelem) = 0;
    };

    class Medicine // 药品父类
    {
    public:
        virtual ~Medicine() {}
        virtual void Accept(shared_ptr<Visitor> &pvisitor) { pvisitor->Visit(this); }

    public:
        virtual string getMdcName() const = 0; // 药品名称
        virtual float getPrice() const = 0;    // 药品总价格，单位：元
    };

    class M_asplcrp : public Medicine // 药品：阿司匹林肠溶片
    {
    public:
        string getMdcName() const override { return "Aspirin enteric-coated tablets"; }
        float getPrice() const override { return 46.8f; }
    };

    class M_fftdnhsp : public Medicine // 药品：氟伐他汀钠缓释片
    {
    public:
        string getMdcName() const override { return "Fluvastatin sodium sustained-release tablets"; }
        float getPrice() const override { return 111.3f; }
    };

    class M_dlx : public Medicine // 药品：黛力新
    {
    public:
        string getMdcName() const override { return "Deanxit"; }
        float getPrice() const override { return 122.0f; }
    };

    class Visitor_SFRY : public Visitor // 收费人员访问者子类
    {
        float m_totalcost = 0.0f; // 总费用
    public:
        void Visit(Medicine *m) override
        {
            float tmpprice = m->getPrice();
            m_totalcost += tmpprice;

            cout << "yao: " << m->getMdcName() << ", price: " << tmpprice << endl;
        }

        float getTotalCost() const { return m_totalcost; } // 返回总费用
    };

    class Visitor_QYRY : public Visitor // 取药人员访问者子类
    {
    public:
        void Visit(Medicine *m) override
        {
            cout << "Drug taking personnel give " << m->getMdcName() << " to me!" << endl;
        }
    };

    class Visitor_YYS : public Visitor // 营养师访问者子类
    {
    public:
        void Visit(Medicine *m) override
        {
            cout << "The nutritionist suggested: Eating more coarse grains and less oil can effectively prevent blood clots!" << endl;
            cout << "The nutritionist suggested: Eating more mushrooms, onions, and kiwifruit can effectively reduce blood lipids!" << endl;
            cout << "The nutritionist suggested: Go out more to breathe fresh air, get more sunshine, go to more crowded places, and keep an optimistic and cheerful mood!" << endl;
        }
    };

    class ObjectStructure // 对象结构
    {
        list<shared_ptr<Medicine>> m_mdclist; // 药品列表
    public:
        // 增加药品到药品列表中
        void addMedicine(const shared_ptr<Medicine> &p_mdc)
        {
            m_mdclist.push_back(p_mdc);
        }

        void procAction(shared_ptr<Visitor> &pvisitor)
        {
            for (auto iter = m_mdclist.begin(); iter != m_mdclist.end(); ++iter)
                (*iter)->Accept(pvisitor);
        }
    };
}

int main()
{
#if 0
    using namespace ns1;
    shared_ptr<Medicine> pm1(new M_asplcrp());  // 药品阿司匹林肠溶片
    shared_ptr<Medicine> pm2(new M_fftdnhsp()); // 药品氟伐他汀钠缓释片
    shared_ptr<Medicine> pm3(new M_dlx());      // 药品黛力新

    MedicineProc mdcprocobj;
    mdcprocobj.addMedicine(pm1);
    mdcprocobj.addMedicine(pm2);
    mdcprocobj.addMedicine(pm3);
    mdcprocobj.procAction("Toll collector");
    mdcprocobj.procAction("Drug taking personnel");
#endif

#if 1
    using namespace ns2;
    //using namespace ns3;
    shared_ptr<Visitor> visitor_sf(new Visitor_SFRY()); // 收费人员访问者子类，里面承载着向我（患者）收费的算法
    shared_ptr<Medicine> mdc_asplcrp(new M_asplcrp());
    shared_ptr<Medicine> mdc_fftdnhsp(new M_fftdnhsp());
    shared_ptr<Medicine> mdc_dlx(new M_dlx());

    // 各个元素子类调用Accept接受访问者的访问，就可以实现访问者要实现的功能
    mdc_asplcrp->Accept(visitor_sf);  // 累加“阿司匹林肠溶片”的价格
    mdc_fftdnhsp->Accept(visitor_sf); // 累加“氟伐他汀钠缓释片”的价格
    mdc_dlx->Accept(visitor_sf);      // 累加“黛力新”的价格
    cout << "total price: " << dynamic_pointer_cast<Visitor_SFRY>(visitor_sf)->getTotalCost() << ", the toll collector charged me!" << endl;

    shared_ptr<Visitor> visitor_qy(new Visitor_QYRY()); // 取药人员访问者子类，里面承载着向我发放药品的算法
    mdc_asplcrp->Accept(visitor_qy);                    // 我取得“阿司匹林肠溶片”
    mdc_fftdnhsp->Accept(visitor_qy);                   // 我取得“氟伐他汀钠缓释片”
    mdc_dlx->Accept(visitor_qy);                        // 我取得“黛力新”

    shared_ptr<Visitor> visitor_yys(new Visitor_YYS()); // 营养师访问者子类，里面承载着为我配置营养餐的算法
    mdc_asplcrp->Accept(visitor_yys);                   // 营养师针对治疗预防血栓药“阿司匹林肠溶片”给出的对应的营养餐建议
    mdc_fftdnhsp->Accept(visitor_yys);                  // 营养师针对降血脂药“氟伐他汀钠缓释片”给出的对应的营养餐建议
    mdc_dlx->Accept(visitor_yys);                       // 营养师针对治疗神经紊乱药“黛力新”给出的对应的营养餐建议

    shared_ptr<ObjectStructure> objstruc(new ObjectStructure());
    objstruc->addMedicine(mdc_asplcrp);
    objstruc->addMedicine(mdc_fftdnhsp);
    objstruc->addMedicine(mdc_dlx);
    objstruc->procAction(visitor_yys); // 将一个访问者对象（visitor_yys）应用到一批元素上，以实现对一批元素进行同一种（营养方面的）操作
#endif

    cout << "Over!\n";
    return 0;
}
