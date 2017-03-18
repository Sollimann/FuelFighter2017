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
isKit					I		struct objekt				Peker til angitt IK-objekt
servo					I		struct objekt				Peker til angitt servo-objekt

Interne variable:
Navn							Type						Innhold/Beskrivelse
--------------------------------------------------------------------------------------------
error							int							Error true/false
errorMessage					char


Rutiner som kalles:
Navn														Hensikt
--------------------------------------------------------------------------------------------
CPhidgetInterfaceKit_create()								Lager objekt for interface kit
CPhidgetServo_create()										Lager objekt for servo
CPhidget_open()												Åpner en phidget-enhet
CPhidget_getErrorDescription()								Gir beskrivelse av en errorkode
CPhidget_waitForAttachment()								Setter tid for enhetstilkobling
CPhidgetServo_setEngaged()									Skrur på motoren
CPhidget_close()											Lukker en phidget-enhet
CPhidget_delete()											Sletter en phidget-enhet
*********************************************************************************************
Programmert av: Kristoffer Rakstad Solberg
Dato: 08.03.2017
*********************************************************************************************/
#include "stdafx.h"
#include "Header.h"

/********************************************************************************************/
/******************************************* Åpne *******************************************/

CPhidgetInterfaceKitHandle openifKit() {

	//..........................................................................................//
	//......................................Åpne Intefacekit....................................//

		//Deklarasjon av interne variable
	int error;
	const char *errorMessage;

	//Lager en "interfacehandle"
	CPhidgetInterfaceKitHandle ifKit = 0;

	//Lager objekt for interfacekit
	CPhidgetInterfaceKit_create(&ifKit);

	//Åpner interfacekit
	//"-1" spesifiserer vilkårlig serienummer
	CPhidget_open((CPhidgetHandle)ifKit, -1);

	//Setter tidsbegrensning for tilkobling av enhet
	printf("Venter på at <<Interface kit>> skal tilkobles...\n");
	if ((error = CPhidget_waitForAttachment((CPhidgetHandle)ifKit, 8000)))
	{
		CPhidget_getErrorDescription(error, &errorMessage);
		printf("Problemer oppstod ved tilkobling: %s\n", errorMessage);
		return 0;
	}
	printf("Tilkobling av enhet lykkes\n");

	return ifKit;
}
//........................................................................................//
//........................................Åpne servo......................................//

CPhidgetServoHandle openServo() {
	//Deklarasjon av interne variable
	int error;
	const char *errorMessage;

	//Lager en "interfacehandle"
	CPhidgetServoHandle servo = 0;

	//Lager objekt for interfacekit
	CPhidgetServo_create(&servo);

	CPhidget_open((CPhidgetHandle)servo, -1);

	//Setter tidsbegrensning for tilkobling av enhet
	printf("Venter på at <<Servo controller>> skal tilkobles...\n");
	if ((error = CPhidget_waitForAttachment((CPhidgetHandle)servo, 8000)))
	{
		CPhidget_getErrorDescription(error, &errorMessage);
		printf("Problemer oppstod ved tilkobling: %s\n", errorMessage);
		return 0;
	}
	printf("Tilkobling av enhet lykkes\n");

	return servo;
}

/**********************************************************************************************/
/************************************** Lukke & slette ****************************************/

void closePhidgets(CPhidgetServoHandle *servo, CPhidgetInterfaceKitHandle *ifKit) {

	//Lukker og sletter "interfacehandle"
	CPhidget_close((CPhidgetHandle)*ifKit);
	CPhidget_delete((CPhidgetHandle)*ifKit);

	//Lukker og sletter "servohandle"
	CPhidget_close((CPhidgetHandle)*servo);
	CPhidget_delete((CPhidgetHandle)*servo);

	return;
}
