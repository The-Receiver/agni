#include <vfs.h>
#include <vmm.h>
#include <klib.h>
#include <conf.h>
#include <initrd.h>

int conf_parse(char *path, conf_t * conf)
{
	int newlines = 0;
	int original_i = 0;
	int handle = vfs_open(path, 0);
	if (handle == -1) {
		return -1;
	}
	char *filebuf = vmm_alloc(1, 0);
	initrd_stat_t *statbuf = vmm_alloc(1, 0);

	vfs_fstat(handle, statbuf);
	vfs_read(handle, filebuf, statbuf->size);

	char *ptr = filebuf;
	for (; *ptr; ptr++) {
		if (*ptr == '\n') {
			newlines++;
		}
	}

	uintptr_t arith = (uintptr_t) filebuf;
	for (int i = 0; i < newlines; filebuf++, arith++, original_i++) {
		if (*filebuf == '\n') {
			i++;
		}
		char key[16];
		int k, l;
		for (int j = 0; j < 16; j++) {
			char *ptr = (char *)(arith * i + j);
			if (*(ptr) == ':') {
				k = j;
				key[j] = 0;
				break;
			}
			key[j] = *(ptr);
		}
		char value[16];
		for (int j = 0; j < 16; j++) {
			char *ptr = (char *)(arith * i + j);
			if (*(ptr) == ';') {
				l = j;
				value[j] = 0;
				i++;
				break;
			}
			value[j] = *(ptr);
		}
		kmemcpy(conf[original_i].key, key, k);
		kmemcpy(conf[original_i].value, value, l);
	}
	
	vmm_free(filebuf, 1);
    vmm_free(statbuf, 1);
	
	return 0;
}
