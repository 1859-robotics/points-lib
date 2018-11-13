// mimic malloc and free
#define MAX_LIST_SIZE 32

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


// mimic malloc and free in regular C
bool _allocStatus[MAX_LIST_SIZE];
NodeList _listNodePool[MAX_LIST_SIZE]

void FREE(Node *node) {
  for (int i = 0; i < MAX_LIST_SIZE; i++) {
    if (_allocStatus[i]) {
      if (node == _listNodePool[i]) {
        writeDebugStreamLine("FREE: deleting node[%d]: %p", i, _listNodePool[i]);
        _allocStatus[i] = false;
        memset(node, 0, sizeof(Node));
        return;
      }
    }
  }
  writeDebugStreamLine("FREE: could not find node: %p", node);
}
