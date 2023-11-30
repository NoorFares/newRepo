/*
 * File:   time_unit.c
 * Author: Mohammad Alawneh
 *
 * Created on November 26, 2023, 1:08 PM
 */

#include "time_unit.h"
#include <xc.h>

void increment_hours(TimeUnit *time_unit) {
    time_unit->hours++;
    
    if (time_unit->hours == 24) {
        time_unit->hours = 0;
    }
}

void increment_minutes(TimeUnit *time_unit) {
    time_unit->minutes++;
    
    if (time_unit->minutes == 60) {
        time_unit->minutes = 0;
    }
}

void increment_seconds(TimeUnit *time_unit) {
    time_unit->seconds++;
    
    if (time_unit->seconds == 60) {
        time_unit->seconds = 0;
    }
}

void decrement_hours(TimeUnit *time_unit) {  
    time_unit->hours--;
    
    if (time_unit->hours == -1) {
        time_unit->hours = 23;
    }
    
}

void decrement_minutes(TimeUnit *time_unit) {
    time_unit->minutes--;
    
    if (time_unit->minutes == -1) {
        time_unit->minutes = 59;
    }
}

void decrement_seconds(TimeUnit *time_unit) {
    time_unit->seconds--;
    
    if (time_unit->minutes == -1) {
        time_unit->minutes = 59;
    }
}

char is_zero(TimeUnit *time_unit) {
    return time_unit->seconds == 0 && time_unit->minutes == 0 && time_unit->hours == 0;
}

void add_1s(TimeUnit *time_unit) {
    time_unit->seconds++;
    
    if (time_unit->seconds == 60) {
        time_unit->seconds = 0;
        
        time_unit->minutes++;
        if (time_unit->minutes == 60) {
            time_unit->minutes = 0;
            
            time_unit->hours++;
            if (time_unit->hours == 24) {
                time_unit->hours = 0;
            }
        }
    }
}

void remove_1s(TimeUnit *time_unit) {
    if (is_zero(time_unit))
        return; 
    
    time_unit->seconds--;
    
    if (time_unit->seconds == -1) {
        time_unit->seconds = 59;
        
        time_unit->minutes--;
        if (time_unit->minutes == -1) {
            time_unit->minutes = 59;
            
            time_unit->hours--;
            if (time_unit->hours == -1) {
                time_unit->hours = 0;
            }
        }
    }
}