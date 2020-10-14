#include <kernel.h>
#include <util/stdio.h>
#include <stdarg.h> // Provided by GCC

static stdio_ops_t* stdio[STDIO_CLASS_COUNT];

void RegisterStdioInterface(stdio_class_t cls, stdio_ops_t* ops){
  stdio[cls] = ops;
}

char *itoa(unsigned int val, char* buffer, int base) { 
	static char digits[]= "0123456789ABCDEF";
	char tmp[50]; 
  char* p = buffer;
	int i = 0;

	do {
    tmp[i++] = digits[val % base];
    val = val / base;
  } while (val);
	
  while (i){
    *p = tmp[--i];
    p++;
  }
  *p = 0;
	return buffer; 
}


static void printf(stdio_ops_t* ops, const char* fmt, va_list args){
	const char *cur;
	int i;
	char *s;
  char num[50];
	
	for(cur = fmt; *cur != '\0'; cur++) {
    if (*cur != '%'){
      ops->putc(*cur);
      continue;
		} 
		
    // Parse the parameter
    cur++; 
		switch(*cur) {
    case '\0':
      return; //premature end of the string
    case '%':
      ops->putc('%');
      break;
		case 'c':
      i = va_arg(args,int);		//Fetch char argument
			ops->putc(i);
			break; 
		case 'd':
      i = va_arg(args, int); 		//Fetch Decimal/Integer argument
			if (i < 0) { 
				i = -i;
				ops->putc('-'); 
			} 
			ops->puts(itoa(i,num, 10));
			break; 			
		case 'o':
      i = va_arg(args,unsigned int); //Fetch Octal representation
			ops->puts(itoa(i,num,8));
			break; 				
		case 's':
      s = va_arg(args,char *); 		//Fetch string
			ops->puts(s); 
			break; 			
		case 'x':
      i = va_arg(args,unsigned int); //Fetch Hexadecimal representation
			ops->puts(itoa(i,num,16));
			break; 
		}	
	} 
}

void KInfo(const char* fmt, ...){
  va_list args;
  va_start(args, fmt);
  printf(stdio[STDIO_KERN_INFO], fmt, args);
  va_end(args);
}
void KWarn(const char* fmt, ...){
  va_list args;
  va_start(args, fmt);
  printf(stdio[STDIO_KERN_WARN], fmt, args);
  va_end(args);
}
void KError(const char* fmt, ...){
  va_list args;
  va_start(args, fmt);
  printf(stdio[STDIO_KERN_ERR], fmt, args);
  va_end(args);
}