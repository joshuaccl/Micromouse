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

#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3
#define UNKNOWN 4
#define FLOOD_ONE_CELL 9375

// define structures needed for flood fill
struct cell_info{
	// variables for north,east,south,west walls
	bool walls[4];
	bool visited;
};

// struct to hold cell info
struct wall_maze{
	struct cell_info cells[16][16];
};

// struct to hold distance info
struct dist_maze{
	int distance[16][16];
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

// Initalize wall maze to unvisited for all cells
void init_wall_maze(struct wall_maze* wm);

// Initialize coordinate
void init_coor(struct coor* c, int x, int y);
struct coor pop_stack(struct stack* s);
void push_stack(struct stack* s, struct coor c);

void advanceTicksFlood(uint32_t ticks, int d, struct coor* c, struct wall_maze* wm);

// Called to flood to a target cell
void floodFill(struct dist_maze* dm, int x, int y, struct wall_maze* wm, int a);

// Used to check for walls in current cell
void checkForWalls(struct wall_maze* wm, struct coor* c, int e, int w);

// Check if a neighbor exists with distance one less than current
int minusOneNeighbor(struct dist_maze* dm, struct wall_maze* wm, struct coor* c, struct stack* s, int a);

void showCoor(int x, int y);

void turnOnCenterLEDS(void);
void turnOffCenterLEDS(void);

void advanceOneCell(int direction, struct coor* c, struct wall_maze* wm);
void advanceOneCellVisited(void);

#endif /* FLOOD_H_ */
