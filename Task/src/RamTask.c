#include "../inc/RamTask.h"
#include "u_stdio.h"

extern DeviceTree_t rootDeviceTree;

void ram_init(){
    tlsf_create();
}

void ram_main(){
    // ram����ʹ��
    ThreadLock;
    u_print("ram_main\r\n");
    ThreadUnlock;
}