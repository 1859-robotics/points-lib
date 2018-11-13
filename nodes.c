// node and node list structs
typedef struct Node; // Node for forward referencing
typedef struct {
    float x;
    float y;
    Node *next;
    Node *prev;
} Node;

typedef struct {
    Node *head;
    Node *tail;

    int size;
} NodeList


void initNodeList(NodeList *list) {
    list->head = NULL
    list->tail = NULL
    list->size = 0
}
