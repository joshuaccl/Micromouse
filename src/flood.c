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
	// Initialize the distance maze with the center four cells as zero
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
	// Generalized floodfill for any other cell
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
			// put the walls surrounding the maze
			if(i==0) wm->cells[i][j].walls[WEST] = 1;
			if(j==0) wm->cells[i][j].walls[SOUTH] = 1;
			if(i==15) wm->cells[i][j].walls[EAST] = 1;
			if(j==15) wm->cells[i][j].walls[NORTH] = 1;
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
	s->index = s->index -1;
	return s->array[s->index + 1];
}

// Push to top of stack
void push_stack(struct stack* s, struct coor c){
	s->index = s->index +1;
	s->array[s->index] = c;
}


// move one cell forward and also poll adc for wall values
void advanceTicksFlood(uint32_t ticks, int d, struct coor* c, struct wall_maze* wm) {
	uint32_t encoder_val = MAX_ENCODER_VALUE;
	resetLeftEncoder();
	// put walls up for the cell we are moving to. For each direction put a wall to the
	// east and the west.
	switch(d)
	{
	case NORTH:
		wm->cells[c->x][c->y].walls[WEST] = 1;
		wm->cells[c->x][c->y].walls[EAST] = 1;
		// checks for out of bounds
		if(c->x > 0) wm->cells[c->x-1][c->y].walls[EAST] = 1;
		if(c->x < 15) wm->cells[c->x+1][c->y].walls[WEST] = 1;
		break;
	case EAST:
		wm->cells[c->x][c->y].walls[NORTH] = 1;
		wm->cells[c->x][c->y].walls[SOUTH] = 1;
		if(c->y > 0) wm->cells[c->x][c->y-1].walls[NORTH] = 1;
		if(c->y < 15) wm->cells[c->x][c->y+1].walls[SOUTH] = 1;
		break;
	case SOUTH:
		wm->cells[c->x][c->y].walls[WEST] = 1;
		wm->cells[c->x][c->y].walls[EAST] = 1;
		if(c->x > 0) wm->cells[c->x-1][c->y].walls[EAST] = 1;
		if(c->x < 15) wm->cells[c->x+1][c->y].walls[WEST] = 1;
		break;
	case WEST:
		wm->cells[c->x][c->y].walls[NORTH] = 1;
		wm->cells[c->x][c->y].walls[SOUTH] = 1;
		if(c->y > 0) wm->cells[c->x][c->y-1].walls[NORTH] = 1;
		if(c->y < 15) wm->cells[c->x][c->y+1].walls[SOUTH] = 1;
		break;
	default:
		break;
	}
	// while we have not finished moving one cell length
	while(encoder_val > (MAX_ENCODER_VALUE - ticks) ) {
		// if we have moved half of a cell length
		if (encoder_val < (MAX_ENCODER_VALUE - (ticks / 2 ) ) )
		{
			// check for walls to the east and the west
			switch(d)
			{
			case NORTH: checkForWalls(wm, c, d, NORTH, EAST, SOUTH, WEST);
			break;
			case EAST: checkForWalls(wm, c, d, EAST, SOUTH, WEST, NORTH);
			break;
			case SOUTH: checkForWalls(wm, c, d, SOUTH, WEST, NORTH, EAST);
			break;
			case WEST: checkForWalls(wm, c, d, WEST, NORTH, EAST, SOUTH);
			break;
			default:
				break;
			}
		}
		setLeftEncoderValue(TIM2->CNT);
		encoder_val = getLeftEncoderValue();
	}
	resetLeftEncoder();
}

/* Parameters
 * dm = the distance array containing the distances to the spot we want to
 * flood to
 * x, y = the current coordinates
 * wm = structure containing information on the cell walls and visitation
 *
 */
int floodFill(struct dist_maze* dm, struct coor* c, struct wall_maze* wm, int a, int direction, struct stack* upst)
{
	// Disable tracking interrupts because we do not want to move yet
	lockInterruptDisable_TIM3();

	// coordinate for future use in popping stack
	int next_move;
	struct coor next;

	// set the base speed of traversal
	setBaseSpeed(40);

	// while we are not at target destination
	while(1)
	{
		// update coordinates for next cell we are going to visit
		switch(direction)
		{
		case NORTH: c->y += 1;
		break;
		case EAST: c->x += 1;
		break;
		case SOUTH: c->y -= 1;
		break;
		case WEST: c->x -= 1;
		break;
		default:
			break;
		}

		// If we haven't visited the next cell
		if(wm->cells[c->x][c->y].visited == 0)
		{
			// advance one cell
			advanceOneCell(direction, c, wm);

			// check for wall straight ahead
			if(getLeftADCValue() >= FLOOD_WALL_IN_FRONT_LEFT &&
					getRightADCValue() >= FLOOD_WALL_IN_FRONT_RIGHT)
			{
				// put wall ahead of us
				wm->cells[c->x][c->y].walls[direction] = 1;
				switch(direction)
				{
				// put wall ahead of us in the cell ahead of us
				case NORTH:
					if(c->y+1 < 16) wm->cells[c->x][c->y+1].walls[SOUTH] = 1;
					break;
				case EAST:
					if(c->x+1 < 16) wm->cells[c->x+1][c->y].walls[WEST] = 1;
					break;
				case SOUTH:
					if(c->y-1 > -1) wm->cells[c->x][c->y-1].walls[NORTH] = 1;
					break;
				case WEST:
					if(c->x-1 > -1) wm->cells[c->x-1][c->y].walls[EAST] = 1;
					break;
				}
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
			}
			else HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
			// update the current cell as visited
			wm->cells[c->x][c->y].visited = 1;

		}
		else
		{
			// advance one cell without scanning for walls
			advanceOneCellVisited();
		}

		// if we are at target destination
		if (dm->distance[c->x][c->y]==0)
		{
			lockInterruptDisable_TIM3();
			return direction;
			break;
		}

		// check if there is a neighbor with one less distance
		// next_move is the direction we should move next
		next_move = minusOneNeighbor(dm, wm, c, upst, a);

		// If we couldn't find a valid cell
		if(next_move == UNKNOWN)
		{
			// while stack is not empty
			lockInterruptDisable_Gyro_Delay();
			while(upst->index!=0)
			{
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
				// get the cell to test from the stack
				next = pop_stack(upst);
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET);
				// find a neighbor cell with distance one less than current
				minusOneNeighbor(dm, wm, &next, upst, a);
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
			}
			// get next cell to traverse to
			// next_move is actually the direction we need to go next
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);
			next_move = minusOneNeighbor(dm, wm, c, upst, a);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET);
		}
		lockInterruptEnable_Gyro_Delay();
		// Move to next cell
		// First turn to face the correct direction
		int difference = direction - next_move;
		switch(difference)
		{
		case -3:
			leftStillTurn();
			// calibration because left turn backs up mouse a little bit
			uncontrolledAdvanceTicks(450);
			break;
		case -2:
			backward180StillTurn();
			// calibration by backing into wall behind us
			if(wm->cells[c->x][c->y].walls[direction]==1)
			{
				lockInterruptDisable_TIM3();
				leftMotorPWMChangeBackward(200);
				rightMotorPWMChangeBackward(200);
				custom_delay(2000);
				motorStop();
				lockInterruptEnable_TIM3();
				uncontrolledAdvanceTicks(3000);
			}
			break;
		case -1:
			rightStillTurn();
			uncontrolledAdvanceTicks(450);
			break;
		case 0:
			break;
		case 1:
			leftStillTurn();
			uncontrolledAdvanceTicks(450);
			break;
		case 2:
			backward180StillTurn();
			if(wm->cells[c->x][c->y].walls[direction]==1)
			{
				lockInterruptDisable_TIM3();
				leftMotorPWMChangeBackward(200);
				rightMotorPWMChangeBackward(200);
				custom_delay(2000);
				motorStop();
				lockInterruptEnable_TIM3();
				uncontrolledAdvanceTicks(3000);
			}
			break;
		case 3:
			rightStillTurn();
			uncontrolledAdvanceTicks(450);
			break;
		default:
			turnOnLEDS();
			break;
		}

		// update the direction we are currently facing
		direction = next_move;
	}
}

void checkForWalls(struct wall_maze* wm, struct coor* c, int direction, int n, int e, int s, int w)
{
	// if there is a wall in memory
	if(wm->cells[c->x][c->y].walls[w]==1)
	{
		// check for wall to the west
		// if at anytime the value drops below that means there is no wall
		if(getLeftFrontADCValue() < FLOOD_LEFT_WALL )
		{
			wm->cells[c->x][c->y].walls[w] = 0;
			switch(direction)
			{
			// also put walls in the other adjacent cells
			case NORTH:
				if(c->x-1 > -1) wm->cells[c->x-1][c->y].walls[EAST] = 0;
				break;
			case EAST:
				if(c->y+1 < 16) wm->cells[c->x][c->y+1].walls[SOUTH] = 0;
				break;
			case SOUTH:
				if(c->x+1 < 16)wm->cells[c->x+1][c->y].walls[WEST] = 0;
				break;
			case WEST:
				if(c->y-1 > -1) wm->cells[c->x][c->y-1].walls[NORTH] = 0;
				break;
			}
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET);
		}
		else HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET);
	}
	if(wm->cells[c->x][c->y].walls[e]==1)
	{
		// check for wall to the east
		if(getRightFrontADCValue() < FLOOD_RIGHT_WALL)
		{
			wm->cells[c->x][c->y].walls[e] = 0;
			switch(direction)
			{
			case NORTH:
				if(c->x+1 < 16)wm->cells[c->x+1][c->y].walls[WEST] = 0;
				break;
			case EAST:
				if(c->y-1 > -1) wm->cells[c->x][c->y-1].walls[NORTH] = 0;
				break;
			case SOUTH:
				if(c->x-1 > -1) wm->cells[c->x-1][c->y].walls[EAST] = 0;
				break;
			case WEST:
				if(c->y+1 < 16) wm->cells[c->x][c->y+1].walls[SOUTH] = 0;
				break;
			}
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET);
		}
		else HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_SET);
	}
}

int minusOneNeighbor(struct dist_maze* dm, struct wall_maze* wm, struct coor* c, struct stack* s, int a)
{
	int i;
	// minimum distance
	int md=260;
	// get target distance we're looking for
	int target = dm->distance[c->x][c->y] - 1;
	// check neighbor cells
	for(i=0; i<4; i++)
	{
		// choice of direction preference
		int j = (i + a) % 4;
		// If there is no wall blocking the way
		if(wm->cells[c->x][c->y].walls[j]==0)
		{
			switch(j)
			{
			case NORTH:
				if(dm->distance[c->x][c->y+1]==target)
				{
					// if the cell exists return the direction we want to move
					return j;
				}
				if(dm->distance[c->x][c->y+1] < md) md = dm->distance[c->x][c->y+1];
				break;
			case EAST:
				if(dm->distance[c->x+1][c->y]==target)
				{
					// if the cell exists return the direction we want to move
					return j;
				}
				if(dm->distance[c->x+1][c->y] < md) md = dm->distance[c->x+1][c->y];
				break;
			case SOUTH:
				if(dm->distance[c->x][c->y-1]==target)
				{
					// if the cell exists return the direction we want to move
					return j;
				}
				if(dm->distance[c->x][c->y-1] < md) md = dm->distance[c->x][c->y-1];
				break;
			case WEST:
				if(dm->distance[c->x-1][c->y]==target)
				{
					// if the cell exists return the direction we want to move
					return j;
				}
				if(dm->distance[c->x-1][c->y] < md) md = dm->distance[c->x-1][c->y];
				break;
			default:

				break;
			}
		}
	}

	// update distance of coordinate to 1 plus minimum distance
	dm->distance[c->x][c->y] = md + 1;

	// Since we did not find a cell we push onto the stack
	for(i=0; i<4; i++)
	{
		// choice of direction preference
		int j = (i + a) % 4;
		// If there is no wall blocking the way
		if(wm->cells[c->x][c->y].walls[j]==0)
		{
			struct coor temp;
			switch(j)
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
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
			push_stack(s, temp);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
		}
	}
	// return unknown
	return UNKNOWN;
}

// Show the coordinates
void showCoor(int x, int y)
{
	// initialize coordinates binary representation
	int binaryx[4];
	int binaryy[4];
	int xcoor = x;
	int ycoor = y;

	// get the binary values
	for(int i = 3; i>-1; i--)
	{
		if(xcoor - (1 << i) >= 0)
		{
			binaryx[i] = 1;
			xcoor -= (1 << i);
		}
		else binaryx[i] = 0;

		if(ycoor - (1 << i) >= 0)
		{
			binaryy[i] = 1;
			ycoor -= (1 << i);
		}
		else binaryy[i] = 0;
	}

	// blink leds to show binary values
	turnOffCenterLEDS();
	if(binaryx[3]) HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);
	if(binaryx[2]) HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);
	if(binaryx[1]) HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
	if(binaryx[0]) HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
	HAL_Delay(500);
	turnOffCenterLEDS();
	if(binaryy[3]) HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);
	if(binaryy[2]) HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);
	if(binaryy[1]) HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
	if(binaryy[0]) HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
	HAL_Delay(500);
	turnOffCenterLEDS();
}

void turnOnCenterLEDS()
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
}
void turnOffCenterLEDS()
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
}

void advanceOneCell(int direction, struct coor* c, struct wall_maze* wm)
{
	// go forward one cell
	lockInterruptEnable_TIM3();
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
	advanceTicksFlood(FLOOD_ONE_CELL, direction, c, wm);
	lockInterruptDisable_TIM3();
	motorStop();
	custom_delay(500);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
	resetLeftEncoder();
}

void advanceOneCellVisited()
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
	// Go forward one cell
	lockInterruptEnable_TIM3();
	uncontrolledAdvanceTicks(FLOOD_ONE_CELL);
	lockInterruptDisable_TIM3();
	motorStop();
	custom_delay(500);
	// showCoor(c.x, c.y);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
	resetLeftEncoder();
}

int centerMovement(struct wall_maze* wm, struct coor* c, int d)
{
	// 1 for turn right. 0 for turn left
	int turn = 0;
	//	switch(d)
	//	{
	//	case NORTH:
	//		if(c->x==7 && c->y==7) turn = 1;
	//		break;
	//	case EAST:
	//		if(c->x==7 && c->y==8) turn = 1;
	//		break;
	//	case SOUTH:
	//		if(c->x==8 && c->y==8) turn = 1;
	//		break;
	//	case WEST:
	//		if(c->x==8 && c->y==7) turn = 1;
	//		break;
	//	}
	//
	//	for(int i=0 ; i<4 ; i++)
	//	{
	//		switch(d)
	//		{
	//		case NORTH: c->y += 1;
	//		break;
	//		case EAST: c->x += 1;
	//		break;
	//		case SOUTH: c->y -= 1;
	//		break;
	//		case WEST: c->x -= 1;
	//		break;
	//		default:
	//			break;
	//		}
	//
	//		advanceOneCell(d, c, wm);
	//		// showCoor(c.x, c.y);
	//
	//		// check for wall straight ahead
	//		if(getLeftADCValue() >= FLOOD_WALL_IN_FRONT_LEFT &&
	//				getRightADCValue() >= FLOOD_WALL_IN_FRONT_RIGHT)
	//		{
	//			wm->cells[c->x][c->y].walls[d] = 1;
	//			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
	//		}
	//		else HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
	//		// update the current cell as visited
	//		wm->cells[c->x][c->y].visited = 1;
	//
	//		if(turn)
	//		{
	//			rightStillTurn();
	//			switch(d)
	//			{
	//			case NORTH: d = EAST;
	//			break;
	//			case EAST: d = SOUTH;
	//			break;
	//			case SOUTH: d = WEST;
	//			break;
	//			case WEST: d = NORTH;
	//			break;
	//			default:
	//				break;
	//			}
	//		}
	//		else
	//		{
	//			leftStillTurn();
	//			switch(d)
	//			{
	//			case NORTH: d = WEST;
	//			break;
	//			case EAST: d = NORTH;
	//			break;
	//			case SOUTH: d = EAST;
	//			break;
	//			case WEST: d = SOUTH;
	//			break;
	//			default:
	//				break;
	//			}
	//		}
	//	}
	//	return d;
	int direction;
	// put down all the walls in the center depending on which cell we entered
	// and also the direction we are currently facing
	switch(d)
	{
	case NORTH:
		if(c->x==7 && c->y==7)
		{
			wm->cells[c->x+1][c->y].walls[SOUTH] = 1;
			wm->cells[c->x+1][c->y].walls[EAST] = 1;
			wm->cells[c->x+1][c->y-1].walls[NORTH] = 1;
			wm->cells[c->x+2][c->y].walls[WEST] = 1;
			wm->cells[c->x][c->y+1].walls[NORTH] = 1;
			wm->cells[c->x][c->y+1].walls[WEST] = 1;
			wm->cells[c->x][c->y+2].walls[SOUTH] = 1;
			wm->cells[c->x-1][c->y+1].walls[EAST] = 1;
			wm->cells[c->x+1][c->y+1].walls[NORTH] = 1;
			wm->cells[c->x+1][c->y+1].walls[EAST] = 1;
			wm->cells[c->x+1][c->y+2].walls[SOUTH] = 1;
			wm->cells[c->x+2][c->y+1].walls[WEST] = 1;
		}
		else
		{
			wm->cells[c->x-1][c->y].walls[SOUTH] = 1;
			wm->cells[c->x-1][c->y].walls[WEST] = 1;
			wm->cells[c->x][c->y+1].walls[NORTH] = 1;
			wm->cells[c->x][c->y+1].walls[EAST] = 1;
			wm->cells[c->x-1][c->y+1].walls[NORTH] = 1;
			wm->cells[c->x-1][c->y+1].walls[WEST] = 1;
		}
		direction = SOUTH;
		break;
	case EAST:
		if(c->x==7 && c->y==7)
		{
			wm->cells[c->x+1][c->y].walls[SOUTH] = 1;
			wm->cells[c->x+1][c->y].walls[EAST] = 1;
			wm->cells[c->x-1][c->y+1].walls[EAST] = 1;
			wm->cells[c->x][c->y+2].walls[SOUTH] = 1;
			wm->cells[c->x][c->y+1].walls[NORTH] = 1;
			wm->cells[c->x][c->y+1].walls[WEST] = 1;
			wm->cells[c->x+1][c->y-1].walls[NORTH] = 1;
			wm->cells[c->x+2][c->y].walls[EAST] = 1;
			wm->cells[c->x+1][c->y+1].walls[NORTH] = 1;
			wm->cells[c->x+1][c->y+1].walls[EAST] = 1;
			wm->cells[c->x+1][c->y+2].walls[SOUTH] = 1;
			wm->cells[c->x+2][c->y+1].walls[EAST] = 1;
		}
		else
		{
			wm->cells[c->x+1][c->y].walls[NORTH] = 1;
			wm->cells[c->x+1][c->y].walls[EAST] = 1;
			wm->cells[c->x][c->y-1].walls[SOUTH] = 1;
			wm->cells[c->x][c->y-1].walls[WEST] = 1;
			wm->cells[c->x+1][c->y-1].walls[SOUTH] = 1;
			wm->cells[c->x+1][c->y-1].walls[EAST] = 1;
		}
		direction = WEST;
		break;
	case SOUTH:
		if(c->x==7 && c->y==8)
		{
			wm->cells[c->x+1][c->y].walls[NORTH] = 1;
			wm->cells[c->x+1][c->y].walls[EAST] = 1;
			wm->cells[c->x][c->y-1].walls[SOUTH] = 1;
			wm->cells[c->x][c->y-1].walls[WEST] = 1;
			wm->cells[c->x+1][c->y-1].walls[SOUTH] = 1;
			wm->cells[c->x+1][c->y-1].walls[EAST] = 1;
		}
		else
		{
			wm->cells[c->x-1][c->y].walls[NORTH] = 1;
			wm->cells[c->x-1][c->y].walls[WEST] = 1;
			wm->cells[c->x][c->y-1].walls[SOUTH] = 1;
			wm->cells[c->x][c->y-1].walls[EAST] = 1;
			wm->cells[c->x-1][c->y-1].walls[SOUTH] = 1;
			wm->cells[c->x-1][c->y-1].walls[WEST] = 1;
		}
		direction = NORTH;
		break;
	case WEST:
		if(c->x==8 && c->y==8)
		{
			wm->cells[c->x-1][c->y].walls[NORTH] = 1;
			wm->cells[c->x-1][c->y].walls[WEST] = 1;
			wm->cells[c->x][c->y-1].walls[SOUTH] = 1;
			wm->cells[c->x][c->y-1].walls[EAST] = 1;
			wm->cells[c->x-1][c->y-1].walls[SOUTH] = 1;
			wm->cells[c->x-1][c->y-1].walls[WEST] = 1;
		}
		else
		{
			wm->cells[c->x-1][c->y].walls[SOUTH] = 1;
			wm->cells[c->x-1][c->y].walls[WEST] = 1;
			wm->cells[c->x][c->y+1].walls[NORTH] = 1;
			wm->cells[c->x][c->y+1].walls[EAST] = 1;
			wm->cells[c->x-1][c->y+1].walls[NORTH] = 1;
			wm->cells[c->x-1][c->y+1].walls[WEST] = 1;
		}
		direction = EAST;
		break;
	}
	// all center cells are visited
	wm->cells[7][7].visited=1;
	wm->cells[7][8].visited=1;
	wm->cells[8][7].visited=1;
	wm->cells[8][8].visited=1;
	// recalibrate on center wall, then drive out
	advanceOneCellVisited();
	backward180StillTurn();
	lockInterruptDisable_TIM3();
	leftMotorPWMChangeBackward(200);
	rightMotorPWMChangeBackward(200);
	custom_delay(2000);
	motorStop();
	lockInterruptEnable_TIM3();
	uncontrolledAdvanceTicks(3000);
	advanceOneCellVisited();
	return direction;
}

int logicalFlood(struct dist_maze* dm, struct coor* c, struct wall_maze* wm, int a, int direction, struct stack* upst)
{
	// Disable tracking interrupts because we do not want to move yet
	lockInterruptDisable_TIM3();
	int next_move;
	struct coor next;
	int x = c->x;
	int y = c->y;
	// while we are not at target destination
	while(1)
	{
		// update coordinates for next cell we are going to visit
		switch(direction)
		{
		case NORTH: c->y += 1;
		break;
		case EAST: c->x += 1;
		break;
		case SOUTH: c->y -= 1;
		break;
		case WEST: c->x -= 1;
		break;
		default:
			break;
		}

		// return if we have reached our target
		if (dm->distance[c->x][c->y]==0)
		{
			lockInterruptDisable_TIM3();
			// reassign the previous coordinates before logical flood
			c->x = x;
			c->y = y;
			return direction;
			break;
		}

		// check if there is a neighbor with one less distance
		// next_move is the direction we should move next
		next_move = minusOneNeighbor(dm, wm, c, upst, a);

		// If we couldn't find a valid cell
		if(next_move == UNKNOWN)
		{
			// while stack is not empty
			lockInterruptDisable_Gyro_Delay();
			while(upst->index!=0)
			{
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
				// get the cell to test from the stack
				next = pop_stack(upst);
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET);
				// find a neighbor cell with distance one less than current
				minusOneNeighbor(dm, wm, &next, upst, a);
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
			}
			// get next cell to traverse to
			// next_move is actually the direction we need to go next
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);
			next_move = minusOneNeighbor(dm, wm, c, upst, a);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET);
		}
		lockInterruptEnable_Gyro_Delay();

		// update the direction we are currently facing
		direction = next_move;
	}
}

void shortestPath(struct dist_maze* dm, struct coor* c, struct wall_maze* wm, int a, int direction, struct stack* upst)
{
	// Disable tracking interrupts because we do not want to move yet
	lockInterruptDisable_TIM3();

	// coordinate for future use in popping stack
	int next_move;
	int d;
	d = direction;

	// set the base traversal speed
	setBaseSpeed(40);

	// while not at target destination
	while(1)
	{
		// update coordinates
		switch(d)
		{
		case NORTH: c->y += 1;
		break;
		case EAST: c->x += 1;
		break;
		case SOUTH: c->y -= 1;
		break;
		case WEST: c->x -= 1;
		break;
		default:
			break;
		}
		// move forward one cell
		advanceOneCellVisited();

		// if we are at the center exit function
		if (c->x==7 && c->y==7)
		{
			lockInterruptDisable_TIM3();
			break;
		}
		if (c->x==7 && c->y==8)
		{
			lockInterruptDisable_TIM3();
			break;
		}
		if (c->x==8 && c->y==7)
		{
			lockInterruptDisable_TIM3();
			break;
		}
		if (c->x==8 && c->y==8)
		{
			lockInterruptDisable_TIM3();
			break;
		}

		// find the target which is the distance of current cell plus one
		int target = dm->distance[c->x][c->y] + 1;

		// check neighbor cells
		for(int i=0; i<4; i++)
		{
			switch(i)
			{
			case NORTH:
				if(c->y+1<16)
				{
					if(dm->distance[c->x][c->y+1]==target)
					{
						// if the cell exists return the direction we want to move
						next_move = i;
					}
				}
				break;
			case EAST:
				if(c->x+1<16)
				{
					if(dm->distance[c->x+1][c->y]==target)
					{
						// if the cell exists return the direction we want to move
						next_move = i;
					}
				}
				break;
			case SOUTH:
				if(c->y-1>-1)
				{
					if(dm->distance[c->x][c->y-1]==target)
					{
						// if the cell exists return the direction we want to move
						next_move = i;
					}
				}
				break;
			case WEST:
				if(c->x-1>-1)
				{
					if(dm->distance[c->x-1][c->y]==target)
					{
						// if the cell exists return the direction we want to move
						next_move = i;
					}
				}
				break;
			default:
				break;
			}
		}

		// find the direction we must turn to
		int difference = d - next_move;
		switch(difference)
		{
		case -3:
			leftStillTurn();
			uncontrolledAdvanceTicks(450);
			break;
		case -2:
			backward180StillTurn();
			if(wm->cells[c->x][c->y].walls[d]==1)
			{
				lockInterruptDisable_TIM3();
				leftMotorPWMChangeBackward(200);
				rightMotorPWMChangeBackward(200);
				custom_delay(2000);
				motorStop();
				lockInterruptEnable_TIM3();
				uncontrolledAdvanceTicks(3000);
			}
			break;
		case -1:
			rightStillTurn();
			uncontrolledAdvanceTicks(450);
			break;
		case 0:
			break;
		case 1:
			leftStillTurn();
			uncontrolledAdvanceTicks(450);
			break;
		case 2:
			backward180StillTurn();
			if(wm->cells[c->x][c->y].walls[d]==1)
			{
				lockInterruptDisable_TIM3();
				leftMotorPWMChangeBackward(200);
				rightMotorPWMChangeBackward(200);
				custom_delay(2000);
				motorStop();
				lockInterruptEnable_TIM3();
				uncontrolledAdvanceTicks(3000);
			}
			break;
		case 3:
			rightStillTurn();
			uncontrolledAdvanceTicks(450);
			break;
		default:
			turnOnLEDS();
			break;
		}

		// update the direction we are currently facing
		d = next_move;
	}
}
