#ifndef LCD
#define	LCD

#define _XTAL_FREQ   4000000UL 

#include <xc.h>

#define LCD_TYPE 2

#define LCD_LINE_TWO 0x40   
#define LCD_LINE_SIZE 16

struct lcd_pin_map {
    unsigned un1 : 1; 
    unsigned rs : 1;       
    unsigned rw : 1; 
    unsigned enable : 1; 
    unsigned data : 4; 

} lcd __at(0xF83); 

#define lcd_output_enable(x) PORTEbits.RE1 = x 
#define lcd_output_rs(x) PORTEbits.RE2 = x 

void delay_cycles(unsigned char n);
void delay_ms(unsigned int n);
void lcd_send_nibble(unsigned char n);
void lcd_send_byte(unsigned char cm_data, unsigned char n);
void lcd_init(void);
void lcd_gotoxy(unsigned char x, unsigned char y);
void lcd_putc(char c);
void lcd_puts(char *s);
void Lcd_Shift_Right(void);
void Lcd_Shift_Left(void);

#endif	