#ifndef LCD_U_STDIO_H
#define LCD_U_STDIO_H

// ����ֵ
#define TRUE 1
#define FALSE 0

// �ַ���������
#define EOL '\n'
#define EOLR '\r'
#define EOLN '\0'

#define ThreadLock    __disable_irq()
#define ThreadUnlock  __enable_irq()

// ��ʽ�ַ�
#define _INTSIZEOF(n)  ((sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1))
#define va_start(ap,v) (ap = (va_list)&v + _INTSIZEOF(v))
#define va_arg(ap,t)   (*(t *)((ap += _INTSIZEOF(t)) - _INTSIZEOF(t)))
#define va_end(ap)     ( ap = (va_list)0 )

typedef char * va_list;

#define _OS_WEAK __attribute__((weak))

/**
 * @brief �����ʽ���ַ���������
 * @param fmt: ��ʽ���ַ���
 * @param ...: �ɱ����
 * @retval 0
 * */
int u_print(const char *fmt, ...);

/**
 * @brief ����ַ�������
 * @param fmt: ��ʽ���ַ���
 * @param ap: �ɱ�����б�
 * */
static int v_printf(const char *fmt, va_list ap);

/**
 * @brief ����ַ�������
 * @param c: �ַ�
 * */
void put_char(char c);

/**
 * @brief ������ֵ�����
 * @param num: ����
 * */
void put_num(int num, int base, int sign);

/**
 * @brief ����ַ���������
 * @param str: �ַ���
 * */
void put_s(char *str);

/**
 * @brief get the length of a string
 * @param str: string
 * @retval length of the string
 * */
int strlen(char *str);

/**
 * @brief split a string by a delimiter
 * @param str: string
 * @param delim: delimiter
 * @retval the first token
 * */
char* strtok(char *str, char *delim);

/**
 * @brief split a string by a delimiter
 * @param str: string
 * @param delim: delimiter
 * @param outlist: output list
 * @retval number of elements in the output list
 * */
int strspilt(char *str, char *delim, char *outlist[]);

/**
 * @brief output formatted string to a buffer
 * @param out: output buffer
 * @param fmt: format string
 * @param ...: variable arguments
 * */
void u_sprintf(char *out, const char *fmt, ...);

/**
 * @brief copy a string
 * @param dest: destination string
 * @param src: source string
 * */
void strcopy(char *dest, char *src);

/**
 * @brief concatenate two strings
 * @param dest: destination string
 * @param src: source string
 * */
void strconcat(char *dest, char *src);

/**
 * @brief compare two strings
 * @param str1: string 1
 * @param str2: string 2
 * @retval 0 if equal, 1 if not equal
 * */
int strcmp(char *str1, char *str2);

#endif //LCD_U_STDIO_H
