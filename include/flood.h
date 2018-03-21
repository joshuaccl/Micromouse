/*
 * flood.h
 *
 *  Created on: Mar 20, 2018
 *      Author: jlammwe
 */

#ifndef FLOOD_H_
#define FLOOD_H_

#include "stm32f4xx_hal.h"
#include "motors.h"
#include "adc.h"
#include "pdT.h"
#include "pdV.h"
#include "encoder.h"
#include "lock.h"
#include "stdbool.h"

// define structures needed for flood fill
struct cell_info{
	// variables for north,east,south,west walls
	bool n;
	bool e;
	bool s;
	bool w;
	bool visited;
};

// struct to hold cell info
struct wall_maze{
	struct cell_info cell[15][15];
};

// struct to hold distance info
struct dist_maze{
	int distance[15][15];
};

// struct to hold coordinates
struct coor{
	int x;
	int y;
};

// stack for iterative floodfill
struct stack{
	struct coor array[256];
	int index;
};

// Used to initialize distance array
// dm is pointer to distance maze to fill out
// c is coordinate which we want to flood to
// center is flag to let us know if we are trying to get to the center
void init_distance_maze(struct dist_maze* dm, struct coor* c, int center);

// Initialize coordinate
void init_coor(struct coor* c, int x, int y);
struct coor pop_stack(struct stack* s);
void push_stack(struct stack* s, struct coor c);

void floodFill(void);
#endif /* FLOOD_H_ */
