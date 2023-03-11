struct node {
    int data;
    char key[64];
    struct node *next;
};
void insert(struct node **head, char key[64], int data);
struct node* create(char key[64], int data);
void deleteFirst(struct node **head);
void deleteAll(struct node **head);
int isEmpty(struct node *head);
int length(struct node *head);
struct node *find(struct node *head, char key[64]);
