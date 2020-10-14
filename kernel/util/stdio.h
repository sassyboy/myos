#ifndef UTIL_STDIO_H
#define UTIL_STDIO_H

typedef enum{
  STDIO_DEFAULT,
  STDIO_KERN_ERR,
  STDIO_KERN_INFO,
  STDIO_KERN_WARN,
  STDIO_CLASS_COUNT
} stdio_class_t;

typedef struct {
  void (*putc)(int ch);
  void (*puts)(char* s);
} stdio_ops_t;

void RegisterStdioInterface(stdio_class_t cls, stdio_ops_t* ops);

void KInfo(const char* fmt, ...);
void KWarn(const char* fmt, ...);
void KError(const char* fmt, ...);

#endif