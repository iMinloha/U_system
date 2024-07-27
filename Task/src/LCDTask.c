#include "../inc/LCDTask.h"
#include "cmsis_os.h"

void LCD_Task_Init(){
    LCD_SetTextFont(&ASCII_Font32);
    LCD_SetBackColor(LCD_BLACK);
    LCD_SetColor(LCD_WHITE);
    LCD_Clear();
}

void LCD_main(void){
    // u_print("LCD_main\r\n");
    lcd_print("bash /home> vim test");
    osDelay(1000);
}
