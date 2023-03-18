all: proctopk threadtopk
proctopk: process.c linkedlist.c getNextWord.c
	gcc -Wall -g -o proctopk process.c linkedlist.c getNextWord.c
threadtopk: thread.c linkedlist.c getNextWord.c
	gcc -Wall -g -o threadtopk thread.c  linkedlist.c getNextWord.c
clean:
	rm -fr proctopk process.o threadtopk thread.o linkedlist.o *~
