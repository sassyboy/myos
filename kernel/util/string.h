#ifndef UTIL_STRING_H
#define UTIL_STRING_H

static inline void *memset(void *str, int c, size_t n){
  char* p = str;
  size_t i;
  for (i = 0 ; i < n ; i++)
    p[i] = c;
  return str;
}

#endif
