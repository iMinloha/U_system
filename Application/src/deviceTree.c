#include <stddef.h>
#include "deviceTree.h"

/**
 * @brief 添加子目录
 * @param root: 根节点
 * @param node: 子节点
 * */
void addParent(DeviceTree_t root, DeviceTree_t node){
    DeviceTree_t tmp = root;
    while(tmp->next != NULL) tmp = tmp->next;
    tmp->next = node;
    node->depth = tmp->depth + 1;
}

// 构造函数
/**
 * @brief 节点构造方法
 * @param name: 节点名称(路径)
 * @param description: 节点描述
 * @retval DeviceTree_t: 实例化节点
 * */
DeviceTree_t buildDeviceTree(char* name, char* description){
    DeviceTree_t deviceTree = (DeviceTree_t)tlsf_malloc(sizeof(struct DeviceTree));
    deviceTree->devices = (Device_t)tlsf_malloc(sizeof(struct Device));
    deviceTree->deviceCount = 0;
    deviceTree->depth = 0;

    // -------------------------------
    deviceTree->addDevice = addDevice;
    deviceTree->addParent = addParent;
    deviceTree->removeDevice = removeDevice;
    deviceTree->getDevice = getDevice;
    deviceTree->getDepth = getDepth;
    deviceTree->destroy = destroy;
    // -------------------------------

    deviceTree->name = tlsf_malloc(sizeof(char) * strlen(name) + 1);
    deviceTree->description = tlsf_malloc(sizeof(char) * strlen(description) + 1);

    strcopy(deviceTree->name, name);
    strcopy(deviceTree->description, description);

    deviceTree->next = NULL;
    return deviceTree;
}

// 对当前目录添加设备的方法
/**
 * @brief 在这个节点添加设备
 * @param root: 当前节点
 * @param name: 设备名称
 * @param description: 设备描述
 * @param authority: 设备权限
 * @param type: 设备类型
 * @param deviceAddr: 设备指针
 * */
void addDevice(DeviceTree_t root, char* name, char* description, Authority_t authority, DeviceType_t type, void* deviceAddr){
    Device_t newDevice = (Device_t) tlsf_malloc(sizeof(struct Device));
    newDevice->name = tlsf_malloc(sizeof(char) * strlen(name) + 1);
    newDevice->description = tlsf_malloc(sizeof(char) * strlen(description) + 1);
    strcopy(newDevice->name, name);
    strcopy(newDevice->description, description);

    newDevice->authority = authority;
    newDevice->type = type;
    newDevice->deviceAddr = deviceAddr;
    newDevice->next = NULL;

    Device_t tmp = root->devices;
    for (int i = 0; i < root->deviceCount; i++) tmp = tmp->next;
    tmp->next = newDevice;
    root->deviceCount++;
}

// 删除设备
/**
 * @brief 在这个节点删除设备
 * @param root: 当前节点
 * @param device: 设备指针
 * */
void removeDevice(DeviceTree_t root, void* device){
    Device_t removeDevice = (Device_t)device;
    Device_t tmp = root->devices;
    for (int i = 0; i < root->deviceCount; i++){
        if(tmp->next == removeDevice){
            tmp->next = removeDevice->next;
            root->deviceCount--;
            return;
        }
        tmp = tmp->next;
    }
}

// 获取设备
/**
 * @brief 获取这个节点的设备
 * @param root: 当前节点
 * @param name: 设备名称
 * @retval Device_t: 设备指针
 * */
Device_t getDevice(DeviceTree_t root, char* name){
    Device_t tmp = root->devices;
    for (int i = 0; i < root->deviceCount; i++){
        if(strcmp(tmp->name, name) == 0) return tmp;
        tmp = tmp->next;
    }
    return NULL;
}

// 获取深度
/**
 * @brief 获取这个节点的深度
 * @param root: 当前节点
 * @retval int: 深度
 * */
int getDepth(DeviceTree_t root){
    return root->depth;
}

// 销毁节点
/**
 * @brief 销毁这个节点(不会销毁子节点, 会返回下一个节点)
 * @param root: 当前节点
 * @retval DeviceTree_t: 下一个节点
 * */
DeviceTree_t destroy(DeviceTree_t root){
    Device_t tmp = root->devices;
    for (int i = 0; i < root->deviceCount; i++){
        Device_t next = tmp->next;
        tlsf_free(tmp->name);
        tlsf_free(tmp->description);
        tlsf_free(tmp);
        tmp = next;
    }
    tlsf_free(root->devices);
    tlsf_free(root->name);
    tlsf_free(root->description);
    DeviceTree_t next = root->next;
    tlsf_free(root);
    return next;
}