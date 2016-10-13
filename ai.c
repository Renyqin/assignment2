#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "ai.h"
#include "utils.h"
#include "priority_queue.h"





struct heap h;


void copy_board(uint8_t board1[SIZE][SIZE], uint8_t board2[SIZE][SIZE]);
node_t *int_node(node_t *node, uint8_t board[SIZE][SIZE]);
int array_size(int max_depth);
void four_direction(node_t *node,uint8_t board[SIZE][SIZE], uint32_t* score, uint32_t scrboard[SIZE]);
node_t *new_node(node_t *node, node_t *prtnode, uint8_t board[SIZE][SIZE], uint32_t* score, int i);
void int_scoreboard(uint32_t scrboard[SIZE]);
void cmp_put_scoreboard(node_t *node, uint32_t scrboard[SIZE]);
int array_size(int max_depth);
void copy_board(uint8_t board1[SIZE][SIZE], uint8_t board2[SIZE][SIZE]);
int compare_board(uint8_t board1[SIZE][SIZE], uint8_t board2[SIZE][SIZE]);
int decision_making(uint32_t scrboard[]);
void free_array(node_t **nodeArr, int size);


void initialize_ai(){
	heap_init(&h);
}

/**
 * Find best action by building all possible paths up to depth max_depth
 * and back propagate using either max or avg
 */
 

move_t get_next_move( uint8_t board[SIZE][SIZE], int max_depth, propagation_t propagation ){
	node_t *node;
	move_t best_action;//rand() % 4;
	node_t **nodeArr;
	int dltIndex=0;
	uint32_t score=0;
	uint32_t scrboard[SIZE];
	
	int_scoreboard(scrboard);
	
	node=(node_t*)malloc(sizeof(*node));
	assert(node!=NULL);
	nodeArr=(node_t**)malloc(array_size(max_depth)*sizeof(node_t*));
	assert(nodeArr!=NULL);
	node=int_node(node, board);
	heap_push(&h, node);  
	
	
	//if the first node exit, the heap not empty
	while (h.heaparr[0]!=NULL){  
		node=h.heaparr[0];
		
		//delete the first node in heap
		heap_delete(&h); 
		nodeArr[dltIndex]=node;
		dltIndex++;
		
		if (node->depth<max_depth){
			four_direction(node,board, &score, scrboard);
		}
		
	}
	
	free_array(nodeArr, dltIndex);
	best_action=decision_making(scrboard);
	//printf("%d\n",best_action);
	
	
	

	//addRandom(board);
	
	return best_action;
}



void
four_direction(node_t *node,uint8_t board[SIZE][SIZE], uint32_t *score, uint32_t scrboard[SIZE]){
	int i;
	uint32_t newscore=*score;
	node_t *oldnode;
	uint8_t newboard[SIZE][SIZE];
	node_t *prtnode;
	prtnode=(node_t*)malloc(sizeof(*prtnode));
		assert(prtnode!=NULL);
	copy_board(newboard, board);
	
	for (i=0; i<SIZE; i++){
		if (i==left){
			moveLeft(newboard, &newscore);
			addRandom(newboard);
		}else if(i==right){
			moveRight(newboard, &newscore);
			addRandom(newboard);
		}else if(i==up){
			moveUp(newboard, &newscore);
			addRandom(newboard);
		}else if(i==down){
			moveDown(newboard, &newscore);
			addRandom(newboard);
		}
		node=(node_t*)malloc(sizeof(*node));
		node=new_node(node, prtnode, newboard, &newscore, i);
		
		if (!compare_board(board, newboard)){
			cmp_put_scoreboard(node, scrboard);
			heap_push(&h, node);
		}else{
			oldnode=(node_t*)malloc(sizeof(*oldnode));
			oldnode=node;
			free(oldnode);
		}
		
		prtnode=(node_t*)malloc(sizeof(*prtnode));
		assert(prtnode!=NULL);
		newscore=prtnode->priority;//
		
		copy_board(newboard, board);
		
		heap_display(&h);
	}
}


node_t
*new_node(node_t *node, node_t *prtnode, uint8_t board[SIZE][SIZE], uint32_t* score, int i){
	node->priority=*score;
	node->depth=prtnode->depth+1;
	if (prtnode->move==stay){
		node->move=i;
	}else{
		node->move=prtnode->move;
	}
	copy_board(node->board, board);
	node->parent=prtnode;
	return node;
}

node_t
*int_node(node_t *node, uint8_t board[SIZE][SIZE]){
	node->priority=0;
	node->depth=0;
	node->move=stay;
	copy_board(node->board, board);
	node->parent=NULL;
	return node;
}

void
int_scoreboard(uint32_t scrboard[SIZE]){
	int i;
	for(i=0; i<SIZE; i++){
		scrboard[i]=0;
	}
}

void
cmp_put_scoreboard(node_t *node, uint32_t scrboard[SIZE]){
	int i;
	for (i=0; i<SIZE; i++){
		//problem in node->move
		if ((node->priority)>scrboard[i]&&node->move==scrboard[i]){
			scrboard[i]=node->priority;
		}
	}
}

int
array_size(int max_depth){
	int i,size;
	for (i=0;i<max_depth;i++){
		size=size+pow(SIZE,i);
	}
	return size;
}

void
copy_board(uint8_t board1[SIZE][SIZE], uint8_t board2[SIZE][SIZE]){
	int i, j;
	for (i=0;i<SIZE;i++){
		for(j=0;j<SIZE;j++){
			board1[i][j]=board2[i][j];
		}
	}
}

int
compare_board(uint8_t board1[SIZE][SIZE], uint8_t board2[SIZE][SIZE]){
	int i, j;
	for (i=0; i<SIZE; i++){
		for(j=0; j<SIZE; j++){
			if (board1[i][j]!=board2[i][j]){
				return 0;
			}
		}
	}
	return 1;
}

int
decision_making(uint32_t scrboard[]){
	int i;
	uint32_t max=0;
	int move=SIZE;
	for (i=0; i<SIZE; i++){
		if (scrboard[i]>max){
			move=scrboard[i];
		}
	}
	return move;
}


void
free_array(node_t **nodeArr, int size){
	int i;
	for (i=0;i<size;i++){
		free(nodeArr[i]);
	}
}
