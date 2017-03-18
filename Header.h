#pragma once
/*******************************************************************************************
Hensikt: Deklarere variable og funksjoner. Definere konstanter.
********************************************************************************************
Parametre:
Navn					I/O		Type						Innhold/Beskrivelse
--------------------------------------------------------------------------------------------


Interne Variable:
Navn							Type						Innhold/Beskrivelse
--------------------------------------------------------------------------------------------

Rutiner som kalles:
Navn														Hensikt
--------------------------------------------------------------------------------------------

--------------------------------------------------------------------------------------------
Funksjoner som deklareres:
Navn														Hensikt
--------------------------------------------------------------------------------------------
CPhidget_set_OnAttach_Handler()								Registrer kontakt med kit
CPhidget_set_OnDetach_Handler()								Registrer avløsning med kit
CPhidget_set_OnError_Handler()								Registrer error ved til-/avkobling
*********************************************************************************************
Programmert av: Kristoffer Rakstad Solberg
Dato: 08.03.2017
*********************************************************************************************/
#include "stdafx.h"				/* Precompiled header file*/
#include <stdio.h>				/* printf, scanf, NULL */
#include <stdlib.h>				/* malloc, free, rand */
#include <time.h>				/* clock_t */
#include <phidget21.h>			/* Phidget integrated functions*/
#include <conio.h>				/* kbhit() */
#include <Windows.h>			/* Sleep(milliseconds) */


/**********************************************************************************************/
/****************************************** Define ********************************************/

//Hoveddimensjoner
#define lB 100 //Lengde bât
#define wB 30  //Bredde båt
#define hB 40  //Hoyde båt
#define lP 270 //Lengde basseng
#define wP 80  //Bredde basseng

/**********************************************************************************************/
/**************************************** Deklarering *****************************************/

//CPhidgetServoHandle servo = 0;
//CPhidgetInterfaceKitHandle ifKit = 0;

/**********************************************************************************************/
/****************************************** Struct ********************************************/

//Forsterkningsgrad
struct Gains {
	double Kd;
	double Kp;
	double Ki;
};

//Klokke
clock_t start_t, end_t,t_1, t_2, total_t;


/**********************************************************************************************/
/**************************************** Funksjoner ******************************************/

//void openPhidgets(CPhidgetServoHandle *servo, CPhidgetInterfaceKitHandle *ifKit);
CPhidgetInterfaceKitHandle openifKit();
CPhidgetServoHandle openServo();

void closePhidgets(CPhidgetServoHandle *servo, CPhidgetInterfaceKitHandle *ifKit);

int regulate(struct Gains gainvalues);

double compute(int *current_pos, int *desired_pos, int *i_Temp, struct Gains gainvalues, 
	double *dt_clock, int *counter, int *error_last);

void plot();

