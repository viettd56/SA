#include <memory.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <openssl/md5.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "common.h"
#include "metadata.h"

metadata player_meta;
static int fd = -1;
static int dirty = 0;

void metadata_set(char** field, const char* value) {
    if (*field) {
        if (!strcmp(*field, value))
            return;
        free(*field);
    }
    *field = strdup(value);
    dirty = 1;
}

void metadata_open(void) {
    if (!config.meta_dir)
        return;

    const char fn[] = "now_playing";
	printf("playing\n");
    size_t pl = strlen(config.meta_dir) + 1 + strlen(fn);

    char* path = malloc(pl+1);
    snprintf(path, pl+1, "%s/%s", config.meta_dir, fn);

    if (mkfifo(path, 0644) && errno != EEXIST)
        die("Could not create metadata FIFO %s", path);

    fd = open(path, O_WRONLY | O_NONBLOCK);
    if (fd < 0)
        debug(1, "Could not open metadata FIFO %s. Will try again later.", path);

    free(path);
}

static void metadata_close(void) {
    close(fd);
    fd = -1;
}

static void print_one(const char *name, const char *value) {
    write_unchecked(fd, name, strlen(name));
    write_unchecked(fd, "=", 1);
    if (value)
        write_unchecked(fd, value, strlen(value));
    write_unchecked(fd, "\n", 1);
}

#define write_one(name) \
    print_one(#name, player_meta.name)

void metadata_write(void) {
    int ret;

    // readers may go away and come back
    if (fd < 0)
        metadata_open();
    if (fd < 0)
        return;

    if (!dirty)
        return;

    dirty = 0;

    write_one(artist);
    write_one(title);
    write_one(album);
    write_one(artwork);
    write_one(genre);
    write_one(comment);

    ret = write(fd, "\n", 1);
    if (ret < 1)    // no reader
        metadata_close();
}

void metadata_cover_image(const char *buf, int len, const char *ext) {
    if (!config.meta_dir)
        return;

    if (buf) {
        debug(1, "Cover Art set\n");
    } else {
        debug(1, "Cover Art cleared\n");
        return;
    }

    uint8_t img_md5[16];
    MD5_CTX ctx;
    MD5_Init(&ctx);
    MD5_Update(&ctx, buf, len);
    MD5_Final(img_md5, &ctx);

    char img_md5_str[33];
    int i;
    for (i = 0; i < 16; i++)
        sprintf(&img_md5_str[i*2], "%02x", (uint8_t)img_md5[i]);

    char *dir = config.meta_dir;
    char *prefix = "cover-";

    size_t pl = strlen(dir) + 1 + strlen(prefix) + strlen(img_md5_str) + 1 + strlen(ext);

    char *path = malloc(pl+1);
    snprintf(path, pl+1, "%s/%s%s.%s", dir, prefix, img_md5_str, ext);

    int cover_fd = open(path, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);

    if (cover_fd < 0) {
        warn("Could not open file %s for writing cover art", path);
        return;
    }

    if (write(cover_fd, buf, len) < len) {
        warn("writing %s failed\n", path);
        free(path);
        return;
    }
    close(cover_fd);

    debug(1, "Cover Art file is %s\n", path);
    metadata_set(&player_meta.artwork, path+strlen(dir)+1);

    free(path);
}
