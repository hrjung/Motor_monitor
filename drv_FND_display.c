/*
 * drv_FND_display.c
 *
 *  Created on: 2017. 9. 19.
 *      Author: hrjung
 */
#include <xdc/runtime/Error.h>

#include <ti/drivers/Power.h>
#include <ti/drivers/power/PowerCC26XX.h>
#include <ti/drivers/pin/PINCC26XX.h>
#include <ti/sysbios/BIOS.h>

#include <string.h>
#include "icall.h"
#include "hal_assert.h"
#include "bcomdef.h"

#include "drv_FND_code.h"
#include "Board.h"

/*******************************************************************************
 * MACROS
 */


/*******************************************************************************
 * CONSTANTS
 */


/*******************************************************************************
 * TYPEDEFS
 */

/*******************************************************************************
 * LOCAL VARIABLES
 */


/*******************************************************************************
 * LOCAL FUNCTIONS
 */



/*******************************************************************************
 * GLOBAL VARIABLES
 */

/*******************************************************************************
 * EXTERNS
 */

extern PIN_Handle hGpioPins;

/*
 *  ======== local function ========
 */

uint8_t FND_getIntCode(uint8_t digit)
{
	uint8_t code;

	switch(digit)
	{
	case 1 : code = FND_CHAR_1; break;
	case 2 : code = FND_CHAR_2; break;
	case 3 : code = FND_CHAR_3; break;
	case 4 : code = FND_CHAR_4; break;
	case 5 : code = FND_CHAR_5; break;
	case 6 : code = FND_CHAR_6; break;
	case 7 : code = FND_CHAR_7; break;
	case 8 : code = FND_CHAR_8; break;
	case 9 : code = FND_CHAR_9; break;
	case 10 : code = FND_CHAR_A; break;
	case 11 : code = FND_CHAR_B; break;
	case 12 : code = FND_CHAR_C; break;
	case 13 : code = FND_CHAR_D; break;
	case 14 : code = FND_CHAR_E; break;
	case 15 : code = FND_CHAR_F; break;
	case 0 : code = FND_CHAR_0; break;
	default : code = FND_CHAR_SP; break;
	}

	return code;
}

uint8_t FND_getCharCode(uint8_t ch)
{
	uint8_t code;

	switch(ch)
	{
	case '0' : code = FND_CHAR_0; break;
	case '1' : code = FND_CHAR_1; break;
	case '2' : code = FND_CHAR_2; break;
	case '3' : code = FND_CHAR_3; break;
	case '4' : code = FND_CHAR_4; break;
	case '5' : code = FND_CHAR_5; break;
	case '6' : code = FND_CHAR_6; break;
	case '7' : code = FND_CHAR_7; break;
	case '8' : code = FND_CHAR_8; break;
	case '9' : code = FND_CHAR_9; break;

	case 'A' :
	case 'a' : code = FND_CHAR_A; break;
	case 'B' :
	case 'b' : code = FND_CHAR_B; break;
	case 'C' :
	case 'c' : code = FND_CHAR_C; break;
	case 'D' :
	case 'd' : code = FND_CHAR_D; break;
	case 'E' :
	case 'e' : code = FND_CHAR_E; break;
	case 'F' :
	case 'f' : code = FND_CHAR_F; break;
	case 'G' :
	case 'g' : code = FND_CHAR_G; break;
	case 'H' :
	case 'h' : code = FND_CHAR_H; break;
	case 'I' :
	case 'i' : code = FND_CHAR_I; break;
	case 'J' :
	case 'j' : code = FND_CHAR_J; break;
	case 'K' :
	case 'k' : code = FND_CHAR_K; break;
	case 'L' :
	case 'l' : code = FND_CHAR_L; break;
	case 'M' :
	case 'm' : code = FND_CHAR_M; break;
	case 'N' :
	case 'n' : code = FND_CHAR_N; break;
	case 'O' :
	case 'o' : code = FND_CHAR_O; break;
	case 'P' :
	case 'p' : code = FND_CHAR_P; break;
	case 'Q' :
	case 'q' : code = FND_CHAR_Q; break;
	case 'R' :
	case 'r' : code = FND_CHAR_R; break;
	case 'S' :
	case 's' : code = FND_CHAR_S; break;
	case 'T' :
	case 't' : code = FND_CHAR_T; break;
	case 'U' :
	case 'u' : code = FND_CHAR_U; break;
	case 'V' :
	case 'v' : code = FND_CHAR_V; break;
	case 'W' :
	case 'w' : code = FND_CHAR_W; break;
	case 'X' :
	case 'x' : code = FND_CHAR_X; break;
	case 'Y' :
	case 'y' : code = FND_CHAR_Y; break;
	case 'Z' :
	case 'z' : code = FND_CHAR_Z; break;
	default  : code = FND_CHAR_SP; break;
	}

	return code;
}


/*
 *  ======== public function ========
 */

int FND_getStrCode(char *text_in, uint8_t *code_out)
{
	int len = strlen(text_in);
	int i, diff=0;

	if(len > FND_DISP_DIGITS) return 1;

	if(len < FND_DISP_DIGITS)
	{
		diff = FND_DISP_DIGITS-len;
		for(i=0; i<diff; i++)
			code_out[i] = FND_CHAR_SP;
	}
	else
		diff = 0;

	for(i=0; i<len; i++)
		code_out[i+diff] = FND_getCharCode(text_in[i]);

	return 0;
}


int FND_getDecimalCode(int value, uint8_t *code_out)
{
	int i, remain;
	int conv_value = value;

	for(i=0; i<FND_DISP_DIGITS; i++)
	{
		if(conv_value == 0)
		{
			remain = 20; // dummy for SP
		}
		else
		{
			remain = conv_value%10;
		}
		code_out[FND_DISP_DIGITS-1-i] = FND_getIntCode(remain);
		conv_value /= 10;
	}

	return 0;
}

int FND_getHexaCode(uint16_t value, uint8_t *code_out)
{
	int i, remain;
	uint8_t conv_value = value;

	for(i=0; i<FND_DISP_DIGITS; i++)
	{
		if(conv_value == 0)
		{
			remain = 20; // dummy for SP
		}
		else
		{
			remain = conv_value%16;
		}
		code_out[FND_DISP_DIGITS-1-i] = FND_getIntCode(remain);
		conv_value /= 16;
	}

	//UARTprintf(" %d = %d %d %d %d %d %d\n", value, code_out[0], code_out[1], code_out[2], code_out[3], code_out[4], code_out[5]);

	return 0;
}

int FND_getFloatCode(float value, uint8_t *code_out)
{
	int i, remain;
	float conv_value = value;
	long l_value;

	if(value < 0.0) conv_value = (-1)*value; // make positive value

	// display range
	if(conv_value > 99.99 || (conv_value < 0.01 && conv_value != 0.0)) return 1;

	l_value = (long)(conv_value * 100.0);
	for(i=0; i<FND_DISP_DIGITS; i++)
	{
		if(l_value == 0)
		{
			remain = 20; // dummy for SP
		}
		else
		{
			remain = l_value%10;
		}
		code_out[FND_DISP_DIGITS-1-i] = FND_getIntCode(remain);
		if(i == FND_DOT_POSITION) code_out[FND_DISP_DIGITS-1-i] |= FND_CHAR_DOT;
		l_value /= 10.0;
	}

	//UARTprintf(" %f = %d %d %d %d %d %d\n", value, code_out[0], code_out[1], code_out[2], code_out[3], code_out[4], code_out[5]);

	return 0;
}

void FND_putCode(uint8_t code, uint8_t digit_pos)
{
	// write code to FND
	if(code&0x01) PIN_setOutputValue(hGpioPins, Board_FND_A, 1);
	else PIN_setOutputValue(hGpioPins, Board_FND_A, 0);

	if(code&0x02) PIN_setOutputValue(hGpioPins, Board_FND_B, 1);
	else PIN_setOutputValue(hGpioPins, Board_FND_B, 0);

	if(code&0x04) PIN_setOutputValue(hGpioPins, Board_FND_C, 1);
	else PIN_setOutputValue(hGpioPins, Board_FND_C, 0);

	if(code&0x08) PIN_setOutputValue(hGpioPins, Board_FND_D, 1);
	else PIN_setOutputValue(hGpioPins, Board_FND_D, 0);

	if(code&0x10) PIN_setOutputValue(hGpioPins, Board_FND_E, 1);
	else PIN_setOutputValue(hGpioPins, Board_FND_E, 0);

	if(code&0x20) PIN_setOutputValue(hGpioPins, Board_FND_F, 1);
	else PIN_setOutputValue(hGpioPins, Board_FND_F, 0);

	if(code&0x40) PIN_setOutputValue(hGpioPins, Board_FND_G, 1);
	else PIN_setOutputValue(hGpioPins, Board_FND_G, 0);

	if(code&0x80) PIN_setOutputValue(hGpioPins, Board_FND_DP, 1);
	else PIN_setOutputValue(hGpioPins, Board_FND_DP, 0);

	// disable FND
	PIN_setOutputValue(hGpioPins, Board_FND_EN1, 0);
	PIN_setOutputValue(hGpioPins, Board_FND_EN2, 0);
	PIN_setOutputValue(hGpioPins, Board_FND_EN3, 0);
	PIN_setOutputValue(hGpioPins, Board_FND_EN4, 0);

	switch(digit_pos)
	{
	case 0 : PIN_setOutputValue(hGpioPins, Board_FND_EN1, 1); break;
	case 1 : PIN_setOutputValue(hGpioPins, Board_FND_EN2, 1); break;
	case 2 : PIN_setOutputValue(hGpioPins, Board_FND_EN3, 1); break;
	case 3 : PIN_setOutputValue(hGpioPins, Board_FND_EN4, 1); break;
	default : break;
	}
}
