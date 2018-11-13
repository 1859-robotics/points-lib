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
