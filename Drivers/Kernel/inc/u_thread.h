#ifndef U_SYSTEM_U_THREAD_H
#define U_SYSTEM_U_THREAD_H

#include <stdatomic.h>

typedef struct param* param_t;

/**
 * @brief ����ָ��
 * */
typedef void (*Func_t)(param_t param);

/**
 * @brief ������
 * @param lockflag ����־
 * @param count ������
 * @param owner ��ӵ����
 * @namespace lock ��������
 * @namespace unlock ����
 * @namespace status ��״̬
 * */
struct _mutex {
    atomic_int lockflag;
    atomic_int count;
    atomic_int owner;

    void (*lock)(struct _mutex *self);
    void (*unlock)(struct _mutex *self);
    int (*status)(struct _mutex *self);
};
/**
 * @brief ����������
 * */
typedef struct _mutex* Mutex_t;

/**
 * @brief ��������ʼ��
 * @param self ������
 */
void mutex_init(Mutex_t self);

/**
 * @brief ����������
 * @param self ������
 */
void mutex_lock(Mutex_t self);

/**
 * @brief ����������
 * @param self ������
 * */
void mutex_unlock(Mutex_t self);

/**
 * @brief ������״̬
 * @param self ������
 * */
int mutex_status(Mutex_t self);

/**
 * @brief ������ִ�к���
 * @param self ������
 * @param func ����
 * */
void LockFunc(Mutex_t self, Func_t func, struct param *param);


#endif //U_SYSTEM_U_THREAD_H
