#include <vmm.h>
#include <vfs.h>
#include <initrd.h>

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

static uint8_t *archive;
static initrd_handle_t *handles;

typedef signed long int ssize_t;

static uint32_t atoi_octal(char *in)
{
	uint32_t size = 0;
	uint32_t j;

	uint32_t count = 1;

	for (j = 11; j > 0; j--, count *= 8)
		size += ((in[j - 1] - '0') * count);

	return size;
}

static void alloc_handles()
{
	handles = vmm_alloc(1, 0);
	for (size_t i = 0; i < 128; i++) {
		handles[i].used = 0;
	}
}

static int find_free_handle()
{
	for (size_t i = 0; i < 128; i++) {
		if (!handles[i].used) {
			return i;
		}
	}
	return -1;
}

int initrd_open(char *path, int perms)
{
	uint8_t *ptr = (uint8_t *) archive;
	tarhdr_t *hdr = (tarhdr_t *) ptr;

	while (kstrcmp(hdr->magic, "ustar") == 0) {
		hdr = (tarhdr_t *) ptr;
		size_t sz = atoi_octal(hdr->size);
		if (kstrcmp(hdr->name, path) == 0) {
			int free_handle = find_free_handle();
			if (free_handle < 0) {
				return -1;
			}
			handles[free_handle].used = 1;
			handles[free_handle].offset = 0;
			handles[free_handle].size = sz;
			handles[free_handle].perms = perms;
			handles[free_handle].start = (uint8_t *) ptr + 512;
			return free_handle;
		}
		ptr += (((sz + 511) / 512) + 1) * 512;
	}
	return -1;
}

int initrd_read(int handle, void *buf, size_t n)
{
	if (handle == -1) {
		return -1;
	}
	kmemcpy(buf, handles[handle].start + handles[handle].offset, n);
	return n;
}

int initrd_write(int handle, void *buf, size_t n)
{
	return -1;
}

int initrd_stat(char *path, void *statbuf)
{
	int handle = initrd_open(path, 0);
	if (handle == -1) {
		return -1;
	}

	kmemcpy((initrd_stat_t *) statbuf, (initrd_stat_t *) & handles[handle],
		sizeof(initrd_stat_t));

	initrd_close(handle);
	return 0;
}

int initrd_fstat(int handle, void *statbuf)
{
	kmemcpy((initrd_stat_t *) statbuf, (initrd_stat_t *) & handles[handle],
		sizeof(initrd_stat_t));
	return 0;
}

int initrd_lseek(int handle, int offset, int whence)
{
	switch (whence) {
	case SEEK_SET:
		handles[handle].offset = offset;
		break;

	case SEEK_CUR:
		handles[handle].offset = (handles[handle].offset + offset);
		break;

	case SEEK_END:
		handles[handle].offset = (handles[handle].size + offset);
		break;
	}
	return handles[handle].offset;
}

int initrd_close(int handle)
{
	if (!handles[handle].used) {
		return -1;
	}
	kmemset(&handles[handle], 0, sizeof(initrd_handle_t));
	return 0;
}

int initrd_mount(int dev)
{
	return 0;
}

void initrd_install(uint32_t addr)
{
	alloc_handles();
	vfs_install_fs("initrd", initrd_open, initrd_close, initrd_read,
		       initrd_write, initrd_stat, initrd_fstat, initrd_lseek,
		       initrd_mount);
	archive = (uint8_t *) addr;
}
