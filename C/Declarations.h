#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

typedef struct Line
{
    char* name;
    char* direction;
    bool isDay;
    int numOfStations;
    char** stationsGoingThrough;
}Line;

Line* lines[39];

typedef struct Station
{
    char* name;
    int numOfLines;
    char** linesGoingThrough;
    char** directions;
    bool* isDays;
}Station;

Station* stations[267];

Line* newLine(char* name, char* direction, bool isDay, int numOfStations){
    Line *line = malloc(sizeof(Line));
    line->name = malloc(sizeof(char) * 25);
    line->name = strdup(name);
    line->direction = malloc(sizeof(char) * 50);
    line->direction = strdup(direction);
    line->isDay = isDay;
    line->numOfStations = numOfStations;
    line->stationsGoingThrough = malloc(sizeof(char) * 40 * numOfStations);
    return line;
}

Station* newStation(char* name, int numOfLines){
    Station *station = malloc(sizeof(Station));
    station->name = malloc(sizeof(char) * 30);
    station->name = strdup(name);
    station->numOfLines = numOfLines;
    station->linesGoingThrough = malloc(sizeof(char) * 25 * numOfLines);
    station->directions = malloc(sizeof(char) * 100 * numOfLines);
    station->isDays = malloc(sizeof(bool) * numOfLines);
    return station;
}

void addStation(char* name, int numOfLines){
    int i;
    for(i = 0; i < 267; i++){
        if(stations[i] == NULL){
            stations[i] = newStation(name, numOfLines);
            break;
        }
    }
}

void addLine(char* name, char* direction, bool isDay, int numOfStations){
    int i;
    for(i = 0; i < 39; i++){
        if(lines[i] == NULL){
            lines[i] = newLine(name, direction, isDay, numOfStations);
            break;
        }
    }
}

void addStationToLine(char* line, char* direction, bool isDay, char* station){
    int i;
    for(i = 0; i < 39; i++){
        if(strcmp(lines[i]->name, line) == 0){
            if(strcmp(lines[i]->direction, direction) == 0){
                if(lines[i]->isDay == isDay){
                    int num = 0;
                    while(true){
                        if(lines[i]->stationsGoingThrough[num] == NULL){
                            lines[i]->stationsGoingThrough[num] = strdup(station);
                            return;
                        }
                        num++;
                    }
                }
            }
        }
    }
}

void addLineToStation(char* station, char* line, char* direction, bool isDay){
    int i;
    for(i = 0; i < 267; i++){
        if(strcmp(stations[i]->name, station) == 0){
            int num = 0;
            while(true){
                if(stations[i]->linesGoingThrough[num] == NULL){
                    stations[i]->linesGoingThrough[num] = strdup(line);
                    stations[i]->directions[num] = strdup(direction);
                    stations[i]->isDays[num] = isDay;
                    return;
                }
                num++;
            }
        }
    }
}

bool isDayToBool(char* isDay){
    if(strcmp(isDay, "day") == 0){
        return true;
    }
    else{
        return false;
    }
}

void readFileStations(){
    FILE* file = fopen("Stations.txt", "r");
    char stat[100];
    while(fgets(stat, sizeof stat, file)) {
        stat[strlen(stat) - 1] = '\0';
        char* name = strtok(stat, ",");
        char* num = strtok(NULL, " ");
        int nums = atoi(num);
        addStation(name, nums);
    }
    fclose(file);
}

void readFileLines(){
    FILE *file = fopen("Lines.txt", "r");
    char line[100];
    while(fgets(line, sizeof line, file)){
        line[strlen(line) - 1] = '\0';
        char* name = strtok(line, ",");
        char* direction = strtok(NULL, ",");
        char* isDay = strtok(NULL, ",");
        char* num = strtok(NULL, " ");
        bool isDays = isDayToBool(isDay);
        int nums = atoi(num);
        addLine(name, direction, isDays, nums);
        int i;
        char station[100];
        for(i = 0; i < nums; i++){
            fgets(station, sizeof station, file);
            station[strlen(station) - 1] = '\0';
            addStationToLine(name, direction, isDays, station);
            addLineToStation(station, name, direction, isDays);
        }
        fgets(station, sizeof station, file);
    }
}
