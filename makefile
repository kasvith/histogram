all:
	gcc -o prog main.c

debug:
	gcc main.c -g

clean:
	rm -r -f prog

test:
	make debug
	gdb --args ./a.out english.txt
