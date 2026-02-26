#ifndef LCD_H
#define LCD_H
#include "SPI.h"
#include "vector"
#include "Color.h"
#include "stdarg.h"
#include "string.h"
#include "stdlib.h"
#include "Define.h"
#include "ST7789.h"
#include "ST7735.h"
#include "Arduino.h"
#include "soc/soc.h"
#include "Arduino.h"
class LCD
{
    public:
    LCD(uint32_t WIDTH = 160, uint32_t HEIGHT = 80);
    void Config(uint16_t*BgrSprite, int8_t CS = -1, int8_t DC = -1, int8_t RST = 1, int8_t MOSI = -1, int8_t MISO = -1, int8_t SCLK = -1, uint32_t FREQ = 40000000, uint8_t Type = LCD_ST7735);
    void Init(SPIClass *SPI_CLASS = &SPI);
    void Setting();
    uint32_t PushSprite(uint16_t Mode);
    uint16_t* ImgSprite;
    uint16_t**ImgArea;
    LCD();
    ~LCD();

    void SetRotation(uint8_t Rotation);
    uint8_t Check(uint8_t*Str,uint32_t*Str_Code);
    uint8_t GetWidthChar(uint32_t Char, uint8_t Font);

    void FillScreen(uint16_t Color);
    void DrawPixel(uint16_t X,uint16_t Y, uint16_t Color);
    void DrawPixels(uint16_t X,uint16_t Y, uint16_t Color);
    void CreateSprite(uint16_t X,uint16_t Y,uint16_t Width,uint16_t Height);
    // void FillRect(uint16_t X,uint16_t Y,uint16_t Width,uint16_t Height,uint16_t Color);
    String Convert(int32_t Number,uint8_t Type,uint8_t Len);

    void SetStrings(uint8_t Font);
    void SetString(uint16_t Color);
    void SetString(uint16_t Color,uint16_t BgrColor);
    void SetString(uint16_t X,uint16_t Y,uint16_t Color);
    void SetStrings(uint16_t X,uint16_t Y,uint16_t Color, uint8_t Font);
    void SetString(uint16_t X,uint16_t Y,uint16_t Color,uint16_t BgrColor);

    void Print(String Str);
    void Print(double NumBer);
    void Print(int32_t NumBer);
    void Print(const char*Str);
    void Print(String Str,uint16_t Color);
    void Print(int32_t NumBer,uint8_t Type,uint8_t Len);
    void Print(uint16_t X,uint16_t Y, String Str,uint8_t Font,uint16_t Color);
    void Print(int16_t X,int16_t Y, String Str,uint8_t Font,uint16_t Color,uint16_t MaxWidth,uint16_t MaxHeight,bool Stt,int16_t Limit = 0);
    
    void Println(String Str);
    void Println(double NumBer);
    void Println(int32_t NumBer);
    void Println(const char*Str);
    void Println(String Str,uint16_t Color);
    void Println(int32_t NumBer,uint8_t Type,uint8_t Len);
    void Println(uint16_t X,uint16_t Y, String Str,uint8_t Font,uint16_t Color);

    void DrawChar(uint16_t X,uint16_t Y,uint32_t Char, uint8_t Font,uint16_t Color);

    void Printf(const char*Char,...);
    void PrintRight(const char*Char,...);


    bool CheckHover();
    void LockScreen();
    bool CheckClick();
    bool CheckHovers();
    bool CheckClicks();
    bool CheckClick(uint16_t X,uint16_t Y,uint16_t Width,uint16_t Height);
    bool CheckHover(uint16_t X,uint16_t Y,uint16_t Width,uint16_t Height);
    bool CheckClicks(uint16_t X,uint16_t Y,uint16_t Width,uint16_t Height);
    bool CheckHovers(uint16_t X,uint16_t Y,uint16_t Width,uint16_t Height);

    void DrawPoint();
    void DrawPoint(int16_t Dx,int16_t Dy,uint8_t Mode);
    void DrawIcon(uint16_t X,uint16_t Y,uint32_t Icon,uint16_t Color);
    void DrawIcons(uint16_t X,uint16_t Y,uint32_t Icon,uint16_t Color);
    void ValueClick(uint8_t (*GetValueClick)(void));
    void PushIcons(uint16_t X,uint16_t Y,uint32_t Icon,uint16_t Color);
    void CreatePoint(uint16_t X,uint16_t Y,uint8_t Size,uint8_t BgrSize,uint16_t Color,uint16_t BgrColor);

    void FillRects(uint16_t X,uint16_t Y,uint16_t Width,uint16_t Height,uint16_t Color);
    void FillRect(uint16_t X,uint16_t Y,uint16_t Width,uint16_t Height,uint16_t Color);
    void DrawRect(uint16_t X,uint16_t Y,uint16_t Width,uint16_t Height,uint16_t Line,uint16_t Color);
    void DrawLine(int16_t X0, int16_t Y0,int16_t X1,int16_t Y1,int16_t Line,uint16_t Color,uint8_t FastMode = 0);
    void DrawRects(uint16_t X,uint16_t Y,uint16_t Width,uint16_t Height,uint16_t Line,uint16_t Color);

    void DrawImg(uint16_t X, uint16_t Y, uint16_t Width, uint16_t Height, uint16_t*Img);

    struct POINT
    {
        uint16_t X;
        uint16_t Y;
        uint16_t Width;
        uint16_t Height;
        uint16_t Mode;
        uint16_t Running;
        uint32_t TimeLast;
        void (*FunOnHover)(void);
        void (*FunUnHover)(void);
        void (*FunOnClick)(void);
        void (*FunUnClick)(void);
    };
    void RunAllFun(uint16_t Mode);
    bool CheckHover(POINT*OnHover);
    bool CheckClick(POINT*OnClick);
    bool CheckHovers(POINT*OnHover);
    bool CheckClicks(POINT*OnClick);
    POINT*Point(uint16_t X,uint16_t Y,uint16_t Width,uint16_t Height,uint16_t Mode);
    void Point(void (*OnHover)(void),void (*UnHover)(void),void (*OnClick)(void),void (*UnClick)(void),POINT*Point);
    struct FUNPUSH
    {
        uint16_t Mode;
        void (*FunPoint)();
    };
    void RunFunSprite(uint16_t Mode);
    void SetFun(uint16_t Mode,FUNPUSH*FunPush);
    FUNPUSH*PushSprite(void (*Fun)(),uint16_t Mode);
    struct FUNCTION
    {
        uint16_t Mode;
        uint32_t TimeSet;
        uint32_t TimeLast = 0;
        uint16_t LimitRun = 1;
        uint16_t NumOfRun = 0;
        void (*Function)(void);
    };
    FUNCTION*SetTimeOut(void (*Fun)(),uint16_t Mode,uint32_t Time,uint16_t Limit);
    void SetTimeOut(uint16_t Mode,uint32_t Time,FUNCTION*Function);
    struct ICON
    {
        uint16_t X;
        uint16_t Y;
        uint16_t Color;
        uint16_t Mode;
        uint32_t CodeIcon;
        void (*FunOnHover)(void);
        void (*FunOnClick)(void);
    };
    ICON*CreateIcon(uint16_t X,uint16_t Y,uint32_t CodeIcon,uint16_t Color,uint16_t Mode);
    void SetIcon(void (*FunOnHover)(void),void (*FunOnClick)(void),ICON*Icon);
    void SetPoint(int8_t Type,uint16_t Color,uint16_t BgrColor);
    void SetPoint(uint8_t Type);
    private:

    FUNCTION *FUN;
    ICON *ARRICON;
    POINT*FUNPOINT;
    FUNPUSH *PUSHSPRITE;


    void EndWrite(void);
    void BeginWrite(void);
    void WriteCmd(uint8_t Data);
    void WriteData(uint8_t Data);

    uint16_t**ImgAreaH;
    uint16_t**ImgAreaW;

    static uint16_t NUM_ICON;
    static uint16_t NUM_CHAR_16X8;
    static uint16_t NUM_CHAR_32X32;

    uint32_t LCD_WIDTH = 160;
    uint32_t LCD_HEIGHT = 80;
    uint32_t LCD_RT_WIDTH = 160;
    uint32_t LCD_RT_HEIGHT = 80;

    uint32_t LCD_START_WIDTH = 160;
    uint32_t LCD_START_HEIGHT = 80;
    uint32_t LCD_RT_START_WIDTH = 160;
    uint32_t LCD_RT_START_HEIGHT = 80;

    uint16_t LCD_MAX_WIDTH = 0;
    uint16_t LCD_MAX_HEIGHT = 0;
    uint16_t LCD_RT_MAX_WIDTH = 0;
    uint16_t LCD_RT_MAX_HEIGHT = 0;

    uint16_t LCD_SPACE_LEFT = 2;
    uint16_t LCD_STR_X = 3;
    uint16_t LCD_STR_Y = 0;

    uint8_t  LCD_CHAR_TAB_W = 8;
    uint8_t  LCD_CHAR_TAB_H = 32;
    uint8_t  LCD_CHAR_SPACE = 3;
    int8_t   LCD_CHAR_R_X = 1;
    int8_t   LCD_CHAR_R_Y = 0;

    SPIClass* SPI_LCD = &SPI;
    uint8_t TYPE_LCD = LCD_ST7735;
    uint32_t TIME_WRITE = 0;
    uint16_t TEXT_COLOR = 0xFFFF;
    uint16_t BGR_COLOR = 0x00;
    uint8_t LCD_ROTATION = 0;
    uint8_t LCD_FONT = CHAR_16x8;
    uint8_t LCD_NEW_ROTATION = 0;
    int8_t PIN_CS = 12;
    int8_t PIN_DC = 11;
    int8_t PIN_RST = 7;
    int8_t PIN_MOSI = 10;
    int8_t PIN_MISO = 10;
    int8_t PIN_SCLK = 15;
    uint32_t SPI_FREQ = 40000000;
    int8_t SPI_PORT = HSPI;
    int16_t COL_START = 26;
    int16_t ROW_START = 1;
    bool ON_WRITE_SPI = false;
    bool LCD_AUTO_NEWLINE = false;
    bool LCD_USE_BGR_COLOR = false;


    bool     LCD_POINT                        =        false;
    bool     LCD_SPRITE                       =        false;
    bool     LCD_ON_CLICK                     =        false;
    bool     LCD_ON_WRITE_SPI                 =        false;
    uint8_t  LCD_POINT_SIZE                   =            5;
    uint8_t  LCD_POINT_TYPE                   =            0;
    uint8_t  LCD_POINT_BGR_SIZE               =            1;
    uint8_t  LCD_POWER_VALUE                  =         HIGH;
    uint8_t  LCD_PIN_CLICK                    =            0;
    uint8_t  LCD_MODE_CLICK                   =          LOW;
    int16_t LCD_POINT_X                      =            0;
    int16_t LCD_POINT_Y                      =            0;
    uint16_t LCD_POINT_COLOR                  =       0xFFFF;
    uint16_t LCD_POINT_BGR_COLOR              =       0x0000;

    uint16_t LCD_NUM_ARR_ICON                 =            0;
    uint16_t LCD_NUM_FUN_SETTIME              =            0;
    uint16_t LCD_NUM_FUN_POINT                =            0;
    uint16_t LCD_NUM_FUN_RUN_PUSH             =            0;
    uint8_t (*GETCLICK)(void);


};
#endif