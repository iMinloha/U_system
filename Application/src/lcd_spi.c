#include "../inc/lcd_spi.h"
#include "cmsis_os.h"

#define LCD_SPI hspi5

static pFONT *LCD_AsciiFonts;
uint16_t  LCD_Buff[1024];

struct{
    uint32_t Color;  				//	LCD��ǰ������ɫ
    uint32_t BackColor;			//	����ɫ
    uint8_t  ShowNum_Mode;		// ������ʾģʽ
    uint8_t  Direction;			//	��ʾ����
    uint16_t Width;            // ��Ļ���س���
    uint16_t Height;           // ��Ļ���ؿ��
    uint8_t  X_Offset;         // X����ƫ�ƣ�����������Ļ���������Դ�д�뷽ʽ
    uint8_t  Y_Offset;         // Y����ƫ�ƣ�����������Ļ���������Դ�д�뷽ʽ
} LCD;

// ��ʾ�ַ�������������
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
    LCD_WriteCommand(0x36);       // �Դ���ʿ��� ָ��������÷����Դ�ķ�ʽ
    LCD_WriteData_8bit(0x00);     // ���ó� ���ϵ��¡������ң�RGB���ظ�ʽ

    LCD_WriteCommand(0x3A);			// �ӿ����ظ�ʽ ָ���������ʹ�� 12λ��16λ����18λɫ
    LCD_WriteData_8bit(0x05);     // �˴����ó� 16λ ���ظ�ʽ

// �������ܶ඼�ǵ�ѹ����ָ�ֱ��ʹ�ó��Ҹ��趨ֵ
    LCD_WriteCommand(0xB2);
    LCD_WriteData_8bit(0x0C);
    LCD_WriteData_8bit(0x0C);
    LCD_WriteData_8bit(0x00);
    LCD_WriteData_8bit(0x33);
    LCD_WriteData_8bit(0x33);

    LCD_WriteCommand(0xB7);		   // դ����ѹ����ָ��
    LCD_WriteData_8bit(0x35);     // VGH = 13.26V��VGL = -10.43V

    LCD_WriteCommand(0xBB);			// ������ѹ����ָ��
    LCD_WriteData_8bit(0x19);     // VCOM = 1.35V

    LCD_WriteCommand(0xC0);
    LCD_WriteData_8bit(0x2C);

    LCD_WriteCommand(0xC2);       // VDV �� VRH ��Դ����
    LCD_WriteData_8bit(0x01);     // VDV �� VRH ���û���������

    LCD_WriteCommand(0xC3);			// VRH��ѹ ����ָ��
    LCD_WriteData_8bit(0x12);     // VRH��ѹ = 4.6+( vcom+vcom offset+vdv)

    LCD_WriteCommand(0xC4);		   // VDV��ѹ ����ָ��
    LCD_WriteData_8bit(0x20);     // VDV��ѹ = 0v

    LCD_WriteCommand(0xC6); 		// ����ģʽ��֡�ʿ���ָ��
    LCD_WriteData_8bit(0x0F);   	// ������Ļ��������ˢ��֡��Ϊ60֡

    LCD_WriteCommand(0xD0);			// ��Դ����ָ��
    LCD_WriteData_8bit(0xA4);     // ��Ч���ݣ��̶�д��0xA4
    LCD_WriteData_8bit(0xA1);     // AVDD = 6.8V ��AVDD = -4.8V ��VDS = 2.3V


    LCD_WriteCommand(0xE0);       // ������ѹ٤��ֵ�趨
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

    LCD_WriteCommand(0xE1);      // ������ѹ٤��ֵ�趨
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

    LCD_WriteCommand(0x21);       // �򿪷��ԣ���Ϊ����ǳ����ͣ�������Ҫ������

    // �˳�����ָ�LCD�������ڸ��ϵ硢��λʱ�����Զ���������ģʽ ����˲�����Ļ֮ǰ����Ҫ�˳�����
    LCD_WriteCommand(0x11);       // �˳����� ָ��

    osDelay(120);               // ��Ҫ�ȴ�120ms���õ�Դ��ѹ��ʱ�ӵ�·�ȶ�����

    LCD_WriteCommand(0x29);       // ����ʾ

    LCD_SetDirection(Direction_H_Flip);  	      //	������ʾ����
    LCD_SetBackColor(LCD_BLACK);           // ���ñ���ɫ
    LCD_SetColor(LCD_WHITE);               // ���û���ɫ
    LCD_Clear();                           // ����

    LCD_SetAsciiFont(&ASCII_Font24);       // ����Ĭ������
    LCD_ShowNumMode(Fill_Zero);	      	// ���ñ�����ʾģʽ������λ���ո������0

    LCD_Backlight_ON;  // ��������ߵ�ƽ��������
}

void LCD_SetAddress(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2){
    LCD_WriteCommand(0x2a);			//	�е�ַ���ã���X����
    LCD_WriteData_16bit(x1+LCD.X_Offset);
    LCD_WriteData_16bit(x2+LCD.X_Offset);

    LCD_WriteCommand(0x2b);			//	�е�ַ���ã���Y����
    LCD_WriteData_16bit(y1+LCD.Y_Offset);
    LCD_WriteData_16bit(y2+LCD.Y_Offset);

    LCD_WriteCommand(0x2c);			//	��ʼд���Դ棬��Ҫ��ʾ����ɫ����
}

void LCD_SetColor(uint32_t Color){
    uint16_t Red_Value = 0, Green_Value = 0, Blue_Value = 0; //������ɫͨ����ֵ

    Red_Value   = (uint16_t)((Color&0x00F80000)>>8);   // ת���� 16λ ��RGB565��ɫ
    Green_Value = (uint16_t)((Color&0x0000FC00)>>5);
    Blue_Value  = (uint16_t)((Color&0x000000F8)>>3);

    LCD.Color = (uint16_t)(Red_Value | Green_Value | Blue_Value);  // ����ɫд��ȫ��LCD����
}

void LCD_SetBackColor(uint32_t Color){
    uint16_t Red_Value = 0, Green_Value = 0, Blue_Value = 0; //������ɫͨ����ֵ
    Red_Value   = (uint16_t)((Color&0x00F80000)>>8);   // ת���� 16λ ��RGB565��ɫ
    Green_Value = (uint16_t)((Color&0x0000FC00)>>5);
    Blue_Value  = (uint16_t)((Color&0x000000F8)>>3);

    LCD.BackColor = (uint16_t)(Red_Value | Green_Value | Blue_Value);	// ����ɫд��ȫ��LCD����
}

void LCD_SetDirection(uint8_t direction){
    LCD.Direction = direction;    // д��ȫ��LCD����
    if( direction == Direction_H ){
        LCD_WriteCommand(0x36);    		// �Դ���ʿ��� ָ��������÷����Դ�ķ�ʽ
        LCD_WriteData_8bit(0x70);        // ������ʾ
        LCD.X_Offset   = 0;             // ���ÿ���������ƫ����
        LCD.Y_Offset   = 0;
        LCD.Width      = LCD_Height;		// ���¸�ֵ������
        LCD.Height     = LCD_Width;
    } else if( direction == Direction_V ) {
        LCD_WriteCommand(0x36);    		// �Դ���ʿ��� ָ��������÷����Դ�ķ�ʽ
        LCD_WriteData_8bit(0x00);        // ��ֱ��ʾ
        LCD.X_Offset   = 0;              // ���ÿ���������ƫ����
        LCD.Y_Offset   = 0;
        LCD.Width      = LCD_Width;		// ���¸�ֵ������
        LCD.Height     = LCD_Height;
    } else if( direction == Direction_H_Flip ) {
        LCD_WriteCommand(0x36);   			 // �Դ���ʿ��� ָ��������÷����Դ�ķ�ʽ
        LCD_WriteData_8bit(0xA0);         // ������ʾ�������·�ת��RGB���ظ�ʽ
        LCD.X_Offset   = 0;              // ���ÿ���������ƫ����
        LCD.Y_Offset   = 0;
        LCD.Width      = LCD_Height;		 // ���¸�ֵ������
        LCD.Height     = LCD_Width;
    } else if( direction == Direction_V_Flip ) {
        LCD_WriteCommand(0x36);    		// �Դ���ʿ��� ָ��������÷����Դ�ķ�ʽ
        LCD_WriteData_8bit(0xC0);        // ��ֱ��ʾ �������·�ת��RGB���ظ�ʽ
        LCD.X_Offset   = 0;              // ���ÿ���������ƫ����
        LCD.Y_Offset   = 0;
        LCD.Width      = LCD_Width;		// ���¸�ֵ������
        LCD.Height     = LCD_Height;
    }
}

void LCD_SetAsciiFont(pFONT *Asciifonts){
    LCD_AsciiFonts = Asciifonts;
}


void LCD_Clear(void){
    LCD_SetAddress(0,0,LCD.Width-1,LCD.Height-1);	// ��������
    LCD_DC_Data;     // ����ָ��ѡ�� ��������ߵ�ƽ�������δ��� ����
    LCD_SPI.Init.DataSize 	= SPI_DATASIZE_16BIT;   //	16λ���ݿ��
    HAL_SPI_Init(&LCD_SPI);

    LCD_SPI_Transmit(&LCD_SPI, LCD.BackColor, LCD.Width * LCD.Height) ;   // ��������
    // �Ļ�8λ���ݿ�ȣ���Ϊָ��Ͳ������ݶ��ǰ���8λ�����
    LCD_SPI.Init.DataSize 	= SPI_DATASIZE_8BIT;    //	8λ���ݿ��
    HAL_SPI_Init(&LCD_SPI);
}


void LCD_ClearRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height){
    LCD_SetAddress( x, y, x+width-1, y+height-1);	// ��������

    LCD_DC_Data;     // ����ָ��ѡ�� ��������ߵ�ƽ�������δ��� ����

    LCD_SPI.Init.DataSize 	= SPI_DATASIZE_16BIT;   //	16λ���ݿ��
    HAL_SPI_Init(&LCD_SPI);

    LCD_SPI_Transmit(&LCD_SPI, LCD.BackColor, width*height) ;  // ��������

    LCD_SPI.Init.DataSize 	= SPI_DATASIZE_8BIT;    //	8λ���ݿ��
    HAL_SPI_Init(&LCD_SPI);

}


void LCD_DrawPoint(uint16_t x,uint16_t y,uint32_t color){
    LCD_SetAddress(x,y,x,y);	//	��������
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
    LCD_SetAddress( x, y, x+LCD_AsciiFonts->Width-1, y+LCD_AsciiFonts->Height-1);	   // ��������
    LCD_WriteBuff(LCD_Buff,LCD_AsciiFonts->Width*LCD_AsciiFonts->Height);          // д���Դ�
}


void LCD_DisplayString( uint16_t x, uint16_t y, char *p){
    while ((x < LCD.Width) && (*p != 0)){
        LCD_DisplayChar( x,y,*p);
        x += LCD_AsciiFonts->Width; //��ʾ��һ���ַ�
        p++;	//ȡ��һ���ַ���ַ
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
    if( LCD.ShowNum_Mode == Fill_Zero) u_sprintf( Number_Buffer , "%0*.*lf",len,decs, decimals );	// �� number ת�����ַ�����������ʾ
    else u_sprintf( Number_Buffer , "%*.*lf",len,decs, decimals );	// �� number ת�����ַ�����������ʾ
    LCD_DisplayString( x, y,(char *)Number_Buffer) ;	// ��ת���õ����ַ�����ʾ����
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
    uint16_t i ; // ��������

    for (i = 0; i < height; i++) LCD_Buff[i] =  LCD.Color;
    LCD_SetAddress( x, y, x, y+height-1);	     // ��������
    LCD_WriteBuff(LCD_Buff,height);          // д���Դ�
}


void LCD_DrawLine_H(uint16_t x, uint16_t y, uint16_t width){
    uint16_t i ; // ��������

    for (i = 0; i < width; i++) LCD_Buff[i] =  LCD.Color;  // д�뻺����
    LCD_SetAddress( x, y, x+width-1, y);	     // ��������
    LCD_WriteBuff(LCD_Buff,width);          // д���Դ�
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
    LCD_SetAddress( x, y, x+width-1, y+height-1);	// ��������

    LCD_DC_Data;
    LCD_SPI.Init.DataSize 	= SPI_DATASIZE_16BIT;   //	16λ���ݿ��
    HAL_SPI_Init(&LCD_SPI);

    LCD_SPI_Transmit(&LCD_SPI, LCD.Color, width*height) ;
    LCD_SPI.Init.DataSize 	= SPI_DATASIZE_8BIT;    //	8λ���ݿ��
    HAL_SPI_Init(&LCD_SPI);
}


void LCD_DrawImage(uint16_t x,uint16_t y,uint16_t width,uint16_t height,const uint8_t *pImage){
    uint8_t   disChar;	         // ��ģ��ֵ
    uint16_t  Xaddress = x;       // ˮƽ����
    uint16_t  Yaddress = y;       // ��ֱ����
    uint16_t  i=0,j=0,m=0;        // ��������
    uint16_t  BuffCount = 0;      // ����������
    uint16_t  Buff_Height = 0;    // ������������

    Buff_Height = (sizeof(LCD_Buff)/2) / height;    // ���㻺�����ܹ�д��ͼƬ�Ķ�����

    for(i = 0; i <height; i++){
        for(j = 0; j <(float)width/8; j++){
            disChar = *pImage;
            for(m = 0; m < 8; m++){
                if(disChar & 0x01) LCD_Buff[BuffCount] =  LCD.Color;			// ��ǰģֵ��Ϊ0ʱ��ʹ�û���ɫ���
                else LCD_Buff[BuffCount] = LCD.BackColor;		//����ʹ�ñ���ɫ���Ƶ�
                disChar >>= 1;     // ģֵ��λ
                Xaddress++;        // ˮƽ�����Լ�
                BuffCount++;       // ����������
                if( (Xaddress - x)==width ) {
                    Xaddress = x;
                    break;
                }
            }
            pImage++;
        }
        if( BuffCount == Buff_Height*width) {
            BuffCount = 0; // ������������0
            LCD_SetAddress( x, Yaddress , x+width-1, Yaddress+Buff_Height-1);	// ��������
            LCD_WriteBuff(LCD_Buff,width*Buff_Height);          // д���Դ�
            Yaddress = Yaddress+Buff_Height;    // ������ƫ�ƣ���ʼд����һ��������
        } if( (i+1)== height ) {
            LCD_SetAddress( x, Yaddress , x+width-1,i+y);	   // ��������
            LCD_WriteBuff(LCD_Buff,width*(i+1+y-Yaddress));    // д���Դ�
        }
    }
}



void LCD_CopyBuffer(uint16_t x, uint16_t y,uint16_t width,uint16_t height,uint16_t *DataBuff){
    LCD_SetAddress(x,y,x+width-1,y+height-1);

    LCD_DC_Data;     // ����ָ��ѡ�� ��������ߵ�ƽ�������δ��� ����

    LCD_SPI.Init.DataSize 	= SPI_DATASIZE_16BIT;   //	16λ���ݿ��
    HAL_SPI_Init(&LCD_SPI);

    LCD_SPI_TransmitBuffer(&LCD_SPI, DataBuff,width * height) ;
    LCD_SPI.Init.DataSize 	= SPI_DATASIZE_8BIT;    //	8λ���ݿ��
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
    // TODO: ʹ��DMA2D�������ݴ���
    uint32_t    tickstart;
    uint32_t    Timeout = 1000;      // ��ʱ�ж�
    uint32_t    LCD_pData_32bit;     // ��32λ����ʱ������
    uint32_t    LCD_TxDataCount;     // �������
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

    LCD_TxDataCount   = Size;                // ��������ݳ���
    LCD_pData_32bit   = (pData<<16)|pData ;  // ��32λ����ʱ���ϲ�2�����ص����ɫ

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

// ��ʹ��Ӳ�� TSIZE ���ƣ��˴�����Ϊ0���������ƴ�������ݳ���
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
    SET_BIT((hspi)->Instance->IFCR , SPI_IFCR_SUSPC);  // ��������־λ

    __HAL_UNLOCK(hspi);

    hspi->State = HAL_SPI_STATE_READY;

    if (hspi->ErrorCode != HAL_SPI_ERROR_NONE){
        return HAL_ERROR;
    }
    return errorcode;
}

HAL_StatusTypeDef LCD_SPI_TransmitBuffer (SPI_HandleTypeDef *hspi, uint16_t *pData, uint32_t Size){
    uint32_t    tickstart;
    uint32_t    Timeout = 1000;      // ��ʱ�ж�
    uint32_t    LCD_TxDataCount;     // �������
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

    LCD_TxDataCount   = Size;                // ��������ݳ���

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

    SET_BIT((hspi)->Instance->CR1 , SPI_CR1_CSUSP); // �������SPI����
    /* �ȴ�SPI���� */
    if (LCD_SPI_WaitOnFlagUntilTimeout(hspi, SPI_FLAG_SUSP, RESET, tickstart, Timeout) != HAL_OK)
        SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_FLAG);

    LCD_SPI_CloseTransfer(hspi);   /* Call standard close procedure with error check */
    SET_BIT((hspi)->Instance->IFCR , SPI_IFCR_SUSPC);  // ��������־λ

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
    // �ж��Ƿ񳬳���Ļ
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
