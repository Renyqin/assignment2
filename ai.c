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
void four_direction(node_t *node, uint32_t scrboard[SIZE]);
node_t *new_node(node_t *newnode, node_t *node, uint8_t board[SIZE][SIZE], uint32_t score, int i);
void int_scoreboard(uint32_t scrboard[SIZE]);
void cmp_put_scoreboard(node_t *node, uint32_t scrboard[SIZE]);
int array_size(int max_depth);
void copy_board(uint8_t board1[SIZE][SIZE], uint8_t board2[SIZE][SIZE]);
int compare_board(uint8_t board1[SIZE][SIZE], uint8_t board2[SIZE][SIZE]);
int decision_making(uint32_t scrboard[]);
void free_array(node_t **nodeArr, int size);

//debug function
void scoreboard_print(uint32_t scrboard[]);


void initialize_ai(){
	heap_init(&h);
}

/**
 * Find best action by building all possible paths up to depth max_depth
 * and back propagate using either max or avg
 */
 

move_t get_next_move( uint8_t board[SIZE][SIZE], int max_depth, propagation_t propagation ){
	node_t *node;
	move_t best_action;    //rand() % 4;
	node_t **nodeArr;
	int dltIndex=0;
	uint32_t scrboard[SIZE];
	
	int_scoreboard(scrboard);
	
	node=(node_t*)malloc(sizeof(*node));
	assert(node!=NULL);
	nodeArr=(node_t**)malloc((array_size(max_depth)+1)*sizeof(node_t*));
	assert(nodeArr!=NULL);
	node=int_node(node, board);
	heap_push(&h, node);  
	
	
	//if the first node exit, the heap not empty
	while (h.count>0){  
		//printf("%d\n",h.count);
		nodeArr[dltIndex]=node;
		dltIndex++;
		node=h.heaparr[0];
		
		
		//delete the first node in heap
		heap_delete(&h); 
		
		if (node->depth<max_depth){
			four_direction(node, scrboard);
		}
		
	}
	printf("survive!\n");
	//scoreboard_print(scrboard);
	//free(node);
	printf("num: %d",dltIndex);
	free_array(nodeArr, dltIndex);
	dltIndex=0;
	free(nodeArr);
	best_action=decision_making(scrboard);
	//printf("%d\n",best_action);

	return best_action;
}



void
four_direction(node_t *node, uint32_t scrboard[SIZE]){
	int i;
	uint32_t newscore=node->priority;
	node_t *oldnode;
	uint8_t newboard[SIZE][SIZE];
	node_t *newnode;
	newnode=(node_t*)malloc(sizeof(*newnode));
	assert(newnode!=NULL);
	
	copy_board(newboard, node->board);
	
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
		
		newnode=new_node(newnode, node, newboard, newscore, i);
		
		if (!compare_board(newnode->board, node->board)){
			cmp_put_scoreboard(newnode, scrboard);
			//scoreboard_print(scrboard);
			heap_push(&h, newnode);
		}else{
			oldnode=(node_t*)malloc(sizeof(*oldnode));
			oldnode=newnode;
			free(oldnode);
		}
		
		newnode=(node_t*)malloc(sizeof(*newnode));
		assert(newnode!=NULL);
		newscore=node->priority;
		copy_board(newboard, node->board);
		
		//heap_display(&h);
	}
}


node_t
*new_node(node_t *newnode, node_t *node, uint8_t board[SIZE][SIZE], uint32_t score, int i){
	newnode->priority=score;
	newnode->depth=node->depth+1;
	if (newnode->move>stay-1){ //not sure here
		newnode->move=i;
	}else{
		newnode->move=node->move;
	}
	copy_board(newnode->board, board);
	newnode->parent=node;
	return newnode;
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
		//problem in node->move
	if (node->move<4){
	if ((node->priority)>scrboard[node->move]){
		scrboard[node->move]=node->priority;
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
			move=i;
		}
	}
	return move;
}


//debug function
void
scoreboard_print(uint32_t scrboard[]){
	int i;
	for(i=0; i<SIZE; i++){
		printf("%d: %d\n",i,scrboard[i]);
	}
}

void
free_array(node_t **nodeArr, int size){
	int i;
	for (i=0;i<size;i++){
		free(nodeArr[i]);
	}
}
