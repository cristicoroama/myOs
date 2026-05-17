#include "../include/fs.h"
#include "../include/string.h"
#include "../include/printf.h"

static File files[MAX_FILES];
static int  fs_ready = 0;

void fs_init() {
    for (int i = 0; i < MAX_FILES; i++) {
        files[i].used = 0;
        files[i].size = 0;
        memset(files[i].name, 0, MAX_FILENAME);
        memset(files[i].data, 0, MAX_FILESIZE);
    }
    fs_ready = 1;
    printf("[FS] Filesystem initializat. Max fisiere: %d\n", MAX_FILES);
}

static File* find_file(const char* name) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (files[i].used && strcmp(files[i].name, name) == 0)
            return &files[i];
    }
    return (File*)0;
}

static File* find_free() {
    for (int i = 0; i < MAX_FILES; i++) {
        if (!files[i].used) return &files[i];
    }
    return (File*)0;
}

int fs_exists(const char* name) {
    return find_file(name) != (File*)0;
}

int fs_create(const char* name) {
    if (find_file(name)) return FS_ERR_EXISTS;
    File* f = find_free();
    if (!f) return FS_ERR_FULL;
    memset(f->name, 0, MAX_FILENAME);
    memset(f->data, 0, MAX_FILESIZE);
    strcpy(f->name, name);
    f->size = 0;
    f->used = 1;
    return FS_OK;
}

int fs_write(const char* name, const char* data, unsigned int size) {
    File* f = find_file(name);
    if (!f) return FS_ERR_NOTFOUND;
    if (size > MAX_FILESIZE) size = MAX_FILESIZE;
    memcpy(f->data, data, size);
    f->size = size;
    return FS_OK;
}

int fs_read(const char* name, char* buf, unsigned int size) {
    File* f = find_file(name);
    if (!f) return FS_ERR_NOTFOUND;
    unsigned int to_read = f->size < size ? f->size : size;
    memcpy(buf, f->data, to_read);
    return (int)to_read;
}

int fs_delete(const char* name) {
    File* f = find_file(name);
    if (!f) return FS_ERR_NOTFOUND;
    f->used = 0;
    f->size = 0;
    memset(f->name, 0, MAX_FILENAME);
    memset(f->data, 0, MAX_FILESIZE);
    return FS_OK;
}

void fs_list() {
    int count = 0;
    printf("\nFisiere:\n");
    for (int i = 0; i < MAX_FILES; i++) {
        if (files[i].used) {
            printf("  %s (%u bytes)\n", files[i].name, files[i].size);
            count++;
        }
    }
    if (count == 0) printf("  (gol)\n");
    printf("Total: %d fisiere\n\n", count);
}
