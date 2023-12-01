#include <iostream>
#include <memory>
using namespace std;

// 将抽象部分（业务功能）与其实现部分（具体的平台实现）分离，使它们都可以独立地变化和扩展
// 4种角色
// Abstraction（抽象部分相关接口），定义抽象类接口，包含指向Implementor类型对象的指针
// RefinedAbstraction（扩充抽象部分接口），实现Abstraction接口，并调用Implementor方法
// Implementor（实现部分相关接口），定义实现类接口
// ConcreteImplementor（实现部分具体类），实现Implementor接口

namespace ns1
{
    class Image // 图像显示相关类
    {
    public:
        virtual ~Image() {}
        // 根据pData（缓冲区）中的内容以及iDataLen所指示的缓冲区的长度，将这些数据显示出来
        void draw(const char *pfilename)
        {
            int iLen = 0;
            char *pData = parsefile(pfilename, iLen);
            if (iLen > 0)
            {
                cout << "Displays the image data in the buffer pointed by pData" << endl;
                //......
                delete pData; // 模拟代码中因为pData的内存是new出来的，这里需要释放该内存
            }
        }

    private:
        virtual char *parsefile(const char *pfilename, int &iLen) = 0; // 根据文件名分析文件内容，每个子类因为图像文件格式不同，会有不同的读取和处理代码
    };

    class Image_png : public Image // 处理png格式图像文件的显示
    {
    private:
        // 读取png文件内容并进行解析，最终整理成统一的二进制数据格式返回
        char *parsefile(const char *pfilename, int &iLen) override
        {
            // 以下是模拟代码：模拟从图像文件中读取到了数据，最终转换成了100个字节的数据格式（事先约定好的格式规范）并返回
            cout << "Start analyzing the data in the png file and put the analysis results in pData";
            iLen = 100;
            char *presult = new char[iLen];
            return presult;
        }
    };

    class Image_jpg : public Image // 处理jpg格式图像文件的显示
    {
    private:
        char *parsefile(const char *pfilename, int &iLen) override
        {
            cout << "Start analyzing the data in the jpg file and put the analysis results in pData";
            iLen = 0;
            return NULL;
        }
    };

    class Image_bmp : public Image // 处理bmp格式图像文件的显示
    {
    private:
        char *parsefile(const char *pfilename, int &iLen) override
        {
            cout << "Start analyzing the data in the bmp file and put the analysis results in pData";
            iLen = 0;
            return NULL;
        }
    };
}

namespace ns2
{
    class ImageOS // 操作系统类型相关类
    {
    public:
        virtual ~ImageOS() {}
        virtual void draw(char *pData, int iLen) = 0;
    };

    class ImageOS_Windows : public ImageOS // Windows操作系统
    {
    public:
        void draw(char *pData, int iLen) override
        {
            cout << "Display the image data in the buffer pointed by pData under Windows operating system" << endl;
            //...具体处理代码略
        }
    };

    class ImageOS_Linux : public ImageOS // Linux操作系统
    {
    public:
        void draw(char *pData, int iLen) override
        {
            cout << "Display the image data in the buffer pointed by pData under Linux operating system" << endl;
            //...具体处理代码略
        }
    };

    class ImageOS_Mac : public ImageOS // Mac操作系统
    {
    public:
        void draw(char *pData, int iLen) override
        {
            cout << "Display the image data in the buffer pointed by pData under Mac operating system" << endl;
            //...具体处理代码略
        }
    };

    class ImageFormat // 图像文件格式相关类
    {
    protected:
        shared_ptr<ImageOS> m_pImgOS; // 委托
    public:
        virtual ~ImageFormat() {}
        ImageFormat(const shared_ptr<ImageOS> &pimgos) : m_pImgOS(pimgos) {}
        virtual void parsefile(const char *pfilename) = 0; // 根据文件名分析文件内容，每个子类因为图像文件格式不同，会有不同的读取和处理代码
    };

    class Image_png : public ImageFormat // png格式的图像文件
    {
    public:
        Image_png(const shared_ptr<ImageOS> &pimgos) : ImageFormat(pimgos) {} // 构造函数
        void parsefile(const char *pfilename) override
        {
            cout << "Start analyzing the data in the png file and put the analysis results in pData, ";
            // 以下是模拟代码：模拟从图像文件中读取到了数据，最终转换成了100个字节的数据格式（事先约定好的格式规范）并返回
            int iLen = 100;
            char *presult = new char[iLen];
            m_pImgOS->draw(presult, iLen);

            // 释放资源
            delete presult;
        }
    };

    class Image_jpg : public ImageFormat // jpg格式的图像文件
    {
    public:
        Image_jpg(const shared_ptr<ImageOS> &pimgos) : ImageFormat(pimgos) {} // 构造函数
        void parsefile(const char *pfilename) override
        {
            cout << "Start analyzing the data in the jpg file and put the analysis results in pData, ";
            //......
        }
    };

    class Image_bmp : public ImageFormat // bmp格式的图像文件
    {
    public:
        Image_bmp(const shared_ptr<ImageOS> &pimgos) : ImageFormat(pimgos) {} // 构造函数
        void parsefile(const char *pfilename) override
        {
            cout << "Start analyzing the data in the bmp file and put the analysis results in pData, ";
            //......
        }
    };
}

int main()
{
#if 0
    using namespace ns1;
    shared_ptr<Image> pImg(new Image_png());
    pImg->draw("c:\\somedir\\filename.jpg");
#endif

#if 1
    using namespace ns2;
    shared_ptr<ImageOS> pimgos_windows(new ImageOS_Windows());       // 针对Windows操作系统
    shared_ptr<ImageFormat> pimg_png(new Image_png(pimgos_windows)); // 运行时把图像文件格式(png)和操作系统(Windows) 动态组合到一起
    pimg_png->parsefile("c:\\somedir\\filename.png");
#endif

    cout << "Over!\n";
    return 0;
}