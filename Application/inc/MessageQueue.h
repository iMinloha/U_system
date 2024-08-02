#ifndef U_SYSTEM_MESSAGEQUEUE_H
#define U_SYSTEM_MESSAGEQUEUE_H

#include "hash.h"
#include "u_thread.h"


/**
 * @brief ��Ϣ����
 * @param DEVICE_INFO �豸��Ϣ
 * @param DEVICE_STATUS �豸״̬
 * @param DEVICE_CONTROL �豸����
 * @param DEVICE_DATA �豸����
 * @param DEVICE_EVENT �豸�¼�
 * @param DEVICE_ERROR �豸����
 * @param DEVICE_WARNING �豸����
 * @param DEVICE_MESSAGE �豸��Ϣ
 * @param DEVICE_COMMAND �豸����
 * @param DEVICE_RESPONSE �豸��Ӧ
 * @param DEVICE_REQUEST �豸����
 * @param DEVICE_HEARTBEAT �豸����
 * @param DEVICE_LOG �豸��־
 * @param DEVICE_UNKNOWN δ֪��Ϣ
 * */
typedef enum{
    DEVICE_INFO,
    DEVICE_STATUS,
    DEVICE_CONTROL,
    DEVICE_DATA,
    DEVICE_EVENT,
    DEVICE_ERROR,
    DEVICE_WARNING,
    DEVICE_MESSAGE,
    DEVICE_COMMAND,
    DEVICE_RESPONSE,
    DEVICE_REQUEST,
    DEVICE_HEARTBEAT,
    DEVICE_LOG,
    DEVICE_UNKNOWN
} MessageType;

/**
 * @brief ��Ϣ�ڵ�
 * @param messageName ��Ϣ����
 * @param messageType ��Ϣ����
 * @param message ��Ϣ
 * @param messageSize ��Ϣ��С
 * @param next ��һ����Ϣ
 * */
typedef struct MessageNode {
    char *messageName;  // ��Ϣ����
    MessageType messageType;  // ��Ϣ����
    void *message;  // ��Ϣ
    int messageSize;  // ��Ϣ��С
    struct MessageNode *next;  // ��һ����Ϣ
    struct MessageNode *subnode;  // ��Ϣ�ӽڵ�

    int height;  // �߶�
}* MessageNode_t;


/*==================================[��Ϣ�ڵ�ʾ��ͼ]==================================*/
/*
 *        ---------------------------   ---------------------------
 *        |                         |   |                         |
 *        |      MessageNode1       |-->|      MessageNode2       |->...
 *        |                         |   |                         |
 *        ---------------------------   ---------------------------
 *
 * */


/**
 * @brief ��Ϣ����
 * @param head ��Ϣ����ͷ
 * @param tail ��Ϣ����β
 * @param length ��Ϣ���г���
 * @param publishMessage �����Ϣ
 * @param getMessage ��ȡ��Ϣ
 * @param removeMessage ɾ����Ϣ
 * @param mutex ������
 * @note ��Ϣ����λ��RAM�У����ݽṹΪ����,�����̰߳�ȫ
 * */
typedef struct MessageQueue {
    MessageNode_t head;  // ��Ϣ����ͷ
    MessageNode_t tail;  // ��Ϣ����β
    int length;  // ��Ϣ���г���

    void (*publishMessage)(char *messageName, void *message, int messageSize, MessageType messageType);  // �����Ϣ
    void* (*getMessage)(char *messageName);  // ��ȡ��Ϣ
    void (*removeMessage)(char *messageName);  // ɾ����Ϣ

    Mutex_t mutex;  // ������
}* MessageQueue_t;

/*===================================[ϵͳ��Ϣ�ṹ]===================================*/
/**
 *              --------------------------------------
 *              |                                    |
 *              |        SystemMessageQueue          |
 *              |                                    |
 *              --------------------------------------
 *               |
 *               |
 *           -------------------         -------------------         -------------------
 *           |                 |         |                 |         |                 |
 *           |     head        |-------->|     node...     |-------->|     tail        |
 *           |                 |         |                 |         |                 |
 *           -------------------         -------------------         -------------------
 *
 *         [publishMessage]               [getMessage]               [removeMessage]
 *         |                             |                             |
 *         |                             |                             |
 *         ������Ϣ                      ��ȡ��Ϣ                      ɾ����Ϣ
 * */

/**
 * @brief ��ʼ����Ϣ����
 * @note ��Ϣ����λ��RAM��
 * */
void initMessageQueue();

/**
 * @brief ������Ϣ
 * @param messageName ��Ϣ����
 * @param message ��Ϣ
 * @param messageSize ��Ϣ��С
 * */
void publishMessage(char *messageName, void *message, int messageSize, MessageType messageType);

/**
 * @brief ��ȡ��Ϣ
 * */
void* getMessage(char *messageName);

/**
 * @brief ɾ����Ϣ
 * @param messageName ��Ϣ����
 * */
void removeMessage(char *messageName);


#endif
