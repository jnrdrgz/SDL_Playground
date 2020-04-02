#ifndef HORSEDATA_H
#define HORSEDATA_H

class HorseData
{
public:
	int meters_to_finnish = 0;
	int laps_to_finnish = 0;
    int position = 0;
    int meters_to_travelled = 0;
    int max_vel = 0;
    int map_x = 0;

    HorseData(){}
    HorseData(int meters_to_finnish, int laps_to_finnish, int max_vel){
    	this->meters_to_finnish = meters_to_finnish;
    	this->laps_to_finnish = laps_to_finnish;
    	this->max_vel = max_vel;
    }
};

#endif