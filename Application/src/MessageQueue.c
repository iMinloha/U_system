#include "MessageQueue.h"
#include "tlsf.h"
#include "u_stdio.h"

// TODO ������Ϣ���ж������⣬��Ҫ�������

/**
 * @brief ϵͳ��Ϣ����
 * @note ��Ϣ����λ��RAM��
 * */
static MessageQueue_t SystemMessageQueue;

/**
 * @brief ��ʼ����Ϣ����
 * @note ��Ϣ����λ��RAM��
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
 * @brief ������Ϣ
 * @param messageName ��Ϣ����
 * @param message ��Ϣ
 * @param messageSize ��Ϣ��С
 * @param messageType ��Ϣ����
 * */
void publishMessage(char *messageName, void *message, int messageSize, MessageType messageType){
    // u_print("%d\n", SystemMessageQueue->head->next == SystemMessageQueue->tail);
    mutex_lock(SystemMessageQueue->mutex);
    u_print("%d\n", SystemMessageQueue->mutex->lockflag);
    // ������Ϣ�ڵ�
    MessageNode_t messageNode = tlsf_malloc(sizeof(struct MessageNode));
    // ��ʼ����Ϣ�ڵ�, ��Ϣ����, ��Ϣ����, ��Ϣ, ��Ϣ��С
    messageNode->messageType = messageType;
    messageNode->message = tlsf_malloc(messageSize);
    messageNode->messageName = tlsf_malloc(strlen(messageName) + 1);    // +1Ϊ�˴���ַ���������
    // ������Ϣ���ƺ���Ϣ
    strcopy(messageNode->messageName, messageName);
    // ��Ϣ��С
    messageNode->messageSize = messageSize;

    // ��ѯ�Ƿ�����ͬ����Ϣ
    MessageNode_t temp = SystemMessageQueue->head;

    while(temp->next != SystemMessageQueue->tail){
        if(strcmp(temp->next->messageName, messageName) == 0) break;
        temp = temp->next;
    }


    if(strcmp(temp->next->messageName, messageName) == 0){
        // ��subnode�������Ϣ
        MessageNode_t subnode = temp->next->subnode;
        while(subnode->subnode != NULL) subnode = subnode->subnode;
        subnode->subnode = messageNode;
        temp->next->height++;
    }else{
        // �����Ϣ
        temp->next = messageNode;
        messageNode->next = SystemMessageQueue->tail;
        SystemMessageQueue->length++;
    }
    u_print("OK");

    SystemMessageQueue->mutex->unlock(SystemMessageQueue->mutex);
}

/**
 * @brief ��ȡ��Ϣ
 * @param messageName ��Ϣ����
 * */
void* getMessage(char *messageName){
    SystemMessageQueue->mutex->lock(SystemMessageQueue->mutex); // ����

    MessageNode_t messageNode = SystemMessageQueue->head;   // ��ͷ��ʼ����
    while(messageNode->next != SystemMessageQueue->tail){   // ������Ϣ����
        if(strcmp(messageNode->next->messageName, messageName) == 0){   // �ҵ���Ϣ
            void *message = tlsf_malloc(messageNode->next->messageSize);    // �����ڴ�, ���ڴ����Ϣ
            // ������Ϣ
            memcopy(message, messageNode->next->message, messageNode->next->messageSize);

            // �ͷ�messageNode->subnode
            // messageNode --> temp --> messageNode->subnode->subnode
            MessageNode_t temp = messageNode->subnode;
            messageNode->subnode = messageNode->subnode->subnode;
            // �ͷ��ڴ�
            tlsf_free(temp->message);
            tlsf_free(temp->messageName);
            tlsf_free(temp);
            // ���ȼ�һ
            SystemMessageQueue->length--;

            // ����
            SystemMessageQueue->mutex->unlock(SystemMessageQueue->mutex);
            // ������Ϣ
            return message;
        }
        messageNode = messageNode->next;
    }

    SystemMessageQueue->mutex->unlock(SystemMessageQueue->mutex);
    return NULL;
}

/**
 * @brief ɾ����Ϣ
 * @param messageName ��Ϣ����
 * */
void removeMessage(char *messageName){
    SystemMessageQueue->mutex->lock(SystemMessageQueue->mutex);

    MessageNode_t messageNode = SystemMessageQueue->head;
    while(messageNode->next != SystemMessageQueue->tail){
        if(strcmp(messageNode->next->messageName, messageName) == 0){
            // ɾ���Ƕ�Ӧ�����µ�������Ϣ
            // messageNode --> temp --> messageNode->subnode->subnode
            MessageNode_t temp = messageNode->next;
            messageNode->next = messageNode->next->next;

            // ���temp��subnode
            while(temp->subnode != NULL){
                MessageNode_t subnode = temp->subnode;
                temp->subnode = temp->subnode->subnode;
                tlsf_free(subnode->message);
                tlsf_free(subnode->messageName);
                tlsf_free(subnode);
            }

            // ��֪ϵͳ��Ϣ���г��ȼ�һ
            SystemMessageQueue->length--;
            // ����������
            SystemMessageQueue->mutex->unlock(SystemMessageQueue->mutex);
            return;
        }
        messageNode = messageNode->next;
    }

    // û�з�����Ϣ������
    SystemMessageQueue->mutex->unlock(SystemMessageQueue->mutex);
}