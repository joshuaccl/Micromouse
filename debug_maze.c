/*
 * maze.c
 *
 *  Created on: Nov 13, 2017
 *      Author: jlammwe
 */

#include "debug_maze.h"
#include <stdio.h>

void init_distance_maze(dist_maze* m){
	int i,j;
	for(i=0;i<16;i++){
		for(j=0;j<16;j++){
			m->distance[i][j] = -1;
		}
	}
}

int abs(int x);

int abs(int x){
	if( x<0 ) return -1 * x;
	else return x;
}

void fill_in_distance_maze(dist_maze* m, coor* c, int center){
	int i,j;
	if(center == 1){//Initializing a center array
		for(i=15;i>-1;i--){
			for(j=0;j<16;j++){
				if(i<=7 && j>=8) m->distance[i][j] = abs(7-i) + abs(8-j);
				else if(i<=7 && j<=7) m->distance[i][j] = abs(7-i) + abs(7-j);
				else if(i>=8 && j>=8) m->distance[i][j] = abs(8-i) + abs(8-j);
				else if(i>=8 && j<=7) m->distance[i][j] = abs(8-i) + abs(7-j);
			}
		}
	}
	else{
		int x = c->x;
		int y = c->y;
		for(i=0;i<16;i++){
			for(j=0;j<16;j++){
				m->distance[i][j] = abs(x-i) + abs(y-j);
			}
		}
	}	
}



