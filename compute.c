#pragma once
/*******************************************************************************************
Hensikt: Foretar utregning av neste servoverdi basert på forrige posisjon.

Metode:	 Tar inn forsterkningsverdier Kp, Ki og Kp fra main. Et påsatt lavpassfilter sørger
		 for å skille ut store sprang i verdier og gir en jevnere output.
********************************************************************************************
Parametre:
Navn					I/O		Type						Innhold/Beskrivelse
--------------------------------------------------------------------------------------------
*gainvalues				I		struct objekt				Peker til struct med gainverdier
*answer					I		int							Valg av reguleringsalgoritme
userposition[]			I		int							Array med et sett av posisjoner
npositions				I		int							Ant posisjoner i posisjonsarray
servo_pos				I/O		double						Faktisk posisjon til servo
servo_Temp				I/O		double						Posisjon ved forrige itterasjon
i_Temp					I/O		double						Akkumulert feil over itterasjoner
d_Temp					I/O		double						Feil ved forrige itterasjon
dt_clock				O		double						Tid brukt i hver itterasjon
error_past				O		double						Feil 30 itterasjoner tilbake


Interne Variable:
Navn							Type						Innhold/Beskrivelse
--------------------------------------------------------------------------------------------
dt								double						Tid før ny servoposisjon beregnes
dt_clock						double
addtime							double
i_max							int							Maks størrelse på integralledd
i_min							int							Minimum størrelse på integralledd
itterations						int
memory							int
counter							int
desired_pos						int							Ønsket posisjon relativt karet
current_pos						int							Posisjon fra potensiometer
init_pos						int							Start/initielle posisjon 


Rutiner som kalles:
Navn														Hensikt
--------------------------------------------------------------------------------------------
regulate()

*********************************************************************************************
Programmert av: Kristoffer Rakstad Solberg
Dato: 08.03.2017
*********************************************************************************************/
#include "stdafx.h"
#include "Header.h"

double compute(int *current_pos, int *desired_pos, int *i_Temp, struct Gains gainvalues, 
	double *dt_clock, int *counter, int *error_last) {

//...........................................................................................
//.......................................Deklarering.........................................

	double P_val, I_val, D_val;							//PID-variable
	double servo_Temp;
	double servo_Thrust;
	int error;
	int i_max = 30;										//Gyldighetsområde maks lengde
	int i_min = -30;									//Gyldigshetsområde min lengde
	int MINSPEED = 93;									//minimumfart på en gitt dag
	int MAXSPEED = 85;									//maksfart på en gitt dag
	int TOP_RANGE = 150;								//TOPP for overladning av thrust
	int LOW_RANGE = 0;									//BUNN for overladning av thrust

	error = (*current_pos-*desired_pos);			//Distanse fra ønsket posisjon

/**********************************************************************************************/
/****************************************** P-term ********************************************/

	P_val = (gainvalues.Kp) * (error);

/**********************************************************************************************/
/****************************************** I-term ********************************************/
	float klokke = *dt_clock;
	//Ny integral-error beregnes fra forrige gjennomkjøring 
	*i_Temp = *i_Temp + (error * (*dt_clock)) *(gainvalues.Ki);
	I_val = *i_Temp;


/**********************************************************************************************/
/**************************************** Lavpassfilter ***************************************



										  lavpassfilter
										 ----------------
							u(s)		 |H	~~~~//~~~~	|		y(s)
					-------------------->				 ------------------->
										 |L	~~~~~~~~~~	|
  										 --------------*/

	//Sørger for at integralleddet holder seg innenfor gyldighetsområde

	if (I_val > i_max) {
		I_val = i_max;
	}
	else if (I_val < i_min) {
		I_val = i_min;
	}

	//Sørger for at derivatleddet er innenfor

	if ((*counter > 200) && abs(*error_last - error) > 100 ) {
		*error_last = error;
	}

/**********************************************************************************************/
/****************************************** D-term ********************************************/
	//&& ((*counter%30==0)
	if(dt_clock!=0){
	D_val = gainvalues.Kd*((error - *error_last) / (*dt_clock));
	}
/**********************************************************************************************/
/************************************** Anti-overladning **************************************/
	
	servo_Temp = (P_val +I_val + D_val);

	//Counter intuitive
	if (servo_Temp < LOW_RANGE) {
		servo_Temp = LOW_RANGE;
	}
	else if (servo_Temp > TOP_RANGE) {
		servo_Temp = TOP_RANGE;
	}


/**********************************************************************************************/
/**************************************** Servo output ****************************************/
	
	//servoverdi skaleres innenfor en topp- og bunngrense
	servo_Temp = MINSPEED - (servo_Temp / (TOP_RANGE - LOW_RANGE))*(MINSPEED-MAXSPEED);
	servo_Thrust = servo_Temp;
	
	//Henter ut error 10 itterasjoner tilbake
	if (((*counter) % 10) == 0) {
		*error_last = error;
	}

	//Skriver ut hver 10-ende itterasjon
	if (((*counter) % 10) == 0) {

		//printf("\n %d\t| Des: %d\t| Cur: %d\t| last_e: %d\t| e: %d\t| servo: %f\t| P: %f\t| I: %f\t| D: %f\t \n"
		//	,*counter,*desired_pos,*current_pos,*error_last,error,servo_Thrust,P_val,I_val,D_val);

		printf("\n %d\t| Des: %d\t| Cur: %d\t| e: %d\t| servo: %f\t| P: %f\t| I: %f\t| D: %f\t \n"
			, *counter, *desired_pos, *current_pos, error, servo_Thrust, P_val, I_val, D_val);
		
		//printf(" %d\t| Des: %d\t| Cur: %d\t| last_e: %d\t| e: %d\t| servo: %f\t \n"
			//, *counter, *desired_pos, *current_pos, *error_last, error, servo_Thrust);

		//printf("Des: %d\t| Cur: %d\t| last_e: %d\t| e: %d\t\n"
		//	, *desired_pos, *current_pos, *error_last, error);

		//printf("dt: %f\t| last_e: %d\t| e: %d\t\n"
		//	, *dt_clock, *error_last, error);
		
	}
	return servo_Thrust;
}
