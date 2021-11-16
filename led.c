#include "MKL25Z4.h"                    // Device header
#include "cmsis_os2.h"

#define RED_LED 18 // PortB Pin 18
#define GREEN_LED 19 // PortB Pin 19
#define BLUE_LED 1 // PortD Pin 1
#define MASK(x) (1 << (x))

osEventFlagsId_t front_led_Id, rear_led_off_Id;

void initLED(void)
{
	// Enable Clock to PORTB and PORTD
	SIM->SCGC5 |= ((SIM_SCGC5_PORTB_MASK) | (SIM_SCGC5_PORTD_MASK));
	
	// Configure MUX settings to make all 3 pins GPIO
	PORTB->PCR[RED_LED] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[RED_LED] |= PORT_PCR_MUX(1);		//PORTB18 as input pin
	
	PORTB->PCR[GREEN_LED] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[GREEN_LED] |= PORT_PCR_MUX(1);
	
	PORTD->PCR[BLUE_LED] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[BLUE_LED] |= PORT_PCR_MUX(1);
	
	// Set Data Direction Registers for PortB and PortD
	PTB->PDDR |= (MASK(RED_LED) | MASK(GREEN_LED));
	PTD->PDDR |= MASK(BLUE_LED);
}

enum colour_t {
	red = 18, green = 19, blue = 1
};

// Function to determine which colour LED to output
void led_control(enum colour_t color) {
	switch(color) {
		case RED_LED:
			GPIOB_PCOR |= MASK(RED_LED);
			GPIOB_PSOR |= MASK(GREEN_LED);
			GPIOD_PSOR |= MASK(BLUE_LED);
		return;
		case GREEN_LED:
			GPIOB_PCOR |= MASK(GREEN_LED);
			GPIOD_PSOR |= MASK(BLUE_LED);
			GPIOB_PSOR |= MASK(RED_LED);
		return;
		case BLUE_LED:
			GPIOD_PCOR |= MASK(BLUE_LED);
			GPIOB_PSOR |= MASK(RED_LED);
			GPIOB_PSOR |= MASK(GREEN_LED);
		return;
	}
}

// Delay Function
static void delay(volatile uint32_t nof) {
	while(nof!=0) {
		__asm("NOP");
		nof--;
	}
}

// Function to turn off LED
void offLed(void) {
	GPIOB_PSOR |= MASK(RED_LED);
	GPIOB_PSOR |= MASK(GREEN_LED);
	GPIOD_PSOR |= MASK(BLUE_LED);
}

/*----------------------------------------------------------------------------
 * Application Front LED thread
 *---------------------------------------------------------------------------*/
void front_led_thread (void *argument) {

  // ...
  for (;;) {
		
		led_control(GREEN_LED);
		osDelay(1000);
		offLed();
		osDelay(1000);
		
	}
}

void rear_led_moving (void *argument) {
	for (;;) {
		osThreadFlagsSet(
		led_control(RED_LED);
		osDelay(500);
	}
}

void rear_led_off (void *argument) {
	for (;;) {
		osThreadFlagsWait(0x0001, osFlagsWaitAny, osWaitForever);
		delay(250);
		offLed();
	}
}
/*----------------------------------------------------------------------------
 * Application Rear LED thread
 *---------------------------------------------------------------------------*/
void rear_led_thread (void *argument) {

  // When robot not moving, rear led flash 500ms ON 500ms OFF
  for (;;) {
		led_control(RED_LED);
		osThreadFlagsWait(0x0001, osFlagsWaitAny, osWaitForever);
		osDelay(2000);
		
	}
	
	// When robot is moving, rear led flash 250ms ON 250ms OFF
	for (;;) {
		led_control(RED_LED);
		osThreadFlagsWait(0x0001, osFlagsWaitAny, osWaitForever);
	}
}
	
void tLed (void *argument) {
	// Wait for start event flag to be set
	osEventFlagsWait(start_event, 0x0001, osFlagsWaitAny, osWaitForever);
	// Blink green led twice
	for (int i = 0; i < 2; i++) {
		led_control(GREEN_LED);
		osDelay(500);
		offLed();
		osDelay(500);
	}
	
	// When robot is stationary, front LED light up continuously
	// and rear LED flash 250ms on, 250ms off
	for (;;) {
		if (state == 0) {
			led_control(GREEN_LED[]);
			led_control(RED_LED);
			osDelay(250);
			offRearLed();
		} else {
			// When robot moves, front LED in running mode
			// and rear LED flash 500ms on, 500ms off
			led_control(RED_LED);
			for(int i = 0; i < 8; i++) {
				led_control(GREEN_LED[i]);
			}
			osDelay(500);
			offRearLed();
		}
	}
}
			