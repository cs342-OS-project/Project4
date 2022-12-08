compile: diskprint.c block_processing.h
	gcc diskprint.c -o diskprint
clean:
	rm diskprint