all:
	gcc -o histogram main.c

debug:
	gcc main.c -g

clean:
	rm -r -f histogram

test:
	make debug
	gdb --args ./a.out english.txt
