#include <iostream>
#include <memory>
using namespace std;

// 中间层角色，隔离接口，两部分模块通过中间层打交道
// 提供简单接口，不与底层直接打交道
// 提供统一接口，访问子系统中的一群接口
// 定义高层接口，让子系统更容易使用

// 迪米特法则
// 一个对象对其他对象的了解应尽可能少，降低对象间耦合，提高系统的可维护性

namespace ns1
{
    class graphic // 图形相关类
    {
        graphic(){};
        graphic(const graphic &tmpobj);
        graphic &operator=(const graphic &tmpobj);
        ~graphic(){};

    public:
        static graphic &getInstance()
        {
            static graphic instance;
            return instance;
        }

    public:
        void display(bool enable) // 是否全屏显示(true：是)
        {
            cout << "full screen->" << enable << endl;
        }
        void effect(bool enable) // 是否开启特效(true：是)
        {
            cout << "special effects->" << enable << endl;
        }
        void resolution(int index) // 设置窗口分辨率
        {
            cout << "resolution ratio->" << index << endl;
        }
        void antialiasing(bool enable) // 是否开启抗锯齿(true：是)
        {
            cout << "anti-aliasing->" << enable << endl;
        }
    };

    class sound // 声音相关类
    {
        sound(){};
        sound(const sound &tmpobj);
        sound &operator=(const sound &tmpobj);
        ~sound(){};

    public:
        static sound &getInstance()
        {
            static sound instance;
            return instance;
        }

    public:
        void bgsound(bool enable) // 是否开启背景声音(true：是)
        {
            cout << "background sound->" << enable << endl;
        }
        void envirsound(bool enable) // 是否开启环境音效(true：是)
        {
            cout << "environmental sound effect->" << enable << endl;
        }
        void expsound(bool enable) // 是否开启表情声音(true：是)
        {
            cout << "expressional voice->" << enable << endl;
        }
        void setvolume(int level) // 音量大小设置(0-100)
        {
            cout << "volume->" << level << endl;
        }
    };

    class chatvoice // 语音聊天相关类
    {
        chatvoice(){};
        chatvoice(const chatvoice &tmpobj);
        chatvoice &operator=(const chatvoice &tmpobj);
        ~chatvoice(){};

    public:
        static chatvoice &getInstance()
        {
            static chatvoice instance;
            return instance;
        }

    public:
        void micvolume(int level) // 麦克风音量大小设置(0-100)
        {
            cout << "microphone volume->" << level << endl;
        }
        void micsens(int level) // 麦克灵敏度设置(0-100)
        {
            cout << "microphone sensitivity->" << level << endl;
        }
        void chatvolume(int level) // 聊天音量设置(0-100)
        {
            cout << "chat volume->" << level << endl;
        }
    };

    class conffacade // 扮演外观模式角色的类
    {
        conffacade(){};
        conffacade(const conffacade &tmpobj);
        conffacade &operator=(const conffacade &tmpobj);
        ~conffacade(){};

    public:
        static conffacade &getInstance()
        {
            static conffacade instance;
            return instance;
        }

    public:
        void LowConfComputer() // 对于低配置电脑，只开启一些低配置选项
        {
            graphic &g_gp = graphic::getInstance();
            g_gp.display(true); // 全屏耗费资源更低
            g_gp.effect(false);
            g_gp.resolution(2);
            g_gp.antialiasing(false);

            sound &g_snd = sound::getInstance();
            g_snd.bgsound(false);
            g_snd.envirsound(false);
            g_snd.expsound(false);
            g_snd.setvolume(15);

            chatvoice &g_cv = chatvoice::getInstance();
            g_cv.micvolume(20);
            g_cv.micsens(50);
            g_cv.chatvolume(60);
        }

        void HighConfComputer() // 对于高配置电脑，能达到最好效果的项全部开启
        {
            graphic &g_gp = graphic::getInstance();
            g_gp.display(false);
            g_gp.effect(true);
            g_gp.resolution(0);
            g_gp.antialiasing(true);

            sound &g_snd = sound::getInstance();
            g_snd.bgsound(true);
            g_snd.envirsound(true);
            g_snd.expsound(true);
            g_snd.setvolume(50);

            chatvoice &g_cv = chatvoice::getInstance();
            g_cv.micvolume(100);
            g_cv.micsens(100);
            g_cv.chatvolume(100);
        }
    };
}

namespace ns2
{
    class Screen // 屏幕
    {
    public:
        void On() { cout << "screen on!" << endl; }
        void Off() { cout << "screen off!" << endl; }
    };

    class Light // 灯光
    {
    public:
        void On() { cout << "light open!" << endl; }
        void Off() { cout << "light close!" << endl; }
    };

    class Speaker // 音箱
    {
    public:
        void On() { cout << "speaker on!" << endl; }
        void Off() { cout << "speaker off!" << endl; }
    };

    class DvdPlayer // DVD播放器
    {
    public:
        void On() { cout << "dvd open!" << endl; }
        void Off() { cout << "dvd close!" << endl; }
    };

    class PlayerStation // 游戏机
    {
    public:
        void On() { cout << "playerstation on!" << endl; }
        void Off() { cout << "playerstation off!" << endl; }
    };

    class HomeTheaterFacade // 家庭影院外观模式类
    {
        Screen scnobj;
        Light lgobj;
        Speaker spkobj;
        DvdPlayer dpobj;
        PlayerStation psobj;

    public:
        void WatchMovie() // 看电影
        {
            // 屏幕打开，灯光熄灭，音箱打开，DVD播放器打开，游戏机关闭。
            scnobj.On();
            lgobj.Off();
            spkobj.On();
            dpobj.On();
            psobj.Off();
        }
        void PlayGame() // 玩游戏
        {
            // 屏幕打开，灯光打开，音箱打开，DVD播放器关闭，游戏机打开。
            scnobj.On();
            lgobj.On();
            spkobj.On();
            dpobj.Off();
            psobj.On();
        }
    };
}

namespace ns3
{
    class Shape
    {
    public:
        virtual ~Shape() = default;
        virtual void draw() const = 0;
    };

    class Rectangle : public Shape
    {
    public:
        void draw() const override { cout << "Rectangle::draw()" << endl; }
    };

    class Square : public Shape
    {
    public:
        void draw() const override { cout << "Square::draw()" << endl; }
    };

    class Circle : public Shape
    {
    public:
        void draw() const override { cout << "Circle::draw()" << endl; }
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
        void drawCircle() const { circle->draw(); }
        void drawRectangle() const { rectangle->draw(); }
        void drawSquare() const { square->draw(); }
    };
}

int main()
{
#if 0
    using namespace ns1;
    graphic &g_gp = graphic::getInstance();
    g_gp.display(false);
    g_gp.effect(true);
    g_gp.resolution(2);
    g_gp.antialiasing(false);
    cout << "---------------" << endl;
    sound &g_snd = sound::getInstance();
    g_snd.setvolume(80);
    g_snd.envirsound(true);
    g_snd.bgsound(false);
    cout << "---------------" << endl;
    chatvoice &g_cv = chatvoice::getInstance();
    g_cv.chatvolume(70);
    g_cv.micsens(65);
#endif

#if 0
    using namespace ns1;
    conffacade &g_cffde = conffacade::getInstance();
    cout << "Low-configuration computer" << endl;
    g_cffde.LowConfComputer();
    cout << "------------------" << endl;
    cout << "high-configuration computer" << endl;
    g_cffde.HighConfComputer();
#endif

#if 0
    ns2::HomeTheaterFacade htfacobj;
    cout << "movie---------------" << endl;
    htfacobj.WatchMovie();
    cout << "game---------------" << endl;
    htfacobj.PlayGame();
#endif

#if 1
    using namespace ns3;
    shared_ptr<ShapeMaker> shapeMaker(new ShapeMaker());
    shapeMaker->drawCircle();
    shapeMaker->drawRectangle();
    shapeMaker->drawSquare();
#endif

    cout << "Over!\n";
    return 0;
}
