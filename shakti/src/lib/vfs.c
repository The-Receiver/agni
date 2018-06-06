#include <pmm.h>
#include <vfs.h>
#include <klib.h>

static vfs_fs_t *filesystems;
static vfs_handle_t *handles;
static vfs_mountpoint_t *mountpoints;

static int find_free_fs()
{
	for (int i = 0; i < 64; i++) {
		if (filesystems[i].open == NULL) {
			return i;
		}
	}
	return -1;
}

static int find_free_handle()
{
	for (int i = 0; i < 512; i++) {
		if (!handles[i].used) {
			handles[i].used = 1;
			return i;
		}
	}
	return -1;
}

static int find_free_mountpoint()
{
	for (int i = 0; i < 64; i++) {
		if (mountpoints[i].dev == -1) {
			return i;
		}
	}
	return -1;
}

void vfs_init()
{
	handles = pmm_alloc_page();
	mountpoints = pmm_alloc_page();
	filesystems = pmm_alloc_page();
	for (size_t i = 0; i < 64; i++) {
		filesystems[i].open = NULL;
	}
	for (size_t i = 0; i < 64; i++) {
		mountpoints[i].dev = -1;
	}
	for (size_t i = 0; i < 512; i++) {
		handles[i].used = 0;
	}
}

int
vfs_install_fs(char *name, int (*open) (char *, int),
	       int (*close) (int), int (*read) (int, void *, size_t),
	       int (*write) (int, void *, size_t), int (*stat) (char *,
								void
								*),
	       int (*fstat) (int, void *), int (*lseek) (int, int,
							 int),
	       int (*mount) (int))
{
	int fs_index = find_free_fs();
	if (fs_index == -1) {
		return -1;
	}
	kmemcpy(filesystems[fs_index].name, name, 32);
	filesystems[fs_index].open = open;
	filesystems[fs_index].close = close;
	filesystems[fs_index].read = read;
	filesystems[fs_index].write = write;
	filesystems[fs_index].stat = stat;
	filesystems[fs_index].fstat = fstat;
	filesystems[fs_index].lseek = lseek;
	filesystems[fs_index].mount = mount;
	return 0;
}

int vfs_mount(char *filesystem, int dev)
{
	for (int i = 0; i < 64; i++) {
		if (kstrcmp(filesystems[i].name, filesystem) == 0) {
			int point = find_free_mountpoint();
			if (point == -1) {
				return -1;

			}
			int status = filesystems[i].mount(dev);
			if (status != 0) {
				return -1;
			}
			mountpoints[i].dev = dev;
			mountpoints[i].fs_ptr = &filesystems[i];
			return 0;
		}
	}
	return -1;
}

int vfs_open(char *path, int perms)
{
	int dev;
	size_t i;
	size_t j = 0;
	char dev_number[3];
	char path_name[120];
	for (i = 0; path[i] != ':'; i++) {
		dev_number[i] = path[i];
	}
	dev_number[i] = 0;
	i++;
	for (; path[i] != 0; i++, j++) {
		path_name[j] = path[i];
	}
	path_name[j] = 0;
	dev = katoi(dev_number);
	for (i = 0; i < 64; i++) {
		if (mountpoints[i].dev == dev) {
			vfs_fs_t *fs = mountpoints[i].fs_ptr;
			int handle = fs->open(path_name, perms);
			if (handle == -1) {
				return -1;
			}
			int vfs_handle = find_free_handle();
			handles[vfs_handle].parent_handle = handle;
			handles[vfs_handle].fs = fs;
			return vfs_handle;
		}
	}
	return -1;
}

int vfs_read(int handle, void *buf, size_t n)
{
	return handles[handle].fs->read(handles[handle].parent_handle, buf, n);
}

int vfs_fstat(int handle, void *statbuf)
{
	return filesystems[handle].fstat(handle, statbuf);
}

int vfs_close(int handle)
{
	handles[handle].used = 0;
	handles[handle].fs->close(handles[handle].parent_handle);
	handles[handle].parent_handle = 0;
	handles[handle].fs = NULL;
	return 0;
}
