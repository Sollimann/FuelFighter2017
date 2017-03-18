#pragma once
/*******************************************************************************************
Hensikt: Koble til Servo og InterfaceKit slik at disse kan styres/leses av.
		 Gir tilbakemelding om tilkoblingen lykkes eller ikke.

Metode:  Lager InterfaceKit - og Servo-objekt og kobler til disse ved bruk av innebygde
		 funksjoner fra phidgets21.h
********************************************************************************************
Parametre:
Navn					I/O		Type						Innhold/Beskrivelse
--------------------------------------------------------------------------------------------
PIDgains				O		struct objekt				Forsterkningsverdi for PID-reg
PIgains					O		struct objekt				Forsterkningsverdi for PI-reg
Pgains					O		struct objekt				Forsterkningsverdi for P-reg
PIDngains				O		struct objekt				Forsterkningsverdi for PIDn-reg

Interne Variable:
Navn							Type						Innhold/Beskrivelse
--------------------------------------------------------------------------------------------
ifKit							struct objekt	
servo							struct objekt

Rutiner som kalles:
Navn														Hensikt
--------------------------------------------------------------------------------------------
openPhidgets();							
closePhidgets();											Registrer avløsning med kit
malloc()
regulate();													
*********************************************************************************************
Programmert av: Kristoffer Rakstad Solberg
Dato: 08.03.2017
*********************************************************************************************/
#include "stdafx.h"
#include "Header.h"


int main(void) {

//..........................................................................................//
//..........................................Start-up........................................//
	
	

//..........................................................................................//
//..............................Deklarerer forsterkningverdier/gains........................//

	struct Gains PIDgains, PIgains, Pgains;

	//PID
	PIDgains.Kp = 6.0;
	PIDgains.Ki = 1.2;
	PIDgains.Kd = 0.3;

	//PI
	PIgains.Kp = 1.4;
	PIgains.Ki = 0.6;
	PIgains.Kd = 0;				//ingen derivatledd

	//P
	Pgains.Kp = 6.0;
	Pgains.Ki = 0;				//Ingen intergratorledd
	Pgains.Kd = 0.7;				//Ingen derivatladd
	
	
//.........................................................................................//
//........................Array for brukerdefinerte måleposisjoner.........................//

//AVVENTER!!

	

//........................................................................................//
//..........................................MENY..........................................//
	printf("\n\n");
	printf("******************************************************************\n");
	printf("************************ Test din regulator **********************\n");
	printf("******************************************************************\n");

	printf("\t Velg et alternativ: \n\n");
	printf("\t Tast 1 for: Dynamisk posisjonering med PID-regulering\n");
	printf("\t Tast 2 for: Dynamisk posisjonering med PI-regulering\n");
	printf("\t Tast 3 for: Dynamisk posisjonering med P-regulering\n");
	printf("\t Tast 4 for: Dynamisk posisjonering med PIDn-regulering\n");
	printf("\t Tast P for: Skriv ut plott fra siste kjøring\n");
	printf("\t Tast Q for: Avslutt program\n");

	int answer = 0;
	scanf_s(" %d", &answer);

	switch (answer)
	{
	case 1:
		regulate(PIDgains);
		break;
	case 2:
		regulate(PIgains);
		break;
	case 3:
		regulate(Pgains);
		break;
	case 'p': case 'P':
		printf("\n Plotting... \n");
		break;
	case 'q': case 'Q':
		printf("\n Quitting... \n");
		//closePhidgets(&servo, &ifKit); //shut-down
		break;
	default:
		printf("\n  not respond correctly...");
		break;
	} //switch
	
	//closePhidgets(&servo, &ifKit);
}
