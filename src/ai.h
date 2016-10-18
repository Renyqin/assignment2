#ifndef __AI__
#define __AI__

#include <stdint.h>
#include <unistd.h>
#include "node.h"
#include "priority_queue.h"

#define DIFF 0.01


void initialize_ai();

move_t get_next_move( uint8_t board[SIZE][SIZE], int max_depth, propagation_t propagation, uint32_t score,uint32_t *generated, uint32_t *expanded );
void copy_board(uint8_t board1[SIZE][SIZE], uint8_t board2[SIZE][SIZE]);
node_t *int_node(node_t *node, uint8_t board[SIZE][SIZE],uint32_t score);
void four_direction(node_t *node, uint32_t scrboard[SIZE], uint32_t scrboardavg[SIZE],uint32_t avgboardnum[SIZE],uint32_t *generated);
node_t *new_node(node_t *newnode, node_t *node, uint8_t board[SIZE][SIZE], uint32_t score, int i);
void int_scoreboard(uint32_t scrboard[SIZE]);
void cmp_put_scoreboard(node_t *node, uint32_t scrboard[SIZE]);
int array_size(int max_depth);
void copy_board(uint8_t board1[SIZE][SIZE], uint8_t board2[SIZE][SIZE]);
int compare_board(uint8_t board1[SIZE][SIZE], uint8_t board2[SIZE][SIZE]);
int decision_making(uint32_t scrboard[]);
void put_avg_scoreboard(node_t *node, uint32_t scrboardavg[SIZE], uint32_t avgboardnum[SIZE]);
int avg_decision_making(uint32_t scrboardavg[],uint32_t avgboardnum[]);
int broke_tie(uint32_t scrboard[],uint32_t max);
uint32_t max_tile(uint8_t board[SIZE][SIZE]); 
int decision_making_double(double scrboard[]);
int broke_tie_double(double scrboard[],double max);

#endif
