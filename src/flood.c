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
		for(int i=0; i<16; i++)
		{
			for(int j=0; j<16; j++)
			{
				// get the distance to target cell
				int x = i - c->x;
				if(x<0) x = -x;
				int y = j - c->y;
				if(y<0) y = -y;
				dm->distance[i][j] = x + y;
			}
		}
	}
}

// Initialize all cells to unvisited
void init_wall_maze(struct wall_maze* wm)
{
	for(int i=0; i<16 ; i++)
	{
		for(int j=0; j<16 ; j++)
		{
			wm->cells[i][j].walls[NORTH] = 0;
			wm->cells[i][j].walls[EAST] = 0;
			wm->cells[i][j].walls[SOUTH] = 0;
			wm->cells[i][j].walls[WEST] = 0;
			wm->cells[i][j].visited = 0;
		}
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

/* Parameters
 * dm = the distance array containing the distances to the spot we want to
 * flood to
 * x, y = the current coordinates
 * wm = structure containing information on the cell walls and visitation
 *
 */
void floodFill(struct dist_maze* dm, int x, int y, struct wall_maze* wm)
{
	// Disable tracking interrupts because we do not want to move yet
	lockInterruptDisable_TIM3();

	// store current coordinates into local variable
	struct coor c;
	init_coor(&c, x, y);
	// coordinate for future use in popping stack
	int next_move;
	struct coor next;
	// direction we are facing
	int direction = NORTH;
	// create stack for update of distances iteratively
	struct stack update_stack;
	update_stack.index = 0;

	// while we are not at target destination
	while(dm->distance[c.x][c.y]!=0)
	{
		// update the current cell as visited
		wm->cells[c.x][c.y].visited = 1;

		// check for surrounding walls
		switch(direction)
		{
			case NORTH: checkForWalls(wm, &c, NORTH, EAST, WEST);
			break;
			case EAST: checkForWalls(wm, &c, EAST, SOUTH, NORTH);
			break;
			case SOUTH: checkForWalls(wm, &c, SOUTH, WEST, EAST);
			break;
			case WEST: checkForWalls(wm, &c, WEST, NORTH, SOUTH);
			break;
		}

		// check if there is a neighbor with one less distance
		// next_move is the direction we should move next
		next_move = minusOneNeighbor(dm, wm, &c, &update_stack);

		// If we couldn't find a valid cell
		if(next_move == UNKNOWN)
		{
			// while stack is not empty
			while(update_stack.index!=0)
			{
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
				// get the cell to test from the stack
				next = pop_stack(&update_stack);
				// find a neighbor cell with distance one less than current
				minusOneNeighbor(dm, wm, &next, &update_stack);
			}

			// get next cell to traverse to
			// next_move is actually the direction we need to go next
			next_move = minusOneNeighbor(dm, wm, &c, &update_stack);
		}
		// Move to next cell
		// First turn to face the correct direction
		int difference = direction - next_move;
		switch(difference)
		{
		case -3:
			leftTurn();
			// Enable movement
			lockInterruptEnable_TIM3();
			// Move one cell forward
			advanceTicks(ENCODER_TICKS_ONE_CELL);
			// Disable interrupts
			lockInterruptDisable_TIM3();
			motorStop();
			break;
		case -2:
			backward180Turn();
			// Enable movement
			lockInterruptEnable_TIM3();
			// Move one cell forward
			advanceTicks(ENCODER_TICKS_ONE_CELL);
			// Disable interrupts
			lockInterruptDisable_TIM3();
			motorStop();
			break;
		case -1:
			rightTurn();
			// Enable movement
			lockInterruptEnable_TIM3();
			// Move one cell forward
			advanceTicks(ENCODER_TICKS_ONE_CELL);
			// Disable interrupts
			lockInterruptDisable_TIM3();
			motorStop();
			break;
		case 0:
			// Enable movement
			lockInterruptEnable_TIM3();
			// Move one cell forward
			advanceTicks(ENCODER_TICKS_ONE_CELL);
			// Disable interrupts
			lockInterruptDisable_TIM3();
			motorStop();
			break;
		case 1:
			leftTurn();
			// Enable movement
			lockInterruptEnable_TIM3();
			// Move one cell forward
			advanceTicks(ENCODER_TICKS_ONE_CELL);
			// Disable interrupts
			lockInterruptDisable_TIM3();
			motorStop();
			break;
		case 2:
			backward180Turn();
			// Enable movement
			lockInterruptEnable_TIM3();
			// Move one cell forward
			advanceTicks(ENCODER_TICKS_ONE_CELL);
			// Disable interrupts
			lockInterruptDisable_TIM3();
			motorStop();
			break;
		case 3:
			rightTurn();
			// Enable movement
			lockInterruptEnable_TIM3();
			// Move one cell forward
			advanceTicks(ENCODER_TICKS_ONE_CELL);
			// Disable interrupts
			lockInterruptDisable_TIM3();
			motorStop();
			break;
		}


		// update variables
		switch(next_move)
		{
			case NORTH: c.y += 1;
			break;
			case EAST: c.x += 1;
			break;
			case SOUTH: c.y -= 1;
			break;
			case WEST: c.x -= 1;
			break;
		}

		// update the direction we are currently facing
		direction = next_move;
	}
}

void checkForWalls(struct wall_maze* wm, struct coor* c, int n, int e, int w)
{
	// If there is a wall in front
	if(getLeftADCValue() > WALL_IN_FRONT_LEFT_SENSOR &&
			getRightADCValue() > WALL_IN_FRONT_RIGHT_SENSOR)
	{
		// Put wall to north
		wm->cells[c->x][c->y].walls[n] = 1;

		if(getLeftFrontADCValue() < WALL_IN_FRONT_OPENING_LEFT) wm->cells[c->x][c->y].walls[w] = 0;
		else wm->cells[c->x][c->y].walls[w] = 1;

		// check for wall to the east
		if(getRightFrontADCValue() < WALL_IN_FRONT_OPENING_RIGHT) wm->cells[c->x][c->y].walls[e] = 0;
		else wm->cells[c->x][c->y].walls[e] = 1;
	}
	else
	{
		// check for wall to the west
		if(getLeftFrontADCValue() < NO_LEFT_WALL) wm->cells[c->x][c->y].walls[w] = 0;
		else wm->cells[c->x][c->y].walls[w] = 1;

		// check for wall to the east
		if(getRightFrontADCValue() < NO_RIGHT_WALL) wm->cells[c->x][c->y].walls[e] = 0;
		else wm->cells[c->x][c->y].walls[e] = 1;

		// Put no wall to north
		wm->cells[c->x][c->y].walls[n] = 0;
	}
}

int minusOneNeighbor(struct dist_maze* dm, struct wall_maze* wm, struct coor* c, struct stack* s)
{
	int i;
	// minimum distance
	int md=260;
	// get target distance we're looking for
	int target = dm->distance[c->x][c->y] - 1;
	// check neighbor cells
	for(i=0; i<4; i++)
	{
		// If there is no wall blocking the way
		if(wm->cells[c->x][c->y].walls[i]==0)
		{
			switch(i)
			{
			case NORTH:
				if(dm->distance[c->x][c->y+1]==target)
				{
					// if the cell exists return the direction we want to move
					return i;
				}
				if(dm->distance[c->x][c->y+1] < md) md = dm->distance[c->x][c->y+1];
				break;
			case EAST:
				if(dm->distance[c->x+1][c->y]==target)
				{
					// if the cell exists return the direction we want to move
					return i;
				}
				if(dm->distance[c->x+1][c->y] < md) md = dm->distance[c->x+1][c->y];
				break;
			case SOUTH:
				if(dm->distance[c->x][c->y-1]==target)
				{
					// if the cell exists return the direction we want to move
					return i;
				}
				if(dm->distance[c->x][c->y-1] < md) md = dm->distance[c->x][c->y-1];
				break;
			case WEST:
				if(dm->distance[c->x-1][c->y]==target)
				{
					// if the cell exists return the direction we want to move
					return i;
				}
				if(dm->distance[c->x-1][c->y] < md) md = dm->distance[c->x-1][c->y];
				break;
			}
		}
	}

	// update distance of coordinate to 1 plus minimum distance
	dm->distance[c->x][c->y] = md + 1;

	// Since we did not find a cell we push onto the stack
	for(i=0; i<4; i++)
	{
		// If there is no wall blocking the way
		if(wm->cells[c->x][c->y].walls[i]==0)
		{
			struct coor temp;
			switch(i)
			{
				case NORTH:
					init_coor(&temp, c->x, c->y + 1);
					break;
				case EAST:
					init_coor(&temp, c->x + 1, c->y);
					break;
				case SOUTH:
					init_coor(&temp, c->x, c->y - 1);
					break;
				case WEST:
					init_coor(&temp, c->x - 1, c->y);
					break;
			}
			push_stack(s, temp);
		}
	}
	// return unknown
	return UNKNOWN;
}
