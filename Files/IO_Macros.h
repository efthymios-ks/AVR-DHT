#ifndef IO_MACROS_H_INCLUDED
#define IO_MACROS_H_INCLUDED
/*
||
||  Filename:	 		IO_Macros.h
||  Title: 			    IO manipulation macros
||  Author: 			Efthymios Koktsidis
||	Email:				efthymios.ks@gmail.com
||  Compiler:		 	AVR-GCC
||	Description:		This library contains macros for 
||						easy port manipulation (similar 
||						to Arduino).
||
||	Demo:
|| 1.	#define LED		A, 0		|| 6. 	PinModeToggle(BUTTON);
|| 2.	#define BUTTON	A, 1		|| 7. 	DigitalWrite(LED, LOW);
|| 3.								|| 8. 	DigitalWrite(LED, HIGH);
|| 4. 	PinMode(BUTTON, OUTPUT);	|| 9. 	DigitalLevelToggle(LED);
|| 5. 	PinMode(LED, OUTPUT);		||10.	int a = DigitalRead(BUTTON);
||
*/
#include <avr/io.h>

//----- I/O Macros -----
//Macros to edit PORT, DDR and PIN
#define PinMode(			x, y)	( 		y 			?	_SET(DDR, x)	:	_CLEAR(DDR, x)		)
#define DigitalWrite(		x, y)	( 		y 			?	_SET(PORT, x)	:	_CLEAR(PORT, x)		)
#define DigitalRead(		x)		(						_GET(PIN, x)							)
#define PinModeToggle(		x)		(						_TOGGLE(DDR, x)							)
#define DigitalLevelToggle(	x)		(						_TOGGLE(PORT, x)						)

//General use bit manipulating commands
#define BitSet(		x, y)			(	x |=	 (1UL<<y)			)
#define BitClear(	x, y)			(	x &=	(~(1UL<<y))			)
#define BitToggle(	x, y)			(	x ^=	 (1UL<<y)			)
#define BitCheck(	x, y)			(	x &		 (1UL<<y)	? 1 : 0	)

//Access PORT, DDR and PIN
#define PORT(	port)				(_PORT(	port))
#define DDR(	port)				(_DDR(	port))
#define PIN(	port)				(_PIN(	port))

#define _PORT(	port)				(PORT##	port)
#define _DDR(	port)				(DDR##	port)
#define _PIN(	port)				(PIN##	port)

#define _SET(	type, port, bit)	(	BitSet(		(type##port),	bit)	)
#define _CLEAR(	type, port, bit)	(	BitClear(	(type##port),	bit)	)
#define _TOGGLE(type, port, bit)	(	BitToggle(	(type##port),	bit)	)
#define _GET(	type, port, bit)	(	BitCheck(	(type##port),	bit)	)

//Definitions
#define Input		0
#define Output		!Input
#define Low			0
#define High		!Low
#define False		0
#define True		!False
//------------------
#endif
