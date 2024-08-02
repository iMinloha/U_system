#ifndef U_SYSTEM_TLSF_H
#define U_SYSTEM_TLSF_H

#include "stddef.h"

typedef void* tlsf_t;
typedef void* pool_t;


// ===================================[基础声明]=====================================

/**
 * @brief tlsf_create: 创建双链表结构内存管理
 * @retval None
 * */
void tlsf_create(); // 创建双链表结构内存管理

/**
 * @brief tlsf_destroy: 销毁双链表结构内存管理
 * @retval None
 * */
void tlsf_destroy(); // 销毁双链表结构内存管理

/**
 * @brief tlsf_get_pool: 返回void*类型的pool_t
 * @retval pool_t
 * */
pool_t tlsf_get_pool(); // 返回void*类型的pool_t


// =====================================[内存管理]==================================

/**
 * @brief tlsf_malloc: 分配内存
 * @param bytes: 分配内存大小
 * @retval None
 * */
void* tlsf_malloc(size_t bytes);

/**
 * @brief tlsf_memalign: 分配内存
 * @param align: 内存对齐
 * @param bytes: 分配内存大小
 * */
void* tlsf_memalign(size_t align, size_t bytes);

/**
 * @brief tlsf_realloc: 重新分配内存
 * @param ptr: 内存指针
 * @param size: 内存大小
 * */
void* tlsf_realloc(void* ptr, size_t size);

/**
 * @brief tlsf_free: 释放内存
 * @param ptr: 内存指针
 * */
void tlsf_free(void* ptr);


// =====================================[内存信息]==================================

/**
 * @brief tlsf_block_size: 返回内存块大小
 * @param ptr: 内存指针
 * */
size_t tlsf_block_size(void* ptr);

/**
 * @brief tlsf_size: 返回内存大小
 * */
size_t tlsf_size(void);

/**
 * @brief tlsf_align_size: 返回内存对齐大小
 * */
size_t tlsf_align_size(void);

/**
 * @brief tlsf_block_size_min: 返回内存块最小大小
 * */
size_t tlsf_block_size_min(void);

/**
 * @brief tlsf_block_size_max: 返回内存块最大大小
 * */
size_t tlsf_block_size_max(void);

/**
 * @brief tlsf_pool_overhead: 返回内存池开销
 * */
size_t tlsf_pool_overhead(void);

/**
 * @brief tlsf_alloc_overhead: 返回内存分配开销
 * */
size_t tlsf_alloc_overhead(void);


// ===================================[池管理]=====================================


/**
 * @brief tlsf_walk_pool: 遍历内存池
 * @param pool: 内存池
 * @param walker: 回调函数
 * @param user: 用户数据
 * */
typedef void (*tlsf_walker)(void* ptr, size_t size, int used, void* user);

/**
 * @brief tlsf_check: 检查内存管理
 * @param tlsf: 内存管理
 * */
void tlsf_walk_pool(pool_t pool, tlsf_walker walker, void* user);

/**
 * @brief tlsf_check: 检查内存管理
 * @param tlsf: 内存管理
 * */
int tlsf_check(tlsf_t tlsf);

/**
 * @brief tlsf_check_pool: 检查内存池
 * @param pool: 内存池
 * */
int tlsf_check_pool(pool_t pool);

/**
 * @brief 内存拷贝
 * @param dst: 目标地址
 * @param src: 源地址
 * @param size: 拷贝大小
 * */
void memcopy(void* dst, const void* src, size_t size);

#endif
