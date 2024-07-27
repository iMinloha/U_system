#ifndef U_SYSTEM_DEVICETREE_H
#define U_SYSTEM_DEVICETREE_H

#include "u_stdio.h"
#include "tlsf.h"

// �豸����
/**
 * @brief �豸����
 * @note DEVICE_TYPE_UNKNOWN: δ֪�豸
 * @note DEVICE_TYPE_SAMPLER: �����豸
 * @note DEVICE_TYPE_CAMERA: ����ͷ
 * @note DEVICE_TYPE_TIMER: ��ʱ��
 * @note DEVICE_TYPE_COMMUNICATION: ͨ���豸
 * @note DEVICE_TYPE_STORAGE: �洢�豸
 * @note DEVICE_TYPE_DISPLAY: ��ʾ�豸
 * @note DEVICE_TYPE_NETWORK: �����豸
 * @note DEVICE_TYPE_SECURITY: ��ȫ�豸
 * @note DEVICE_TYPE_CONTROL: �ײ��豸
 * @note DEVICE_TYPE_OTHER: �����豸
 * @param DeviceType_t: �豸����
 * */
typedef enum DeviceType{
    DEVICE_TYPE_UNKNOWN = 0,
    DEVICE_TYPE_SAMPLER,    // �����豸, ADC, DAC
    DEVICE_TYPE_CAMERA, // ����ͷ, CAMERA
    DEVICE_TYPE_TIMER,   // ��ʱ��, TIM, RTC
    DEVICE_TYPE_COMMUNICATION,  // ͨ���豸, OTG, UART, SPI, I2C
    DEVICE_TYPE_STORAGE,    // �洢�豸, FLASH, SD, RAM, USB, QSPI
    DEVICE_TYPE_DISPLAY,    // ��ʾ�豸, LCD
    DEVICE_TYPE_NETWORK,    // �����豸, ETH, WIFI
    DEVICE_TYPE_SECURITY,   // ��ȫ�豸, IWDG, WWDG
    DEVICE_TYPE_CONTROL,    // �ײ��豸, GPIO, CPU, FPU
    DEVICE_TYPE_OTHER   // �����豸
} DeviceType_t;

// Ȩ����
/**
 * @brief Ȩ����
 * @note AUTHORITY_UNKNOWN: δ֪Ȩ��
 * @note AUTHORITY_READ: ֻ��
 * @note AUTHORITY_WRITE: ֻд
 * @note AUTHORITY_READ_WRITE: ��д
 * */
typedef enum Authority{
    AUTHORITY_UNKNOWN = 0,
    AUTHORITY_READ, // ֻ��
    AUTHORITY_WRITE,    // ֻд
    AUTHORITY_READ_WRITE    // ��д
} Authority_t;


// �豸���ڵ�
typedef struct Device* Device_t;

// �豸��
typedef struct DeviceTree* DeviceTree_t;

// �豸�ڵ�
/**
 * @brief �豸�ڵ�
 * @param Device: �豸�ڵ�
 * */
struct Device{
    char *name; // �豸����
    char *description;  // �豸����
    Authority_t authority;  // �豸Ȩ��
    DeviceType_t type;  // �豸����
    void *data; // �豸���ݻ�����(��Ҫ��������)
    void *deviceAddr;   // �豸ָ��

    Device_t next;  // ��һ���豸
};

struct DeviceTree{
    Device_t devices;  // ��ǰ�ڵ��豸����
    int deviceCount;    // ��ǰ�ڵ��豸����

    int depth;  // ��ǰ�ڵ����

    void (*addParent)(DeviceTree_t, DeviceTree_t); // ������ڵ�����ӽڵ�
    void (*addDevice)(DeviceTree_t, char*, char*, Authority_t, DeviceType_t, void*); // ������ڵ�����豸
    void (*removeDevice)(DeviceTree_t, void*);  // ������ڵ�ɾ���豸
    Device_t (*getDevice)(DeviceTree_t, char*); // ��ȡ����ڵ���豸
    int (*getDepth)(DeviceTree_t); // ��ȡ����ڵ�����
    DeviceTree_t (*destroy)(DeviceTree_t);  // ��������ڵ�(���������ӽڵ�, �᷵����һ���ڵ�)

    char *name; // �ڵ�·��
    char *description;  // �ڵ�����

    DeviceTree_t next; // ��һ���ڵ�
};

// �ڵ㹹�췽��
DeviceTree_t buildDeviceTree(char* name, char* description);

// ������
void addParent(DeviceTree_t, DeviceTree_t);
void addDevice(DeviceTree_t, char*, char*, Authority_t, DeviceType_t, void*);
void removeDevice(DeviceTree_t, void*);
Device_t getDevice(DeviceTree_t, char*);
int getDepth(DeviceTree_t);
DeviceTree_t destroy(DeviceTree_t);


// -----------------[����ϵͳ�ڵ��ʼ��]---------------------
extern DeviceTree_t rootDeviceTree;
// ------------------------------------------------------

#endif
