#ifndef FS_H
#define FS_H

#define MAX_FILES       32
#define MAX_FILENAME    32
#define MAX_FILESIZE    4096

#define FS_OK           0
#define FS_ERR_NOTFOUND -1
#define FS_ERR_FULL     -2
#define FS_ERR_EXISTS   -3

typedef struct {
    char name[MAX_FILENAME];
    char data[MAX_FILESIZE];
    unsigned int size;
    int used;
} File;

void  fs_init();
int   fs_create(const char* name);
int   fs_write(const char* name, const char* data, unsigned int size);
int   fs_read(const char* name, char* buf, unsigned int size);
int   fs_delete(const char* name);
void  fs_list();
int   fs_exists(const char* name);

#endif
