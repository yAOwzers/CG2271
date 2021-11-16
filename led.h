#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"

#define GREEN_1 7 // PortC Pin 7
#define GREEN_2 0 // PortC Pin 0
#define GREEN_3 3 // PortC Pin 3
#define GREEN_4 4 // PortC Pin 4 
#define GREEN_5 5 // PortC Pin 5
#define GREEN_6 6 // PortC Pin 6
#define GREEN_7 10  // PortC Pin 10
#define GREEN_8 11 // PortC Pin 11

#define RED_LED 0 //PortD Pin 0
#define MASK(x) (1 << (x))

char green_led_mapping[8][2] = {{0, GREEN_1}, {1, GREEN_2}, {2, GREEN_3}, {3, GREEN_4}, {4, GREEN_5}, {5,GREEN_6}, {6, GREEN_7}, {7, GREEN_8}};

void initLED(void) {
	// Enable Clock to PORTC & PORTD
	SIM->SCGC5 |= ((SIM_SCGC5_PORTC_MASK) | (SIM_SCGC5_PORTD_MASK));
	
	// Configure MUX settings to make pins GPIO for GREEN
	PORTC->PCR[GREEN_1] &= ~PORT_PCR_MUX_MASK;
	PORTC->PCR[GREEN_1] |= PORT_PCR_MUX(1);
	PORTC->PCR[GREEN_2] &= ~PORT_PCR_MUX_MASK;
	PORTC->PCR[GREEN_2] |= PORT_PCR_MUX(1);
	PORTC->PCR[GREEN_3] &= ~PORT_PCR_MUX_MASK;
	PORTC->PCR[GREEN_3] |= PORT_PCR_MUX(1);
	PORTC->PCR[GREEN_4] &= ~PORT_PCR_MUX_MASK;
	PORTC->PCR[GREEN_4] |= PORT_PCR_MUX(1);
	PORTC->PCR[GREEN_5] &= ~PORT_PCR_MUX_MASK;
	PORTC->PCR[GREEN_5] |= PORT_PCR_MUX(1);
	PORTC->PCR[GREEN_6] &= ~PORT_PCR_MUX_MASK;
	PORTC->PCR[GREEN_6] |= PORT_PCR_MUX(1);
	PORTC->PCR[GREEN_7] &= ~PORT_PCR_MUX_MASK;
	PORTC->PCR[GREEN_7] |= PORT_PCR_MUX(1);
	PORTC->PCR[GREEN_8] &= ~PORT_PCR_MUX_MASK;
	PORTC->PCR[GREEN_8] |= PORT_PCR_MUX(1);
	
	PORTD->PCR[RED_LED] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[RED_LED] |= PORT_PCR_MUX(1);
	
	// Set Data Direction
	PTC->PDDR |= (MASK(GREEN_1) | MASK(GREEN_2) | MASK(GREEN_3) | MASK(GREEN_4) | 
	MASK(GREEN_5) | MASK(GREEN_6) | MASK(GREEN_7) | MASK(GREEN_8));
	
	PTD->PDDR |= MASK(RED_LED);
}

void switchOnAllRedLEDs() {
	PTD->PDOR = MASK(RED_LED);
}
void switchOffAllRedLEDs() {
	PTD->PCOR = MASK(RED_LED);
}

void switchOnAllGreenLEDs() {
	PTC->PDOR = (MASK(GREEN_1) | MASK(GREEN_2) | MASK(GREEN_3) | MASK(GREEN_4) | MASK(GREEN_5) | MASK(GREEN_6) | MASK(GREEN_7) | MASK(GREEN_8));
}
void switchOffAllGreenLEDs() {
	PTC->PDOR = ~(MASK(GREEN_1) | MASK(GREEN_2) | MASK(GREEN_3) | MASK(GREEN_4) | MASK(GREEN_5) | MASK(GREEN_6) | MASK(GREEN_7) | MASK(GREEN_8));
}

void blinkGreenTwice() {
	for (int i = 0; i < 2; i++) {	
		switchOnAllGreenLEDs();
		osDelay(500);
		switchOffAllGreenLEDs();
		osDelay(500);
	}
}

// green led flashes at a rate of 50ms
void greenLedPattern_50() {
	for (char i = 0; i < 8; i++) {
		switchOffAllGreenLEDs();
		PTC->PDOR = MASK(green_led_mapping[i][1]);
		osDelay(50);
	}
}

void redLedBlink_250() {
	switchOnAllRedLEDs();
	osDelay(250);
	switchOffAllRedLEDs();
	osDelay(250);
}

void redLedBlink_500() {
	switchOnAllRedLEDs();
	osDelay(500);
	switchOffAllRedLEDs();
	osDelay(500);
}
