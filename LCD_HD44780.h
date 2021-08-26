#ifndef INC_LCD_HD44780_H_
#define INC_LCD_HD44780_H_

#ifdef __cplusplus
 extern "C" {
#endif

 /* REMEMBER "x" = CHAR*, 'x' = CHAR */
 /* REMEMBER TO UNLOCK TIM MODULE IN *STM32L4XX_HAL_CONF.H*/
 /* REMEMBER INIT FUNCTIONS MUST BE FIXED SO THAT CLOCK FOR GPIO IS UNLOCKED AUTOMATICLY*/

 /* INIT OF CONTROL AND DATA GPIO PINS AS WELL AS INIT LCD IN 4 BITS MODE */
void LCD_Init(void);

/* CLEARS LCD SCREEN */
void LCD_Clear(void);

/* CHANGES LCD CURSOR POSITION TO ROW = 0 v 1, COL = 0 -> 15 */
void LCD_Goto(uint8_t row, uint8_t col);

/* SENDS Command TO LCD */
void LCD_Send_Command(uint8_t Command);

/* SENDS Character TO LCD */ /* REMEMBER "x" = CHAR*, 'x' = CHAR */
void LCD_Send_Character(char Character);

/* SENDS String TO LCD */ /* REMEMBER "x" = CHAR*, 'x' = CHAR */
void LCD_Send_String(char* String);


#endif /* INC_LCD_HD44780_H_ */
