/*
 * File:   modes.c
 * Author: Mohammad Alawneh
 *
 * Created on November 26, 2023, 1:45 PM
 */

#include "modes.h"
#include <xc.h>

void update_clock_operation_mode(Modes *modes) {
    modes->clock_operation_mode++;
    
    if (modes->clock_operation_mode == 3) {
        modes->clock_operation_mode = 0;
    }
}

void update_setup_mode(Modes *modes) {
    modes->setup_mode++;
    
    if (modes->setup_mode == 3) {
        modes->setup_mode = 0;
    }
}

void update_clock_timer_mode(Modes *modes) {
    modes->clock_timer_mode++;
    
    if (modes->clock_timer_mode == 2) {
        modes->clock_timer_mode = 0;
    }
}

void update_heater_cnt(Modes *modes) {
    modes->heater_cnt++;
    
    if (modes->heater_cnt == 2) {
        modes->heater_cnt = 0;
    }
}