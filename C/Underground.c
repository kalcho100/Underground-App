#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "Declarations.h"

#define MAXLINELENGTH 100

void initializeData(){
	readFileStations();
	readFileLines();
}

bool stationExists(char* name){
    int i = 0;
    while(i < 267 && stations[i] != NULL){
        if(strcmp(stations[i]->name, name) == 0){
            return true;
        }
        i++;
    }
    return false;
}

bool lineExists(char* name, char* direction, bool isDay){
    int i = 0;
    while(i < 39 && lines[i] != NULL){
        if(strcmp(lines[i]->name, name) == 0){
            if(strcmp(lines[i]->direction, direction) == 0){
                if(lines[i]->isDay == isDay){
                    return true;
                }
            }
        }
        i++;
    }
    return false;
}

int lengthOfArray(char** array){
	int i = 0;
	while(array[i] != NULL){
		i++;
	}
	return i;
}

Station* findStation(char* name){
	int i;
	for(i = 0; i < 267; i++){
		if(strcmp(stations[i]->name, name) == 0){
			return stations[i];
		}
	}
}

Line* findLine(char* name, char* direction, bool isDay){
	int i;
	for(i = 0; i < 39; i++){
		if(strcmp(lines[i]->name, name) == 0){
			if(strcmp(lines[i]->direction, direction) == 0){
				if(lines[i]->isDay == isDay){
					return lines[i];
				}
			}
		}
	}
}

bool isThereALineBetweenStations(Station* station1, Station* station2){
	int i = 0;
	while(station1->linesGoingThrough[i] != NULL){
		int j = 0;
		while(station2->linesGoingThrough[j] != NULL){
			if(strcmp(station2->linesGoingThrough[j], station1->linesGoingThrough[i]) == 0){
				if(strcmp(station2->directions[j], station1->directions[i]) == 0){
					if(station2->isDays[j] == station1->isDays[i]){
						return true;
					}
				}
			}
			j++;
		}
		i++;
	}
	return false;
}

int numOfStationsBetweenTwo(Station* station1, Station* station2){
	Line* line;
	int num = 100;
	int i = 0;
	int first;
	int second;
	while(station1->linesGoingThrough[i] != NULL){
		int j = 0;
		while(station2->linesGoingThrough[j] != NULL){
			if(strcmp(station2->linesGoingThrough[j], station1->linesGoingThrough[i]) == 0){
				if(strcmp(station2->directions[j], station1->directions[i]) == 0){
					if(station2->isDays[j] == station1->isDays[i]){
						int k;
						for(k = 0; k < 39; k++){
							if(strcmp(lines[k]->name, station2->linesGoingThrough[j]) == 0){
								if(strcmp(lines[k]->direction, station2->directions[j]) == 0){
									if(lines[k]->isDay == station2->isDays[j]){
										line = lines[k];
										int g = 0;
										while(line->stationsGoingThrough[g] != NULL){
											if(strcmp(line->stationsGoingThrough[g], station1->name) == 0){
												first = g;
											}
											if(strcmp(line->stationsGoingThrough[g], station2->name) == 0){
												second = g;
											}
											g++;
										}
										if(abs(first - second) < num){
											num = abs(first - second);
										}
									}
								}
							}
						}
					}
				}
			}
			j++;
		}
		i++;
	}
	return num;
}

int routeLength(char** route){
	int i = 0, j = 1;
	int length = lengthOfArray(route);
	int num = 0;
	while(j < length){
		num += numOfStationsBetweenTwo(findStation(route[i]), findStation(route[j]));
		i++;
		j++;
	}
	return num;
}

void copyRoute(char** route, char** result){
	int i = 0;
	while(route[i] != NULL){
		result[i] = strdup(route[i]);
		i++;
	}
}

bool isStationUsed(char* station, char** usedStations){
	int i = 0;
	while(usedStations[i] != NULL){
		if(strcmp(usedStations[i], station) == 0){
			return true;
		}
		i++;
	}
	return false;
}

void addToUsed(char* station, Line* line, char** usedStations){
	char* result = malloc(sizeof(char) * 200);
	strcpy(result, station);
	strcat(result, ",");
	strcat(result, line->name);
	strcat(result, ",");
	strcat(result, line->direction);
	int i = 0;
	while(usedStations[i] != NULL){
		i++;
	}
	usedStations[i] = strdup(result);
}

char** splitArray(char** route, int num){
	char** result = malloc(sizeof(char) * 40 * 70);
	int i = 0;
	while(route[num] != NULL){
		result[i] = strdup(route[num]);
		num++;
		i++;
	}
	return result;
}

int stationIndex(char* station, Line* line){
	int i;
	for(i = 0; i < line->numOfStations; i++){
		if(strcmp(line->stationsGoingThrough[i], station) == 0){
			return i;
		}
	}
}

char** optimizeRoute(char** route){
	// int z = 0;
	// while(route[z] != NULL){
	// 	printf("%s, ", route[z]);
	// 	z++;
	// }
	// printf("\n\n");
	if(lengthOfArray(route) <= 2){
		return route;
	}
	char** result = malloc(sizeof(char) * 40 * 70);
	int length = lengthOfArray(route);
	result[0] = strdup(route[0]);
	if(isThereALineBetweenStations(findStation(route[0]), findStation(route[length - 1]))){
        result[1] = strdup(route[length - 1]);
        return result;
    }
	int i, j = 0;
	char* best = malloc(sizeof(char) * 30);
	int bestNum = 0;
	for(i = 1; i < length; i++){
		if(isThereALineBetweenStations(findStation(route[0]), findStation(route[i]))){
			best = strdup(route[i]);
			bestNum = i;
		}
	}
	result[1] = strdup(best);
	free(best);
	char** secondHalf = optimizeRoute(splitArray(route, bestNum + 1));
	i = 2;
	while(secondHalf[j] != NULL){
		result[i] = strdup(secondHalf[j]);
		i++;
		j++;
	}
	return result;
}

void bestRoute(char** result, char** route, char* starts, char* finishes, char** usedStations){
	int i, j;
	Station* start;
	Station* finish;
	for(i = 0; i < 267; i++){
		if(strcmp(stations[i]->name, starts) == 0){
			start = stations[i];
		}
		if(strcmp(stations[i]->name, finishes) == 0){
			finish = stations[i];
		}
	}
	route[lengthOfArray(route)] = strdup(starts);
	usedStations[lengthOfArray(usedStations)] = strdup(starts);
	if(isThereALineBetweenStations(start, finish) == true){
		route[lengthOfArray(route)] = strdup(finishes);
		route = optimizeRoute(route);
		if(lengthOfArray(result) == 0){
			copyRoute(route, result);
		}
		else if(routeLength(route) < routeLength(result)){
			copyRoute(route, result);
		}
		route[lengthOfArray(route) - 1] = '\0';
		return;
	}
	for(i = 0; i < start->numOfLines; i++){
		Line* current = findLine(start->linesGoingThrough[i], start->directions[i], start->isDays[i]);
		int index = stationIndex(starts, current);
		for(j = 0; j < current->numOfStations; j++){
			if(index - j >= 0){
				char* dummy = current->stationsGoingThrough[index - j];
				if(!isStationUsed(dummy, usedStations)){
					bestRoute(result, route, dummy, finishes, usedStations);
				}
			}
			if(index + j < current->numOfStations){
				char* dummy = current->stationsGoingThrough[index + j];
				if(!isStationUsed(dummy, usedStations)){
					bestRoute(result, route, dummy, finishes, usedStations);
				}
			}
		}
	}
	route[lengthOfArray(route) - 1] = '\0';
}

int main(){
	initializeData();
	//check if stations exist
	char* start = malloc(sizeof(char) * MAXLINELENGTH);
	char* end = malloc(sizeof(char) * MAXLINELENGTH);
	while(1){
		printf("Enter start station: ");
		fgets(start, MAXLINELENGTH, stdin);
		start[strlen(start) - 1] = '\0';
		if(stationExists(start)){
			break;
		}
	}
	while(1){
		printf("Enter final station: ");
		fgets(end, MAXLINELENGTH, stdin);
		end[strlen(end) - 1] = '\0';
		if(stationExists(end)){
			break;
		}
	}
	char** usedStations = malloc(sizeof(char) * 40 * 267);
	char** result = malloc(sizeof(char) * 40 * 50);
	char** route = malloc(sizeof(char) * 40 * 70);
	bestRoute(result, route, start, end, usedStations);
	int i = 0;
	while(result[i] != NULL){
		printf("%s\n", result[i]);
		i++;
	}
	return 0;
}