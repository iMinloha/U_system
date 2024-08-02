#ifndef U_SYSTEM_TLSF_H
#define U_SYSTEM_TLSF_H

#include "stddef.h"

typedef void* tlsf_t;
typedef void* pool_t;


// ===================================[��������]=====================================

/**
 * @brief tlsf_create: ����˫����ṹ�ڴ����
 * @retval None
 * */
void tlsf_create(); // ����˫����ṹ�ڴ����

/**
 * @brief tlsf_destroy: ����˫����ṹ�ڴ����
 * @retval None
 * */
void tlsf_destroy(); // ����˫����ṹ�ڴ����

/**
 * @brief tlsf_get_pool: ����void*���͵�pool_t
 * @retval pool_t
 * */
pool_t tlsf_get_pool(); // ����void*���͵�pool_t


// =====================================[�ڴ����]==================================

/**
 * @brief tlsf_malloc: �����ڴ�
 * @param bytes: �����ڴ��С
 * @retval None
 * */
void* tlsf_malloc(size_t bytes);

/**
 * @brief tlsf_memalign: �����ڴ�
 * @param align: �ڴ����
 * @param bytes: �����ڴ��С
 * */
void* tlsf_memalign(size_t align, size_t bytes);

/**
 * @brief tlsf_realloc: ���·����ڴ�
 * @param ptr: �ڴ�ָ��
 * @param size: �ڴ��С
 * */
void* tlsf_realloc(void* ptr, size_t size);

/**
 * @brief tlsf_free: �ͷ��ڴ�
 * @param ptr: �ڴ�ָ��
 * */
void tlsf_free(void* ptr);


// =====================================[�ڴ���Ϣ]==================================

/**
 * @brief tlsf_block_size: �����ڴ���С
 * @param ptr: �ڴ�ָ��
 * */
size_t tlsf_block_size(void* ptr);

/**
 * @brief tlsf_size: �����ڴ��С
 * */
size_t tlsf_size(void);

/**
 * @brief tlsf_align_size: �����ڴ�����С
 * */
size_t tlsf_align_size(void);

/**
 * @brief tlsf_block_size_min: �����ڴ����С��С
 * */
size_t tlsf_block_size_min(void);

/**
 * @brief tlsf_block_size_max: �����ڴ������С
 * */
size_t tlsf_block_size_max(void);

/**
 * @brief tlsf_pool_overhead: �����ڴ�ؿ���
 * */
size_t tlsf_pool_overhead(void);

/**
 * @brief tlsf_alloc_overhead: �����ڴ���俪��
 * */
size_t tlsf_alloc_overhead(void);


// ===================================[�ع���]=====================================


/**
 * @brief tlsf_walk_pool: �����ڴ��
 * @param pool: �ڴ��
 * @param walker: �ص�����
 * @param user: �û�����
 * */
typedef void (*tlsf_walker)(void* ptr, size_t size, int used, void* user);

/**
 * @brief tlsf_check: ����ڴ����
 * @param tlsf: �ڴ����
 * */
void tlsf_walk_pool(pool_t pool, tlsf_walker walker, void* user);

/**
 * @brief tlsf_check: ����ڴ����
 * @param tlsf: �ڴ����
 * */
int tlsf_check(tlsf_t tlsf);

/**
 * @brief tlsf_check_pool: ����ڴ��
 * @param pool: �ڴ��
 * */
int tlsf_check_pool(pool_t pool);

/**
 * @brief �ڴ濽��
 * @param dst: Ŀ���ַ
 * @param src: Դ��ַ
 * @param size: ������С
 * */
void memcopy(void* dst, const void* src, size_t size);

#endif
