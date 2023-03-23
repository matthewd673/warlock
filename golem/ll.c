#include <stdlib.h>
#include "ll.h"

struct LNode {
    LNode next;
    void *content;
};

LNode new_LNode(void *content) {
    LNode this = (LNode)malloc(sizeof(struct LNode));

    this->content = content;

    return this;
}

void *LNode_getContent(LNode n) {
    return n->content;
}

int LNode_hasNext(LNode n) {
    return n->next != NULL;
}

LNode LNode_getNext(LNode n) {
    return n->next;
}

struct LL {
    LNode head;
    LNode last;
};

LL new_LL() {
    LL this = (LL)malloc(sizeof(struct LL));
    return this;
}

void LL_add(LL ll, void *item) {
    LNode n = new_LNode(item);

    if (ll->head == NULL) {
        ll->head = n;
        ll->last = n;
        return;
    }

    ll->last->next = n;
    ll->last = n;
}

LNode LL_getHead(LL ll) {
    return ll->head;
}