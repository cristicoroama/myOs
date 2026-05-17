#ifndef STRING_H
#define STRING_H

int     strlen(const char* s);
char*   strcpy(char* dst, const char* src);
char*   strcat(char* dst, const char* src);
int     strcmp(const char* a, const char* b);
int     strncmp(const char* a, const char* b, int n);
char*   strchr(const char* s, char c);
void*   memset(void* ptr, int val, unsigned long n);
void*   memcpy(void* dst, const void* src, unsigned long n);
int     memcmp(const void* a, const void* b, unsigned long n);

#endif
