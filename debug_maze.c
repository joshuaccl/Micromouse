/*
 * maze.c
 *
 *  Created on: Nov 13, 2017
 *      Author: jlammwe
 */

#include "debug_maze.h"

void init_distance_maze(dist_maze* m){
	int i,j;
	for(i=0;i<16;i++){
		for(j=0;j<16;j++){
			m->distance[i][j] = -1;
		}
	}
}

void fill_in_distances(dist_maze* m, coor* c, int i);

void fill_in_distances(dist_maze* m, coor* c, int i){
	int x = c->x;
	if(x > 15) return;
	if(x < 0) return;
	int y = c->y;
	if(y > 15) return;
	if(y > 15) return;
	if(m->distance[c->x][c->y] == -1){
		m->distance[c->x][c->y] = i + 1;
	}
	coor up;
	init_coor(&up, x, y + 1);
	coor down;
	init_coor(&down, x, y - 1);
	coor left;
	init_coor(&left, x - 1, y);
	coor right;
	init_coor(&right, x + 1, y);

	fill_in_distances(&m, &up, i + 1);
	fill_in_distances(&m, &down, i + 1);
	fill_in_distances(&m, &left, i + 1);
	fill_in_distances(&m, &right, i + 1);

}

void fill_in_distance_maze(dist_maze* m, coor* c, int center){
	if(center == 1){//Initializing a center array
		m->distance[7][7] = 0;
		m->distance[7][8] = 0;
		m->distance[8][7] = 0;
		m->distance[8][8] = 0;
		coor left_bottom;
		init_coor(&left_bottom, 7, 7);
		coor right_bottom;
		init_coor(&right_bottom, 8, 7);
		coor left_top;
		init_coor(&left_top, 7, 8);
		coor right_top;
		init_coor(&right_top, 8, 8);
		fill_in_distances(&m, &left_bottom, 0);
		fill_in_distances(&m, &right_bottom, 0);
		fill_in_distances(&m, &left_top, 0);
		fill_in_distances(&m, &right_top, 0);
	}
	else{
		m->distance[c->x][c->y] = 0;
		fill_in_distances(&m, c, 0);
	}
}



