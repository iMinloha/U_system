#ifndef U_SYSTEM_MESSAGEQUEUE_H
#define U_SYSTEM_MESSAGEQUEUE_H

#include "hash.h"
#include "u_thread.h"


/**
 * @brief 消息类型
 * @param DEVICE_INFO 设备信息
 * @param DEVICE_STATUS 设备状态
 * @param DEVICE_CONTROL 设备控制
 * @param DEVICE_DATA 设备数据
 * @param DEVICE_EVENT 设备事件
 * @param DEVICE_ERROR 设备错误
 * @param DEVICE_WARNING 设备警告
 * @param DEVICE_MESSAGE 设备消息
 * @param DEVICE_COMMAND 设备命令
 * @param DEVICE_RESPONSE 设备响应
 * @param DEVICE_REQUEST 设备请求
 * @param DEVICE_HEARTBEAT 设备心跳
 * @param DEVICE_LOG 设备日志
 * @param DEVICE_UNKNOWN 未知消息
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
 * @brief 消息节点
 * @param messageName 消息名称
 * @param messageType 消息类型
 * @param message 消息
 * @param messageSize 消息大小
 * @param next 下一个消息
 * */
typedef struct MessageNode {
    char *messageName;  // 消息名称
    MessageType messageType;  // 消息类型
    void *message;  // 消息
    int messageSize;  // 消息大小
    struct MessageNode *next;  // 下一个消息
    struct MessageNode *subnode;  // 消息子节点

    int height;  // 高度
}* MessageNode_t;


/*==================================[消息节点示意图]==================================*/
/*
 *        ---------------------------   ---------------------------
 *        |                         |   |                         |
 *        |      MessageNode1       |-->|      MessageNode2       |->...
 *        |                         |   |                         |
 *        ---------------------------   ---------------------------
 *
 * */


/**
 * @brief 消息队列
 * @param head 消息队列头
 * @param tail 消息队列尾
 * @param length 消息队列长度
 * @param publishMessage 添加消息
 * @param getMessage 获取消息
 * @param removeMessage 删除消息
 * @param mutex 互斥锁
 * @note 消息队列位于RAM中，数据结构为链表,并且线程安全
 * */
typedef struct MessageQueue {
    MessageNode_t head;  // 消息队列头
    MessageNode_t tail;  // 消息队列尾
    int length;  // 消息队列长度

    void (*publishMessage)(char *messageName, void *message, int messageSize, MessageType messageType);  // 添加消息
    void* (*getMessage)(char *messageName);  // 获取消息
    void (*removeMessage)(char *messageName);  // 删除消息

    Mutex_t mutex;  // 互斥锁
}* MessageQueue_t;

/*===================================[系统消息结构]===================================*/
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
 *         发布消息                      获取消息                      删除消息
 * */

/**
 * @brief 初始化消息队列
 * @note 消息队列位于RAM中
 * */
void initMessageQueue();

/**
 * @brief 发布消息
 * @param messageName 消息名称
 * @param message 消息
 * @param messageSize 消息大小
 * */
void publishMessage(char *messageName, void *message, int messageSize, MessageType messageType);

/**
 * @brief 获取消息
 * */
void* getMessage(char *messageName);

/**
 * @brief 删除消息
 * @param messageName 消息名称
 * */
void removeMessage(char *messageName);


#endif
