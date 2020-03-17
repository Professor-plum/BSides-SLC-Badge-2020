/*
 * flags.h
 *
 *  Created on: Nov 15, 2019
 *      Author: plum
 */

#ifndef INC_FLAGS_H_
#define INC_FLAGS_H_

#include <stdint.h>
#include <stdbool.h>

typedef enum {
 FLAG_VOLT		,	//Solved volt puzzle
 FLAG_HIT_BOOKS	,	//User has hit the books
 FLAG_UNK		,
 FLAG_EMAIL		,	//User has received the email
 FLAG_MANAGER	,	//Manager has left the room
 FLAG_KEYCARD	,	//User picked up the keycard
 FLAG_STEGO		,	//User has stego picture
 FLAG_BOX_OPEN	,	//User has opened lock box

 FLAG_FIRE	,
 FLAG_SCREW		,	//User has picked up screwdriver
 FLAG_PANEL		,	//User has opened panel w/ screwdriver
 FLAG_PCAP		,	//User has PCAP file
 FLAG_RANSOM	,	//User has solved ransomware
 FLAG_GLASS		,  //User has broken fire extinguisher glass
 FLAG_EXTINGUISH	,  //User has picked up fire extinguisher
 FLAG_FIRE_OUT	,  //User has put out fire

 FLAG_BROKEN	,
 FLAG_JUMPED	,	//User has jumped service panel door
 FLAG_NFC_RDY	,  //User has NFC script
 FLAG_NFC1		,	//NFC write 1
 FLAG_NFC2		,  //NFC write 2
 FLAG_NFC3		,  //NFC write 3
 FLAG_NFC4		,  //NFC write 4
 FLAG_NFC5		,  //NFC write 5

 FLAG_PUNCH		,  //User has punched enemy, also game complete
 FLAG_DESK		,	//Desk has been opened
 FLAG_TAPE		,	//User has picked up the tape
 FLAG_CAGE		,  //User is in or past cage
 FLAG_HANGER	,  //User has picked up hanger
 FLAG_CAP		,  //User has given capacitor to bert
 FLAG_CONSOLE	,  //User has fixed console
 FLAG_BATTERIES	,	//User has picked up the batteries

 FLAG_BATT_IN	,  //User has put batteries in the radio
 FLAG_ANTENNA	,	//User has fixed radio antenna with the hanger
 FLAG_VIRUS		,	//User has solved the virus puzzle
 FLAG_CAKE		,	//User has picked up the cake
 FLAG_NAME		,	//User has discovered PHANTOM name
 FLAG_MISSING	,	//User has heard some parts are missing
 FLAG_CAMERA	,	//User has seen video footage
 FLAG_MAZE		,	//User has found the secret lab

 FLAG_PRINTS	,  //User has fingerprints
 FLAG_UNK2		,
 FLAG_SERVICE	,	//User has access to the service tunnels
 FLAG_PUZZLE		,  //User has picked up the PUZZLE note
 FLAG_FLASHLIGHT	,	//User has the flashlight
 FLAG_ECE_PART	,  //User has an ECE part
} flags;

/* #define FLAG_VOLT		0	//Solved volt puzzle
#define FLAG_HIT_BOOKS	1	//User has hit the books
#define FLAG_UNK		2
#define FLAG_EMAIL		3	//User has received the email
#define FLAG_MANAGER	4	//Manager has left the room
#define FLAG_KEYCARD	5	//User picked up the keycard
#define	FLAG_STEGO		6	//User has stego picture
#define FLAG_BOX_OPEN	7	//User has opened lock box
#define FLAG_CONFESS	8 	//
#define FLAG_SCREW		9	//User has picked up screwdriver
#define FLAG_PANEL		10	//User has opened panel w/ screwdriver
#define FLAG_PCAP		11	//User has PCAP file
#define FLAG_RANSOM		12	//User has solved ransomware
#define FLAG_GLASS		13  //User has broken fire extinguisher glass
#define FLAG_EXTINGUISH	14  //User has picked up fire extinguisher
#define FLAG_FIRE_OUT	15  //User has put out fire
#define FLAG_BROKEN		16
#define FLAG_JUMPED		17	//User has jumped service panel door
#define FLAG_NFC_RDY	18  //User has NFC script
#define FLAG_NFC1		19	//NFC write 1
#define FLAG_NFC2		20  //NFC write 2
#define FLAG_NFC3		21  //NFC write 3
#define FLAG_NFC4		22  //NFC write 4
#define FLAG_NFC5		23  //NFC write 5
#define FLAG_PUNCH		24  //User has punched enemy, also game complete
#define FLAG_DESK		25	//Desk has been opened
#define FLAG_TAPE		26	//User has picked up the tape
#define FLAG_CAGE		27  //User is in or past cage
#define FLAG_HANGER		28  //User has picked up hanger
#define FLAG_CAP		29  //User has given capacitor to bert
#define FLAG_CONSOLE	30  //User has fixed console
#define FLAG_BATTERIES	31	//User has picked up the batteries
#define FLAG_BATT_IN	32  //User has put batteries in the radio
#define FLAG_ANTENNA	33	//User has fixed radio antenna with the hanger
#define FLAG_VIRUS		34	//User has solved the virus puzzle
#define FLAG_CAKE		35	//User has picked up the cake
#define FLAG_NAME		36	//User has discovered PHANTOM name
#define FLAG_MISSING	37	//User has heard some parts are missing
#define FLAG_CAMERA		38	//User has seen video footage
#define FLAG_MAZE		39	//User has found the secret lab
#define FLAG_PRINTS		40  //USer has fingerprints
#define FLAG_TIME		41  //Time has been set
#define FLAG_SERVICE	42	//User has access to the service tunnels
#define FLAG_PUZZLE		43  //User has picked up the PUZZLE note
#define FLAG_FLASHLIGHT	44	//User has the flashlight
#define FLAG_ECE_PART	45  //User has an ECE part */

#endif /* INC_FLAGS_H_ */
