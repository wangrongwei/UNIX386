/*
 *
 *	Filename:  printk.c
 *
 *	Description:  内核的打印函数实现，依赖于gcc内置变量和函数：
 *  (__builtin_va_start(ap,last)) (__builtin_va_arg(ap,type))
 *
 */

#include "console.h"
#include "string.h"
#include "vargs.h"
#include "debug.h"

static int vsprintf(char *buff, const char *format, va_list args);

/*
 * 内核打印函数，依赖于vsprintf(...)
 */
void printk(const char *format, ...)
{
	// 避免频繁创建临时变量，内核的栈很宝贵
	static char buff[1024];
	va_list args;
	int i;

	// 按照format格式转换到buff
	va_start(args, format);
	i = vsprintf(buff, format, args);
	va_end(args);

	buff[i] = '\0';

	console_puts(buff,0,white);
}

// 判断是否是数字 返回 1：代表是数字
#define is_digit(c)	((c) >= '0' && (c) <= '9')

/*
 * 函数功能：将字符串s转换成数字，如果字符串中有字母，则终止
 * 
 * 例子：	s = "12a9aaa",	 i = 12
 *		  s	= "s12aaa", i = 0
 */
static int skip_atoi(const char **s)
{
	int i = 0;

	while (is_digit(**s)) {
		i = i * 10 + *((*s)++) - '0';
	}

	return i;
}

#define ZEROPAD		1	// pad with zero
#define SIGN	 	2   	// unsigned/signed long
#define PLUS    	4	// show plus
#define SPACE	  	8   	// space if plus
#define LEFT	 	16  	// left justified
#define SPECIAL		32  	// 0x
#define SMALL	  	64  	// use 'abcdef' instead of 'ABCDEF'

#define do_div(n,base) ({ \
		int __res; \
		__asm__("divl %4":"=a" (n),"=d" (__res):"0" (n),"1" (0),"r" (base)); \
		__res; })

/*
 * 
 *
 */
static char *number(char *str, int num, int base, int size, int precision, int type)
{
	char c, sign, tmp[36];
	const char *digits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	int i;

	if (type & SMALL) {
		digits ="0123456789abcdefghijklmnopqrstuvwxyz";
	}
	if (type & LEFT) {
		type &= ~ZEROPAD;
	}
	if (base < 2 || base > 36) {
		return 0;
	}

	c = (type & ZEROPAD) ? '0' : ' ' ;

	if (type & SIGN && num < 0) {
		sign = '-';
		num = -num;
	} 
	else {
		sign = (type&PLUS) ? '+' : ((type&SPACE) ? ' ' : 0);
	}

	if (sign) {
	      size--;
	}
	if (type & SPECIAL) {
		if (base == 16) {
			size -= 2;
		} 
		else if (base == 8) {
			size--;
		}
	}
	
	i = 0;
	if (num == 0) {
		tmp[i++] = '0';
	} 
	else {
		while (num != 0) {
			tmp[i++] = digits[do_div(num,base)];
		}
	}

	if (i > precision) {
		precision = i;
	}
	size -= precision;

	if (!(type&(ZEROPAD+LEFT))) {
		while (size-- > 0) {
			*str++ = ' ';
		}
	}
	if (sign) {
		*str++ = sign;
	}
	if (type & SPECIAL) {
		if (base == 8) {
			*str++ = '0';
		} 
		else if (base == 16) {
			*str++ = '0';
			*str++ = digits[33];
		}
	}
	if (!(type&LEFT)) {
		while (size-- > 0) {
			*str++ = c;
		}
	}
	while (i < precision--) {
		*str++ = '0';
	}
	while (i-- > 0) {
		*str++ = tmp[i];
	}
	while (size-- > 0) {
		*str++ = ' ';
	}

	return str;
}

/*
 * 描述：将args按format的格式输入到buff
 * 返回：正常返回字符串长度（除去\0）
 *	 失败返回负数
 */
static int vsprintf(char *buff, const char *format, va_list args)
{
	int len;
	int i;
	char *str;
	char *s;
	int *ip;

	int flags;		// flags to number()

	int field_width;	// width of output field
	int precision;		// min. # of digits for integers; max number of chars for from string

	for (str = buff ; *format ; ++format) {
		if (*format != '%') {
			//*str++ = *format; //此处代码要改善
			*str = *format;
			str++;
			continue;
		}

		flags = 0;
		repeat:
			++format;		// this also skips first '%'
			switch (*format) {
			case '-': flags |= LEFT;
				  goto repeat;
			case '+': flags |= PLUS;
				  goto repeat;
			case ' ': flags |= SPACE;
				  goto repeat;
			case '#': flags |= SPECIAL;
				  goto repeat;
			case '0': flags |= ZEROPAD;
				  goto repeat;
			}
		// get field width
		field_width = -1;
		if (is_digit(*format)) {
			field_width = skip_atoi(&format);
		}
		else if (*format == '*') {
			// it's the next argument
			field_width = va_arg(args, int);
			if (field_width < 0) {
				field_width = -field_width;
				flags |= LEFT;
			}
		}

		// get the precision
		precision = -1;
		if (*format == '.') {
			++format;
			if (is_digit(*format)) {
				precision = skip_atoi(&format);
			}
			else if (*format == '*') {
				// it's the next argument
				precision = va_arg(args, int);
			}
			if (precision < 0) {
				precision = 0;
			}
		}

		// get the conversion qualifier
		// int qualifier = -1;	// 'h', 'l', or 'L' for integer fields
		if (*format == 'h' || *format == 'l' || *format == 'L') {
			//qualifier = *format;
			++format;
		}

		switch (*format) {
		case 'c':
			if (!(flags & LEFT)) {
				while (--field_width > 0) {
					*str++ = ' ';
				}
			}
			*str++ = (unsigned char) va_arg(args, int);
			while (--field_width > 0) {
				*str++ = ' ';
			}
			break;

		case 's':
			s = va_arg(args, char *);
			len = strlen(s);
			if (precision < 0) {
				precision = len;
			} 
			else if (len > precision) {
				len = precision;
			}

			if (!(flags & LEFT)) {
				while (len < field_width--) {
					*str++ = ' ';
				}
			}
			for (i = 0; i < len; ++i) {
				*str++ = *s++;
			}
			while (len < field_width--) {
				*str++ = ' ';
			}
			break;

		case 'o':
			str = number(str, va_arg(args, unsigned long), 8,
				field_width, precision, flags);
			break;

		case 'p':
			if (field_width == -1) {
				field_width = 8;
				flags |= ZEROPAD;
			}
			str = number(str, (unsigned long) va_arg(args, void *), 16,
				field_width, precision, flags);
			break;

		case 'x':
			flags |= SMALL;
		case 'X':
			str = number(str, va_arg(args, unsigned long), 16,
				field_width, precision, flags);
			break;

		case 'd':
		case 'i':
			flags |= SIGN;
		case 'u':
			str = number(str, va_arg(args, unsigned long), 10,
				field_width, precision, flags);
			break;
		case 'b':
			str = number(str, va_arg(args, unsigned long), 2,
				field_width, precision, flags);
			break;

		case 'n':
			ip = va_arg(args, int *);
			*ip = (str - buff);
			break;

		default:
			if (*format != '%')
				*str++ = '%';
			if (*format) {
				*str++ = *format;
			} else {
				--format;
			}
			break;
		}
	}
	*str = '\0';

	return (str -buff);
}




