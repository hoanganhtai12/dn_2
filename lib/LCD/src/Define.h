#define ABS(X) (X>0?X:-X)
#define TFTMin(a,b) ((a<b)?a:b)
#define TFTMax(a,b) ((a>b)?a:b)
#define TFTABS(A) (A>0?A:-A)
#define DC_C_L if(PIN_DC >= 32)GPIO.out1_w1tc.val = (1 << (PIN_DC - 32));else GPIO.out_w1tc = (1 << PIN_DC)
#define DC_D_H if(PIN_DC >= 32)GPIO.out1_w1ts.val = (1 << (PIN_DC - 32));else GPIO.out_w1ts = (1 << PIN_DC)
#define CS_B_L if(PIN_CS >= 32)GPIO.out1_w1tc.val = (1 << (PIN_CS - 32));else GPIO.out_w1tc = (1 << PIN_CS)
#define CS_S_H if(PIN_CS >= 32)GPIO.out1_w1ts.val = (1 << (PIN_CS - 32));else GPIO.out_w1ts = (1 << PIN_CS)
#define DigitalWrite(Pin,Value) Value!=0?GPIO.out_w1ts = (1<<Pin):GPIO.out_w1tc = (1<<Pin)
#define SET_BUS_WRITE_MODE (*(volatile uint32_t*)(((((0)>1) ? (0x60025000) : (0x60024000)) + 0x10)) = ((1UL << (27))))
#define SET_BUS_READ_MODE  (*(volatile uint32_t*)(((((0)>1) ? (0x60025000) : (0x60024000)) + 0x10)) = ((1UL << (27))) | ((1UL << (28))) | ((1UL << (0))))
#define TFT_SPI_MODE SPI_MODE3
#define SPI_MOSI_DLEN_REG(x) SPI_MS_DLEN_REG(x)
#define TFT_WRITE_BITS(D, B) 
#define TFT_Write_16(C) TFT_WRITE_BITS((C)<<8 | (C)>>8, 16)
#define SPI_BUSY_CHECK while (*(volatile uint32_t*)(((((0)>1) ? (0x60025000) : (0x60024000)) + 0x0))&((1UL << (24))))
#define TFT_Write_8(C) *(volatile uint32_t*)(((((0)>1) ? (0x60025000) : (0x60024000)) + 0x1C)) = 8-1; *(volatile uint32_t*)(((((0)>1) ? (0x60025000) : (0x60024000)) + 0x98)) = C; *(volatile uint32_t*)(((((0)>1) ? (0x60025000) : (0x60024000)) + 0x0)) = ((1UL << (23))); while (*(volatile uint32_t*)(((((0)>1) ? (0x60025000) : (0x60024000)) + 0x0)) & ((1UL << (23)))); *(volatile uint32_t*)(((((0)>1) ? (0x60025000) : (0x60024000)) + 0x0)) = ((1UL << (24))); while (*(volatile uint32_t*)(((((0)>1) ? (0x60025000) : (0x60024000)) + 0x0)) & ((1UL << (24))));
#define SPI_USER_REG(i) (REG_SPI_BASE(i) + 0x10)
#define DAT8TO32(P) ( (uint32_t)P[0]<<8 | P[1] | P[2]<<24 | P[3]<<16 )
#define TFT_Write_32C(C,D)  *(volatile uint32_t*)(((((0)>1) ? (0x60025000) : (0x60024000)) + 0x1C)) = 32-1; *(volatile uint32_t*)(((((0)>1) ? (0x60025000) : (0x60024000)) + 0x98)) = (uint16_t)((D)<<8 | (D)>>8)<<16 | (uint16_t)((C)<<8 | (C)>>8); *(volatile uint32_t*)(((((0)>1) ? (0x60025000) : (0x60024000)) + 0x0)) = ((1UL << (23))); while (*(volatile uint32_t*)(((((0)>1) ? (0x60025000) : (0x60024000)) + 0x0)) & ((1UL << (23)))); *(volatile uint32_t*)(((((0)>1) ? (0x60025000) : (0x60024000)) + 0x0)) = ((1UL << (24))); while (*(volatile uint32_t*)(((((0)>1) ? (0x60025000) : (0x60024000)) + 0x0)) & ((1UL << (24))));
#define TFT_Write_32D(C) TFT_WRITE_BITS((uint16_t)((C)<<8 | (C)>>8)<<16 | (uint16_t)((C)<<8 | (C)>>8), 32)
#define TFT_Write_16N(C)  *SPI_MOSI_DLEN = 16-1;       \
                          *SPI_W0 = ((C)<<8 | (C)>>8); \
                          *SPI_CMN = SPI_USR;
#define SPI_W0          (volatile uint32_t*)(SPI_W0_REG(SPRITE_PORT))
#define SPI_CMN         (volatile uint32_t*)(SPI_CMD_REG(SPRITE_PORT))
#define SPI_CMD         (volatile uint32_t*)(SPI_CMD_REG(SPRITE_PORT))
#define SPI_USR_TFT     (volatile uint32_t*)(SPI_USER_REG(SPRITE_PORT))
#define SPI_MOSI_DLEN   (volatile uint32_t*)(SPI_MOSI_DLEN_REG(SPRITE_PORT))



#define LCD_ST7789 1
#define LCD_ST7735 2

#define DEF_LCD_MAX_WIDTH         80
#define DEF_LCD_MAX_HEIGHT        160
#define DEF_LCD_WIDTH             80
#define DEF_LCD_HEIGHT            160

#define CHAR_16x8                        1
#define CHAR_16x16                       2
#define CHAR_32x32                       3

#define MAXLEN                200

#define EMPTY                           (0x00)
#define DRAW_POINT                   (0x01<<0)
#define RUN_FUN_AFTER                (0x01<<1)
#define RUN_FUN_BEFORE               (0x01<<2)
#define RUN_FUN_ON_HOVER             (0x01<<3)
#define RUN_FUN_ON_CLICK             (0x01<<4)
#define RUN_FUN_ON_HOVERS            (0x01<<5)
#define RUN_FUN_ON_CLICKS            (0x01<<6)
#define RUN_FUN_UN_HOVER             (0x01<<7)
#define RUN_FUN_UN_CLICK             (0x01<<8)
#define RUN_FUN_UN_HOVERS            (0x01<<9)
#define RUN_FUN_UN_CLICKS           (0x01<<10)
#define WHEN_POINT_INT              (0x01<<11)
#define WHEN_POINT_OUT              (0x01<<12)
#define RUN_LIMIT_TIME              (0x01<<13)
#define RUN_EVERY_TIME              (0x01<<14)
#define RUN_IN_TIME                 (0x01<<15)

#define MAX_LINE                       (5000)
#define SCREEN_AUTO_PRINT             (0X01<<0)
#define DRAW_ICON                   (0x01<<0)
#define DRAW_MODE_NONE              (0x00<<1)
#define DRAW_MODE_AXIS              (0x01<<1)
#define DRAW_MODE_TIME              (0x01<<2)
#define DRAW_MODE_FAST              (0x01<<3)