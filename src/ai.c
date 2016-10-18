/*
============================================================
author: Chenqin Zhang
time: 18/10/2016
description: assignment2 for algorithms and data structure
============================================================
*/

#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "ai.h"
#include "utils.h"
#include "priority_queue.h"



struct heap h;



void initialize_ai(){
	heap_init(&h);
}

/**
 * Find best action by building all possible paths up to depth max_depth
 * and back propagate using either max or avg
 */
 

move_t get_next_move( uint8_t board[SIZE][SIZE], int max_depth,
	propagation_t propagation, uint32_t score, uint32_t *generated, 
	uint32_t *expanded){

	node_t *node;
	move_t best_action; 
	
	/*scrboard[SIZE] is the array use for max strategy,which 
	saves the max score for all the childs in each direction.*/
	uint32_t scrboard[SIZE];
	
	/*scrboardavg[SIZE] and avgboardnum[SIZE] is the array use for avg
	strategy, which saves the sum of all the childrens and number of childrens
	in each direction*/
	uint32_t scrboardavg[SIZE];
	uint32_t avgboardnum[SIZE];

	/*when max_depth equals 0, the algorithms wouldn't look further nodes
	therefore, next move chose by randomly would be resonable*/
	if (max_depth==0){
		return rand()%SIZE;
	}
	
	
	/*initialize all the board been declared in this function*/
	int_scoreboard(scrboard);
	int_scoreboard(scrboardavg);
	int_scoreboard(avgboardnum);
	
	
	
	node=(node_t*)malloc(sizeof(*node));
	assert(node!=NULL);
	
	/*initial fist node and put in priority queue*/
	node=int_node(node, board,score);
	heap_push(&h, node);  
	
	
	
	/*if count bigger than 0, the heap not empty*/
	while (h.count>0){  

		/*pop up a node, and delete it in priority queue*/
		node=h.heaparr[0];
		/*record how many node been pop in priority queue*/
		(*expanded)+=1;
		heap_delete(&h); 
		
		if (node->depth<max_depth){
			four_direction(node, scrboard, scrboardavg, avgboardnum, generated);
		}
		
	}
	free(node);
	
	/*two different strategy*/
	if (propagation==max){
		best_action=decision_making(scrboard);
	}else{
		best_action=avg_decision_making(scrboardavg,avgboardnum);
	}

	return best_action;
}


/*simulate further operation and put child nodes
of four different direction to priority queue*/
void
four_direction(node_t *node, uint32_t scrboard[SIZE], 
	uint32_t scrboardavg[SIZE],uint32_t avgboardnum[SIZE], uint32_t *generated){

	int i;
	uint32_t newscore=node->priority;
	node_t *oldnode;
	uint8_t newboard[SIZE][SIZE];
	node_t *newnode;
	newnode=(node_t*)malloc(sizeof(*newnode));
	assert(newnode!=NULL);
	copy_board(newboard, node->board);
	
	
	for (i=0; i<SIZE; i++){
		/*four direction have different operation*/
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
		
		/*create a new node*/
		newnode=new_node(newnode, node, newboard, newscore, i);
		/*record how many node been created*/
		(*generated)+=1;
		
		
		if (!compare_board(newnode->board, node->board)){
			/*generate the boards for each algorithms
			and put a new node in priority queue*/
			
			cmp_put_scoreboard(newnode, scrboard);
			put_avg_scoreboard(newnode, scrboardavg, avgboardnum);
			heap_push(&h, newnode);
			
		/*if the board are same, game over for this node,
			this node should be free*/
		}else{
			oldnode=(node_t*)malloc(sizeof(*oldnode));
			assert(oldnode!=NULL);
			if (newnode!=NULL){
				oldnode=newnode;
				free(oldnode);
			}
		}
		
		/*initialize newnode, newsocore and board for new node*/
		newnode=(node_t*)malloc(sizeof(*newnode));
		assert(newnode!=NULL);
		newscore=node->priority;
		copy_board(newboard, node->board);
		
		
	}
}

/*creating new node*/
node_t
*new_node(node_t *newnode, node_t *node, uint8_t board[SIZE][SIZE],
	uint32_t score, int i){

	newnode->priority=score;
	newnode->depth=node->depth+1;
	/*if it is the first node, the move would be stay*/
	if (node->move==stay){ 
		newnode->move=i;
	}else{
		newnode->move=node->move;
	}
	copy_board(newnode->board, board);
	newnode->parent=node;
	return newnode;
}


/*initialize node*/
node_t
*int_node(node_t *node, uint8_t board[SIZE][SIZE], uint32_t score){
	node->priority=score;
	node->depth=0;
	node->move=stay;
	copy_board(node->board, board);
	node->parent=NULL;
	return node;
}

/*initialize board*/
void
int_scoreboard(uint32_t scrboard[SIZE]){
	int i;
	for(i=0; i<SIZE; i++){
		scrboard[i]=0;
	}
}

/*find the node which have bigger priority and put in scoreboard*/
void
cmp_put_scoreboard(node_t *node, uint32_t scrboard[SIZE]){
	
	if (node->move!=4){
		if ((node->priority)>scrboard[node->move]){
			scrboard[node->move]=node->priority;
		}
	}
}

/*handle avg strategy*/
void
put_avg_scoreboard(node_t *node, uint32_t scrboardavg[SIZE],
	uint32_t avgboardnum[SIZE]){
	if (node->move!=4){
		scrboardavg[node->move]+=node->priority;
		avgboardnum[node->move]+=1;
	}
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

/*decision making for max strategy*/
int
decision_making(uint32_t scrboard[]){
	int i;
	uint32_t max=0;
	for (i=0; i<SIZE; i++){
		if (scrboard[i]>max){
			max=scrboard[i];
			
		}
	}
	
	return broke_tie(scrboard,max);
}

/*the avg board was declared as double, the function fo compare the
element in board as type double*/
int
decision_making_double(double scrboard[]){
	int i,move;
	double max=0;
	for (i=0; i<SIZE; i++){
		/*safety compare for double*/
		if (scrboard[i]-max>DIFF){
			max=scrboard[i];
			move=i;
		}
	}
	
	return broke_tie_double(scrboard,max);
}

/*decision making for avg strategy*/
int
avg_decision_making(uint32_t scrboardavg[],uint32_t avgboardnum[]){
	int i;
	double avgboard[SIZE];
	
	for (i=0; i<SIZE; i++){
		avgboard[i]=(double)scrboardavg[i]/avgboardnum[i];
	}
	return decision_making_double(avgboard);

}

/*when more then two elements in score board have same 
	numbers broke the tie randomly*/
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

/*broke the tie function, but for double elements*/
int
broke_tie_double(double scrboard[],double max){
	int i,j=0;
	int moveboard[SIZE]={0};
	
	for (i=0; i<SIZE; i++){
		/*compare two double values are equal or not*/
		if ((scrboard[i]-max)>-(DIFF/2)&&(scrboard[i]-max)<(DIFF/2)){
			moveboard[j]=i;
			j++;
		}
	}
	
	return moveboard[rand()%j];
}


/*find the max tile in a board*/
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
