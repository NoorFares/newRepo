#ifndef INTS012_DEBOUNCER
#define	INTS012_DEBOUNCER

#include <xc.h>

typedef struct {
    unsigned b0 : 1;
    unsigned b1 : 1;
    unsigned b2 : 1;
} Ints012Debouncer ;

#endif	