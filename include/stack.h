/*
 * stack.h
 *
 *  Created on: Nov 11, 2017
 *      Author: jlammwe
 */

#ifndef STACK_H_
#define STACK_H_

typedef struct{
	int x;
	int y;
} coor;

typedef struct{
	coor array[256];
	int index;
} stack;

void init_coor(coor* c, int x, int y);
coor pop_stack(stack* s);
void push_stack(stack* s, coor c);

#endif /* STACK_H_ */
