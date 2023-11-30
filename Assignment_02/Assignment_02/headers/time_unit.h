#ifndef TIME_UNIT
#define	TIME_UNIT

#include <xc.h> 

typedef struct {
    unsigned short seconds;
    unsigned short minutes;
    unsigned short hours;
} TimeUnit;

void increment_hours(TimeUnit *time_unit);
void increment_minutes(TimeUnit *time_unit);
void increment_seconds(TimeUnit *time_unit);
void decrement_hours(TimeUnit *time_unit);
void decrement_minutes(TimeUnit *time_unit);
void decrement_seconds(TimeUnit *time_unit);
char is_zero(TimeUnit *time_unit);
void add_1s(TimeUnit *time_unit);
void remove_1s(TimeUnit *time_unit);

#endif	