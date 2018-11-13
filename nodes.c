/******************************************************************************

Original work Copyright (c) 2018 Alexa Griffin

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:


THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

******************************************************************************/

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

// debug to write a point
void writePoint(Node node) {
  writeDebugStream("(%f, %f)\n", node.x, node.y);
}
void writePoint(Node *node) {
  writeDebugStream("(%f, %f)\n", node->x, node->y);
}


// Special thanks to Xander Soldaat (xander_at_botbench.com)
// for the linked list stuff. It isn't a 1-1 copy but it is very heavily
// referenced from: http://botbench.com/blog/2013/01/31/tutorial-linked-lists-in-robotc/
// node list manipulation functions
// deleteNode is not technically needed however it is left in for prostarity
void insertNode(NodeList *list, Node *newNode, Node *node = NULL) {
  if (list->size == 0) {
    list->head = newNode;
    list->tail = newNode;
    list->size++;
  } else if (node == NULL) {
    newNode->prev = list->tail;
    list->tail->next = newNode;
    list->tail = newNode;
    list->size++;
  } else {
    newNode->next = node->next;
    node->next = newNode;
    if (list->tail == node) list->tail = newNode;
    list->size++;
  }
}

void deleteNode(NodeList *list, Node *obsoleteNode) {
  Node *nodePtr;
  if (list->size == 1) {
    list->head = NULL;
    list->tail = NULL;
    list->size--;
    FREE(obsoleteNode);
  }else if (list->head == obsoleteNode) {
    list->head = obsoleteNode->next;
    list->size--;
    FREE(obsoleteNode);
    } else {
    nodePtr = list->head;
    while (nodePtr != NULL) {
      if (nodePtr->next == obsoleteNode) {
        if (obsoleteNode == list->tail) {
          list->tail = nodePtr;
        }
        nodePtr->next = obsoleteNode->next;
        list->size--;
        FREE(obsoleteNode);
        return;
      }
      nodePtr = nodePtr->next;
    }
  }
}
