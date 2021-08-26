#include "stm32l4xx_hal.h"
#include "boardef.h"
#include "delay.h"

static void GPIO_Input(GPIO_TypeDef* Port, uint16_t Pin){

	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* CONFIGURE GPIO PIN AS INPUT */

	GPIO_InitStruct.Pin = Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(Port, &GPIO_InitStruct);

}
static void GPIO_Output(GPIO_TypeDef* Port, uint16_t Pin){

	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* CONFIGURE GPIO PIN OUTPUT LEVEL */

	HAL_GPIO_WritePin(Port, Pin, GPIO_PIN_RESET);

	/* CONFIGURE GPIO PIN AS OUTPUT */

	GPIO_InitStruct.Pin = Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(Port, &GPIO_InitStruct);

}

static void DataPin_Input_Mode(void){

	/* CONFIGURE LCD GPIO DATA PINS AS INPUT */

	GPIO_Input(LCD_D4_PORT, LCD_D4_PIN);
	GPIO_Input(LCD_D5_PORT, LCD_D5_PIN);
	GPIO_Input(LCD_D6_PORT, LCD_D6_PIN);
	GPIO_Input(LCD_D7_PORT, LCD_D7_PIN);

}
static void DataPin_Output_Mode(void){

	/* CONFIGURE LCD GPIO DATA PINS AS OUTPUT */

	GPIO_Output(LCD_D4_PORT, LCD_D4_PIN);
	GPIO_Output(LCD_D5_PORT, LCD_D5_PIN);
	GPIO_Output(LCD_D6_PORT, LCD_D6_PIN);
	GPIO_Output(LCD_D7_PORT, LCD_D7_PIN);

}

static void Write_Command_Mode(void){

	/* SETS CONTROL AND DATA GPIO PINS MODE TO GET READY TO SEND COMMAND */
	HAL_GPIO_WritePin(LCD_RS_PORT, LCD_RS_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_RW_PORT, LCD_RW_PIN, GPIO_PIN_RESET);
	DataPin_Output_Mode();

}
static void Write_Character_Mode(void){

	/* SETS CONTROL AND DATA GPIO PINS MODE TO GET READY TO SEND CHARACTER */
	HAL_GPIO_WritePin(LCD_RS_PORT, LCD_RS_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LCD_RW_PORT, LCD_RW_PIN, GPIO_PIN_RESET);
	DataPin_Output_Mode();

}
static void Read_Mode(void){

	/* SETS CONTROL AND DATA GPIO PINS MODE TO GET READY TO READ DATA */
	HAL_GPIO_WritePin(LCD_RS_PORT, LCD_RS_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_RW_PORT, LCD_RW_PIN, GPIO_PIN_SET);
	DataPin_Input_Mode();

}


static int8_t IsBusy(void){

	int8_t busyflag = 1;


	/* READS FIRST 4 BITS AND WRITE TO busyflag 1 IF BUSY OTHERWISE -1 */
	HAL_GPIO_WritePin(LCD_EN_PORT, LCD_EN_PIN, GPIO_PIN_SET);
	Delay_Us(1);

	if(HAL_GPIO_ReadPin(LCD_D7_PORT, LCD_D7_PIN) == GPIO_PIN_SET){

		busyflag = 1;

	}

	if(HAL_GPIO_ReadPin(LCD_D7_PORT, LCD_D7_PIN) == GPIO_PIN_RESET){

		busyflag = -1;

	}

	HAL_GPIO_WritePin(LCD_EN_PORT, LCD_EN_PIN, GPIO_PIN_RESET);
	Delay_Us(1);

	/* READS SECOND 4 BITS, MUST BE DONE ANYWAY */
	HAL_GPIO_WritePin(LCD_EN_PORT, LCD_EN_PIN, GPIO_PIN_SET);
	Delay_Us(1);
	HAL_GPIO_WritePin(LCD_EN_PORT, LCD_EN_PIN, GPIO_PIN_RESET);
	Delay_Us(1);

	return busyflag;

}

static void Send4bits(uint8_t Data){

	/* WRITES HIGH OR LOW OUTPUT TO DATA GPIO DEPENDING ON 4 LOW BITS OF Data */
	HAL_GPIO_WritePin(LCD_D7_PORT, LCD_D7_PIN, ((Data>>3)&0x01));
	HAL_GPIO_WritePin(LCD_D6_PORT, LCD_D6_PIN, ((Data>>2)&0x01));
	HAL_GPIO_WritePin(LCD_D5_PORT, LCD_D5_PIN, ((Data>>1)&0x01));
	HAL_GPIO_WritePin(LCD_D4_PORT, LCD_D4_PIN, ((Data>>0)&0x01));

	/* SENDS 4 LOW BITS TO LCD */
	HAL_GPIO_WritePin(LCD_EN_PORT, LCD_EN_PIN, GPIO_PIN_SET);
	Delay_Us(1);
	HAL_GPIO_WritePin(LCD_EN_PORT, LCD_EN_PIN, GPIO_PIN_RESET);
	Delay_Us(1);

}


void LCD_Send_Command(uint8_t Command){

	Write_Command_Mode();
	Delay_Us(1);

	/* SENDS 4 HIGH BITS */
	Send4bits((Command>>4)&0x0f);

	/* SENDS 4 LOW BITS */
	Send4bits((Command)&0x0f);

	/* READS BUSY FLAG AND WAITS UNTIL IT IS READY TO CARRY ON */
	Read_Mode();
	Delay_Us(1);
	while(IsBusy() != -1){
	}




}
void LCD_Send_Character(char Character){

	Write_Character_Mode();
	Delay_Us(1);

	/* SENDS 4 HIGH BITS */
	Send4bits((Character>>4)&0x0f);

	/* SENDS 4 LOW BITS */
	Send4bits((Character)&0x0f);

	/* READS BUSY FLAG AND WAITS UNTIL IT IS READY TO CARRY ON */
	Read_Mode();
	Delay_Us(1);
	while(IsBusy() != -1){
	}


}
void LCD_Send_String(char* String){

	while(*String) LCD_Send_Character(*String++);

}

void LCD_Clear(void){

	LCD_Send_Command(0x01);

}
void LCD_Goto(uint8_t row, uint8_t col){

	if(row >= 0 && row <2){
		if(col >= 0 && col <16){

			switch (row){

				case 0:
					col |= 0x80;
					break;
				case 1:
					col |= 0xC0;
					break;
			}

			LCD_Send_Command(col);

		}
	}

}

void LCD_Init(void){

	/* GPIO PORTS CLOCK ENABLE */

	__HAL_RCC_GPIOA_CLK_ENABLE(); // Trzeba zmienić tak aby było niezależne od wybranych portów


	/* CONFIGURE LCD GPIO PINS */

	GPIO_Output(LCD_RS_PORT, LCD_RS_PIN);
	GPIO_Output(LCD_RW_PORT, LCD_RW_PIN);
	GPIO_Output(LCD_EN_PORT, LCD_EN_PIN);

	DataPin_Output_Mode();

	/* STARTS INIT AND SETS 4-BITS DATA MODE */

	HAL_Delay(55);
	Send4bits(0x03);
	HAL_Delay(5);
	Send4bits(0x03);
	HAL_Delay(1);
	Send4bits(0x03);
	HAL_Delay(1);
	Send4bits(0x02);
	HAL_Delay(1);

	/* SETS BASIC PARAMETERS, FOR MORE INFO GO AND CHECK DATASHEET */

	LCD_Send_Command(0x28);
	LCD_Send_Command(0x08);
	LCD_Send_Command(0x01);
	LCD_Send_Command(0x06);
	LCD_Send_Command(0x0f);


}
