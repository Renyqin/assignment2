#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "ai.h"
#include "utils.h"
#include "priority_queue.h"





struct heap h;



//debug function
void scoreboard_print(uint32_t scrboard[]);
void board_print(uint8_t board[SIZE][SIZE]);

void initialize_ai(){
	heap_init(&h);
}

/**
 * Find best action by building all possible paths up to depth max_depth
 * and back propagate using either max or avg
 */
 

move_t get_next_move( uint8_t board[SIZE][SIZE], int max_depth, propagation_t propagation, uint32_t score, uint32_t *generated, uint32_t *expanded){
	node_t *node;
	move_t best_action;    //rand() % 4;
	uint32_t scrboard[SIZE];
	uint32_t scrboardavg[SIZE];
	uint32_t avgboardnum[SIZE];

	
	int_scoreboard(scrboard);
	int_scoreboard(scrboardavg);
	int_scoreboard(avgboardnum);
	
	node=(node_t*)malloc(sizeof(*node));
	assert(node!=NULL);
	node=int_node(node, board,score);
	heap_push(&h, node);  
	
	
	//if the first node exit, the heap not empty
	while (h.count>0){  

		//heap_display(&h);
		node=h.heaparr[0];
		(*expanded)+=1;
		//delete the first node in heap
		heap_delete(&h); 
		
		if (node->depth<max_depth){
			four_direction(node, scrboard, scrboardavg, avgboardnum, generated);
		}
		
	}
	free(node);
	//scoreboard_print(scrboard);
	if (propagation==max){
		best_action=decision_making(scrboard);
	}else{
		best_action=avg_decision_making(scrboardavg,avgboardnum);
	}

	return best_action;
}



void
four_direction(node_t *node, uint32_t scrboard[SIZE], uint32_t scrboardavg[SIZE],uint32_t avgboardnum[SIZE], uint32_t *generated){
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
		(*generated)+=1;
		
		if (!compare_board(newnode->board, node->board)){
			cmp_put_scoreboard(newnode, scrboard);
			put_avg_scoreboard(newnode, scrboardavg, avgboardnum);
			
			//scoreboard_print(scrboard);
			heap_push(&h, newnode);
		}else{
			oldnode=(node_t*)malloc(sizeof(*oldnode));
			assert(oldnode!=NULL);
			if (newnode!=NULL){
				oldnode=newnode;
				free(oldnode);
			}
		}
		
		newnode=(node_t*)malloc(sizeof(*newnode));
		assert(newnode!=NULL);
		newscore=node->priority;
		copy_board(newboard, node->board);
		
		
	}
}


node_t
*new_node(node_t *newnode, node_t *node, uint8_t board[SIZE][SIZE], uint32_t score, int i){
	newnode->priority=score;
	newnode->depth=node->depth+1;
	if (node->move==stay){ 
		newnode->move=i;
	}else{
		newnode->move=node->move;
	}
	copy_board(newnode->board, board);
	newnode->parent=node;
	return newnode;
}

node_t
*int_node(node_t *node, uint8_t board[SIZE][SIZE], uint32_t score){
	node->priority=score;
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
	if (node->move!=4){
		if ((node->priority)>scrboard[node->move]){
			scrboard[node->move]=node->priority;
		}
	}
}

void
put_avg_scoreboard(node_t *node, uint32_t scrboardavg[SIZE], uint32_t avgboardnum[SIZE]){
	if (node->move!=4){
		scrboardavg[node->move]+=node->priority;
		avgboardnum[node->move]+=1;
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
			if (getTile(board1,i,j)!=getTile(board2,i,j)){
				return 0;
			}
		}
	}
	return 1;
}

int
decision_making(uint32_t scrboard[]){
	int i,move;
	uint32_t max=0;
	for (i=0; i<SIZE; i++){
		if (scrboard[i]>max){
			max=scrboard[i];
			move=i;
		}
	}
	
	return broke_tie(scrboard,max);
}

int
avg_decision_making(uint32_t scrboardavg[],uint32_t avgboardnum[]){
	int i;
	uint32_t avgboard[SIZE];
	
	for (i=0; i<SIZE; i++){
		avgboard[i]=(double)scrboardavg[i]/(double)avgboardnum[i];
	}
	return decision_making(avgboard);

}

int
broke_tie(uint32_t scrboard[],uint32_t max){
	int i,j=0;
	int moveboard[SIZE]={0};
	
	for (i=0; i<SIZE; i++){
		if (scrboard[i]==max){
			moveboard[j]=i;
			j++;
		}
	}
	return moveboard[rand()%j];
}

//debug function
void
scoreboard_print(uint32_t scrboard[]){
	int i;
	for(i=0; i<SIZE; i++){
		printf("%d: %d\n",i,scrboard[i]);
	}
}


uint32_t
max_tile(uint8_t board[SIZE][SIZE]){
	int i, j;
	uint32_t max=0;
	for (i=0; i<SIZE; i++){
		for(j=0; j<SIZE; j++){
			if (getTile(board,i,j)>max){
				max=getTile(board,i,j);
			}
		}
	}
	return max;

}
