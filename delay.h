#ifndef INC_DELAY_H_
#define INC_DELAY_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* REMEMBER TO FIX INIT FUNCTION SO THAT IT IS DONE AUTOMIATCLY */

/* INITS TIMER TO WORK AS A DEALY TIMER */
void Delay_Init(void);

/* DELAY FUNCTION THAT DELAYS FOR US MICROSECONDS */
void Delay_Us(uint16_t us);

#endif /* INC_DELAY_H_ */
