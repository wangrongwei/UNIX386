/*
 *
 *       Filename:  string.h
 *
 *    Description:  字符串处理函数
 *
 */

#ifndef __STRING_H__
#define __STRING_H__


static inline void memcpy(unsigned char *dest, const unsigned char *src, unsigned int len)
{
	for (; len != 0; len--){
		*dest++ = *src++;
	}
}

static inline void memset(void *dest, unsigned char value, unsigned int len)
{
	unsigned char *dst = (unsigned char *)dest;

	for ( ; len != 0; len--) {
		*dst++ = value;
	}
}

static inline void bzero(void *dest, unsigned int len)
{
	memset(dest, 0, len);
}

/*
 * 描述：比较两个字符串
 * 如果相等：返回0
 * 如果不等：返回正数，则str1大
 *	     返回负数，则str2大
 */
static inline int strcmp(const char *str1, const char *str2)
{
    while (*str1 && *str2 && *str1 == *str2) {
        str1++;
        str2++;
    }

    return *str1 - *str2;
}

static inline char *strcpy(char *dest, const char *src)
{
	char *tmp = dest;

	while (*src) {
	      *dest++ = *src++;
	}

	*dest = '\0';

	return tmp;
}
/*
 * 描述：将src代表的字符串加到dest代表的字符串后面
 */
static inline char *strcat(char *dest, const char *src)
{
	char *cp = dest;

	while(*cp){
	     cp++;
	}

	while ((*cp++ = *src++))
	      ;

	return dest;
}

static inline int strlen(const char *src)
{
	const char *eos = src;

        while (*eos++);

	return (eos - src - 1);
}

//#define sizeof(char *type) ((type+1)-type)


#endif//STRING_H_

