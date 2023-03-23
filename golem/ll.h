typedef struct LNode *LNode;
typedef struct LL *LL;

void *LNode_getContent(LNode n);
int LNode_hasNext(LNode n);
LNode LNode_getNext(LNode n);

LL new_LL();
void LL_add(LL ll, void *item);
LNode LL_getHead(LL ll);