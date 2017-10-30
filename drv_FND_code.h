/*
 * drv_FND_code.h
 *
 *  Created on: 2017. 9. 19.
 *      Author: hrjung
 */

#ifndef DRV_FND_CODE_H_
#define DRV_FND_CODE_H_


/*******************************************************************************
 * CONSTANTS
 */

#define FND_DISP_DIGITS		4
#define FND_DOT_POSITION	2

#define FND_CATHOD_COMMON


#ifdef FND_CATHOD_COMMON

#define	FND_CHAR_DOT	0x80

#define FND_CHAR_0		0x3F
#define FND_CHAR_1		0x06
#define FND_CHAR_2		0x5B
#define FND_CHAR_3		0x4F
#define FND_CHAR_4		0x66
#define FND_CHAR_5		0x6D
#define FND_CHAR_6		0x7D
#define FND_CHAR_7		0x07
#define FND_CHAR_8		0x7F
#define FND_CHAR_9		0x6F

#define FND_CHAR_0_DOT	0xBF
#define FND_CHAR_1_DOT	0x86
#define FND_CHAR_2_DOT	0xDB
#define FND_CHAR_3_DOT	0xCF
#define FND_CHAR_4_DOT	0xE6
#define FND_CHAR_5_DOT	0xED
#define FND_CHAR_6_DOT	0xFD
#define FND_CHAR_7_DOT	0x87
#define FND_CHAR_8_DOT	0xFF
#define FND_CHAR_9_DOT	0xEF

#define FND_CHAR_A		0x77
#define FND_CHAR_B		0x7C
#define FND_CHAR_C		0x39
#define FND_CHAR_D		0x5E
#define FND_CHAR_E		0x79
#define FND_CHAR_F		0x71
#define FND_CHAR_G		0x3D
#define FND_CHAR_H		0x76
#define FND_CHAR_I		0x30
#define FND_CHAR_J		0x0E
#define FND_CHAR_K		0x7A
#define FND_CHAR_L		0x38
#define FND_CHAR_M		0x15
#define FND_CHAR_N		0x54
#define FND_CHAR_O		0x5C
#define FND_CHAR_P		0x73
#define FND_CHAR_Q		0x67
#define FND_CHAR_R		0x50
#define FND_CHAR_S		0x2D
#define FND_CHAR_T		0x78
#define FND_CHAR_U		0x3E
#define FND_CHAR_V		0x1C
#define FND_CHAR_W		0x2A
#define FND_CHAR_X		0x64
#define FND_CHAR_Y		0x6E
#define FND_CHAR_Z		0x49

#define FND_CHAR_SP		0x00
#define FND_CHAR_BAR	0x40
#else

#define FND_CHAR_0		0xC0
#define FND_CHAR_1		0xF9
#define FND_CHAR_2		0xA4
#define FND_CHAR_3		0xB0
#define FND_CHAR_4		0x99
#define FND_CHAR_5		0x92
#define FND_CHAR_6		0x82
#define FND_CHAR_7		0xF8
#define FND_CHAR_8		0x80
#define FND_CHAR_9		0x90

#define FND_CHAR_A		0x88
#define FND_CHAR_B		0x83
#define FND_CHAR_C		0xC6
#define FND_CHAR_D		0xA1
#define FND_CHAR_E		0x86
#define FND_CHAR_F		0x8E
#define FND_CHAR_G		0xC2
#define FND_CHAR_H		0x89
#define FND_CHAR_I		0xCF
#define FND_CHAR_J		0xF1
#define FND_CHAR_K		0x85
#define FND_CHAR_L		0xC7
#define FND_CHAR_M		0xEA
#define FND_CHAR_N		0xAB
#define FND_CHAR_O		0xA3
#define FND_CHAR_P		0x8C
#define FND_CHAR_Q		0x98
#define FND_CHAR_R		0xAF
#define FND_CHAR_S		0xD2
#define FND_CHAR_T		0x87
#define FND_CHAR_U		0xC1
#define FND_CHAR_V		0xE3
#define FND_CHAR_W		0xD5
#define FND_CHAR_X		0x9B
#define FND_CHAR_Y		0x91
#define FND_CHAR_Z		0xB6

#define FND_CHAR_SP		0xFF
#endif


/*******************************************************************************
 * EXTERNS
 */

#endif /* DRV_FND_CODE_H_ */
