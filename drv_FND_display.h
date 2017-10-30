/*
 * drv_FND_display.h
 *
 *  Created on: 2017. 9. 21.
 *      Author: hrjung
 */

#ifndef DRV_FND_DISPLAY_H_
#define DRV_FND_DISPLAY_H_

/*******************************************************************************
 * CONSTANTS
 */


/*******************************************************************************
 * EXTERNS
 */

extern int FND_getStrCode(char *text_in, uint8_t *code_out);
extern int FND_getDecimalCode(int value, uint8_t *code_out);
extern int FND_getHexaCode(uint16_t value, uint8_t *code_out);
extern int FND_getFloatCode(float value, uint8_t *code_out);
extern void FND_putCode(uint8_t code, uint8_t digit_pos);

#endif /* DRV_FND_DISPLAY_H_ */
