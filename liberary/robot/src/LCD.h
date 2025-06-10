#ifndef LCD_h
#define LCD_h

//zet de cursor posietie=X+20Y
void set_cursor(uint8_t positie);
//wist het hele scherm
void clear_screen();
//zet de backlight (aan of af) volgens vairalble aan 
void backlight(bool aan);
//print 1 karacter op het scherm
void print_char(char karacter);
//print een lijst met karacters of string
void print_text(char* text);

#endif