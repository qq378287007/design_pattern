#include <iostream>
#include <sstream>
#include <vector>
#include <memory>
using namespace std;

// 按顺序分步骤创建复杂对象
// 复杂对象的构建与表示分离，同样的构建过程创建不同的表示

namespace ns1
{
    class Monster // 怪物父类
    {
    public:
        virtual ~Monster() {}
        virtual void LoadTrunkModel(const string &strno) = 0; // 这里也可以写为空函数体，子类决定是否重新实现
        virtual void LoadHeadModel(const string &strno) = 0;
        virtual void LoadLimbsModel(const string &strno) = 0;

    public:
        void Assemble(const string &strmodelno) // 参数：模型编号，形如“1253679201245”等，每些位的组合都有一些特别的含义，这里无需深究
        {
            LoadTrunkModel(strmodelno.substr(4, 3));  // 载入躯干模型，截取某部分字符串以表示躯干模型的编号
            LoadHeadModel(strmodelno.substr(7, 3));   // 载入头部模型并挂接到躯干模型上
            LoadLimbsModel(strmodelno.substr(10, 3)); // 载入四肢模型并挂接到躯干模型上
        }
    };

    class M_Undead : public Monster // 亡灵类怪物
    {
    public:
        void LoadTrunkModel(const string &strno) override
        {
            cout << "body + other M_Undead function" << endl;
        }
        void LoadHeadModel(const string &strno) override
        {
            cout << "head + other M_Undead function" << endl;
        }
        void LoadLimbsModel(const string &strno) override
        {
            cout << "limb + other M_Undead function" << endl;
        }
    };

    class M_Element : public Monster // 元素类怪物
    {
    public:
        void LoadTrunkModel(const string &strno) override
        {
            cout << "body + other M_Element function" << endl;
        }
        void LoadHeadModel(const string &strno) override
        {
            cout << "head + other M_Element function" << endl;
        }
        void LoadLimbsModel(const string &strno) override
        {
            cout << "limb + other M_Element function" << endl;
        }
    };

    class M_Mechanic : public Monster // 机械类怪物
    {
    public:
        void LoadTrunkModel(const string &strno) override
        {
            cout << "body + other M_Mechanic function" << endl;
        }
        void LoadHeadModel(const string &strno) override
        {
            cout << "head + other M_Mechanic function" << endl;
        }
        void LoadLimbsModel(const string &strno) override
        {
            cout << "limb + other M_Mechanic function" << endl;
        }
    };

    class MonsterBuilder // 怪物构建器父类
    {
    protected:
        shared_ptr<Monster> m_pMonster;

    public:
        virtual ~MonsterBuilder() {}
        MonsterBuilder(const shared_ptr<Monster> &monster = nullptr) : m_pMonster(monster) {}

        virtual void LoadTrunkModel(const string &strno) = 0; // 这里也可以写为空函数体，子类决定是否重新实现
        virtual void LoadHeadModel(const string &strno) = 0;
        virtual void LoadLimbsModel(const string &strno) = 0;

    public:
        void Assemble(const string &strmodelno) // 参数：模型编号，形如“1253679201245”等，每些位的组合都有一些特别的含义，这里无需深究
        {
            LoadTrunkModel(strmodelno.substr(4, 3));  // 载入躯干模型，截取某部分字符串以表示躯干模型的编号
            LoadHeadModel(strmodelno.substr(7, 3));   // 载入头部模型并挂接到躯干模型上
            LoadLimbsModel(strmodelno.substr(10, 3)); // 载入四肢模型并挂接到躯干模型上
        }

        shared_ptr<Monster> GetResult() const { return m_pMonster; }
    };

    class M_UndeadBuilder : public MonsterBuilder // 亡灵类怪物构建器类
    {
    public:
        M_UndeadBuilder() : MonsterBuilder(make_shared<M_Undead>()) {}

        void LoadTrunkModel(const string &strno) override
        {
            cout << "body + other M_Undead function" << endl;
            // 具体要做的事情其实是委托给怪物子类来完成，委托指把本该自己实现的功能转给其他类实现
            //  m_pMonster->......略
        }
        void LoadHeadModel(const string &strno) override
        {
            cout << "head + other M_Undead function" << endl;
            // m_pMonster->......略
        }
        void LoadLimbsModel(const string &strno) override
        {
            cout << "limb + other M_Undead function" << endl;
            // m_pMonster->......略
        }
    };

    class M_ElementBuilder : public MonsterBuilder // 元素类怪物构建器类
    {
    public:
        M_ElementBuilder() : MonsterBuilder(make_shared<M_Element>()) {}

        void LoadTrunkModel(const string &strno) override
        {
            cout << "body + other M_Element function" << endl;
            // m_pMonster->......略
        }
        void LoadHeadModel(const string &strno) override
        {
            cout << "head + other M_Element function" << endl;
            // m_pMonster->......略
        }
        void LoadLimbsModel(const string &strno) override
        {
            cout << "limb + other M_Element function" << endl;
            // m_pMonster->......略
        }
    };

    class M_MechanicBuilder : public MonsterBuilder // 机械类怪物构建器类
    {
    public:
        M_MechanicBuilder() : MonsterBuilder(make_shared<M_Mechanic>()) {}

        void LoadTrunkModel(const string &strno) override
        {
            cout << "body + other M_Mechanic function" << endl;
            // m_pMonster->......略
        }
        void LoadHeadModel(const string &strno) override
        {
            cout << "head + other M_Mechanic function" << endl;
            // m_pMonster->......略
        }
        void LoadLimbsModel(const string &strno) override
        {
            cout << "limb + other M_Mechanic function" << endl;
            // m_pMonster->......略
        }
    };

    class MonsterDirector // 指挥者类
    {
        shared_ptr<MonsterBuilder> m_pMonsterBuilder; // 指向所有构建器类的父类
    public:
        MonsterDirector(const shared_ptr<MonsterBuilder> &ptmpBuilder) : m_pMonsterBuilder(ptmpBuilder) {}

        void SetBuilder(const shared_ptr<MonsterBuilder> &ptmpBuilder) { m_pMonsterBuilder = ptmpBuilder; }

        // 原MonsterBuilder类中的Assemble成员函数
        shared_ptr<Monster> Construct(const string &strmodelno) // 参数：模型编号，形如“1253679201245”等，每些位的组合都有一些特别的含义，这里无需深究
        {
            m_pMonsterBuilder->LoadTrunkModel(strmodelno.substr(4, 3));  // 载入躯干模型，截取某部分字符串以表示躯干模型的编号
            m_pMonsterBuilder->LoadHeadModel(strmodelno.substr(7, 3));   // 载入头部模型并挂接到躯干模型上
            m_pMonsterBuilder->LoadLimbsModel(strmodelno.substr(10, 3)); // 载入四肢模型并挂接到躯干模型上
            return m_pMonsterBuilder->GetResult();                       // 返回构建后的对象
        }
    };
}

namespace ns2
{
    class DailyHeaderData // 日报中的“标题”部分
    {
        string m_strDepName; // 部门名称
        string m_strGenDate; // 日报生成日期
    public:
        DailyHeaderData(const string &strDepName, const string &strGenDate) : m_strDepName(strDepName), m_strGenDate(strGenDate) {}
        string getDepName() const { return m_strDepName; }    // 获取部门名称
        string getExportDate() const { return m_strGenDate; } // 获取日报生成日期
    };

    class DailyContentData // 日报中的“内容主体”部分 中的 每一条描述数据
    {
        string m_strContent; // 该项工作内容描述
        double m_dspendTime; // 完成该项工作花费的时间（单位：小时）
    public:
        DailyContentData(const string &strContent, double dspendTime) : m_strContent(strContent), m_dspendTime(dspendTime) {}
        string getContent() const { return m_strContent; }   // 获取该项工作内容描述
        double getSpendTime() const { return m_dspendTime; } // 获取完成该项工作花费的时间
    };

    class DailyFooterData // 日报中的“结尾”部分
    {
        string m_strUserName; // 日报所属员工姓名
    public:
        DailyFooterData(const string &strUserName) : m_strUserName(strUserName) {}
        string getUserName() const { return m_strUserName; } // 获取日报所属员工姓名
    };

    class ExportToTxtFile // 将日报导出到纯文本格式文件 相关的类
    {
    public:
        void doExport(const shared_ptr<DailyHeaderData> &dailyheaderobj, const vector<shared_ptr<DailyContentData>> &vec_dailycontobj, const shared_ptr<DailyFooterData> &dailyfooterobj)
        {
            string strtmp = "";

            //(1)拼接标题
            strtmp += dailyheaderobj->getDepName() + "," + dailyheaderobj->getExportDate() + "\n";

            //(2)拼接内容主体，内容主体中的描述数据会有多条，因此需要迭代
            for (auto iter = vec_dailycontobj.begin(); iter != vec_dailycontobj.end(); ++iter)
            {
                ostringstream oss; // 记得#include头文件sstream
                oss << (*iter)->getSpendTime();
                strtmp += (*iter)->getContent() + ":(spend time: " + oss.str() + "hour)" + "\n";
            }

            //(3)拼接结尾
            strtmp += "reporter: " + dailyfooterobj->getUserName() + "\n";

            //(4)导出到真实文件的代码略，只展示在屏幕上文件的内容
            cout << strtmp;
        }
    };

    // 将日报导出到XML格式文件 相关的类
    class ExportToXmlFile
    {
    public:
        void doExport(const shared_ptr<DailyHeaderData> &dailyheaderobj, const vector<shared_ptr<DailyContentData>> &vec_dailycontobj, const shared_ptr<DailyFooterData> &dailyfooterobj)
        {
            string strtmp = "";

            //(1)拼接标题
            strtmp += "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n";
            strtmp += "<DailyReport>\n";
            strtmp += "    <Header>\n";
            strtmp += "        <DepName>" + dailyheaderobj->getDepName() + "</DepName>\n";
            strtmp += "        <GenDate>" + dailyheaderobj->getExportDate() + "</GenDate>\n";
            strtmp += "    </Header>\n";

            //(2)拼接内容主体，内容主体中的描述数据会有多条，因此需要迭代
            strtmp += "    <Body>\n";
            for (auto iter = vec_dailycontobj.begin(); iter != vec_dailycontobj.end(); ++iter)
            {
                ostringstream oss; // 记得#include头文件sstream
                oss << (*iter)->getSpendTime();
                strtmp += "        <Content>" + (*iter)->getContent() + "</Content>\n";
                strtmp += "        <SpendTime>spend time: " + oss.str() + "hour" + "</SpendTime>\n";
            }
            strtmp += "    </Body>\n";

            //(3)拼接结尾
            strtmp += "    <Footer>\n";
            strtmp += "        <UserName>reporter: " + dailyfooterobj->getUserName() + "</UserName>\n";
            strtmp += "    </Footer>\n";

            strtmp += "</DailyReport>\n";

            //(4)导出到真实文件的代码略，只展示在屏幕上文件的内容
            cout << strtmp;
        }
    };

    class FileBuilder // 抽象构建器类（文件构建器父类）
    {
    protected:
        string m_strResult;

    public:
        virtual ~FileBuilder() {}
        virtual void buildHeader(const shared_ptr<DailyHeaderData> &dailyheaderobj) = 0;          // 拼接标题
        virtual void buildBody(const vector<shared_ptr<DailyContentData>> &vec_dailycontobj) = 0; // 拼接内容主体
        virtual void buildFooter(const shared_ptr<DailyFooterData> &dailyfooterobj) = 0;          // 拼接结尾
        void clearResult() { m_strResult = ""; }
        string GetResult() const { return m_strResult; }
    };

    class TxtFileBuilder : public FileBuilder // 纯文本文件构建器类
    {
    public:
        void buildHeader(const shared_ptr<DailyHeaderData> &dailyheaderobj) override // 拼接标题
        {
            m_strResult += dailyheaderobj->getDepName() + "," + dailyheaderobj->getExportDate() + "\n";
        }
        void buildBody(const vector<shared_ptr<DailyContentData>> &vec_dailycontobj) override // 拼接内容主体
        {
            for (auto iter = vec_dailycontobj.begin(); iter != vec_dailycontobj.end(); ++iter)
            {
                ostringstream oss; // 记得#include头文件sstream
                oss << (*iter)->getSpendTime();
                m_strResult += (*iter)->getContent() + ":(spend time: " + oss.str() + "hour)" + "\n";
            }
        }
        void buildFooter(const shared_ptr<DailyFooterData> &dailyfooterobj) override // 拼接结尾
        {
            m_strResult += "reporter: " + dailyfooterobj->getUserName() + "\n";
        }
    };

    class XmlFileBuilder : public FileBuilder // XML文件构建器类
    {
    public:
        void buildHeader(const shared_ptr<DailyHeaderData> &dailyheaderobj) override // 拼接标题
        {
            m_strResult += "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n";
            m_strResult += "<DailyReport>\n";

            m_strResult += "    <Header>\n";
            m_strResult += "        <DepName>" + dailyheaderobj->getDepName() + "</DepName>\n";
            m_strResult += "        <GenDate>" + dailyheaderobj->getExportDate() + "</GenDate>\n";
            m_strResult += "    </Header>\n";
        }
        void buildBody(const vector<shared_ptr<DailyContentData>> &vec_dailycontobj) override // 拼接内容主体
        {
            m_strResult += "    <Body>\n";
            for (auto iter = vec_dailycontobj.begin(); iter != vec_dailycontobj.end(); ++iter)
            {
                ostringstream oss; // 记得#include头文件sstream
                oss << (*iter)->getSpendTime();
                m_strResult += "        <Content>" + (*iter)->getContent() + "</Content>\n";
                m_strResult += "        <SpendTime>spend time: " + oss.str() + "hour" + "</SpendTime>\n";
            }
            m_strResult += "    </Body>\n";
        }
        void buildFooter(const shared_ptr<DailyFooterData> &dailyfooterobj) override // 拼接结尾
        {
            m_strResult += "    <Footer>\n";
            m_strResult += "        <UserName>reporter: " + dailyfooterobj->getUserName() + "</UserName>\n";
            m_strResult += "    </Footer>\n";

            m_strResult += "</DailyReport>\n";
        }
    };

    class FileDirector // 文件指挥者类
    {
        shared_ptr<FileBuilder> m_pFileBuilder; // 指向所有构建器类的父类
    public:
        FileDirector(const shared_ptr<FileBuilder> &ptmpBuilder) : m_pFileBuilder(ptmpBuilder) {}

        // 组装文件
        string Construct(const shared_ptr<DailyHeaderData> &dailyheaderobj, const vector<shared_ptr<DailyContentData>> &vec_dailycontobj, const shared_ptr<DailyFooterData> &dailyfooterobj)
        {
            m_pFileBuilder->clearResult(); // 先清空结果
            // 注意，有时指挥者需要和构建器通过参数传递的方式交换数据，这里指挥者通过委托的方式把功能交给构建器完成
            m_pFileBuilder->buildHeader(dailyheaderobj);
            m_pFileBuilder->buildBody(vec_dailycontobj);
            m_pFileBuilder->buildFooter(dailyfooterobj);
            return m_pFileBuilder->GetResult();
        }
    };
}

int main()
{
#if 0
    using namespace ns1;
    shared_ptr<Monster> pmonster(new M_Element()); // 创建一只元素类怪物
    pmonster->Assemble("1253679201245");
#endif

#if 0
    using namespace ns1;
    shared_ptr<MonsterBuilder> pMonsterBuilder(new M_UndeadBuilder()); // 创建亡灵类怪物构建器类对象
    shared_ptr<MonsterDirector> pDirector(new MonsterDirector(pMonsterBuilder));
    shared_ptr<Monster> pMonster = pDirector->Construct("1253679201245"); // 这里就构造出了一个完整的怪物对象
#endif

#if 0
    using namespace ns2;
    shared_ptr<DailyHeaderData> pdhd(new DailyHeaderData("R&D Department I", "November 1st"));

    shared_ptr<DailyContentData> pdcd1(new DailyContentData("Complete the requirement analysis of Project A", 3.5));
    shared_ptr<DailyContentData> pdcd2(new DailyContentData("Determine the tools used for project A development", 4.5));
    vector<shared_ptr<DailyContentData>> vec_dcd;
    vec_dcd.push_back(pdcd1);
    vec_dcd.push_back(pdcd2);

    shared_ptr<DailyFooterData> pdfd(new DailyFooterData("Xiao Li"));

    shared_ptr<ExportToTxtFile> file_ettxt(new ExportToTxtFile());
    file_ettxt->doExport(pdhd, vec_dcd, pdfd);

    shared_ptr<ExportToXmlFile> file_etxml(new ExportToXmlFile());
    file_etxml->doExport(pdhd, vec_dcd, pdfd);
#endif

#if 1
    using namespace ns2;
    shared_ptr<DailyHeaderData> pdhd(new DailyHeaderData("R&D Department I", "November 1st"));
    shared_ptr<DailyContentData> pdcd1(new DailyContentData("Complete the requirement analysis of Project A", 3.5));
    shared_ptr<DailyContentData> pdcd2(new DailyContentData("Determine the tools used for project A development", 4.5));
    vector<shared_ptr<DailyContentData>> vec_dcd;
    vec_dcd.push_back(pdcd1);
    vec_dcd.push_back(pdcd2);
    shared_ptr<DailyFooterData> pdfd(new DailyFooterData("Xiao Li"));

    shared_ptr<FileBuilder> pfb(new TxtFileBuilder());
    shared_ptr<FileDirector> pDtr(new FileDirector(pfb));
    cout << pDtr->Construct(pdhd, vec_dcd, pdfd) << endl;

    pfb.reset(new XmlFileBuilder());
    pDtr.reset(new FileDirector(pfb));
    cout << pDtr->Construct(pdhd, vec_dcd, pdfd) << endl;
#endif

    cout << "Over!\n";
    return 0;
}
