CC=gcc
C-FLAGS=-O3

cpu: cpu.c
	${CC} ${C-FLAGS} $^ -o $@

.PHONY:
clean:
	rm -f cpu