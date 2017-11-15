/*
 * stack.c
 *
 *  Created on: Nov 11, 2017
 *      Author: jlammwe
 */
#include "debug_stack.h"


coor pop_stack(stack* s){
	s->index--;
	return s->array[s->index+1];

}
void push_stack(stack* s, coor c){
	s->index++;
	s->array[s->index] = c;
}

// Constructor for initializing coordinates
void init_coor(coor* c, int x, int y){
	c->x = x;
	c->y = y;
}



