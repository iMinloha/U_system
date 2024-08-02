#include "../inc/RamTask.h"
#include "u_stdio.h"
#include "cmsis_os.h"
#include "MessageQueue.h"

extern DeviceTree_t rootDeviceTree;

void ram_init(){
    tlsf_create();
    initMessageQueue();
}

void ram_main(){
    // ram可以使用
    ThreadLock;
    publishMessage("LCD_Disp", "ram can be used", 15, DEVICE_DATA);
    // lcd_print("bash /home> ok");
    osDelay(1000);
    ThreadUnlock;
}