#include "stdio.h"
#include <fcntl.h>
#include "string.h"
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include "stdlib.h"
#include <sys/wait.h>
#include <ctype.h>
#define SNAME "shmname"

#include "linkedlist.h"

struct WordCount {
    char word[64];
    unsigned int count;
};

int main(int argc, char **argv) {
    // parse arguments
    int K = atoi(argv[1]);
    char *outPath = argv[2];
    int inFileCount = atoi(argv[3]);
    char **inFileNames = malloc(inFileCount * sizeof(char *));
    for (int i = 0; i < inFileCount; ++i) {
        inFileNames[i] = argv[i + 4];
    }
    // allocate shared memory
    // K
    const int BLOCK_SIZE = sizeof(struct WordCount) * K;
    // KxN
    const int SIZE = BLOCK_SIZE * inFileCount;

    int shm_fd;
    struct WordCount *rootPtr;
    shm_fd = shm_open(SNAME, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, SIZE); // set size of shared memory
    rootPtr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (rootPtr == MAP_FAILED) {
        printf("Map failed\n");
        return -1;
    }
    // create the child processes
    FILE *fptr;
    pid_t pid;
    for (int i = 0; i < inFileCount; ++i) {
        pid = fork();
        if (pid == 0) {
            // open file
            fptr = fopen(inFileNames[i], "r");
            if (fptr == NULL) {
                printf("Error! couldn't open file named %s.", inFileNames[i]);
                exit(1);
            }
            // get the allocated memory position of this child process
            struct WordCount *memoryPtr = rootPtr + K * i;
            // read and fill the linked list
            char word[64];
            struct node *head = NULL;
            while (fscanf(fptr, " %63s", word) == 1) {
                // convert to the upper-cased version
                int k = 0;
                while (word[k]) {
                    char ch = word[k];
                    word[k] = (char) toupper(ch);
                    k++;
                }
                // insert
                if (head) {
                    struct node *res = find(head, word);
                    if (res)
                        res->data++;
                    else
                        insert(&head, word, 1);
                } else
                    head = create(word, 1);
            }
            // retrieve K top elements
            struct node *link = head;
            for (int j = 0; j < K; ++j) {
                // copy to the shared memory
                memoryPtr[j].count = link->data;
                strcpy(memoryPtr[j].word, link->key);

                link = link->next;
            }
            // free memory
            if (head) {
                deleteAll(&head);
            }
            exit(0);
        }
    }
    // now wait for the child processes
    for (int i = 0; i < inFileCount; ++i)
        wait(NULL);
    // create the main sorted linked list
    struct node *head = NULL;
    for (int i = 0; i < K * inFileCount; ++i) {
        struct WordCount pair = rootPtr[i];
        if (head) {
            struct node *res = find(head, pair.word);
            if (res) {
                res->data += pair.count;
            }
            else {
                insert(&head, pair.word, pair.count);
            }
        } else {
            head = create(pair.word, pair.count);
        }
    }
    struct node *pair = head;
    FILE* outFile = fopen(outPath, "w");
    // write K top elements
    for (int i = 0; i < K; ++i) {
        fprintf(outFile,"%s %d\n", pair->key, pair->data);
        pair = pair->next;
    }
    free(inFileNames);
    return 0;
}