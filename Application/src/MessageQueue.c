#include "MessageQueue.h"
#include "tlsf.h"
#include "u_stdio.h"

// TODO 整个消息队列都有问题，需要重新设计

/**
 * @brief 系统消息队列
 * @note 消息队列位于RAM中
 * */
static MessageQueue_t SystemMessageQueue;

/**
 * @brief 初始化消息队列
 * @note 消息队列位于RAM中
 * */
void initMessageQueue(){
    SystemMessageQueue = (MessageQueue_t) tlsf_malloc(sizeof(struct MessageQueue));
    SystemMessageQueue->head = (MessageNode_t) tlsf_malloc(sizeof(MessageNode_t));

    SystemMessageQueue->mutex = (Mutex_t) tlsf_malloc(sizeof(struct _mutex));
    mutex_init(SystemMessageQueue->mutex);

    SystemMessageQueue->head->subnode = NULL;
    SystemMessageQueue->length = 0;
    SystemMessageQueue->head->next = SystemMessageQueue->tail;
//    u_print("%d\n", SystemMessageQueue->head->next == SystemMessageQueue->tail);
}


/**
 * @brief 发布消息
 * @param messageName 消息名称
 * @param message 消息
 * @param messageSize 消息大小
 * @param messageType 消息类型
 * */
void publishMessage(char *messageName, void *message, int messageSize, MessageType messageType){
    // u_print("%d\n", SystemMessageQueue->head->next == SystemMessageQueue->tail);
    mutex_lock(SystemMessageQueue->mutex);
    u_print("%d\n", SystemMessageQueue->mutex->lockflag);
    // 创建消息节点
    MessageNode_t messageNode = tlsf_malloc(sizeof(struct MessageNode));
    // 初始化消息节点, 消息类型, 消息名称, 消息, 消息大小
    messageNode->messageType = messageType;
    messageNode->message = tlsf_malloc(messageSize);
    messageNode->messageName = tlsf_malloc(strlen(messageName) + 1);    // +1为了存放字符串结束符
    // 拷贝消息名称和消息
    strcopy(messageNode->messageName, messageName);
    // 消息大小
    messageNode->messageSize = messageSize;

    // 查询是否有相同的消息
    MessageNode_t temp = SystemMessageQueue->head;

    while(temp->next != SystemMessageQueue->tail){
        if(strcmp(temp->next->messageName, messageName) == 0) break;
        temp = temp->next;
    }


    if(strcmp(temp->next->messageName, messageName) == 0){
        // 在subnode中添加消息
        MessageNode_t subnode = temp->next->subnode;
        while(subnode->subnode != NULL) subnode = subnode->subnode;
        subnode->subnode = messageNode;
        temp->next->height++;
    }else{
        // 添加消息
        temp->next = messageNode;
        messageNode->next = SystemMessageQueue->tail;
        SystemMessageQueue->length++;
    }
    u_print("OK");

    SystemMessageQueue->mutex->unlock(SystemMessageQueue->mutex);
}

/**
 * @brief 获取消息
 * @param messageName 消息名称
 * */
void* getMessage(char *messageName){
    SystemMessageQueue->mutex->lock(SystemMessageQueue->mutex); // 加锁

    MessageNode_t messageNode = SystemMessageQueue->head;   // 从头开始遍历
    while(messageNode->next != SystemMessageQueue->tail){   // 遍历消息队列
        if(strcmp(messageNode->next->messageName, messageName) == 0){   // 找到消息
            void *message = tlsf_malloc(messageNode->next->messageSize);    // 分配内存, 用于存放消息
            // 拷贝消息
            memcopy(message, messageNode->next->message, messageNode->next->messageSize);

            // 释放messageNode->subnode
            // messageNode --> temp --> messageNode->subnode->subnode
            MessageNode_t temp = messageNode->subnode;
            messageNode->subnode = messageNode->subnode->subnode;
            // 释放内存
            tlsf_free(temp->message);
            tlsf_free(temp->messageName);
            tlsf_free(temp);
            // 长度减一
            SystemMessageQueue->length--;

            // 解锁
            SystemMessageQueue->mutex->unlock(SystemMessageQueue->mutex);
            // 返回消息
            return message;
        }
        messageNode = messageNode->next;
    }

    SystemMessageQueue->mutex->unlock(SystemMessageQueue->mutex);
    return NULL;
}

/**
 * @brief 删除消息
 * @param messageName 消息名称
 * */
void removeMessage(char *messageName){
    SystemMessageQueue->mutex->lock(SystemMessageQueue->mutex);

    MessageNode_t messageNode = SystemMessageQueue->head;
    while(messageNode->next != SystemMessageQueue->tail){
        if(strcmp(messageNode->next->messageName, messageName) == 0){
            // 删除是对应名字下的所有消息
            // messageNode --> temp --> messageNode->subnode->subnode
            MessageNode_t temp = messageNode->next;
            messageNode->next = messageNode->next->next;

            // 清空temp的subnode
            while(temp->subnode != NULL){
                MessageNode_t subnode = temp->subnode;
                temp->subnode = temp->subnode->subnode;
                tlsf_free(subnode->message);
                tlsf_free(subnode->messageName);
                tlsf_free(subnode);
            }

            // 告知系统消息队列长度减一
            SystemMessageQueue->length--;
            // 解锁并返回
            SystemMessageQueue->mutex->unlock(SystemMessageQueue->mutex);
            return;
        }
        messageNode = messageNode->next;
    }

    // 没有发现消息，解锁
    SystemMessageQueue->mutex->unlock(SystemMessageQueue->mutex);
}