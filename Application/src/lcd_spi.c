#include "../inc/lcd_spi.h"
#include "cmsis_os.h"

#define LCD_SPI hspi5

static pFONT *LCD_AsciiFonts;
uint16_t  LCD_Buff[1024];

struct{
    uint32_t Color;  				//	LCD当前画笔颜色
    uint32_t BackColor;			//	背景色
    uint8_t  ShowNum_Mode;		// 数字显示模式
    uint8_t  Direction;			//	显示方向
    uint16_t Width;            // 屏幕像素长度
    uint16_t Height;           // 屏幕像素宽度
    uint8_t  X_Offset;         // X坐标偏移，用于设置屏幕控制器的显存写入方式
    uint8_t  Y_Offset;         // Y坐标偏移，用于设置屏幕控制器的显存写入方式
} LCD;

// 显示字符的行数与列数
uint8_t LCD_GetDispLine(uint8_t linehigh){
    return (uint8_t) (LCD.Height / (linehigh + LCD_AsciiFonts->Height));
}

uint8_t LCD_GetDispCol(){
    return (uint8_t) (LCD.Width / LCD_AsciiFonts->Width);
}


HAL_StatusTypeDef LCD_SPI_Transmit(SPI_HandleTypeDef *hspi, uint16_t pData, uint32_t Size);
HAL_StatusTypeDef LCD_SPI_TransmitBuffer (SPI_HandleTypeDef *hspi, uint16_t *pData, uint32_t Size);


void  LCD_WriteCommand(uint8_t lcd_command){
    LCD_DC_Command;
    HAL_SPI_Transmit(&LCD_SPI, &lcd_command, 1, 0xff);
}

void  LCD_WriteData_8bit(uint8_t lcd_data){
    LCD_DC_Data;
    HAL_SPI_Transmit(&LCD_SPI, &lcd_data, 1, 0xff);
}

void  LCD_WriteData_16bit(uint16_t lcd_data){
    uint8_t lcd_data_buff[2];
    LCD_DC_Data;
    lcd_data_buff[0] = lcd_data>>8;
    lcd_data_buff[1] = lcd_data;
    HAL_SPI_Transmit(&LCD_SPI, lcd_data_buff, 2, 0xff);
}

void  LCD_WriteBuff(uint16_t *DataBuff, uint16_t DataSize){
    LCD_DC_Data;
    LCD_SPI.Init.DataSize = SPI_DATASIZE_16BIT;
    HAL_SPI_Init(&LCD_SPI);
    HAL_SPI_Transmit(&LCD_SPI, (uint8_t *)DataBuff, DataSize, 1000);
    LCD_SPI.Init.DataSize 	= SPI_DATASIZE_8BIT;
    HAL_SPI_Init(&LCD_SPI);
}

void SPI_LCD_Init(void){
    osDelay(10);
    LCD_WriteCommand(0x36);       // 显存访问控制 指令，用于设置访问显存的方式
    LCD_WriteData_8bit(0x00);     // 配置成 从上到下、从左到右，RGB像素格式

    LCD_WriteCommand(0x3A);			// 接口像素格式 指令，用于设置使用 12位、16位还是18位色
    LCD_WriteData_8bit(0x05);     // 此处配置成 16位 像素格式

// 接下来很多都是电压设置指令，直接使用厂家给设定值
    LCD_WriteCommand(0xB2);
    LCD_WriteData_8bit(0x0C);
    LCD_WriteData_8bit(0x0C);
    LCD_WriteData_8bit(0x00);
    LCD_WriteData_8bit(0x33);
    LCD_WriteData_8bit(0x33);

    LCD_WriteCommand(0xB7);		   // 栅极电压设置指令
    LCD_WriteData_8bit(0x35);     // VGH = 13.26V，VGL = -10.43V

    LCD_WriteCommand(0xBB);			// 公共电压设置指令
    LCD_WriteData_8bit(0x19);     // VCOM = 1.35V

    LCD_WriteCommand(0xC0);
    LCD_WriteData_8bit(0x2C);

    LCD_WriteCommand(0xC2);       // VDV 和 VRH 来源设置
    LCD_WriteData_8bit(0x01);     // VDV 和 VRH 由用户自由配置

    LCD_WriteCommand(0xC3);			// VRH电压 设置指令
    LCD_WriteData_8bit(0x12);     // VRH电压 = 4.6+( vcom+vcom offset+vdv)

    LCD_WriteCommand(0xC4);		   // VDV电压 设置指令
    LCD_WriteData_8bit(0x20);     // VDV电压 = 0v

    LCD_WriteCommand(0xC6); 		// 正常模式的帧率控制指令
    LCD_WriteData_8bit(0x0F);   	// 设置屏幕控制器的刷新帧率为60帧

    LCD_WriteCommand(0xD0);			// 电源控制指令
    LCD_WriteData_8bit(0xA4);     // 无效数据，固定写入0xA4
    LCD_WriteData_8bit(0xA1);     // AVDD = 6.8V ，AVDD = -4.8V ，VDS = 2.3V


    LCD_WriteCommand(0xE0);       // 正极电压伽马值设定
    LCD_WriteData_8bit(0xD0);
    LCD_WriteData_8bit(0x04);
    LCD_WriteData_8bit(0x0D);
    LCD_WriteData_8bit(0x11);
    LCD_WriteData_8bit(0x13);
    LCD_WriteData_8bit(0x2B);
    LCD_WriteData_8bit(0x3F);
    LCD_WriteData_8bit(0x54);
    LCD_WriteData_8bit(0x4C);
    LCD_WriteData_8bit(0x18);
    LCD_WriteData_8bit(0x0D);
    LCD_WriteData_8bit(0x0B);
    LCD_WriteData_8bit(0x1F);
    LCD_WriteData_8bit(0x23);

    LCD_WriteCommand(0xE1);      // 负极电压伽马值设定
    LCD_WriteData_8bit(0xD0);
    LCD_WriteData_8bit(0x04);
    LCD_WriteData_8bit(0x0C);
    LCD_WriteData_8bit(0x11);
    LCD_WriteData_8bit(0x13);
    LCD_WriteData_8bit(0x2C);
    LCD_WriteData_8bit(0x3F);
    LCD_WriteData_8bit(0x44);
    LCD_WriteData_8bit(0x51);
    LCD_WriteData_8bit(0x2F);
    LCD_WriteData_8bit(0x1F);
    LCD_WriteData_8bit(0x1F);
    LCD_WriteData_8bit(0x20);
    LCD_WriteData_8bit(0x23);

    LCD_WriteCommand(0x21);       // 打开反显，因为面板是常黑型，操作需要反过来

    // 退出休眠指令，LCD控制器在刚上电、复位时，会自动进入休眠模式 ，因此操作屏幕之前，需要退出休眠
    LCD_WriteCommand(0x11);       // 退出休眠 指令

    osDelay(120);               // 需要等待120ms，让电源电压和时钟电路稳定下来

    LCD_WriteCommand(0x29);       // 打开显示

    LCD_SetDirection(Direction_H_Flip);  	      //	设置显示方向
    LCD_SetBackColor(LCD_BLACK);           // 设置背景色
    LCD_SetColor(LCD_WHITE);               // 设置画笔色
    LCD_Clear();                           // 清屏

    LCD_SetAsciiFont(&ASCII_Font24);       // 设置默认字体
    LCD_ShowNumMode(Fill_Zero);	      	// 设置变量显示模式，多余位填充空格还是填充0

    LCD_Backlight_ON;  // 引脚输出高电平点亮背光
}

void LCD_SetAddress(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2){
    LCD_WriteCommand(0x2a);			//	列地址设置，即X坐标
    LCD_WriteData_16bit(x1+LCD.X_Offset);
    LCD_WriteData_16bit(x2+LCD.X_Offset);

    LCD_WriteCommand(0x2b);			//	行地址设置，即Y坐标
    LCD_WriteData_16bit(y1+LCD.Y_Offset);
    LCD_WriteData_16bit(y2+LCD.Y_Offset);

    LCD_WriteCommand(0x2c);			//	开始写入显存，即要显示的颜色数据
}

void LCD_SetColor(uint32_t Color){
    uint16_t Red_Value = 0, Green_Value = 0, Blue_Value = 0; //各个颜色通道的值

    Red_Value   = (uint16_t)((Color&0x00F80000)>>8);   // 转换成 16位 的RGB565颜色
    Green_Value = (uint16_t)((Color&0x0000FC00)>>5);
    Blue_Value  = (uint16_t)((Color&0x000000F8)>>3);

    LCD.Color = (uint16_t)(Red_Value | Green_Value | Blue_Value);  // 将颜色写入全局LCD参数
}

void LCD_SetBackColor(uint32_t Color){
    uint16_t Red_Value = 0, Green_Value = 0, Blue_Value = 0; //各个颜色通道的值
    Red_Value   = (uint16_t)((Color&0x00F80000)>>8);   // 转换成 16位 的RGB565颜色
    Green_Value = (uint16_t)((Color&0x0000FC00)>>5);
    Blue_Value  = (uint16_t)((Color&0x000000F8)>>3);

    LCD.BackColor = (uint16_t)(Red_Value | Green_Value | Blue_Value);	// 将颜色写入全局LCD参数
}

void LCD_SetDirection(uint8_t direction){
    LCD.Direction = direction;    // 写入全局LCD参数
    if( direction == Direction_H ){
        LCD_WriteCommand(0x36);    		// 显存访问控制 指令，用于设置访问显存的方式
        LCD_WriteData_8bit(0x70);        // 横屏显示
        LCD.X_Offset   = 0;             // 设置控制器坐标偏移量
        LCD.Y_Offset   = 0;
        LCD.Width      = LCD_Height;		// 重新赋值长、宽
        LCD.Height     = LCD_Width;
    } else if( direction == Direction_V ) {
        LCD_WriteCommand(0x36);    		// 显存访问控制 指令，用于设置访问显存的方式
        LCD_WriteData_8bit(0x00);        // 垂直显示
        LCD.X_Offset   = 0;              // 设置控制器坐标偏移量
        LCD.Y_Offset   = 0;
        LCD.Width      = LCD_Width;		// 重新赋值长、宽
        LCD.Height     = LCD_Height;
    } else if( direction == Direction_H_Flip ) {
        LCD_WriteCommand(0x36);   			 // 显存访问控制 指令，用于设置访问显存的方式
        LCD_WriteData_8bit(0xA0);         // 横屏显示，并上下翻转，RGB像素格式
        LCD.X_Offset   = 0;              // 设置控制器坐标偏移量
        LCD.Y_Offset   = 0;
        LCD.Width      = LCD_Height;		 // 重新赋值长、宽
        LCD.Height     = LCD_Width;
    } else if( direction == Direction_V_Flip ) {
        LCD_WriteCommand(0x36);    		// 显存访问控制 指令，用于设置访问显存的方式
        LCD_WriteData_8bit(0xC0);        // 垂直显示 ，并上下翻转，RGB像素格式
        LCD.X_Offset   = 0;              // 设置控制器坐标偏移量
        LCD.Y_Offset   = 0;
        LCD.Width      = LCD_Width;		// 重新赋值长、宽
        LCD.Height     = LCD_Height;
    }
}

void LCD_SetAsciiFont(pFONT *Asciifonts){
    LCD_AsciiFonts = Asciifonts;
}


void LCD_Clear(void){
    LCD_SetAddress(0,0,LCD.Width-1,LCD.Height-1);	// 设置坐标
    LCD_DC_Data;     // 数据指令选择 引脚输出高电平，代表本次传输 数据
    LCD_SPI.Init.DataSize 	= SPI_DATASIZE_16BIT;   //	16位数据宽度
    HAL_SPI_Init(&LCD_SPI);

    LCD_SPI_Transmit(&LCD_SPI, LCD.BackColor, LCD.Width * LCD.Height) ;   // 启动传输
    // 改回8位数据宽度，因为指令和部分数据都是按照8位传输的
    LCD_SPI.Init.DataSize 	= SPI_DATASIZE_8BIT;    //	8位数据宽度
    HAL_SPI_Init(&LCD_SPI);
}


void LCD_ClearRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height){
    LCD_SetAddress( x, y, x+width-1, y+height-1);	// 设置坐标

    LCD_DC_Data;     // 数据指令选择 引脚输出高电平，代表本次传输 数据

    LCD_SPI.Init.DataSize 	= SPI_DATASIZE_16BIT;   //	16位数据宽度
    HAL_SPI_Init(&LCD_SPI);

    LCD_SPI_Transmit(&LCD_SPI, LCD.BackColor, width*height) ;  // 启动传输

    LCD_SPI.Init.DataSize 	= SPI_DATASIZE_8BIT;    //	8位数据宽度
    HAL_SPI_Init(&LCD_SPI);

}


void LCD_DrawPoint(uint16_t x,uint16_t y,uint32_t color){
    LCD_SetAddress(x,y,x,y);	//	设置坐标
    LCD_WriteData_16bit(color)	;
}


void LCD_DisplayChar(uint16_t x, uint16_t y,uint8_t c){
    uint16_t  index = 0, counter = 0 ,i = 0, w = 0;
    uint8_t   disChar;
    c = c - 32;

    for(index = 0; index < LCD_AsciiFonts->Sizes; index++){
        disChar = LCD_AsciiFonts->pTable[c*LCD_AsciiFonts->Sizes + index];
        for(counter = 0; counter < 8; counter++){
            if(disChar & 0x01) LCD_Buff[i] =  LCD.Color;
            else LCD_Buff[i] = LCD.BackColor;
            disChar >>= 1;
            i++;
            w++;
            if( w == LCD_AsciiFonts->Width ) {
                w = 0;
                break;
            }
        }
    }
    LCD_SetAddress( x, y, x+LCD_AsciiFonts->Width-1, y+LCD_AsciiFonts->Height-1);	   // 设置坐标
    LCD_WriteBuff(LCD_Buff,LCD_AsciiFonts->Width*LCD_AsciiFonts->Height);          // 写入显存
}


void LCD_DisplayString( uint16_t x, uint16_t y, char *p){
    while ((x < LCD.Width) && (*p != 0)){
        LCD_DisplayChar( x,y,*p);
        x += LCD_AsciiFonts->Width; //显示下一个字符
        p++;	//取下一个字符地址
    }
}


void LCD_SetTextFont(pFONT *fonts){
    switch(fonts->Width ){
        case 12:	LCD_AsciiFonts = &ASCII_Font12;	break;
        case 16:	LCD_AsciiFonts = &ASCII_Font16;	break;
        case 20:	LCD_AsciiFonts = &ASCII_Font20;	break;
        case 24:	LCD_AsciiFonts = &ASCII_Font24;	break;
        case 32:	LCD_AsciiFonts = &ASCII_Font32;	break;
        default: break;
    }
}



void LCD_DisplayText(uint16_t x, uint16_t y, char *pText){
    while(*pText != 0){
        if(*pText<=0x7F){
            LCD_DisplayChar(x,y,*pText);
            x+=LCD_AsciiFonts->Width;
            pText++;
        }
    }
}


void LCD_ShowNumMode(uint8_t mode){
    LCD.ShowNum_Mode = mode;
}


void  LCD_DisplayNumber( uint16_t x, uint16_t y, int32_t number, uint8_t len){
    char   Number_Buffer[15];
    if( LCD.ShowNum_Mode == Fill_Zero)
        u_sprintf( Number_Buffer, "%0.*d",len, number );
    else
        u_sprintf( Number_Buffer, "%*d",len, number );
    LCD_DisplayString( x, y,(char *)Number_Buffer);
}


void  LCD_DisplayDecimals( uint16_t x, uint16_t y, double decimals, uint8_t len, uint8_t decs){
    char  Number_Buffer[20];
    if( LCD.ShowNum_Mode == Fill_Zero) u_sprintf( Number_Buffer , "%0*.*lf",len,decs, decimals );	// 将 number 转换成字符串，便于显示
    else u_sprintf( Number_Buffer , "%*.*lf",len,decs, decimals );	// 将 number 转换成字符串，便于显示
    LCD_DisplayString( x, y,(char *)Number_Buffer) ;	// 将转换得到的字符串显示出来
}

#define ABS(X)  ((X) > 0 ? (X) : -(X))

void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2){
    int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0,
            yinc1 = 0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0,
            curpixel = 0;

    deltax = ABS(x2 - x1);        /* The difference between the x's */
    deltay = ABS(y2 - y1);        /* The difference between the y's */
    x = x1;                       /* Start x off at the first pixel */
    y = y1;                       /* Start y off at the first pixel */

    if (x2 >= x1){
        xinc1 = 1;
        xinc2 = 1;
    }else {
        xinc1 = -1;
        xinc2 = -1;
    }

    if (y2 >= y1) {
        yinc1 = 1;
        yinc2 = 1;
    } else {
        yinc1 = -1;
        yinc2 = -1;
    }
    if (deltax >= deltay) {
        xinc1 = 0;                  /* Don't change the x when numerator >= denominator */
        yinc2 = 0;                  /* Don't change the y for every iteration */
        den = deltax;
        num = deltax / 2;
        numadd = deltay;
        numpixels = deltax;         /* There are more x-values than y-values */
    } else {
        xinc2 = 0;                  /* Don't change the x for every iteration */
        yinc1 = 0;                  /* Don't change the y when numerator >= denominator */
        den = deltay;
        num = deltay / 2;
        numadd = deltax;
        numpixels = deltay;         /* There are more y-values than x-values */
    }
    for (curpixel = 0; curpixel <= numpixels; curpixel++){
        LCD_DrawPoint(x,y,LCD.Color);             /* Draw the current pixel */
        num += numadd;              /* Increase the numerator by the top of the fraction */
        if (num >= den) {
            num -= den;               /* Calculate the new numerator value */
            x += xinc1;               /* Change the x as appropriate */
            y += yinc1;               /* Change the y as appropriate */
        }
        x += xinc2;                 /* Change the x as appropriate */
        y += yinc2;                 /* Change the y as appropriate */
    }
}


void LCD_DrawLine_V(uint16_t x, uint16_t y, uint16_t height){
    uint16_t i ; // 计数变量

    for (i = 0; i < height; i++) LCD_Buff[i] =  LCD.Color;
    LCD_SetAddress( x, y, x, y+height-1);	     // 设置坐标
    LCD_WriteBuff(LCD_Buff,height);          // 写入显存
}


void LCD_DrawLine_H(uint16_t x, uint16_t y, uint16_t width){
    uint16_t i ; // 计数变量

    for (i = 0; i < width; i++) LCD_Buff[i] =  LCD.Color;  // 写入缓冲区
    LCD_SetAddress( x, y, x+width-1, y);	     // 设置坐标
    LCD_WriteBuff(LCD_Buff,width);          // 写入显存
}


void LCD_DrawRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height){
    LCD_DrawLine_H( x,  y,  width);
    LCD_DrawLine_H( x,  y+height-1,  width);
    LCD_DrawLine_V( x,  y,  height);
    LCD_DrawLine_V( x+width-1,  y,  height);
}



void LCD_DrawCircle(uint16_t x, uint16_t y, uint16_t r){
    int Xadd = -r, Yadd = 0, err = 2-2*r, e2;
    do {
        LCD_DrawPoint(x-Xadd,y+Yadd,LCD.Color);
        LCD_DrawPoint(x+Xadd,y+Yadd,LCD.Color);
        LCD_DrawPoint(x+Xadd,y-Yadd,LCD.Color);
        LCD_DrawPoint(x-Xadd,y-Yadd,LCD.Color);

        e2 = err;
        if (e2 <= Yadd) {
            err += ++Yadd*2+1;
            if (-Xadd == Yadd && e2 <= Xadd) e2 = 0;
        }
        if (e2 > Xadd) err += ++Xadd*2+1;
    }
    while (Xadd <= 0);
}



void LCD_DrawEllipse(int x, int y, int r1, int r2){
    int Xadd = -r1, Yadd = 0, err = 2-2*r1, e2;
    float K = 0, rad1 = 0, rad2 = 0;
    rad1 = r1;
    rad2 = r2;
    if (r1 > r2){
        do {
            K = (float)(rad1/rad2);

            LCD_DrawPoint(x-Xadd,y+(uint16_t)(Yadd/K),LCD.Color);
            LCD_DrawPoint(x+Xadd,y+(uint16_t)(Yadd/K),LCD.Color);
            LCD_DrawPoint(x+Xadd,y-(uint16_t)(Yadd/K),LCD.Color);
            LCD_DrawPoint(x-Xadd,y-(uint16_t)(Yadd/K),LCD.Color);

            e2 = err;
            if (e2 <= Yadd) {
                err += ++Yadd*2+1;
                if (-Xadd == Yadd && e2 <= Xadd) e2 = 0;
            }
            if (e2 > Xadd) err += ++Xadd*2+1;
        }
        while (Xadd <= 0);
    }else{
        Yadd = -r2;
        Xadd = 0;
        do {
            K = (float)(rad2/rad1);
            LCD_DrawPoint(x-(uint16_t)(Xadd/K),y+Yadd,LCD.Color);
            LCD_DrawPoint(x+(uint16_t)(Xadd/K),y+Yadd,LCD.Color);
            LCD_DrawPoint(x+(uint16_t)(Xadd/K),y-Yadd,LCD.Color);
            LCD_DrawPoint(x-(uint16_t)(Xadd/K),y-Yadd,LCD.Color);

            e2 = err;
            if (e2 <= Xadd) {
                err += ++Xadd*3+1;
                if (-Yadd == Xadd && e2 <= Yadd) e2 = 0;
            }
            if (e2 > Yadd) err += ++Yadd*3+1;
        }
        while (Yadd <= 0);
    }
}


void LCD_FillCircle(uint16_t x, uint16_t y, uint16_t r){
    int32_t  D;    /* Decision Variable */
    uint32_t  CurX;/* Current X Value */
    uint32_t  CurY;/* Current Y Value */

    D = 3 - (r << 1);

    CurX = 0;
    CurY = r;

    while (CurX <= CurY){
        if(CurY > 0){
            LCD_DrawLine_V(x - CurX, y - CurY,2*CurY);
            LCD_DrawLine_V(x + CurX, y - CurY,2*CurY);
        }

        if(CurX > 0){

            LCD_DrawLine_V(x - CurY, y - CurX,2*CurX);
            LCD_DrawLine_V(x + CurY, y - CurX,2*CurX);
        }
        if (D < 0){
            D += (CurX << 2) + 6;
        }
        else{
            D += ((CurX - CurY) << 2) + 10;
            CurY--;
        }
        CurX++;
    }
    LCD_DrawCircle(x, y, r);
}


void LCD_FillRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height){
    LCD_SetAddress( x, y, x+width-1, y+height-1);	// 设置坐标

    LCD_DC_Data;
    LCD_SPI.Init.DataSize 	= SPI_DATASIZE_16BIT;   //	16位数据宽度
    HAL_SPI_Init(&LCD_SPI);

    LCD_SPI_Transmit(&LCD_SPI, LCD.Color, width*height) ;
    LCD_SPI.Init.DataSize 	= SPI_DATASIZE_8BIT;    //	8位数据宽度
    HAL_SPI_Init(&LCD_SPI);
}


void LCD_DrawImage(uint16_t x,uint16_t y,uint16_t width,uint16_t height,const uint8_t *pImage){
    uint8_t   disChar;	         // 字模的值
    uint16_t  Xaddress = x;       // 水平坐标
    uint16_t  Yaddress = y;       // 垂直坐标
    uint16_t  i=0,j=0,m=0;        // 计数变量
    uint16_t  BuffCount = 0;      // 缓冲区计数
    uint16_t  Buff_Height = 0;    // 缓冲区的行数

    Buff_Height = (sizeof(LCD_Buff)/2) / height;    // 计算缓冲区能够写入图片的多少行

    for(i = 0; i <height; i++){
        for(j = 0; j <(float)width/8; j++){
            disChar = *pImage;
            for(m = 0; m < 8; m++){
                if(disChar & 0x01) LCD_Buff[BuffCount] =  LCD.Color;			// 当前模值不为0时，使用画笔色绘点
                else LCD_Buff[BuffCount] = LCD.BackColor;		//否则使用背景色绘制点
                disChar >>= 1;     // 模值移位
                Xaddress++;        // 水平坐标自加
                BuffCount++;       // 缓冲区计数
                if( (Xaddress - x)==width ) {
                    Xaddress = x;
                    break;
                }
            }
            pImage++;
        }
        if( BuffCount == Buff_Height*width) {
            BuffCount = 0; // 缓冲区计数清0
            LCD_SetAddress( x, Yaddress , x+width-1, Yaddress+Buff_Height-1);	// 设置坐标
            LCD_WriteBuff(LCD_Buff,width*Buff_Height);          // 写入显存
            Yaddress = Yaddress+Buff_Height;    // 计算行偏移，开始写入下一部分数据
        } if( (i+1)== height ) {
            LCD_SetAddress( x, Yaddress , x+width-1,i+y);	   // 设置坐标
            LCD_WriteBuff(LCD_Buff,width*(i+1+y-Yaddress));    // 写入显存
        }
    }
}



void LCD_CopyBuffer(uint16_t x, uint16_t y,uint16_t width,uint16_t height,uint16_t *DataBuff){
    LCD_SetAddress(x,y,x+width-1,y+height-1);

    LCD_DC_Data;     // 数据指令选择 引脚输出高电平，代表本次传输 数据

    LCD_SPI.Init.DataSize 	= SPI_DATASIZE_16BIT;   //	16位数据宽度
    HAL_SPI_Init(&LCD_SPI);

    LCD_SPI_TransmitBuffer(&LCD_SPI, DataBuff,width * height) ;
    LCD_SPI.Init.DataSize 	= SPI_DATASIZE_8BIT;    //	8位数据宽度
    HAL_SPI_Init(&LCD_SPI);

}

HAL_StatusTypeDef LCD_SPI_WaitOnFlagUntilTimeout(SPI_HandleTypeDef *hspi, uint32_t Flag, FlagStatus Status,
                                                 uint32_t Tickstart, uint32_t Timeout){
    while ((__HAL_SPI_GET_FLAG(hspi, Flag) ? SET : RESET) == Status)
        if ((((HAL_GetTick() - Tickstart) >=  Timeout) && (Timeout != HAL_MAX_DELAY)) || (Timeout == 0U))
            return HAL_TIMEOUT;
    return HAL_OK;
}


void LCD_SPI_CloseTransfer(SPI_HandleTypeDef *hspi){
    uint32_t itflag = hspi->Instance->SR;
    __HAL_SPI_CLEAR_EOTFLAG(hspi);
    __HAL_SPI_CLEAR_TXTFFLAG(hspi);

    /* Disable SPI peripheral */
    __HAL_SPI_DISABLE(hspi);

    /* Disable ITs */
    __HAL_SPI_DISABLE_IT(hspi, (SPI_IT_EOT | SPI_IT_TXP | SPI_IT_RXP | SPI_IT_DXP | SPI_IT_UDR | SPI_IT_OVR | SPI_IT_FRE | SPI_IT_MODF));

    /* Disable Tx DMA Request */
    CLEAR_BIT(hspi->Instance->CFG1, SPI_CFG1_TXDMAEN | SPI_CFG1_RXDMAEN);

    /* Report UnderRun error for non RX Only communication */
    if (hspi->State != HAL_SPI_STATE_BUSY_RX){
        if ((itflag & SPI_FLAG_UDR) != 0UL){
            SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_UDR);
            __HAL_SPI_CLEAR_UDRFLAG(hspi);
        }
    }

    /* Report OverRun error for non TX Only communication */
    if (hspi->State != HAL_SPI_STATE_BUSY_TX){
        if ((itflag & SPI_FLAG_OVR) != 0UL){
            SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_OVR);
            __HAL_SPI_CLEAR_OVRFLAG(hspi);
        }
    }

    /* SPI Mode Fault error interrupt occurred -------------------------------*/
    if ((itflag & SPI_FLAG_MODF) != 0UL){
        SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_MODF);
        __HAL_SPI_CLEAR_MODFFLAG(hspi);
    }

    /* SPI Frame error interrupt occurred ------------------------------------*/
    if ((itflag & SPI_FLAG_FRE) != 0UL){
        SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_FRE);
        __HAL_SPI_CLEAR_FREFLAG(hspi);
    }

    hspi->TxXferCount = (uint16_t)0UL;
    hspi->RxXferCount = (uint16_t)0UL;
}


HAL_StatusTypeDef LCD_SPI_Transmit(SPI_HandleTypeDef *hspi,uint16_t pData, uint32_t Size){
    // TODO: 使用DMA2D进行数据传输
    uint32_t    tickstart;
    uint32_t    Timeout = 1000;      // 超时判断
    uint32_t    LCD_pData_32bit;     // 按32位传输时的数据
    uint32_t    LCD_TxDataCount;     // 传输计数
    HAL_StatusTypeDef errorcode = HAL_OK;


    /* Check Direction parameter */
    assert_param(IS_SPI_DIRECTION_2LINES_OR_1LINE_2LINES_TXONLY(hspi->Init.Direction));

    /* Process Locked */
    __HAL_LOCK(hspi);

    /* Init tickstart for timeout management*/
    tickstart = HAL_GetTick();

    if (hspi->State != HAL_SPI_STATE_READY){
        errorcode = HAL_BUSY;
        __HAL_UNLOCK(hspi);
        return errorcode;
    }

    if ( Size == 0UL){
        errorcode = HAL_ERROR;
        __HAL_UNLOCK(hspi);
        return errorcode;
    }

    /* Set the transaction information */
    hspi->State       = HAL_SPI_STATE_BUSY_TX;
    hspi->ErrorCode   = HAL_SPI_ERROR_NONE;

    LCD_TxDataCount   = Size;                // 传输的数据长度
    LCD_pData_32bit   = (pData<<16)|pData ;  // 按32位传输时，合并2个像素点的颜色

    /*Init field not used in handle to zero */
    hspi->pRxBuffPtr  = NULL;
    hspi->RxXferSize  = (uint16_t) 0UL;
    hspi->RxXferCount = (uint16_t) 0UL;
    hspi->TxISR       = NULL;
    hspi->RxISR       = NULL;

    /* Configure communication direction : 1Line */
    if (hspi->Init.Direction == SPI_DIRECTION_1LINE){
        SPI_1LINE_TX(hspi);
    }

// 不使用硬件 TSIZE 控制，此处设置为0，即不限制传输的数据长度
    MODIFY_REG(hspi->Instance->CR2, SPI_CR2_TSIZE, 0);

    /* Enable SPI peripheral */
    __HAL_SPI_ENABLE(hspi);

    if (hspi->Init.Mode == SPI_MODE_MASTER){
        /* Master transfer start */
        SET_BIT(hspi->Instance->CR1, SPI_CR1_CSTART);
    }

    /* Transmit data in 16 Bit mode */
    while (LCD_TxDataCount > 0UL){
        /* Wait until TXP flag is set to send data */
        if (__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_TXP)){
            if ((hspi->TxXferCount > 1UL) && (hspi->Init.FifoThreshold > SPI_FIFO_THRESHOLD_01DATA)){
                *((__IO uint32_t *)&hspi->Instance->TXDR) = (uint32_t )LCD_pData_32bit;
                LCD_TxDataCount -= (uint16_t)2UL;
            } else{
                *((__IO uint16_t *)&hspi->Instance->TXDR) =  (uint16_t )pData;
                LCD_TxDataCount--;
            }
        }else {
            /* Timeout management */
            if ((((HAL_GetTick() - tickstart) >=  Timeout) && (Timeout != HAL_MAX_DELAY)) || (Timeout == 0U)){
                /* Call standard close procedure with error check */
                LCD_SPI_CloseTransfer(hspi);

                __HAL_UNLOCK(hspi);

                SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_TIMEOUT);
                hspi->State = HAL_SPI_STATE_READY;
                return HAL_ERROR;
            }
        }
    }

    if (LCD_SPI_WaitOnFlagUntilTimeout(hspi, SPI_SR_TXC, RESET, tickstart, Timeout) != HAL_OK){
        SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_FLAG);
    }

    SET_BIT((hspi)->Instance->CR1 , SPI_CR1_CSUSP);
    if (LCD_SPI_WaitOnFlagUntilTimeout(hspi, SPI_FLAG_SUSP, RESET, tickstart, Timeout) != HAL_OK){
        SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_FLAG);
    }
    LCD_SPI_CloseTransfer(hspi);   /* Call standard close procedure with error check */
    SET_BIT((hspi)->Instance->IFCR , SPI_IFCR_SUSPC);  // 清除挂起标志位

    __HAL_UNLOCK(hspi);

    hspi->State = HAL_SPI_STATE_READY;

    if (hspi->ErrorCode != HAL_SPI_ERROR_NONE){
        return HAL_ERROR;
    }
    return errorcode;
}

HAL_StatusTypeDef LCD_SPI_TransmitBuffer (SPI_HandleTypeDef *hspi, uint16_t *pData, uint32_t Size){
    uint32_t    tickstart;
    uint32_t    Timeout = 1000;      // 超时判断
    uint32_t    LCD_TxDataCount;     // 传输计数
    HAL_StatusTypeDef errorcode = HAL_OK;

    /* Check Direction parameter */
    assert_param(IS_SPI_DIRECTION_2LINES_OR_1LINE_2LINES_TXONLY(hspi->Init.Direction));

    /* Process Locked */
    __HAL_LOCK(hspi);

    /* Init tickstart for timeout management*/
    tickstart = HAL_GetTick();

    if (hspi->State != HAL_SPI_STATE_READY){
        errorcode = HAL_BUSY;
        __HAL_UNLOCK(hspi);
        return errorcode;
    }

    if ( Size == 0UL){
        errorcode = HAL_ERROR;
        __HAL_UNLOCK(hspi);
        return errorcode;
    }

    hspi->State       = HAL_SPI_STATE_BUSY_TX;
    hspi->ErrorCode   = HAL_SPI_ERROR_NONE;

    LCD_TxDataCount   = Size;                // 传输的数据长度

    /*Init field not used in handle to zero */
    hspi->pRxBuffPtr  = NULL;
    hspi->RxXferSize  = (uint16_t) 0UL;
    hspi->RxXferCount = (uint16_t) 0UL;
    hspi->TxISR       = NULL;
    hspi->RxISR       = NULL;

    if (hspi->Init.Direction == SPI_DIRECTION_1LINE){
        SPI_1LINE_TX(hspi);
    }

    MODIFY_REG(hspi->Instance->CR2, SPI_CR2_TSIZE, 0);

    __HAL_SPI_ENABLE(hspi);

    if (hspi->Init.Mode == SPI_MODE_MASTER)
        SET_BIT(hspi->Instance->CR1, SPI_CR1_CSTART);

    /* Transmit data in 16 Bit mode */
    while (LCD_TxDataCount > 0UL){
        if (__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_TXP)){
            if ((LCD_TxDataCount > 1UL) && (hspi->Init.FifoThreshold > SPI_FIFO_THRESHOLD_01DATA)){
                *((__IO uint32_t *)&hspi->Instance->TXDR) = *((uint32_t *)pData);
                pData += 2;
                LCD_TxDataCount -= 2;
            }else {
                *((__IO uint16_t *)&hspi->Instance->TXDR) = *((uint16_t *)pData);
                pData += 1;
                LCD_TxDataCount--;
            }
        } else {
            if ((((HAL_GetTick() - tickstart) >=  Timeout) && (Timeout != HAL_MAX_DELAY)) || (Timeout == 0U)){
                /* Call standard close procedure with error check */
                LCD_SPI_CloseTransfer(hspi);
                __HAL_UNLOCK(hspi);

                SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_TIMEOUT);
                hspi->State = HAL_SPI_STATE_READY;
                return HAL_ERROR;
            }
        }
    }

    if (LCD_SPI_WaitOnFlagUntilTimeout(hspi, SPI_SR_TXC, RESET, tickstart, Timeout) != HAL_OK)
        SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_FLAG);

    SET_BIT((hspi)->Instance->CR1 , SPI_CR1_CSUSP); // 请求挂起SPI传输
    /* 等待SPI挂起 */
    if (LCD_SPI_WaitOnFlagUntilTimeout(hspi, SPI_FLAG_SUSP, RESET, tickstart, Timeout) != HAL_OK)
        SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_FLAG);

    LCD_SPI_CloseTransfer(hspi);   /* Call standard close procedure with error check */
    SET_BIT((hspi)->Instance->IFCR , SPI_IFCR_SUSPC);  // 清除挂起标志位

    __HAL_UNLOCK(hspi);
    hspi->State = HAL_SPI_STATE_READY;

    if (hspi->ErrorCode != HAL_SPI_ERROR_NONE) return HAL_ERROR;
    return errorcode;
}

uint8_t nowLine = 0;

int lcd_print(char *list){
    uint8_t height = LCD.Height / (LCD_AsciiFonts->Height + 8);
    char *back[height];
    int line = strspilt(list, "\n", back);
    // 判断是否超出屏幕
    for (int i = 0; i < line; i++)
        if (strlen(back[i]) * LCD_AsciiFonts->Width > LCD.Width)
            return -1;

    if ((line + nowLine) * (LCD_AsciiFonts->Height + 8) > LCD.Height) {
        LCD_Clear();
        nowLine = 0;
    }

    for (int i = 0; i < line; i++, nowLine++) LCD_DisplayString(0, nowLine * (LCD_AsciiFonts->Height + 8), back[i]);
    return 0;
}
