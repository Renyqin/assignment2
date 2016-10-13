
// Array representation according to Wikipedia article but starts from 0 index.
#include <stdio.h>
#include <stdlib.h>

#include "priority_queue.h"
struct heap h;

node_t *int_node(node_t *node){
	node->priority=1;
	node->depth=0;
	node->parent=NULL;
	return node;
}

int
main(int argc, char *argv[]){
	node_t *node;
	node_t *oldnode;
	//node_t p;
	
	node=(node_t*)malloc(sizeof(*node));
	oldnode=(node_t*)malloc(sizeof(*oldnode));
	
	heap_init(&h);
	node=int_node(node);
	heap_push(&h, node);
	oldnode=node;
	node=(node_t*)malloc(sizeof(*node));
	node->priority=5;
	heap_push(&h, node);
	node=(node_t*)malloc(sizeof(*node));
	node->priority=10;
	heap_push(&h, node);
	

	printf("%d", h.heaparr[0]->priority);
	heap_display(&h);
	return 0;
	
}