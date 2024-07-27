#include <stddef.h>
#include "deviceTree.h"

/**
 * @brief �����Ŀ¼
 * @param root: ���ڵ�
 * @param node: �ӽڵ�
 * */
void addParent(DeviceTree_t root, DeviceTree_t node){
    DeviceTree_t tmp = root;
    while(tmp->next != NULL) tmp = tmp->next;
    tmp->next = node;
    node->depth = tmp->depth + 1;
}

// ���캯��
/**
 * @brief �ڵ㹹�췽��
 * @param name: �ڵ�����(·��)
 * @param description: �ڵ�����
 * @retval DeviceTree_t: ʵ�����ڵ�
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

// �Ե�ǰĿ¼����豸�ķ���
/**
 * @brief ������ڵ�����豸
 * @param root: ��ǰ�ڵ�
 * @param name: �豸����
 * @param description: �豸����
 * @param authority: �豸Ȩ��
 * @param type: �豸����
 * @param deviceAddr: �豸ָ��
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

// ɾ���豸
/**
 * @brief ������ڵ�ɾ���豸
 * @param root: ��ǰ�ڵ�
 * @param device: �豸ָ��
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

// ��ȡ�豸
/**
 * @brief ��ȡ����ڵ���豸
 * @param root: ��ǰ�ڵ�
 * @param name: �豸����
 * @retval Device_t: �豸ָ��
 * */
Device_t getDevice(DeviceTree_t root, char* name){
    Device_t tmp = root->devices;
    for (int i = 0; i < root->deviceCount; i++){
        if(strcmp(tmp->name, name) == 0) return tmp;
        tmp = tmp->next;
    }
    return NULL;
}

// ��ȡ���
/**
 * @brief ��ȡ����ڵ�����
 * @param root: ��ǰ�ڵ�
 * @retval int: ���
 * */
int getDepth(DeviceTree_t root){
    return root->depth;
}

// ���ٽڵ�
/**
 * @brief ��������ڵ�(���������ӽڵ�, �᷵����һ���ڵ�)
 * @param root: ��ǰ�ڵ�
 * @retval DeviceTree_t: ��һ���ڵ�
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