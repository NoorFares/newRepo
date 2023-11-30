/*
 * File:   main.c
 * Author: Najat Mansour
 *
 * Created on November 25, 2023, 1:38 AM
 */

// PIC18F4620 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1H
#pragma config OSC = XT         // Oscillator Selection bits (XT oscillator)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = SBORDIS  // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 3         // Brown Out Reset Voltage bits (Minimum setting)

// CONFIG2H
#pragma config WDT = ON         // Watchdog Timer Enable bit (WDT enabled)
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = PORTC   // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = ON      // PORTB A/D Enable bit (PORTB<4:0> pins are configured as analog input channels on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = ON         // Single-Supply ICSP Enable bit (Single-Supply ICSP enabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-003FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (004000-007FFFh) not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (008000-00BFFFh) not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (00C000-00FFFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-003FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (004000-007FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (008000-00BFFFh) not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (00C000-00FFFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot Block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (004000-007FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (008000-00BFFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (00C000-00FFFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot Block (000000-0007FFh) not protected from table reads executed in other blocks)

#include "lcd.h"
#include "time_unit.h"
#include "adc.h"
#include "en_clock_operation_modes.h"
#include "en_setup_modes.h"
#include "en_clock_timer_modes.h"
#include "en_heater_cnt.h"
#include "modes.h"
#include "ints012_debouncer.h"
#include <xc.h>
#include <stdio.h>
#include <time.h>

#define STARTVALUE  3036

TimeUnit digital_clock = {0, 0, 0};
TimeUnit clock_timer = {45, 2, 0};
Modes modes = {NORMAL, SECONDS, ON, DISABLED};
Ints012Debouncer ints012_debouncer = {1, 1, 1};

float read_tempreture(void) {
    return read_adc_voltage(2) * 100.0;
}

void show_digital_clock_and_tempreture(void) {
    unsigned buffer [64];
    sprintf(buffer, "%02d:%02d:%02d   %.2f", digital_clock.hours, digital_clock.minutes, digital_clock.seconds, read_tempreture());
    lcd_gotoxy(1, 1);
    lcd_puts(buffer);
}

void show_clock_timer_and_its_status(void) {
    unsigned buffer [64];
    sprintf(buffer, "%02d:%02d:%02d   T:%s", clock_timer.hours, clock_timer.minutes, clock_timer.seconds,
            modes.clock_timer_mode == ON ? "ON " : "OFF");
    lcd_gotoxy(1, 2);
    lcd_puts(buffer);
}

void show_mode(void) {
    unsigned buffer [64];
    if (modes.clock_operation_mode == NORMAL) {
        sprintf(buffer, "Normal        ");
    } else {
        
        const char* clock_operation_mode;
        if (modes.clock_operation_mode == DIGITAL_CLOCK_SETUP) {
            clock_operation_mode = "Clock";
        } else if (modes.clock_operation_mode == CLOCK_TIMER_SETUP) {
            clock_operation_mode = "Timer";
        }
        
        const char* setup_mode;
        if (modes.setup_mode == SECONDS) {
            setup_mode = "sec";
        } else if (modes.setup_mode == MINUTES) {
            setup_mode = "min";
        } else {
            setup_mode = "hrs";
        }
        
        sprintf(buffer, "%s      %s", clock_operation_mode, setup_mode);
    }
    lcd_gotoxy(1, 3);
    lcd_puts(buffer);
}

void show_heater_info(void) {
    unsigned buffer [64];
    sprintf(buffer, "H:%s S:%s", modes.heater_cnt ? "Enabled " : "Disabled",
                                 PORTCbits.RC5 ? "ON " : "OFF");
    lcd_gotoxy(1, 4);
    lcd_puts(buffer);
}

void set_heater_status(void) {
    if (modes.clock_timer_mode == ON && modes.heater_cnt == ENABLED) {
        PORTCbits.RC5 = 1;
    } else {
        PORTCbits.RC5 = 0;
    }
}

void reload_timer0(void) {  
    TMR0H = (unsigned char) ((STARTVALUE >>  8) & 0x00FF);
    TMR0L = (unsigned char)(STARTVALUE & 0x00FF );   
}

void timer0_isr(void) {
    INTCONbits.TMR0IF = 0; // clear Timer 0 flag bit
    if (modes.clock_operation_mode != DIGITAL_CLOCK_SETUP) {
        add_1s(&digital_clock);
    }
    if (modes.clock_timer_mode == ON) {
        remove_1s(&clock_timer);
    }
    reload_timer0(); 
}

void int0_isr(void) {
    INTCONbits.INT0IF = 0; // clear INT0 flag bit
    // Debounce INT0
    if (ints012_debouncer.b0) return;
    ints012_debouncer.b0 = 1;
    update_clock_operation_mode(&modes);
}

void int1_isr(void) {
    INTCON3bits.INT1IF = 0; // clear INT0 flag bit
    // Debounce INT1
    if (ints012_debouncer.b1) return;
    ints012_debouncer.b1 = 1;
    if (modes.clock_operation_mode != NORMAL) {
        update_setup_mode(&modes);
    }
}

void int2_isr(void) {
    INTCON3bits.INT2IF = 0; // clear INT0 flag bit
    // Debounce INT2
    if (ints012_debouncer.b2) return;
    ints012_debouncer.b2 = 1;
    update_clock_timer_mode(&modes);
    if (is_zero(&clock_timer)) {
        modes.clock_timer_mode = OFF;
    }
}

void __interrupt(high_priority) high_isr(void) {
    if (INTCONbits.TMR0IF) timer0_isr();
    else if (INTCONbits.INT0IF) int0_isr();
    else if (INTCON3bits.INT1IF) int1_isr();
    else if (INTCON3bits.INT2IF) int2_isr();
}

void setup_ports() {
    ADCON0 = 0;
    ADCON1 = 0x0C;        //3 analog channels in order to use AI2

    TRISA = 0b00000100;   // in order to use AI2 
    TRISB = 0b00111111;   // RB0 to RB6
    TRISC = 0; 
    TRISD = 0;
    TRISE = 0; 
    
    PORTD = 0;
}

void main(void) {
    setup_ports();
    lcd_init();
    lcd_putc('\f');         // clears the LCD display
    init_adc();
    reload_timer0();
    
    RCONbits.IPEN = 0;      // Disable Interrupt priority , All are high
    
    INTCON = 0;             // disable interrupts first, then enable the ones u want
    INTCON2 = 0;
    INTCON3 = 0;
    INTCONbits.GIEH = 1;    // enable global interrupt bits
    INTCONbits.GIEL = 1;    // enable global interrupt bits
    INTCONbits.TMR0IE = 1;  // enable Timer 0 Interrupt
    
    T0CON = 0;
    T0CONbits.T0PS = 0b011; // set the pre-scalar
    T0CONbits.TMR0ON = 1;   // turn on the Timer 0
    
    // INT0, INT1 and INT2 Enables
    INTCONbits.INT0IE = 1;
    INTCON3bits.INT1IE = 1;
    INTCON3bits.INT2IE = 1;
    // INT0, INT1 and INT2 Edges
    INTCON2bits.INTEDG0 = 1;
    INTCON2bits.INTEDG1 = 1;
    INTCON2bits.INTEDG2 = 1;
   
    while (1) {
        CLRWDT();
        show_digital_clock_and_tempreture();
        show_clock_timer_and_its_status();
        show_mode();
        show_heater_info();
        set_heater_status();
        
        if (PORTBbits.RB3 == 0) {
            while (PORTBbits.RB3 == 0) {
                CLRWDT();
                delay_ms(250);
            }
            if (modes.clock_operation_mode == DIGITAL_CLOCK_SETUP) {
                if (modes.setup_mode == SECONDS) {
                    increment_seconds(&digital_clock);
                    
                } else if (modes.setup_mode == MINUTES) {
                    increment_minutes(&digital_clock);
                    
                } else if (modes.setup_mode == HOURS) {
                    increment_hours(&digital_clock);
                    
                }
                
            } else if (modes.clock_operation_mode == CLOCK_TIMER_SETUP) {
                if (modes.setup_mode == SECONDS) {
                    increment_seconds(&clock_timer);
                    
                } else if (modes.setup_mode == MINUTES) {
                    increment_minutes(&clock_timer);
                    
                } else if (modes.setup_mode == HOURS) {
                    increment_hours(&clock_timer);
                    
                }
                
            }
            delay_ms(300);
            
        } else if (PORTBbits.RB4 == 0) {
            while (PORTBbits.RB4 == 0) {
                CLRWDT();
                delay_ms(250);
            }
            if (modes.clock_operation_mode == DIGITAL_CLOCK_SETUP) {
                if (modes.setup_mode == SECONDS) {
                    decrement_seconds(&digital_clock);
                    
                } else if (modes.setup_mode == MINUTES) {
                    decrement_minutes(&digital_clock);
                    
                } else if (modes.setup_mode == HOURS) {
                    decrement_hours(&digital_clock);
                    
                }
                
            } else if (modes.clock_operation_mode == CLOCK_TIMER_SETUP) {
                if (modes.setup_mode == SECONDS) {
                    decrement_seconds(&clock_timer);
                    
                } else if (modes.setup_mode == MINUTES) {
                    decrement_minutes(&clock_timer);
                    
                } else if (modes.setup_mode == HOURS) {
                    decrement_hours(&clock_timer);
                    
                }
                
            }
            delay_ms(300);
            
        } else if (PORTBbits.RB5 == 0) {
            while (PORTBbits.RB0 == 0) {
                CLRWDT();
                delay_ms(250);
            }
            update_heater_cnt(&modes);
            delay_ms(300);
        }
        
        // This piece of code is for debouncing the interrupts 0,1 and 2 
        if (ints012_debouncer.b0) {
            delay_ms(200);
            ints012_debouncer.b0 = 0;
            
        } else if (ints012_debouncer.b1) {
            delay_ms(200);
            ints012_debouncer.b1 = 0;
            
        } else if (ints012_debouncer.b2) {
            delay_ms(200);
            ints012_debouncer.b2 = 0;
            
        }
    }
}