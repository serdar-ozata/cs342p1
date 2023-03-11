all: proctopk threadtopk
proctopk: process.c linkedlist.c
	gcc -Wall -g -o proctopk process.c linkedlist.c
threadtopk: thread.c linkedlist.c
	gcc -Wall -g -o threadtopk thread.c  linkedlist.c
clean:
	rm -fr proctopk process.o threadtopk thread.o linkedlist.o *~
