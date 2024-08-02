#include "../inc/LCDTask.h"
#include "cmsis_os.h"
#include "MessageQueue.h"

void LCD_Task_Init(){
    LCD_SetTextFont(&ASCII_Font32);
    LCD_SetBackColor(LCD_BLACK);
    LCD_SetColor(LCD_WHITE);
    LCD_Clear();
}

void LCD_main(void){
    // ram可以使用
    ThreadLock;
    // char* mem = getMessage("LCD_Disp");
    // u_print("got: %s\r\n", mem);
    ThreadUnlock;
}
