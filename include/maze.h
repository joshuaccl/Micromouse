/*
 * maze.h
 *
 *  Created on: Nov 13, 2017
 *      Author: jlammwe
 */

#ifndef MAZE_H_
#define MAZE_H_

#include "stack.h"

// Please initialize all mazes with -1 for distance
struct dist_maze{
	int distance[15][15];
};

struct wall{
	int north;
	int east;
	int south;
	int west;
};

// Initialize distances to -1
void init_distance_maze(struct dist_maze* m);

// Initialize a distance array for the maze.
// Variable center used to tell the function if this is the center
void fill_in_distance_maze(struct dist_maze* m, struct coor* c, int center);

#endif /* MAZE_H_ */
