#ifndef U_SYSTEM_DEVICETREE_H
#define U_SYSTEM_DEVICETREE_H

#include "u_stdio.h"
#include "tlsf.h"

// 设备类型
/**
 * @brief 设备类型
 * @note DEVICE_TYPE_UNKNOWN: 未知设备
 * @note DEVICE_TYPE_SAMPLER: 采样设备
 * @note DEVICE_TYPE_CAMERA: 摄像头
 * @note DEVICE_TYPE_TIMER: 定时器
 * @note DEVICE_TYPE_COMMUNICATION: 通信设备
 * @note DEVICE_TYPE_STORAGE: 存储设备
 * @note DEVICE_TYPE_DISPLAY: 显示设备
 * @note DEVICE_TYPE_NETWORK: 网络设备
 * @note DEVICE_TYPE_SECURITY: 安全设备
 * @note DEVICE_TYPE_CONTROL: 底层设备
 * @note DEVICE_TYPE_OTHER: 其他设备
 * @param DeviceType_t: 设备类型
 * */
typedef enum DeviceType{
    DEVICE_TYPE_UNKNOWN = 0,
    DEVICE_TYPE_SAMPLER,    // 采样设备, ADC, DAC
    DEVICE_TYPE_CAMERA, // 摄像头, CAMERA
    DEVICE_TYPE_TIMER,   // 定时器, TIM, RTC
    DEVICE_TYPE_COMMUNICATION,  // 通信设备, OTG, UART, SPI, I2C
    DEVICE_TYPE_STORAGE,    // 存储设备, FLASH, SD, RAM, USB, QSPI
    DEVICE_TYPE_DISPLAY,    // 显示设备, LCD
    DEVICE_TYPE_NETWORK,    // 网络设备, ETH, WIFI
    DEVICE_TYPE_SECURITY,   // 安全设备, IWDG, WWDG
    DEVICE_TYPE_CONTROL,    // 底层设备, GPIO, CPU, FPU
    DEVICE_TYPE_OTHER   // 其他设备
} DeviceType_t;

// 权限组
/**
 * @brief 权限组
 * @note AUTHORITY_UNKNOWN: 未知权限
 * @note AUTHORITY_READ: 只读
 * @note AUTHORITY_WRITE: 只写
 * @note AUTHORITY_READ_WRITE: 读写
 * */
typedef enum Authority{
    AUTHORITY_UNKNOWN = 0,
    AUTHORITY_READ, // 只读
    AUTHORITY_WRITE,    // 只写
    AUTHORITY_READ_WRITE    // 读写
} Authority_t;


// 设备树节点
typedef struct Device* Device_t;

// 设备树
typedef struct DeviceTree* DeviceTree_t;

// 设备节点
/**
 * @brief 设备节点
 * @param Device: 设备节点
 * */
struct Device{
    char *name; // 设备名称
    char *description;  // 设备描述
    Authority_t authority;  // 设备权限
    DeviceType_t type;  // 设备类型
    void *data; // 设备数据缓冲区(需要自行设置)
    void *deviceAddr;   // 设备指针

    Device_t next;  // 下一个设备
};

struct DeviceTree{
    Device_t devices;  // 当前节点设备数组
    int deviceCount;    // 当前节点设备数量

    int depth;  // 当前节点深度

    void (*addParent)(DeviceTree_t, DeviceTree_t); // 在这个节点添加子节点
    void (*addDevice)(DeviceTree_t, char*, char*, Authority_t, DeviceType_t, void*); // 在这个节点添加设备
    void (*removeDevice)(DeviceTree_t, void*);  // 在这个节点删除设备
    Device_t (*getDevice)(DeviceTree_t, char*); // 获取这个节点的设备
    int (*getDepth)(DeviceTree_t); // 获取这个节点的深度
    DeviceTree_t (*destroy)(DeviceTree_t);  // 销毁这个节点(不会销毁子节点, 会返回下一个节点)

    char *name; // 节点路径
    char *description;  // 节点描述

    DeviceTree_t next; // 下一个节点
};

// 节点构造方法
DeviceTree_t buildDeviceTree(char* name, char* description);

// 方法们
void addParent(DeviceTree_t, DeviceTree_t);
void addDevice(DeviceTree_t, char*, char*, Authority_t, DeviceType_t, void*);
void removeDevice(DeviceTree_t, void*);
Device_t getDevice(DeviceTree_t, char*);
int getDepth(DeviceTree_t);
DeviceTree_t destroy(DeviceTree_t);


// -----------------[操作系统节点初始化]---------------------
extern DeviceTree_t rootDeviceTree;
// ------------------------------------------------------

#endif
