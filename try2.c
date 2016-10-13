#include <stdio.h>
#include <stdlib.h>

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
	int i;
	for (i=0; i<0; i++){
		printf("yes, I'm excutable");
	}

}


