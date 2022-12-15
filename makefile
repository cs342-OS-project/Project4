compile: diskprint.c block_processing.h
	gcc -w diskprint.c -o diskprint
clean:
	rm diskprint