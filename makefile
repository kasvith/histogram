all:
	gcc -o histogram main.c -std=c99

debug:
	gcc main.c -g

clean:
	rm -r -f histogram

test:
	make debug
	gdb --args ./a.out english.txt
