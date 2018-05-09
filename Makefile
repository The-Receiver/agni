CC=gcc
CFLAGS="-Wall"

debug:clean
	$(CC) $(CFLAGS) -g -o brahmanda main.c
stable:clean
	$(CC) $(CFLAGS) -o brahmanda main.c
clean:
	rm -vfr *~ brahmanda
