/*
 * maze.h
 *
 *  Created on: Nov 13, 2017
 *      Author: jlammwe
 */

#ifndef MAZE_H_
#define MAZE_H_

#include "debug_stack.h"

// Please initialize all mazes with -1 for distance
typedef struct{
	int distance[15][15];
} dist_maze;

typedef struct{
	int north;
	int east;
	int south;
	int west;
} wall;

typedef struct{
	wall walls[15][15];
} wall_maze;

// Initialize distances to -1
void init_distance_maze(dist_maze* m);

// Initialize a distance array for the maze.
// Variable center used to tell the function if this is the center
void fill_in_distance_maze(dist_maze* m, coor* c, int center);





#endif /* MAZE_H_ */
