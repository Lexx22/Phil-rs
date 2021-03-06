/*
 * Без имени.c
 * 
 * Copyright 2014 Lex <lex22@ubuntu>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

typedef struct philosophers{
	int id, eat_count;
	pthread_t thread;
	struct fork *right_neighbor, *left_neighbor;
}ph;

typedef struct fork{
	int num;
	pthread_mutex_t mutex;
}fr;

/*void initialization_of_philosophers(int id1, int algo1, int eat_count1, int eat_count1, int is_fork1){
	id = id1;
	algo = algo1;
	eat_count = eat_count1;
	is_fork = is_fork1 ;
}*/

void* ph_life(ph* p){
	srand(time(NULL));
	int i = 0;
	for(i = 0; i < p -> eat_count; ++i){
		printf("philosopher %d is sleep \n", p->id);
		sleep(rand() % 20);
		
		if(( p -> id) % 2 ){//lefthandler
			pthread_mutex_lock(&p -> left_neighbor -> mutex);
			pthread_mutex_lock(&p -> right_neighbor -> mutex);
		}
		else{
			pthread_mutex_lock(&p -> right_neighbor -> mutex);
			pthread_mutex_lock(&p -> left_neighbor -> mutex);
		}
		
		printf("philosopher %d eat \n", p->id);
		
		if( (p -> id) % 2){//lefthandler
			pthread_mutex_unlock(&p -> right_neighbor -> mutex);
			pthread_mutex_unlock(&p -> left_neighbor -> mutex);
		}
		else{
			pthread_mutex_unlock(&p -> left_neighbor -> mutex);
			pthread_mutex_unlock(&p -> right_neighbor -> mutex);
		}
	}
	pthread_exit(NULL);
}

int main(int argc, char **argv){
	if (argc != 3){
		printf("Error\n");
		return 0;
	}
	int count_of_philosophers = 0, eat_count = 0;
	sscanf(argv[1], "%d", &count_of_philosophers);
	sscanf(argv[2], "%d", &eat_count);
	
	fr forks[count_of_philosophers];
	
	int i;
	for(i = 0; i < count_of_philosophers; ++i){
		forks[i].num = i;
		if(pthread_mutex_init(&forks[i].mutex, NULL)){//error
			fprintf(stderr, "ERROR\n");
		}
	}
	
	ph philosophers[count_of_philosophers];
	//create phil-s
	for(i = 0; i < count_of_philosophers; ++i){
		philosophers[i].id = i;
		philosophers[i].eat_count = eat_count;
		philosophers[i].right_neighbor = &forks[i + 1];
		philosophers[i].left_neighbor = &forks[i];
	}
	philosophers[count_of_philosophers - 1].right_neighbor = &forks[0];
	//launching of phil-s
	for(i = 0; i < count_of_philosophers; ++i){
		pthread_create(&philosophers[i].thread, NULL, (void* (*) (void*))ph_life, &philosophers[i]);
	}
	for(i = 0; i < count_of_philosophers; ++i){
		void *res;
		pthread_join(philosophers[i].thread, &res);
	}
	for(i = 0; i < count_of_philosophers; ++i){
		pthread_mutex_destroy(&forks[i].mutex);
	}
	
	return 0;
}
