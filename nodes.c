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

// utility
#define CONCAT(A, B) A ## B
// mimic malloc and free
#define MAX_LIST_SIZE 32

// for sake of visual seperation and human readability
// this is completely optional however it is recomended that anyone using
// this lib uses this when declaring points
#define PT(a, b) a, b

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


// node manipulation
void setNode(Node *node, int x, int y, Node *prev = NULL, Node *next = NULL) {
  node->x = x;
  node->y = y;
  node->prev = prev;
  node->next = next;
}

// create a new node
#define NEW_NODE(name, x_val, y_val) Node name; \
  setNode(name, x_val, y_val);

// insert new node to list
#define INSERT_NODE(list, name, x_val, y_val) \
  NEW_NODE(CONCAT(name, __LINE__), x_val, y_val); \
  insertNode(list, CONCAT(name, __LINE__));


// inline node list creation macros
//TODO: make this go up higher than 4
#define NODE_LIST1(name, x1, y1) \
  NodeList name; \
  initNodeList(name); \
  INSERT_NODE(name, P1_TEMP, x1, y1);

#define NODE_LIST2(name, x1, y1, x2, y2) \
  NODE_LIST1(name, x1, y1); \
  INSERT_NODE(name, P2_TEMP, x2, y2);

#define NODE_LIST3(name, x1, y1, x2, y2, x3, y3) \
  NODE_LIST2(name, x1, y1, x2, y2); \
  INSERT_NODE(name, P3_TEMP, x3, y3);

#define NODE_LIST4(name, x1, y1, x2, y2, x3, y3, x4, y4) \
  NODE_LIST3(name, x1, y1, x2, y2, x3, y3); \
  INSERT_NODE(name, P4_TEMP, x4, y4);


// for each in a list do a func
//TODO: make more params availible
//NOTE: when porting to V5 this can be done with callbacks
#define FOREACH(list, func) Node *nodePtr = list.head; \
	while (nodePtr != NULL) { \
    bool isHead = !!(nodePtr == list.head); \
    bool isTail = !!(nodePtr == list.tail); \
		func \
		if (nodePtr == nodePtr->next) { \
			nodePtr = NULL; \
		} else { \
			nodePtr = nodePtr->next; \
		} \
} \

// foreach inline declaration of a list up to a list of length 4
#define FOREACH_ILN1(x1, y1, func) \
	NODE_LIST1(CONCAT("_TEMP_", __LINE__), x1, y1) \
	FOREACH(CONCAT("_TEMP_", __LINE__), func)

#define FOREACH_ILN2(x1, y1, x2, y2, func) \
	NODE_LIST2(CONCAT("_TEMP_", __LINE__), x1, y1, x2, y2) \
	FOREACH(CONCAT("_TEMP_", __LINE__), func)

#define FOREACH_ILN3(x1, y1, x2, y2, x3, y3, func) \
	NODE_LIST3(CONCAT("_TEMP_", __LINE__), x1, y1, x2, y2, x3, y3) \
	FOREACH(CONCAT("_TEMP_", __LINE__), func)

#define FOREACH_ILN4(x1, y1, x2, y2, x3, y3, x4, y4, func) \
	NODE_LIST4(CONCAT("_TEMP_", __LINE__), x1, y1, x2, y2, x3, y3, x4, y4) \
	FOREACH(CONCAT("_TEMP_", __LINE__), func)

//////////////////////////////////// END LIB ////////////////////////////////////

#define DEBUG_POINTS
#ifdef DEBUG_POINTS

task main() {
  FOREACH_ILN4(PT(1, 4), PT(4, 7), PT(1,3), PT(3, 5),
    writePoint(nodePtr);
    writeDebugStream("isHead: %s\n", (isHead ? "true" : "false"));
    writeDebugStream("isTail: %s\n", (isTail ? "true" : "false"));
  )
  while(true) { wait1Msec(20) }
}

#endif
