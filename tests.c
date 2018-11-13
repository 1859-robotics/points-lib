#include "./nodes.c"

task main() {
  FOREACH_ILN4(PT(1, 4), PT(4, 7), PT(1,3), PT(3, 5),
    writePoint(nodePtr);
    writeDebugStream("isHead: %s\n", (isHead ? "true" : "false"));
    writeDebugStream("isTail: %s\n", (isTail ? "true" : "false"));
  )
  while(true) { wait1Msec(20) }
}
