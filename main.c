/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/
 
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"
#include "sound.h"
#include "led.h"
#include "uart.h"
#include "motors.h"

#define BAUD_RATE 9600
#define MASK(x) (1 << (x))

osSemaphoreId_t ledSem, audioSem;
volatile uint8_t rxdata; // Read data sent from bt module
int count = 0; // Flag to see if end challenge music has been played

// Change state of robot according to data read by bluetooth
void UART2_IRQHandler(void) {
	NVIC_ClearPendingIRQ(UART2_IRQn);
	if (UART2->S1 & UART_S1_RDRF_MASK) {
		rxdata = UART2->D;
	}
}

void tBrain (void *argument) {
	for (;;) {
		switch (rxdata) {
			case NOTCONNECTED:
				switchOffAllGreenLEDs();
				switchOffAllRedLEDs();
				break;
			case CONNECTED:	
				osSemaphoreRelease(ledSem);
				osSemaphoreRelease(ledSem);
				osSemaphoreRelease(audioSem);
				break;
			default:
				osSemaphoreRelease(ledSem);
				osSemaphoreRelease(ledSem);
			break;
		}
	}
}

void redLed (void *argument) {
	for (;;) {	
		osSemaphoreAcquire(ledSem, osWaitForever);
		if (rxdata == STOP || rxdata == STARTCHALLENGE || rxdata == ENDCHALLENGE) {
			redLedBlink_250();
		} else if (rxdata == CONNECTED) {
			switchOffAllRedLEDs();
		} else {
			redLedBlink_500();
		}
		osSemaphoreRelease(ledSem);
	}
}

void greenLed(void *argument) {
	for (;;) {
		osSemaphoreAcquire(ledSem, osWaitForever);
		if (rxdata == STOP || rxdata == STARTCHALLENGE || rxdata == ENDCHALLENGE) {
			switchOnAllGreenLEDs();
		} else if (rxdata == CONNECTED) {
			blinkGreenTwice();
			rxdata = STOP;
		} else {
			greenLedPattern_50();
		}
		osSemaphoreRelease(ledSem);
	}
}

void tMotor(void *argument) {
	for (;;) {
		switch (rxdata) {
			case UP:
				move(UP);
				break;
			case LEFT:
				move(LEFT);
				break;
			case RIGHT:
				move(RIGHT);
				break;
			case DOWN:
				move(DOWN);
				break;
			case CURVELEFTUP:
				move(CURVELEFTUP);
				break;
			case CURVERIGHTUP:
				move(CURVERIGHTUP);
				break;
			case CURVELEFTDOWN:
				move(CURVELEFTDOWN);
				break;
			case CURVERIGHTDOWN:
				move(CURVERIGHTDOWN);
				break;
			default:
				move(STOP);
				break;
		}
	}
}

void tAudio (void *argument) {
	osSemaphoreAcquire(audioSem, osWaitForever);
	for(;;) {
		if (rxdata == CONNECTED) {
			startRobotTune();
			count = 0;
			for(int i = 0; i < 33; i++) {
				if(rxdata == ENDCHALLENGE) {	
					break;
				} else {
					challenge(i);
					delay(300);
				}
			}		
		}
		if (rxdata == ENDCHALLENGE && count == 0) {
			stopMusic();
			count++;
		}
		osSemaphoreRelease(audioSem);	
	}
}

int main (void) {
 
  // System Initialization
  SystemCoreClockUpdate();
	initPWM();
	initMotors();
	initLED();
	initUART(BAUD_RATE);
  // ...
	ledSem = osSemaphoreNew(2,0,NULL);
	audioSem = osSemaphoreNew(1,0,NULL);
  osKernelInitialize();                 // Initialize CMSIS-RTOS
	osThreadNew(tBrain, NULL, NULL);
	osThreadNew(tMotor, NULL, NULL);
	osThreadNew(redLed, NULL, NULL);
	osThreadNew(greenLed, NULL, NULL);
	osThreadNew(tAudio, NULL, NULL);
  osKernelStart();                      // Start thread execution
  for (;;) {}
}
