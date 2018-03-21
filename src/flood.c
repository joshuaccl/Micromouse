/*
 * flood.c
 *
 *  Created on: Mar 20, 2018
 *      Author: jlammwe
 */

#include "flood.h"

void init_distance_maze(struct dist_maze* dm, struct coor* c, int center)
{
	// If we are trying to get to the center
	if(center == 1)
	{
		for(int i=0; i<16; i++)
		{
			for(int j=0; j<16; j++)
			{
				if(i<=7 && j<=7) dm->distance[i][j] = ((7-i) + (7-j));
				if(i<=7 && j>7) dm->distance[i][j] = ((7-i) + (j-8));
				if(i>7 && j<=7) dm->distance[i][j] = ((i-8) + (7-j));
				if(i>7 && j>7) dm->distance[i][j] = ((i-8) + (j-8));
			}
		}
	}
	// Generalized floodfill
	else
	{

	}
}

// Constructor for initializing coordinates
void init_coor(struct coor* c, int x, int y){
	c->x = x;
	c->y = y;
}

// Pop top of stack
struct coor pop_stack(struct stack* s){
	return s->array[s->index--];
}

// Push to top of stack
void push_stack(struct stack* s, struct coor c){
	s->array[++s->index] = c;
}

