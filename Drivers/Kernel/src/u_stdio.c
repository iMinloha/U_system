#include "u_stdio.h"
#include "usart.h"

_OS_WEAK void put_char(char c){
    HAL_UART_Transmit(&huart1, (uint8_t *)&c, 1, 0xFFFF);
}

_OS_WEAK void put_s(char *str){
    while(*str) put_char(*str++);
}

_OS_WEAK void put_address(void *addr){
    // ���addr�ĵ�ַ
    put_char('0');
    put_char('X');
    put_num((int)addr, 16, 0);
}

_OS_WEAK void put_num(int num, int base, int sign){
    char buf[11];
    int i = 0;
    if(sign && num < 0){
        num = -num;
        put_char('-');
    }
    do{
        buf[i++] = "0123456789ABCDEF"[num % base];
        num /= base;
    } while(num);
    for(int j = i - 1; j >= 0; j--) put_char(buf[j]);
}

static int v_printf(const char *fmt, va_list ap){
    for(; *fmt != '\0'; fmt++){
        if(*fmt != '%'){
            put_char(*fmt);
            continue;      /*��ֹ����forѭ����������ѭ��ִ�е����ﲻ������ִ�У���ʼ��һ��forѭ��*/
        }
        fmt++;
        switch(*fmt){
            case 'd': put_num(va_arg(ap, int), 10, 1);break;
            case 'o': put_num(va_arg(ap, unsigned int), 8, 0); break;
            case 'u': put_num(va_arg(ap, unsigned int), 10, 0); break;
            case 'x': put_num(va_arg(ap, unsigned int), 16, 0); break;
            case 'c': put_char(va_arg(ap, int)); break;
            case 's': put_s(va_arg(ap,char *)); break;
            case 'f': put_num(va_arg(ap, double), 10, 1); break;
            case 'l': put_num(va_arg(ap, long), 10, 1); break;
            case 'p': put_address(va_arg(ap, void *)); break;
            default:
                put_char(*fmt);
                break;
        }
    }
    return 0;
}


/**
 * @brief print formatted string to USART1
 * @param fmt: format string
 * @param ...: variable arguments
 * @retval 0
 **/
int u_print(const char *fmt, ...){
    va_list ap;
    va_start(ap, fmt);
    v_printf(fmt, ap);
    va_end(ap);
    return 0;
}

/**
 * @brief get the length of a string
 * @param str: string
 * @retval length of the string
 * */
int strlen(char *str){
    int len = 0;
    while(*str++) len++;
    return len;
}

/**
 * @brief split a string by a delimiter
 * @param str: string
 * @param delim: delimiter
 * @retval the first token
 * */
char* strtok(char *str, char *delim){
    static char *p = NULL;
    if(str) p = str;
    if(!p) return NULL;
    char *ret = p;
    while(*p){
        if(*p == *delim){
            *p = '\0';
            p++;
            return ret;
        }
        p++;
    }
    p = NULL;
    return ret;
}

/**
 * @brief split a string by a delimiter
 * @param str: string
 * @param delim: delimiter
 * @param outlist: output list
 * @retval number of elements in the output list
 * */
int strspilt(char *str, char *delim, char *outlist[]){
    int i = 0;
    char *p = strtok(str, delim);
    while(p){
        outlist[i++] = p;
        p = strtok(NULL, delim);
    }
    return i;
}

/**
 * @brief output formatted string to a buffer
 * @param out: output buffer
 * @param fmt: format string
 * @param ...: variable arguments
 * */
void u_sprintf(char *out, const char *fmt, ...){
    va_list ap;
    va_start(ap, fmt);
    v_printf(fmt, ap);
    va_end(ap);
}

/**
 * @brief copy a string
 * @param dest: destination string
 * @param src: source string
 * */
void strcopy(char *dest, char *src){
    while(*src) *dest++ = *src++;
    *dest = '\0';
}

void strconcat(char *dest, char *src){
    while(*dest) dest++;
    while(*src) *dest++ = *src++;
    *dest = '\0';
}

/**
 * @brief compare two strings
 * @param str1: string 1
 * @param str2: string 2
 * @retval 0 if equal, 1 if not equal
 * */
int strcmp(char *str1, char *str2){
    while(*str1 && *str2){
        if(*str1 != *str2) return 1;
        str1++;
        str2++;
    }
    if(*str1 || *str2) return 1;
    return 0;
}