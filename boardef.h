#ifndef INC_BOARDEF_H_
#define INC_BOARDEF_H_

#ifdef __cplusplus
 extern "C" {
#endif


/* DELAY TIMER DEFINE BEGIN */

#define DELAY_TIM TIM6
#define DELAY_TIM_STRING "TIM6"
#define DELAY_TIM_APB_FREQ 80

/* DELAY TIMER DEFINE END */


/* 2x16 LCD DISPLAY WITH HD44780U CONTROLLER DEFINE BEGIN */

#define LCD_RS_PIN	GPIO_PIN_1
#define LCD_RS_PORT	GPIOA
#define LCD_RW_PIN	GPIO_PIN_2
#define LCD_RW_PORT	GPIOA
#define LCD_EN_PIN	GPIO_PIN_3
#define LCD_EN_PORT	GPIOA

#define LCD_D4_PIN	GPIO_PIN_4
#define LCD_D4_PORT	GPIOA
#define LCD_D5_PIN	GPIO_PIN_5
#define LCD_D5_PORT	GPIOA
#define LCD_D6_PIN	GPIO_PIN_6
#define LCD_D6_PORT	GPIOA
#define LCD_D7_PIN	GPIO_PIN_7
#define LCD_D7_PORT	GPIOA

/* 2x16 LCD DISPLAY WITH HD44780U CONTROLLER DEFINE END */

#endif /* INC_BOARDEF_H_ */
