#include "debug_stack.h"
#include "debug_maze.h"
#include <stdio.h>

int main(void) {

	dist_maze test_maze;
	init_distance_maze(&test_maze);
	int i,j;
	coor c;
	init_coor(&c, 0, 0);
	for(i=15; i>-1; i--){
		for(j=0; j<16; j++){
			printf("%d ", test_maze.distance[j][i]);
		}
		printf("\n");
	}
	printf("\n");
	fill_in_distance_maze(&test_maze, &c, 1);

	for(i=15; i>-1; i--){
			for(j=0; j<16; j++){
				printf("%d ", test_maze.distance[j][i]);
			}
			printf("\n");
		}
}

