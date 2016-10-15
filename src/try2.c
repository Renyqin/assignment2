#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef enum moves{
	left=0,
	right=1,
	up=2,
	down=3
} move_t;

struct node_s{
    uint32_t priority;
    int depth;
    int num_childs;
    move_t move;
    struct node_s* parent;
};

typedef struct node_s node_t;


void
int_scoreboard(node_t scrboard[4]){
	int i;
	for(i=0; i<4; i++){
		scrboard[i].priority=0;
		scrboard[i].move=4;
	}
}

typedef struct node_s node_t;

int
main(int argc, char *argv[]){
	clock_t start,end;
	int t;
	int i,j=0;
	start=time(NULL);//predefined  function in c
	for (i=0; i<100000000; i++){
		j++;
	}
	//after the user defined function does its work
	end=time(NULL);
	t=(end-start);
	
	printf("%d", t);
}


