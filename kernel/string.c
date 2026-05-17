#include "../include/string.h"

int strlen(const char* s) {
    int n = 0;
    while (s[n]) n++;
    return n;
}

char* strcpy(char* dst, const char* src) {
    char* d = dst;
    while ((*d++ = *src++));
    return dst;
}

char* strcat(char* dst, const char* src) {
    char* d = dst + strlen(dst);
    while ((*d++ = *src++));
    return dst;
}

int strcmp(const char* a, const char* b) {
    while (*a && *b && *a == *b) { a++; b++; }
    return (unsigned char)*a - (unsigned char)*b;
}

int strncmp(const char* a, const char* b, int n) {
    while (n-- && *a && *b && *a == *b) { a++; b++; }
    if (n < 0) return 0;
    return (unsigned char)*a - (unsigned char)*b;
}

char* strchr(const char* s, char c) {
    while (*s) {
        if (*s == c) return (char*)s;
        s++;
    }
    return (char*)0;
}

void* memset(void* ptr, int val, unsigned long n) {
    unsigned char* p = (unsigned char*)ptr;
    while (n--) *p++ = (unsigned char)val;
    return ptr;
}

void* memcpy(void* dst, const void* src, unsigned long n) {
    unsigned char* d = (unsigned char*)dst;
    const unsigned char* s = (const unsigned char*)src;
    while (n--) *d++ = *s++;
    return dst;
}

int memcmp(const void* a, const void* b, unsigned long n) {
    const unsigned char* pa = (const unsigned char*)a;
    const unsigned char* pb = (const unsigned char*)b;
    while (n--) {
        if (*pa != *pb) return *pa - *pb;
        pa++; pb++;
    }
    return 0;
}
