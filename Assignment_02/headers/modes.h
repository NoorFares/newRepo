#ifndef MODES
#define	MODES

#include <xc.h> 

typedef struct {
    unsigned char clock_operation_mode;
    unsigned char setup_mode;
    unsigned char clock_timer_mode;
    unsigned char heater_cnt;
}Modes;

void update_clock_operation_mode(Modes *modes);
void update_setup_mode(Modes *modes);
void update_clock_timer_mode(Modes *modes);
void update_heater_cnt(Modes *modes);

#endif	

