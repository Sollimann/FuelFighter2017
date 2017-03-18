#pragma once
/*******************************************************************************************
Hensikt: Skal regulere fartøyets posisjon.

Metode:	 Regulerer fartøyets posisjon så lenge programmets løpetid ikke er overskredet.
		 Benytter en PID-algoritme beregner nødvendig pådrag i forhold til ønsket posisjon.
		 Resultater skrives til skjerm og filer.
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
dt_clock					O		double						Tid brukt i hver itterasjon


Interne Variable:
Navn							Type						Innhold/Beskrivelse
--------------------------------------------------------------------------------------------
dt								double						Tid før ny servoposisjon beregnes					
error_past						double						Feil 30 itterasjoner tilbake
addtime							double
itterations						int
memory							int
counter							int							
desired_pos						int							Posisjon relativt startposisjon
current_pos						int							Posisjon fra potensiometer
init_pos						int							Posisjon relativt fartøy
maxTime							double						Maks kjøretid for regulering
runTime							double						Kjøretid

Rutiner som kalles:
Navn														Hensikt
--------------------------------------------------------------------------------------------
regulate()

*********************************************************************************************
Programmert av: Kristoffer Rakstad Solberg
Dato: 08.03.2017
********************************************************************************************/
//#define _CRT_SECURE_NO_DEPRECATE
#include "stdafx.h"
#include "Header.h"

//void regulate(&PIDgains, &answer, userpositions, &npositions)
int regulate(struct Gains gainvalues) {
//...........................................................................................
//.......................................Deklarering.........................................

	double Thrust, error_last, i_Temp;
	double maxTime, runTime;
	double dt_clock;
	int counter;

	//Startverdier
	dt_clock = 0;
	maxTime = 200000000000000;
	runTime = 0;
	i_Temp = 0;
	error_last = 0;
	counter = 0;
	start_t = 0;
	//itterations = 0;
	//i_save = 0;

	//Åpner motor og potensometer


	CPhidgetInterfaceKitHandle ifKit = openifKit();
	CPhidgetServoHandle servo = openServo();

/********************************************************************************************/
/************************************** Sett posisjon ***************************************/

	printf("Skriv inn ønsket posisjon fra 0-100: ");
	int answer = 0;
	scanf_s(" %d", &answer);

	int desired_pos = (answer *lP/ 100);		//Ønsket posisjon skalert ift bassenglengde																	////
	int init_pos = 0;								//Posisjon ved oppstart
	int current_pos = 0;							//Poisjon relativt oppstartsposisjon

	//Henter startposisjoner
	CPhidgetInterfaceKit_getSensorValue(ifKit, 2, &init_pos);

	//Setter referanseposisjon riktig ift startposisjon for potensiometer
	desired_pos = init_pos - desired_pos;
	int posd = 0;
	posd = desired_pos;


/********************************************************************************************/
/************************************** Åpne tekstfil ***************************************/

	//Åpner filer som skal skrives til 
	//FILE *vtf; 
	//vtf = fopen("boat.vtf","w"); 
	FILE *gnu; 
	gnu= fopen("plot.dat","w");

	//fprintf(vtf, "%fl\t%d\t%d\t%d\t%d\n", sec, y_0 - y_new, initial_pos - y_new, initial_pos - y_0, (93 - u) * 10);



/**********************************************************************************************/
/**************************************** Regulator *******************************************/
	t_1 = clock();
	while (!(answer==0)) {

		//Loop kjører frem til satt tidsbegrensning på 20sekunder
		//while (runTime < maxTime) {
		while (!_kbhit()) {
			
			t_2 = clock();
			start_t = clock();

			runTime = (t_2 - t_1)/1000.0;
		


			//start_t = clock();

				//Compute()
				CPhidgetInterfaceKit_getSensorValue(ifKit, 2, &current_pos);

				Thrust = compute(&current_pos, &desired_pos, &i_Temp,
					gainvalues, &dt_clock, &counter, &error_last);

				CPhidgetServo_setPosition(servo, 0, Thrust);

				Sleep(50);
			end_t = clock();

			//t_clock = (end_t - start_t) /1000.0;
			//dt_clock = (float)(end_t - start_t)/CLOCKS_PER_SEC;
			dt_clock = (end_t - start_t) / 1000.0;
			counter++;
			
			//printf("\n counter: %d\t| runtime: %lf\t| dt: %lf\t| end: ", counter,
				//runTime, dt_clock);

			fprintf(gnu, "%f\t%d\t%d\t%d\t%f\n", runTime, error_last, current_pos,
				desired_pos, Thrust);

		} //while kbhit()

	printf("Skriv inn ny posisjon fra 0-100: ");
	scanf_s(" %d", &answer);
	desired_pos = (answer *lP/ 100);		//Ønsket posisjon skalert ift bassenglengde
	desired_pos = init_pos - desired_pos;
	Sleep(1500);
	}//While REGULATOR


/********************************************************************************************/
/************************************** Lukke tekstfil **************************************/

	//fclose(vtf); 
	fclose(gnu);

	closePhidgets(&servo, &ifKit);
	return 0;
}
