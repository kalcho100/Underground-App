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
    int length = lengthOfArray(result);
    while(route[i] != NULL){
        result[i] = strdup(route[i]);
        i++;
    }
    while(i < length){
        result[i] = '\0';
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

int stationIndex(char* station, Line* line){
    int i;
    for(i = 0; i < line->numOfStations; i++){
        if(strcmp(line->stationsGoingThrough[i], station) == 0){
            return i;
        }
    }
}

Line* bestLineBetweenTwo(Station* station1, Station* station2){
    Line* result;
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
                                        Line* line = lines[k];
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
                                            result = line;
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
    return result;
}

char* rotateDirection(char* direction, char* result){
    if (strstr(direction, " via ") != NULL) {
        char* first = strtok(direction, "-");
        char* second = strtok(NULL, "v");
        char* via = strtok(NULL, " ");
        via = strtok(NULL, ";");
        strcat(result, second);
        strcat(result, "- ");
        strcat(result, first);
        strcat(result, "via ");
        strcat(result, via);
    }
    else{
        char* first = strtok(direction, "-");
        char* second = strtok(NULL, ";");
        strcat(result, second);
        strcat(result, "- ");
        strcat(result, first);
    }
    return result;
}

char* bestDirection(Line* line, char* station1, char* station2){
    char* result = malloc(sizeof(char) * 60);
    int i;
    for(i = 0; i < 60; i++){
        result[i] = '\0';
    }
    int first = stationIndex(station1, line);
    int second = stationIndex(station2, line);
    char* direction = line->direction;
    char* temp = strdup(direction);
    if(first > second){
        result = strdup(rotateDirection(temp, result));
    }
    else{
        result = strdup(direction);
    }
    free(temp);
    return result;
}

void printRoutes(char** leastInterchanges, char** leastStations){
    if(lengthOfArray(leastStations) == 2){
        printf("\nBest route:\n\n");
        Line* line = bestLineBetweenTwo(findStation(leastStations[0]), findStation(leastStations[1]));
        char* direction = bestDirection(line, leastStations[0], leastStations[1]);
        printf("Take the %s (%s) line to %s.\n\n\n", line->name, direction, leastStations[1]);
        return;
    }
    printf("\nLeast number of stations:\n\n");
    int i = 1, j = 2;
    Line* line = bestLineBetweenTwo(findStation(leastStations[0]), findStation(leastStations[1]));
    char* direction = bestDirection(line, leastStations[0], leastStations[1]);
    printf("Take the %s (%s) line to %s", line->name, direction, leastStations[1]);
    while(j < lengthOfArray(leastStations) - 1){
        line = bestLineBetweenTwo(findStation(leastStations[i]), findStation(leastStations[j]));
        direction = bestDirection(line, leastStations[i], leastStations[j]);
        printf(", then the %s (%s) line to %s", line->name, direction, leastStations[j]);
        i++;
        j++;
    }
    line = bestLineBetweenTwo(findStation(leastStations[i]), findStation(leastStations[j]));
    direction = bestDirection(line, leastStations[i], leastStations[j]);
    printf(" and then the %s (%s) line to %s.\n\n\n", line->name, direction, leastStations[j]);

    printf("Least number of interchanges:\n\n");
    i = 1, j = 2;
    line = bestLineBetweenTwo(findStation(leastInterchanges[0]), findStation(leastInterchanges[1]));
    direction = bestDirection(line, leastInterchanges[0], leastInterchanges[1]);
    printf("Take the %s (%s) line to %s", line->name, direction, leastInterchanges[1]);
    while(j < lengthOfArray(leastInterchanges) - 1){
        line = bestLineBetweenTwo(findStation(leastInterchanges[i]), findStation(leastInterchanges[j]));
        direction = bestDirection(line, leastInterchanges[i], leastInterchanges[j]);
        printf(", then the %s (%s) line to %s", line->name, direction, leastInterchanges[j]);
        i++;
        j++;
    }
    line = bestLineBetweenTwo(findStation(leastInterchanges[i]), findStation(leastInterchanges[j]));
    direction = bestDirection(line, leastInterchanges[i], leastInterchanges[j]);
    printf(" and then the %s (%s) line to %s.\n\n\n", line->name, direction, leastInterchanges[j]);
    free(direction);
}

void optimizeRoute(char** route){
    int i, j, count = 1;
    int length = lengthOfArray(route);
    int bestIndex = 0;
    char** temp = malloc(sizeof(char) * 25 * 70);
    temp[0] = strdup(route[0]);
    char* best = malloc(sizeof(char) * 25);
    for(i = 0; i < length - 1; i++){
        for(j = i + 1; j < length; j++){
            if(isThereALineBetweenStations(findStation(route[i]), findStation(route[j]))){
                best = strdup(route[j]);
                bestIndex = j;
            }
        }
        temp[count] = strdup(best);
        count++;
        i = bestIndex - 1;
    }
    free(best);
    for(i = 0; i < length; i++){
        if(i < count){
            route[i] = strdup(temp[i]);
        }
        else{
            route[i] = '\0';
        }
    }
    free(temp);
}

void bestRoute(char** leastStations, char** leastInterchanges, char** route, char* starts, char* finishes, char** usedStations){
    if(strcmp(starts, finishes) == 0){
        return;
    }
    int i, j;
    Station* start = findStation(starts);
    Station* finish = findStation(finishes);
    route[lengthOfArray(route)] = strdup(starts);
    usedStations[lengthOfArray(usedStations)] = strdup(starts);
    if(isThereALineBetweenStations(start, finish)){
        route[lengthOfArray(route)] = strdup(finishes);
        optimizeRoute(route);
        if(lengthOfArray(leastStations) == 0){
            copyRoute(route, leastStations);
        }
        else if(routeLength(route) < routeLength(leastStations)){
            copyRoute(route, leastStations);
        }
        if(lengthOfArray(leastInterchanges) == 0){
            copyRoute(route, leastInterchanges);
        }
        else if(lengthOfArray(route) < lengthOfArray(leastInterchanges)){
            copyRoute(route, leastInterchanges);
        }
        else if(lengthOfArray(route) == lengthOfArray(leastInterchanges)){
            if(routeLength(route) < routeLength(leastInterchanges)){
                copyRoute(route, leastInterchanges);
            }
        }
        route[lengthOfArray(route) - 1] = '\0';
        return;
    }
    for(i = 0; i < start->numOfLines; i++){
        Line* current = findLine(start->linesGoingThrough[i], start->directions[i], start->isDays[i]);
        int index = stationIndex(starts, current);
        for(j = 1; j < current->numOfStations; j++){
            if(index - j >= 0){
                char* dummy = current->stationsGoingThrough[index - j];
                if(!isStationUsed(dummy, usedStations)){
                    bestRoute(leastStations, leastInterchanges, route, dummy, finishes, usedStations);
                }
            }
            if(index + j < current->numOfStations){
                char* dummy = current->stationsGoingThrough[index + j];
                if(!isStationUsed(dummy, usedStations)){
                    bestRoute(leastStations, leastInterchanges, route, dummy, finishes, usedStations);
                }
            }
        }
    }
    if(strcmp(route[lengthOfArray(route) - 1], starts) == 0) {
        route[lengthOfArray(route) - 1] = '\0';
    }
}

void nullArray(char** array, int num){
    int i;
    for(i = 0; i < num; i++){
        array[i] = '\0';
    }
}

void findRoute(){
    char* start = malloc(sizeof(char) * MAXLINELENGTH);
    char* end = malloc(sizeof(char) * MAXLINELENGTH);
    printf("\nEnter start station: ");
    while(1){
        fgets(start, MAXLINELENGTH, stdin);
        start[strlen(start) - 1] = '\0';
        if(stationExists(start)){
            break;
        }
        else{
            printf("\nThis station doesn't exist.\n");
            printf("Try again: ");
        }
    }
    printf("\nEnter final station: ");
    while(1){
        fgets(end, MAXLINELENGTH, stdin);
        end[strlen(end) - 1] = '\0';
        if(strcmp(start, end) == 0){
            printf("\nStart and final stations can't be the same.\n");
            printf("Try again: ");
        }
        else if(stationExists(end)){
            break;
        }
        else{
            printf("\nThis station doesn't exist.\n");
            printf("Try again: ");
        }
    }
    char** usedStations = malloc(sizeof(char) * 40 * 267);
    nullArray(usedStations, 267);
    char** leastStations = malloc(sizeof(char) * 40 * 30);
    nullArray(leastStations, 30);
    char** leastInterchanges = malloc(sizeof(char) * 40 * 30);
    nullArray(leastInterchanges, 30);
    char** route = malloc(sizeof(char) * 40 * 150);
    nullArray(route, 150);
    bestRoute(leastStations, leastInterchanges, route, start, end, usedStations);
    printRoutes(leastInterchanges, leastStations);
    free(usedStations);
    free(leastStations);
    free(leastInterchanges);
    free(route);
}

void Menu()
{
    printf("\n1. Find best route\n");
    printf("2. Quit\n");
}

int getMenuOption()
{
    int option = 0;
    printf("Enter option: ");
    char* input = malloc(sizeof(char) * MAXLINELENGTH);
    fgets(input, MAXLINELENGTH, stdin);
    sscanf(input,"%i",&option);
    free(input);
    return option;
}

void doOption(int option)
{
    switch (option)
    {
        case 1 :
            findRoute();
            break;
        default : printf("\nSorry - don't recognise that option, try again\n");
    }
}

void go()
{
    while(1)
    {
        Menu();
        int option = getMenuOption();
        if (option == 2)
        {
            break;
        }
        else
        {
            doOption(option);
        }
    }
}

int main ()
{
    initializeData();
    go();
    return 0;
}