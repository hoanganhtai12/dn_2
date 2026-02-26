#include "LCD.h"
#include "Font16x8.h"
#include "Font16x16.h"
#include "Font32x32.h"
uint16_t LCD::NUM_CHAR_16X8 = sizeof(Font16x8)/20;
uint16_t LCD::NUM_CHAR_32X32 = sizeof(Font32x32)/(33*4);
uint16_t LCD::NUM_ICON = sizeof(Font16x16)/36;

LCD::LCD()
{
}

// void LCD::FillRect(uint16_t X,uint16_t Y,uint16_t Width,uint16_t Height,uint16_t Color)
// {
//   if(Color == TFT_SWAP_COLOR)
//     Color = 0xFFFF - Color;
//   else if(Color == TFT_UNCHANGED)
//     return;
//   for(int i = X;i < X + Width;i++)
//   {
//     for (int k = Y; k < Y + Height; k++)
//     {
//       DrawPixel(i,k,Color);
//     }
//   }
// }

void LCD::CreateSprite(uint16_t X,uint16_t Y,uint16_t Width,uint16_t Height)
{
    LCD_WIDTH = Width;
    LCD_HEIGHT = Height;
    LCD_RT_WIDTH = Width;
    LCD_RT_HEIGHT = Height;
    LCD_START_WIDTH = X;
    LCD_START_HEIGHT = Y;
    LCD_RT_START_WIDTH = X;
    LCD_RT_START_HEIGHT = Y;
    ImgAreaH = new uint16_t*[TFTMax(Width, Height)];
    ImgAreaW = new uint16_t*[TFTMax(Width, Height)];
    for (int i = 0; i < Height; i++)
    {  
      ImgAreaH[i] = &ImgSprite[Width*(i)];
    }
    for (int i = 0; i < Height; i++)
    {
      ImgAreaW[i] = &ImgSprite[Height*(i)];
    }
    SetRotation(4);
}


void LCD::DrawPixel(uint16_t X,uint16_t Y, uint16_t Color)
{
    if(X >= LCD_WIDTH || Y >= LCD_HEIGHT)return;
    if(Color == TFT_SWAP_COLOR)
    {
      ImgArea[Y][X] = 0xFFFF - ImgArea[Y][X];
    }
    else if(Color != TFT_UNCHANGED)
    {
      ImgArea[Y][X] = Color;
    }
}

void LCD::DrawPixels(uint16_t X,uint16_t Y, uint16_t Color)
{
  if(X >= LCD_RT_WIDTH || Y >= LCD_RT_HEIGHT)return;
  if(LCD_ROTATION == 2 || LCD_ROTATION ==0)
  {
    if(Color == TFT_SWAP_COLOR)
    {
      ImgArea[Y][X] = 0xFFFF - ImgArea[Y][X];
    }
    else if(Color != TFT_UNCHANGED)
    {
      ImgArea[Y][X] = Color;
    }
  }
  else
  {
    if(Color == TFT_SWAP_COLOR)
    {
      ImgArea[X][Y] = 0xFFFF - ImgArea[X][Y];
    }
    else if(Color != TFT_UNCHANGED)
    {
      ImgArea[X][Y] = Color;
    }
  }
}



void LCD::FillScreen(uint16_t Color)
{
  if((Color & 0xFF) == ((Color<<8) & 0xFF))
  {
    memset((void*)ImgSprite, Color&0xFF, LCD_WIDTH*LCD_HEIGHT*2);
  }
  else
  {
    FillRect(0,0,LCD_WIDTH,LCD_HEIGHT,Color);
  }
}

LCD::LCD(uint32_t WIDTH, uint32_t HEIGHT)
{
  LCD_MAX_WIDTH = WIDTH;
  LCD_MAX_HEIGHT = HEIGHT;
  LCD_RT_MAX_WIDTH = WIDTH;
  LCD_RT_MAX_HEIGHT = HEIGHT;
}

void LCD::Config(uint16_t*BgrSprite, int8_t CS, int8_t DC, int8_t RST, int8_t MOSI, int8_t MISO, int8_t SCLK, uint32_t FREQ, uint8_t Type)
{
    if(BgrSprite) ImgSprite = BgrSprite;
    if(CS >= 0)
    {
        PIN_CS = CS;
        pinMode(PIN_CS, OUTPUT);
    }
    if(DC >= 0)
    {
        PIN_DC = DC;
        pinMode(PIN_DC, OUTPUT);
        digitalWrite(PIN_DC, HIGH);
    }
    if(RST >= 0)
    {
        PIN_RST = RST;
        pinMode(PIN_RST, OUTPUT);
        digitalWrite(PIN_RST, HIGH);
        vTaskDelay(100);
    }
    if(MOSI >= 0){PIN_MOSI = MOSI;}
    if(MISO >= 0){PIN_MISO = MISO;}
    if(SCLK >= 0){PIN_SCLK = SCLK;}
    if(FREQ > 0){SPI_FREQ = FREQ;}
    if(Type > 0){TYPE_LCD = Type;}
}

void LCD::Init(SPIClass *SPI_CLASS)
{
    SPI_LCD = SPI_CLASS;
    SPI_LCD->begin(PIN_SCLK, PIN_MISO, PIN_MOSI, PIN_CS);
}

void LCD::Setting()
{
    if(TYPE_LCD == LCD_ST7735)
    {
        BeginWrite();
        digitalWrite(PIN_DC, HIGH);
        EndWrite();
        WriteCmd(ST7735_SWRESET);delay(150);
        WriteCmd(ST7735_SLPOUT);delay(255);
        WriteCmd(ST7735_FRMCTR1);
        WriteData(0x01);WriteData(0x2C);WriteData(0x2D);
        WriteCmd(ST7735_FRMCTR2);
        WriteData(0x01);WriteData(0x2C);WriteData(0x2D);
        WriteCmd(ST7735_FRMCTR3);
        WriteData(0x01);WriteData(0x2C);WriteData(0x2D);
        WriteData(0x01);WriteData(0x2C);WriteData(0x2D);
        WriteCmd(ST7735_INVCTR);
        WriteData(0x07);
        WriteCmd(ST7735_PWCTR1);
        WriteData(0xA2);WriteData(0x02);WriteData(0x84);
        WriteCmd(ST7735_PWCTR2);
        WriteData(0xC5);
        WriteCmd(ST7735_PWCTR3);
        WriteData(0x0A);WriteData(0x00);
        WriteCmd(ST7735_PWCTR4);
        WriteData(0x8A);WriteData(0x2A);
        WriteCmd(ST7735_PWCTR5);
        WriteData(0x8A);WriteData(0xEE);
        WriteCmd(ST7735_VMCTR1);
        WriteData(0x0E);
        WriteCmd(ST7735_INVON);
        WriteCmd(ST7735_MADCTL);
        WriteData(0xC0);
        WriteCmd(ST7735_COLMOD);
        WriteData(0x05);
        WriteCmd(ST7735_CASET);
        WriteData(0x00);WriteData(0x00);WriteData(0x00);WriteData(0x7F);
        WriteCmd(ST7735_RASET);
        WriteData(0x00);WriteData(0x00);WriteData(0x00);WriteData(0x9F);
        WriteCmd(ST7735_GMCTRP1);
        WriteData(0x02); WriteData(0x1c); WriteData(0x07); WriteData(0x12);
        WriteData(0x37); WriteData(0x32); WriteData(0x29); WriteData(0x2d);
        WriteData(0x29); WriteData(0x25); WriteData(0x2B); WriteData(0x39);
        WriteData(0x00); WriteData(0x01); WriteData(0x03); WriteData(0x10);
        WriteCmd(ST7735_GMCTRN1);
        WriteData(0x03); WriteData(0x1d); WriteData(0x07); WriteData(0x06);
        WriteData(0x2E); WriteData(0x2C); WriteData(0x29); WriteData(0x2D);
        WriteData(0x2E); WriteData(0x2E); WriteData(0x37); WriteData(0x3F);
        WriteData(0x00); WriteData(0x00); WriteData(0x02); WriteData(0x10);
        WriteCmd(ST7735_NORON);delay(10);
        WriteCmd(ST7735_DISPON);delay(100);
        EndWrite();
    }
    else if (TYPE_LCD == LCD_ST7789)
    {
        BeginWrite();
        digitalWrite(PIN_DC, HIGH);
        EndWrite();
        WriteCmd(ST7789_SWRESET);delay(150);
        WriteCmd(ST7789_SLPOUT);delay(120);
        WriteCmd(ST7789_NORON);
        WriteCmd(ST7789_MADCTL);
        WriteData(0x08);WriteCmd(0xB6);WriteData(0x0A);WriteData(0x82);
        WriteCmd(ST7789_RAMCTRL);
        WriteData(0x00);WriteData(0xE0);
        WriteCmd(ST7789_COLMOD);
        WriteData(0x55);delay(10);
        WriteCmd(ST7789_PORCTRL);
        WriteData(0x0c);WriteData(0x0c);WriteData(0x00);
        WriteData(0x33);WriteData(0x33);
        WriteCmd(ST7789_GCTRL);
        WriteData(0x35);
        WriteCmd(ST7789_VCOMS);
        WriteData(0x28);		
        WriteCmd(ST7789_LCMCTRL);
        WriteData(0x0C);
        WriteCmd(ST7789_VDVVRHEN);
        WriteData(0x01);WriteData(0xFF);
        WriteCmd(ST7789_VRHS);
        WriteData(0x10);
        WriteCmd(ST7789_VDVSET);
        WriteData(0x20);
        WriteCmd(ST7789_FRCTR2);
        WriteData(0x0f);
        WriteCmd(ST7789_PWCTRL1);
        WriteData(0xa4);
        WriteData(0xa1);
        WriteCmd(ST7789_PVGAMCTRL);
        WriteData(0xd0);WriteData(0x00);WriteData(0x02);WriteData(0x07);
        WriteData(0x0a);WriteData(0x28);WriteData(0x32);WriteData(0x44);
        WriteData(0x42);WriteData(0x06);WriteData(0x0e);WriteData(0x12);
        WriteData(0x14);WriteData(0x17);
        WriteCmd(ST7789_NVGAMCTRL);
        WriteData(0xd0);WriteData(0x00);WriteData(0x02);WriteData(0x07);
        WriteData(0x0a);WriteData(0x28);WriteData(0x31);WriteData(0x54);
        WriteData(0x47);WriteData(0x0e);WriteData(0x1c);WriteData(0x17);
        WriteData(0x1b);WriteData(0x1e);
        WriteCmd(ST7789_INVON);
        WriteCmd(ST7789_CASET);
        WriteData(0x00);WriteData(0x00);WriteData(0x00);WriteData(0xEF);
        WriteCmd(ST7789_RASET);
        WriteData(0x00);WriteData(0x00);WriteData(0x01);WriteData(0x3F);
        WriteCmd(ST7789_DISPON);delay(120);
        EndWrite();
    }
}

LCD::~LCD()
{
}

uint32_t LCD::PushSprite(uint16_t Mode)
{
  RunAllFun(Mode);
  RunFunSprite(Mode&RUN_FUN_BEFORE);
  if(Mode&DRAW_POINT)DrawPoint();
  uint32_t Len = LCD_WIDTH*LCD_HEIGHT;
  uint32_t S = LCD_WIDTH*LCD_HEIGHT-1;
  ON_WRITE_SPI = true;
  BeginWrite();
  SPI_BUSY_CHECK;DC_C_L;
  TFT_Write_8(ST7789_CASET);DC_D_H;
  TFT_Write_32C(LCD_START_WIDTH + COL_START,LCD_WIDTH-1 + LCD_START_WIDTH + COL_START);DC_C_L;
  TFT_Write_8(ST7789_RASET);DC_D_H;
  TFT_Write_32C(LCD_START_HEIGHT + ROW_START,LCD_HEIGHT-1 + LCD_START_HEIGHT + ROW_START);DC_C_L;
  TFT_Write_8(ST7789_RAMWR);DC_D_H;
  SPI_LCD->writePixels((uint16_t*)ImgSprite,Len*2);
  EndWrite();
  ON_WRITE_SPI = false;
  uint32_t Time = millis() - TIME_WRITE;
  RunFunSprite(Mode&RUN_FUN_AFTER);
  TIME_WRITE = millis();
  return Time;
}


void LCD::BeginWrite(void)
{
  SPI_LCD->beginTransaction(SPISettings(SPI_FREQ,MSBFIRST,TFT_SPI_MODE));
  CS_B_L;
  SET_BUS_WRITE_MODE;
}
void LCD::EndWrite(void)
{
  SPI_BUSY_CHECK;
  CS_S_H;
  SET_BUS_READ_MODE;
  SPI_LCD->endTransaction();
}
void LCD::WriteCmd(uint8_t Data)
{
  BeginWrite();
  DC_C_L;
  TFT_Write_8(Data);
  DC_D_H;
  EndWrite();
}
void LCD::WriteData(uint8_t Data)
{
  BeginWrite();
  DC_D_H;
  TFT_Write_8(Data);
  DC_C_L;
  EndWrite();
}

void LCD::SetRotation(uint8_t Rotation)
{
  if(LCD_NEW_ROTATION != Rotation || LCD_ROTATION != Rotation)
  {
    LCD_ROTATION = Rotation % 4;
    LCD_NEW_ROTATION = LCD_ROTATION;
    CS_B_L;
    WriteCmd(ST7789_MADCTL);
    switch (LCD_ROTATION)
    {
      case 0:WriteData(0x08); break;
      case 1:WriteData(0x68); break;
      case 2:WriteData(0xC8); break;
      case 3:WriteData(0xA8); break;
      default: break;
    }
    EndWrite();
    delay(1);
  }
  LCD_ROTATION = Rotation % 4;
  LCD_STR_X = LCD_SPACE_LEFT;
  LCD_STR_Y = 0;
  if(LCD_ROTATION==0||LCD_ROTATION==2)
  {
    LCD_WIDTH = LCD_RT_WIDTH;
    LCD_HEIGHT = LCD_RT_HEIGHT;
    LCD_MAX_WIDTH = LCD_RT_MAX_WIDTH;
    LCD_MAX_HEIGHT = LCD_RT_MAX_HEIGHT;
    ImgArea = ImgAreaH;
    if(LCD_ROTATION == 0)
    {
      LCD_START_WIDTH = LCD_RT_START_WIDTH;
      LCD_START_HEIGHT = LCD_RT_START_HEIGHT;
    }
    else
    {
      LCD_START_WIDTH = LCD_MAX_WIDTH - LCD_WIDTH - LCD_RT_START_WIDTH;
      LCD_START_HEIGHT = LCD_MAX_HEIGHT - LCD_HEIGHT - LCD_RT_START_HEIGHT;
    }
  }
  else
  {
    LCD_WIDTH = LCD_RT_HEIGHT;
    LCD_HEIGHT = LCD_RT_WIDTH;
    LCD_MAX_HEIGHT = LCD_RT_MAX_WIDTH;
    LCD_MAX_WIDTH = LCD_RT_MAX_HEIGHT;
    ImgArea = ImgAreaW;
    if(LCD_ROTATION == 1)
    {
      LCD_START_WIDTH = LCD_RT_START_HEIGHT;
      LCD_START_HEIGHT = LCD_MAX_HEIGHT - LCD_RT_START_WIDTH - LCD_HEIGHT;
    }
    else
    {
      LCD_START_WIDTH = LCD_MAX_WIDTH - LCD_WIDTH - LCD_RT_START_HEIGHT;
      LCD_START_HEIGHT = LCD_RT_START_WIDTH;
    }
  }
}

void LCD::Print(uint16_t X,uint16_t Y, String Str,uint8_t Font,uint16_t Color)
{
  uint32_t Str_Value;
  uint8_t*Str_Point = (uint8_t*)&Str[0];
  uint8_t*Check_Point = (uint8_t*)&Str[0];
  uint16_t StrLen = 0;
  bool NewWord = true;
  LCD_STR_X = X;LCD_STR_Y = Y;
  while(*Str_Point)
  {
    Check((uint8_t *)Str_Point,&Str_Value);
    if(Str_Value == '\a')
    {
      Str_Point++;
      int Value = 4096;
      uint16_t COLOR_SET = 0;
      for (int i = 0; i < 4; i++)
      {
        Str_Point = Str_Point + Check((uint8_t *)Str_Point,&Str_Value);
        Str_Value = Str_Value&0xFF;
        if(Str_Value >= 'a' && Str_Value <= 'f')
        Str_Value = Str_Value - 87;
        else if(Str_Value >= 'A' && Str_Value <= 'F')
        Str_Value = Str_Value - 55;
        else if(Str_Value >= '0' && Str_Value <= '9')
        Str_Value = Str_Value - 48;
        else Str_Value = 0;
        COLOR_SET = COLOR_SET + Str_Value*Value;
        Value = Value/16;
      }
      if(COLOR_SET >=0 && COLOR_SET <= 0xFFFF)
      Color = COLOR_SET;
      Check_Point = Str_Point;
      continue;
    }
    else if(Str_Value == '\b')
    {
      Str_Point++;
      int Value = 100;
      uint16_t W = 0;
      for (int i = 0; i < 3; i++)
      {
        Str_Point = Str_Point + Check((uint8_t *)Str_Point,&Str_Value);
        Str_Value = Str_Value&0xFF;
        if(Str_Value >= '0' && Str_Value <= '9')
        Str_Value = Str_Value - 48;
        else Str_Value = 0;
        W = W + Str_Value*Value;
        Value = Value/10;
      }
      if(W >=0 && W <= LCD_WIDTH)
      LCD_STR_X = W;
      Check_Point = Str_Point;
      continue;
    }
    else
    {
      if(LCD_AUTO_NEWLINE && NewWord && *Check_Point)
      {
        NewWord = false;
        StrLen = 0;
        do
        {
          Check_Point = Check_Point + Check((uint8_t *)Check_Point,&Str_Value);
          StrLen = StrLen + GetWidthChar(Str_Value, Font) + LCD_CHAR_R_X;
        }
        while (Str_Value != 0x20 && *Check_Point);
        if(LCD_STR_X + StrLen >= LCD_WIDTH && StrLen < LCD_WIDTH)
        {
          LCD_STR_X = LCD_SPACE_LEFT;
          LCD_STR_Y = LCD_STR_Y + LCD_CHAR_TAB_H + LCD_CHAR_R_Y;
        }
        Str_Point = Str_Point + Check((uint8_t *)Str_Point,&Str_Value);
        StrLen = GetWidthChar(Str_Value, Font);
      }
      else
      {
        Str_Point = Str_Point + Check((uint8_t *)Str_Point,&Str_Value);
        StrLen = GetWidthChar(Str_Value, Font);
      }
      if(LCD_STR_X + StrLen >= LCD_WIDTH && StrLen < LCD_WIDTH)
      {
        LCD_STR_X = LCD_SPACE_LEFT;
        LCD_STR_Y = LCD_STR_Y + LCD_CHAR_TAB_H + LCD_CHAR_R_Y;
      }
      DrawChar(LCD_STR_X,LCD_STR_Y,Str_Value,Font,Color);
      LCD_STR_X = LCD_STR_X + StrLen + (Str_Value=='\n'?0:LCD_CHAR_R_X);
      if(Str_Value == ' ' && LCD_AUTO_NEWLINE){NewWord = true;}
    }
  }
}


void LCD::Print(int16_t X,int16_t Y, String Str,uint8_t Font,uint16_t Color,uint16_t MaxWidth,uint16_t MaxHeight,bool Stt ,int16_t Limit)
{
  uint32_t Str_Value;
  uint8_t*Str_Point = (uint8_t*)&Str[0];
  uint8_t*Check_Point = (uint8_t*)&Str[0];
  uint16_t StrLen = 0;
  bool NewWord = true;
  uint16_t StartX = X;
  uint16_t StopX = X + MaxWidth;
  if(Font == CHAR_16x8) LCD_CHAR_TAB_H = 16;
  else if(Font == CHAR_16x16) LCD_CHAR_TAB_H = 16;
  else if(Font == CHAR_32x32) LCD_CHAR_TAB_H = 32;

  while(*Str_Point)
  {
    Check((uint8_t *)Str_Point,&Str_Value);
    if(Str_Value == '\a')
    {
      Str_Point++;
      int Value = 4096;
      uint16_t COLOR_SET = 0;
      for (int i = 0; i < 4; i++)
      {
        Str_Point = Str_Point + Check((uint8_t *)Str_Point,&Str_Value);
        Str_Value = Str_Value&0xFF;
        if(Str_Value >= 'a' && Str_Value <= 'f')
        Str_Value = Str_Value - 87;
        else if(Str_Value >= 'A' && Str_Value <= 'F')
        Str_Value = Str_Value - 55;
        else if(Str_Value >= '0' && Str_Value <= '9')
        Str_Value = Str_Value - 48;
        else Str_Value = 0;
        COLOR_SET = COLOR_SET + Str_Value*Value;
        Value = Value/16;
      }
      Color = COLOR_SET;
      Check_Point = Str_Point;
      continue;
    }
    else if(Str_Value == '\b')
    {
      Str_Point++;
      int Value = 100;
      uint16_t W = 0;
      for (int i = 0; i < 3; i++)
      {
        Str_Point = Str_Point + Check((uint8_t *)Str_Point,&Str_Value);
        Str_Value = Str_Value&0xFF;
        if(Str_Value >= '0' && Str_Value <= '9')
        Str_Value = Str_Value - 48;
        else Str_Value = 0;
        W = W + Str_Value*Value;
        Value = Value/10;
      }
      if(W <= MaxWidth)
      X = W;
      // if(X < StartX || X > StopX)continue;
      Check_Point = Str_Point;
      return;
    }
    else
    {
      if(LCD_AUTO_NEWLINE && NewWord && *Check_Point)
      {
        NewWord = false;
        StrLen = 0;
        do
        {
          Check_Point = Check_Point + Check((uint8_t *)Check_Point,&Str_Value);
          StrLen = StrLen + GetWidthChar(Str_Value, Font) + LCD_CHAR_R_X;
        }
        while (Str_Value != 0x20 && *Check_Point);
        if(X + StrLen >= MaxWidth && StrLen < MaxWidth)
        {
          if(Stt)return;
          X = LCD_SPACE_LEFT;
          Y = Y + LCD_CHAR_TAB_H + LCD_CHAR_R_Y;
        }
        // if(X < StartX)continue;
        Str_Point = Str_Point + Check((uint8_t *)Str_Point,&Str_Value);
        StrLen = GetWidthChar(Str_Value, Font);
      }
      else
      {
        Str_Point = Str_Point + Check((uint8_t *)Str_Point,&Str_Value);
        StrLen = GetWidthChar(Str_Value, Font);
      }
      if(X + StrLen >= MaxWidth && StrLen < MaxWidth)
      {
        if(Stt)return;
        X = LCD_SPACE_LEFT;
        Y = Y + LCD_CHAR_TAB_H + LCD_CHAR_R_Y;
      }
      // if(X < StartX)continue;
      if(X >= Limit)
      DrawChar(X,Y,Str_Value,Font,Color);
      X = X + StrLen + (Str_Value=='\n'?0:LCD_CHAR_R_X);
      if(Str_Value == ' ' && LCD_AUTO_NEWLINE){NewWord = true;}
    }
  }
}

uint8_t LCD::Check(uint8_t*Str,uint32_t*Str_Code)
{
  uint8_t Value = *Str&0xF0;
  uint32_t Str_Val = 0;
  if(Value < 128)
  {
    *Str_Code = *Str;
    return 1;
  }
  else if (Value == 0xC0)
  {
    Str_Val = (uint32_t)*Str<<8;
    Str_Val = Str_Val|(uint32_t)*(Str+1);
    *Str_Code = Str_Val;
    return 2;
  }
  else if (Value == 0xE0)
  {
    Str_Val =(uint32_t)*Str<<16;
    Str_Val = Str_Val|(uint32_t)*(Str+1)<<8;
    Str_Val = Str_Val|(uint32_t)*(Str+2);
    *Str_Code = Str_Val;
    return 3;
  }
  else if (Value == 0xF0)
  {
    Str_Val = (uint32_t)*(Str)<<24;
    Str_Val = Str_Val|(uint32_t)*(Str+1)<<16;
    Str_Val = Str_Val|(uint32_t)*(Str+2)<<8;
    Str_Val = Str_Val|(uint32_t)*(Str+3);
    *Str_Code = Str_Val;
    return 4;
  }
  return 0;
}

void LCD::SetString(uint16_t Color)
{
  if(Color>=0)
  {
    TEXT_COLOR = Color;
  }
}

void LCD::SetStrings(uint8_t Font)
{
  LCD_FONT = Font;
}

void LCD::SetString(uint16_t Color,uint16_t BgrColor)
{
  TEXT_COLOR = Color;
  BGR_COLOR = BgrColor;
}
void LCD::SetStrings(uint16_t X,uint16_t Y,uint16_t Color, uint8_t Font)
{
  if(X>=0) LCD_STR_X = X+LCD_SPACE_LEFT;
  if(Y>=0) LCD_STR_Y = Y;
  TEXT_COLOR = Color;
  LCD_FONT = Font;
}
void LCD::SetString(uint16_t X,uint16_t Y,uint16_t Color)
{
  if(X>=0) LCD_STR_X = X+LCD_SPACE_LEFT;
  if(Y>=0) LCD_STR_Y = Y;
  TEXT_COLOR = Color;
}
void LCD::SetString(uint16_t X,uint16_t Y,uint16_t Color,uint16_t BgrColor)
{
  if(X>=0) LCD_STR_X = X+LCD_SPACE_LEFT;
  if(Y>=0) LCD_STR_Y = Y;
  TEXT_COLOR = Color;
  BGR_COLOR = BgrColor;
}

uint8_t LCD::GetWidthChar(uint32_t Char, uint8_t Font)
{
  uint8_t Len = 0;
  if(Char == ' ') return LCD_CHAR_SPACE;
  if(Char == '\n'||Char == '\t'||Char == '\v') return 0;
  if(Font == CHAR_16x8)
  {
    for (int i = 0; i <= NUM_CHAR_16X8; i++)
    {
      uint32_t Code_Text = Font16x8[i][0]<<24|Font16x8[i][1]<<16|Font16x8[i][2]<<8|Font16x8[i][3];
      if(Char == Code_Text)
      {
        for (int j = 4; j < 20; j++)
        {
          uint32_t Value = Font16x8[i][j];
          for (int k = 0; k < 8; k++)
          {
            if((128&Value)>>7 != 0)
            {
              if(k+1>Len)Len = k+1;
            }
            Value = Value << 1;
          }
        }
        return Len;
      }
    }
    return LCD_CHAR_SPACE;
  }
  else if(Font == CHAR_16x16)
  {
    return 0;
  }
  else if(Font == CHAR_32x32)
  {
    for (int i = 0; i <= NUM_CHAR_32X32; i++)
    {
      if(Font32x32[i][0] == Char)
      {
        for (int j = 1; j < 33; j++)
        {
          uint32_t Value = Font32x32[i][j];
          for (int k = 0; k < 32; k++)
          {
            if((2147483648&Value)>>31 != 0)
            {
              if(k+1>Len)Len = k+1;
            }
            Value = Value << 1;
          }
        }
        return Len;
      }
    }
  }
  return 0;
}

void LCD::DrawChar(uint16_t X,uint16_t Y,uint32_t Char, uint8_t Font,uint16_t Color)
{
  if(Font == CHAR_16x8) LCD_CHAR_TAB_H = 16;
  else if(Font == CHAR_16x16) LCD_CHAR_TAB_H = 16;
  else if(Font == CHAR_32x32) LCD_CHAR_TAB_H = 32;
  if(Char == '\0')
  {
    return;
  }
  if(Char == '\n' || Char == '\r')
  {
    LCD_STR_X = LCD_SPACE_LEFT;
    LCD_STR_Y = LCD_STR_Y + LCD_CHAR_TAB_H + LCD_CHAR_R_Y;
    return;
  }
  else if (Char == '\t')
  {
    LCD_STR_X = LCD_STR_X + LCD_CHAR_TAB_W;
    return;      
  }
  else if (Char == '\v')
  {
    LCD_STR_Y = LCD_STR_Y + LCD_CHAR_TAB_H;
    return;
  }
  if(Font == CHAR_16x8)
  {
    for (int i = 0; i <= NUM_CHAR_16X8; i++)
    {
      uint32_t Code_Text = Font16x8[i][0]<<24|Font16x8[i][1]<<16|Font16x8[i][2]<<8|Font16x8[i][3];
      if(Char == Code_Text)
      {
        for (int j = 4; j < 20; j++)
        {
          uint32_t Value = Font16x8[i][j];
          for (int k = 0; k < 8; k++)
          {
            if((128&Value)>>7 != 0)
            {
              if(LCD_ROTATION == 0 || LCD_ROTATION == 2)
              DrawPixels(X+k,Y+j-4,Color);
              else
              DrawPixels(Y+j-4,X+k,Color);
            }
            else if(LCD_USE_BGR_COLOR)
            {
              if(LCD_ROTATION == 0 || LCD_ROTATION == 2)
              DrawPixels(X+k,Y+j-4,Color);
              else
              DrawPixels(Y+j-4,X+k,Color);
            }
            Value = Value << 1;
          }
        }
        return;
      }
    }
    DrawChar(X,Y,'?',CHAR_16x8,Color);
  }
  else if(Font == CHAR_16x16)
  {
    for (int j = 2; j < 17; j++)
    {
      uint32_t Value = Font16x16[Char][2];
      for (int k = 0; k < 32; k++)
      {
        if((2147483648&Value)>>31 != 0)
        {
          if(LCD_ROTATION == 0 || LCD_ROTATION == 2)
          DrawPixels(X+k,Y+j-4,Color);
          else
          DrawPixels(Y+j-4,X+k,Color);
        }
        else if(LCD_USE_BGR_COLOR)
        {
          if(LCD_ROTATION == 0 || LCD_ROTATION == 2)
          DrawPixels(X+k,Y+j-4,Color);
          else
          DrawPixels(Y+j-4,X+k,Color);
        }
        Value = Value << 1;
      }
    }
  }
  else if(Font == CHAR_32x32)
  {
    for (int i = 0; i <= NUM_CHAR_32X32; i++)
    {
      if(Font32x32[i][0] == Char)
      {
        for (int j = 1; j < 33; j++)
        {
          uint32_t Value = Font32x32[i][j];
          for (int k = 0; k < 32; k++)
          {
            if((2147483648&Value)>>31 != 0)
            {
              if(LCD_ROTATION == 0 || LCD_ROTATION == 2)
              DrawPixels(X+k,Y+j-4,Color);
              else
              DrawPixels(Y+j-4,X+k,Color);
            }
            else if(LCD_USE_BGR_COLOR)
            {
              if(LCD_ROTATION == 0 || LCD_ROTATION == 2)
              DrawPixels(X+k,Y+j-4,Color);
              else
              DrawPixels(Y+j-4,X+k,Color);
            }
            Value = Value << 1;
          }
        }
        break;
      }
    }
  }
}

void LCD::Print(String Str)
{
  Print(LCD_STR_X,LCD_STR_Y,Str,LCD_FONT,TEXT_COLOR);
}
void LCD::Print(const char*Str)
{
  Print(LCD_STR_X,LCD_STR_Y,Str,LCD_FONT,TEXT_COLOR);
}
void LCD::Println(const char*Str)
{
  Print(LCD_STR_X,LCD_STR_Y,Str,LCD_FONT,TEXT_COLOR);
  LCD_STR_X = LCD_SPACE_LEFT;
  LCD_STR_Y = LCD_STR_Y + LCD_CHAR_TAB_H + LCD_CHAR_R_Y;
}
void LCD::Println(String Str)
{
  Print(LCD_STR_X,LCD_STR_Y,Str,LCD_FONT,TEXT_COLOR);
  LCD_STR_X = LCD_SPACE_LEFT;
  LCD_STR_Y = LCD_STR_Y + LCD_CHAR_TAB_H + LCD_CHAR_R_Y;
}
void LCD::Print(int32_t NumBer)
{
  Print(LCD_STR_X,LCD_STR_Y,String(NumBer),LCD_FONT,TEXT_COLOR);
}
void LCD::Println(int32_t NumBer)
{
  Print(NumBer);
  LCD_STR_X = LCD_SPACE_LEFT;
  LCD_STR_Y = LCD_STR_Y + LCD_CHAR_TAB_H + LCD_CHAR_R_Y;
}
void LCD::Print(int32_t NumBer,uint8_t Type,uint8_t Len)
{
  char Buff[MAXLEN];
  char Str[MAXLEN];
  int i = 0;
  ltoa(NumBer,Buff,Type);
  sprintf(Str,String("%0"+String(Len)+"s").c_str(),Buff);
  while(Str[i] != '\0')
  {
    Str[i] = toupper(Str[i]);
    i++;
  }
  String Value;
  if(Type == 16) Value = "0x";
  else if (Type == 2) Value = "0b";
  Print(LCD_STR_X,LCD_STR_Y,Value+String(Str),LCD_FONT,TEXT_COLOR);
}
String LCD::Convert(int32_t Number,uint8_t Type,uint8_t Len)
{
  char Buff[MAXLEN];
  char Str[MAXLEN];
  int i = 0;
  ltoa(Number,Buff,Type);
  sprintf(Str,String("%0"+String(Len)+"S").c_str(),Buff);
  String Value;
  if(Type == 16) Value = "0x";
  else if (Type == 2) Value = "0b";
  Value = Value + String(Buff);
  Value.toUpperCase();
  return Value;
}
void LCD::Println(int32_t NumBer,uint8_t Type,uint8_t Len)
{
  Print(NumBer,Type,Len);
  LCD_STR_X = LCD_SPACE_LEFT;
  LCD_STR_Y = LCD_STR_Y + LCD_CHAR_TAB_H + LCD_CHAR_R_Y;
}
void LCD::Print(String Str,uint16_t Color)
{
  Print(LCD_STR_X,LCD_STR_Y,Str,LCD_FONT,Color);
}
void LCD::Println(String Str,uint16_t Color)
{
  Print(LCD_STR_X,LCD_STR_Y,Str,LCD_FONT,Color);
  LCD_STR_X = LCD_SPACE_LEFT;
  LCD_STR_Y = LCD_STR_Y + LCD_CHAR_TAB_H + LCD_CHAR_R_Y;
}
void LCD::Print(double NumBer)
{
  Print(LCD_STR_X,LCD_STR_Y,String(NumBer),LCD_FONT,TEXT_COLOR);
}
void LCD::Println(double NumBer)
{
  Print(LCD_STR_X,LCD_STR_Y,String(NumBer),LCD_FONT,TEXT_COLOR);
  LCD_STR_X = LCD_SPACE_LEFT;
  LCD_STR_Y = LCD_STR_Y + LCD_CHAR_TAB_H + LCD_CHAR_R_Y;
}
void LCD::Printf(const char*Char,...)
{
  va_list ArrayLish;
  char Str[MAXLEN];
  va_start(ArrayLish,Char);
  vsprintf(Str,Char,ArrayLish);
  Print(LCD_STR_X,LCD_STR_Y,String(Str),LCD_FONT,TEXT_COLOR);
}
void LCD::PrintRight(const char*Char,...)
{
  va_list ArrayLish;
  char S[MAXLEN];
  va_start(ArrayLish,Char);
  vsprintf(S,Char,ArrayLish);
  String Str = String(S);
  uint16_t StrLen = 0;
  uint32_t Str_Value;
  uint8_t*Str_Point = (uint8_t*)&Str[0];
  while (*Str_Point)
  {
    Str_Point = Str_Point + Check((uint8_t *)Str_Point,&Str_Value);
    StrLen = StrLen + GetWidthChar(Str_Value, LCD_FONT) + LCD_CHAR_R_X;
  }
  Print(LCD_WIDTH - LCD_STR_X - StrLen,LCD_STR_Y,Str,LCD_FONT,TEXT_COLOR);
}

void LCD::CreatePoint(uint16_t X,uint16_t Y,uint8_t Size,uint8_t BgrSize,uint16_t Color,uint16_t BgrColor)
{
  LCD_POINT = true;
  LCD_POINT_X = X;
  LCD_POINT_Y = Y;
  LCD_POINT_SIZE = Size;
  LCD_POINT_COLOR = Color;
  LCD_POINT_BGR_SIZE = BgrSize;
  LCD_POINT_BGR_COLOR = BgrColor;
  DrawPoint(0,0,DRAW_POINT);
}
void LCD::SetPoint(uint8_t Type)
{
  LCD_POINT_TYPE = Type;
}
void LCD::SetPoint(int8_t Type,uint16_t Color,uint16_t BgrColor)
{
  if(Type>=0)
  LCD_POINT_TYPE = Type;
  LCD_POINT_COLOR = Color;
  LCD_POINT_BGR_COLOR = BgrColor;
}
void LCD::DrawPoint()
{
  if(LCD_POINT_TYPE == 0)
  {
    PushIcons(LCD_POINT_X,LCD_POINT_Y,15,LCD_POINT_COLOR);
    PushIcons(LCD_POINT_X,LCD_POINT_Y,14,LCD_POINT_BGR_COLOR);
  }
  else if(LCD_POINT_TYPE == 1)
  {
    PushIcons(LCD_POINT_X,LCD_POINT_Y,5,LCD_POINT_COLOR);
    PushIcons(LCD_POINT_X,LCD_POINT_Y,4,LCD_POINT_BGR_COLOR);
  }
}

void LCD::PushIcons(uint16_t X,uint16_t Y,uint32_t Icon,uint16_t Color)
{
  for (int i = 0; i <= NUM_ICON; i++)
  {
    uint32_t Code_Text = Font16x16[i][0]<<16|Font16x16[i][1];
    if(Icon == Code_Text)
    {
      for (int j = 2; j < 18; j++)
      {
        uint16_t Value = Font16x16[i][j];
        for (int k = 0; k < 16; k++)
        {
          if((32768&Value)>>15 != 0)
          {
            if(LCD_ROTATION == 0 || LCD_ROTATION == 2)
            DrawPixels(X+k,Y+j-2,Color);
            else
            DrawPixels(Y+j-2,X+k,Color);
          }
          else if(LCD_USE_BGR_COLOR)
          {
            if(LCD_ROTATION == 0 || LCD_ROTATION == 2)
            DrawPixels(X+k,Y+j-2,BGR_COLOR);
            else
            DrawPixels(Y+j-2,X+k,BGR_COLOR);
          }
          Value = Value << 1;
        }
      }
      return;
    }
  }
}


void LCD::DrawPoint(int16_t Dx, int16_t Dy,uint8_t Mode)
{
  if(LCD_POINT == false) return;
  if(LCD_ROTATION == 2)
  {
    Dx = -Dx;
    Dy = -Dy;
  }
  else if (LCD_ROTATION == 3)
  {
    int16_t Value = Dx;
    Dx = -Dy;
    Dy = Value;
  }
  else if (LCD_ROTATION == 1)
  {
    int16_t Value = Dx;
    Dx = Dy;
    Dy = -Value;
  }
  if(LCD_POINT_X + Dx <= LCD_WIDTH - LCD_POINT_SIZE - 2 && LCD_POINT_X  + Dx >= LCD_POINT_SIZE)
  {
    LCD_POINT_X = LCD_POINT_X + Dx;
  }
  else if(LCD_POINT_X + Dx > LCD_WIDTH - LCD_POINT_SIZE - 2)
  {
    LCD_POINT_X = LCD_WIDTH - LCD_POINT_SIZE - 2;
  }
  else if(LCD_POINT_X + Dx < LCD_POINT_SIZE)
  {
    LCD_POINT_X = LCD_POINT_SIZE;
  }
  if(LCD_POINT_Y + Dy <= LCD_HEIGHT - LCD_POINT_SIZE  - 2 && LCD_POINT_Y + Dy >= LCD_POINT_SIZE)
  {
    LCD_POINT_Y = LCD_POINT_Y + Dy;
  }
  else if(LCD_POINT_Y + Dy > LCD_HEIGHT - LCD_POINT_SIZE - 2)
  {
    LCD_POINT_Y = LCD_HEIGHT - LCD_POINT_SIZE - 2;
  }
  else if(LCD_POINT_Y + Dy < LCD_POINT_SIZE)
  {
    LCD_POINT_Y = LCD_POINT_SIZE;
  }
  if(Mode&DRAW_POINT)
  DrawPoint();
}
bool LCD::CheckHover(uint16_t X,uint16_t Y,uint16_t Width,uint16_t Height)
{
  if(LCD_POINT_X >= LCD_RT_START_WIDTH + X && LCD_POINT_X <= LCD_RT_START_WIDTH  + Width + X && LCD_POINT_Y >= LCD_RT_START_HEIGHT + Y && LCD_POINT_Y <= LCD_RT_START_HEIGHT + Height + Y)
  return true;
  return false;
}
bool LCD::CheckClick(uint16_t X,uint16_t Y,uint16_t Width,uint16_t Height)
{
  if(!CheckHover(X,Y,Width,Height))return false;
  if(LCD_MODE_CLICK == GETCLICK())
  return true;
  return false;
}
void LCD::ValueClick(uint8_t (*GetValueClick)(void))
{
  GETCLICK = GetValueClick;
}
bool LCD::CheckHover()
{
  return CheckHover(0,0,LCD_RT_WIDTH,LCD_RT_HEIGHT);
}
bool LCD::CheckClick()
{
  return CheckClick(0,0,LCD_RT_WIDTH,LCD_RT_HEIGHT);
}
bool LCD::CheckHovers(uint16_t X,uint16_t Y,uint16_t Width,uint16_t Height)
{
  if(LCD_ROTATION == 0 || LCD_ROTATION ==2)
  return CheckHover(X,Y,Width,Height);
  else
  return CheckHover(Y,X,Height,Width);
}
bool LCD::CheckClicks(uint16_t X,uint16_t Y,uint16_t Width,uint16_t Height)
{
  if(LCD_ROTATION == 0 || LCD_ROTATION ==2)
  return CheckClick(X,Y,Width,Height);
  else
  return CheckClick(Y,X,Height,Width);
}
bool LCD::CheckHovers()
{
  return CheckHovers(0,0,LCD_RT_WIDTH,LCD_RT_HEIGHT);
}
bool LCD::CheckClicks()
{
  return CheckClicks(0,0,LCD_RT_WIDTH,LCD_RT_HEIGHT);
}
bool LCD::CheckHover(POINT*OnHover)
{
  if(OnHover)
  {
    return CheckHover(OnHover->X,OnHover->Y,OnHover->Width,OnHover->Height);
  }
  return false;
}
bool LCD::CheckHovers(POINT*OnHover)
{
  if(OnHover)
  {
    return CheckHovers(OnHover->X,OnHover->Y,OnHover->Width,OnHover->Height);
  }
  return false;
}
bool LCD::CheckClick(POINT*OnClick)
{
  if(OnClick)
  {
    return CheckClick(OnClick->X,OnClick->Y,OnClick->Width,OnClick->Height);
  }
  return false;
}
bool LCD::CheckClicks(POINT*OnClick)
{
  if(OnClick)
  {
    return CheckClicks(OnClick->X,OnClick->Y,OnClick->Width,OnClick->Height);
  }
  return false;
}
LCD::POINT*LCD::Point(uint16_t X,uint16_t Y,uint16_t Width,uint16_t Height,uint16_t Mode)
{
  POINT*Point = new POINT[LCD_NUM_FUN_POINT+1];
  memcpy(Point,FUNPOINT,LCD_NUM_FUN_POINT*sizeof(POINT));
  delete[] FUNPOINT;
  FUNPOINT = Point;
  FUNPOINT[LCD_NUM_FUN_POINT].X = X;
  FUNPOINT[LCD_NUM_FUN_POINT].Y = Y;
  FUNPOINT[LCD_NUM_FUN_POINT].Width = Width;
  FUNPOINT[LCD_NUM_FUN_POINT].Height = Height;
  FUNPOINT[LCD_NUM_FUN_POINT].Mode = Mode;
  return &FUNPOINT[LCD_NUM_FUN_POINT++];
}
void LCD::Point(void (*OnHover)(void),void (*UnHover)(void),void (*OnClick)(void),void (*UnClick)(void),POINT*Point)
{
  if(Point)
  {
    Point->FunOnHover = OnHover;
    Point->FunUnHover = UnHover;
    Point->FunOnClick = OnClick;
    Point->FunUnClick = UnClick;
  }
}

void LCD::RunAllFun(uint16_t Mode)
{
  if(Mode&(RUN_FUN_ON_HOVER|RUN_FUN_ON_CLICK|RUN_FUN_ON_HOVERS|RUN_FUN_ON_CLICKS|RUN_FUN_UN_HOVER|RUN_FUN_UN_CLICK|RUN_FUN_UN_HOVERS|RUN_FUN_UN_CLICKS))
  {
    for (int i = 0; i < LCD_NUM_FUN_POINT; i++)
    {
      if(((FUNPOINT[i].Mode&RUN_FUN_ON_HOVER) && CheckHover(&FUNPOINT[i])) || ((FUNPOINT[i].Mode&RUN_FUN_ON_HOVERS) && CheckHovers(&FUNPOINT[i])))
      {
        if(FUNPOINT[i].FunOnHover)FUNPOINT[i].FunOnHover();
      }
      if(((FUNPOINT[i].Mode&RUN_FUN_UN_HOVER) && !CheckHover(&FUNPOINT[i])) || ((FUNPOINT[i].Mode&RUN_FUN_UN_HOVERS) && !CheckHovers(&FUNPOINT[i])))
      {
        if(FUNPOINT[i].FunUnHover)FUNPOINT[i].FunUnHover();
      }
      if(((FUNPOINT[i].Mode&RUN_FUN_ON_CLICK) && CheckClick(&FUNPOINT[i])) || ((FUNPOINT[i].Mode&RUN_FUN_ON_CLICKS) && CheckClicks(&FUNPOINT[i])))
      {
        if(FUNPOINT[i].FunOnClick)FUNPOINT[i].FunOnClick();
      }
      if(((FUNPOINT[i].Mode&RUN_FUN_UN_CLICK) && !CheckClick(&FUNPOINT[i]) && CheckClick()) || ((FUNPOINT[i].Mode&RUN_FUN_UN_CLICKS) && !CheckClicks(&FUNPOINT[i]) && CheckClicks()))
      {
        if(FUNPOINT[i].FunUnClick)FUNPOINT[i].FunUnClick();
      }
    }
  }
  if(Mode&(RUN_LIMIT_TIME|RUN_EVERY_TIME))
  {
    for (int i = 0; i < LCD_NUM_FUN_SETTIME; i++)
    {
      uint32_t Time = millis();
      if((Time - FUN[i].TimeLast >= FUN[i].TimeSet) && (((FUN[i].Mode & RUN_LIMIT_TIME) && (FUN[i].NumOfRun < FUN[i].LimitRun)) || (FUN[i].Mode & RUN_EVERY_TIME)))
      {
        if(FUN[i].Function)
        {
          FUN[i].Function();
          FUN[i].TimeLast = Time;
          FUN[i].NumOfRun++;
        }
      }
    }
  }
  if(Mode&(RUN_FUN_ON_HOVER|RUN_FUN_ON_CLICK))
  {
    for (int i = 0; i < LCD_NUM_ARR_ICON; i++)
    {
      if(ARRICON[i].Mode & DRAW_ICON)
      {
        DrawIcons(ARRICON[i].X,ARRICON[i].Y,ARRICON[i].CodeIcon,ARRICON[i].Color);
      }
      if(ARRICON[i].Mode & RUN_FUN_ON_HOVER && CheckHover(ARRICON[i].X,ARRICON[i].Y,16,16))
      {
        if(ARRICON[i].FunOnHover)ARRICON[i].FunOnHover();
      }
      if(ARRICON[i].Mode & RUN_FUN_ON_CLICK && CheckClick(ARRICON[i].X,ARRICON[i].Y,16,16))
      {
        if (ARRICON[i].FunOnClick)ARRICON[i].FunOnClick();
      }
    }
  }
}

LCD::FUNCTION*LCD::SetTimeOut(void (*Fun)(),uint16_t Mode,uint32_t Time,uint16_t Limit)
{
  FUNCTION*Function = new FUNCTION[LCD_NUM_FUN_SETTIME + 1];
  memcpy(Function,FUN,LCD_NUM_FUN_SETTIME*sizeof(FUNCTION));
  delete[] FUN;
  FUN = Function;
  FUN[LCD_NUM_FUN_SETTIME].Function = Fun;
  FUN[LCD_NUM_FUN_SETTIME].Mode = Mode;
  FUN[LCD_NUM_FUN_SETTIME].TimeSet = Time;
  FUN[LCD_NUM_FUN_SETTIME].TimeLast = millis();
  FUN[LCD_NUM_FUN_SETTIME].LimitRun = Limit;
  return &FUN[LCD_NUM_FUN_SETTIME++];
}
void LCD::SetTimeOut(uint16_t Mode,uint32_t Time,FUNCTION*Function)
{
  Function->Mode = Mode;
  Function->TimeSet = Time;
}
LCD::ICON*LCD::CreateIcon(uint16_t X,uint16_t Y,uint32_t CodeIcon,uint16_t Color,uint16_t Mode)
{
  ICON*Value = new ICON [LCD_NUM_ARR_ICON + 1];
  memcpy(Value,ARRICON,LCD_NUM_ARR_ICON*sizeof(ICON));
  delete[] ARRICON;
  ARRICON = Value;
  ARRICON[LCD_NUM_ARR_ICON].X = X;
  ARRICON[LCD_NUM_ARR_ICON].Y = Y;
  ARRICON[LCD_NUM_ARR_ICON].Color = Color;
  ARRICON[LCD_NUM_ARR_ICON].Mode = Mode;
  ARRICON[LCD_NUM_ARR_ICON].CodeIcon = CodeIcon;
  return &ARRICON[LCD_NUM_ARR_ICON++];
}
void LCD::SetIcon(void (*FunOnHover)(void),void (*FunOnClick)(void),ICON*Icon)
{
  if(Icon)
  {
    Icon->FunOnHover = FunOnHover;
    Icon->FunOnClick = FunOnClick;
  }
}

void LCD::DrawIcons(uint16_t X,uint16_t Y,uint32_t Icon,uint16_t Color)
{
  for (int i = 0; i <= NUM_ICON; i++)
  {
    uint32_t Code_Text = Font16x16[i][0]<<16|Font16x16[i][1];
    if(Icon == Code_Text)
    {
      for (int j = 2; j < 18; j++)
      {
        uint16_t Value = Font16x16[i][j];
        for (int k = 0; k < 16; k++)
        {
          if((32768&Value)>>15 != 0)
          {
            if(LCD_ROTATION == 0 || LCD_ROTATION == 2)
            DrawPixels(X+k,Y+j-2,Color);
            else
            DrawPixels(X+j-2,Y+k,Color);
          }
          else if(LCD_USE_BGR_COLOR)
          {
            if(LCD_ROTATION == 0 || LCD_ROTATION == 2)
            DrawPixels(Y+k,X+j-2,BGR_COLOR);
            else
            DrawPixels(X+j-2,Y+k,BGR_COLOR);
          }
          Value = Value << 1;
        }
      }
      return;
    }
  }
}

void LCD::RunFunSprite(uint16_t Mode)
{
  if(Mode&(RUN_FUN_AFTER|RUN_FUN_BEFORE))
  {
    for (int i = 0; i < LCD_NUM_FUN_RUN_PUSH; i++)
    {
      if((PUSHSPRITE[i].Mode & Mode) && PUSHSPRITE[i].FunPoint)
      PUSHSPRITE[i].FunPoint();
    }
  }
}

void LCD::FillRect(uint16_t X,uint16_t Y,uint16_t Width,uint16_t Height,uint16_t Color)
{
  if(Color == TFT_SWAP_COLOR)
    Color = 0xFFFF - Color;
  else if(Color == TFT_UNCHANGED)
    return;
  for(int i = X;i < X + Width;i++)
  {
    for (int k = Y; k < Y + Height; k++)
    {
      DrawPixel(i,k,Color);
    }
  }
}
void LCD::FillRects(uint16_t X,uint16_t Y,uint16_t Width,uint16_t Height,uint16_t Color)
{
  if(Color == TFT_SWAP_COLOR)
    Color = 0xFFFF - Color;
  else if(Color == TFT_UNCHANGED)
    return;
    for(int i = X;i < X + Width;i++)
    {
      for (int k = Y; k < Y + Height; k++)
      {
        DrawPixels(i,k,Color);
      }
    }
  }
  void LCD::DrawRect(uint16_t X,uint16_t Y,uint16_t Width,uint16_t Height,uint16_t Line,uint16_t Color)
  {
    FillRect(X,Y,Width-Line,Line,Color);
    FillRect(X+Line,Y+Height-Line,Width-Line,Line,Color);
    FillRect(X,Y+Line,Line,Height-Line,Color);
    FillRect(X+Width-Line,Y,Line,Height-Line,Color);
  }
  
  void LCD::DrawImg(uint16_t X, uint16_t Y, uint16_t Width, uint16_t Height, uint16_t*Img)
  {
  for(int i = X;i < X + Width;i++)
  {
    for (int k = Y; k < Y + Height; k++)
    {
      DrawPixels(i,k,Img[i+k*Width]);
    }
  }
}
