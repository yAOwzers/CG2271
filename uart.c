#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"

#define BAUD_RATE 9600
#define PTB0_Pin 0
#define PTB1_Pin 1
#define PTB2_Pin 2
#define PTB3_Pin 3

#define PTE20_Pin 20
#define PTE21_Pin 21
#define PTE22_Pin 22
#define PTE23_Pin 23

#define UART_RX_PORTD2 2
#define MASK(x) (1 << (x))

uint8_t rxdata; // Receive data from bt module

typedef enum dir_t {
	UP = 0x01, LEFT = 0x02, RIGHT = 0x03, DOWN = 0x04, STOP = 0x00, CONNECTED = 0x08
} dir ;

void initUART(uint32_t baud_rate) {
	uint32_t divisor, bus_clock;
	
	// Enable clock to UART2 and PORTD
	SIM->SCGC4 |= SIM_SCGC4_UART2_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;
	
	// Enable muxing to choose UART2 RX function for PORTD2
	PORTD->PCR[UART_RX_PORTD2] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[UART_RX_PORTD2] |= PORT_PCR_MUX(3);
	
	// Disable receiver
	UART2->C2 &= ~(UART_C2_RE_MASK);
	
	// UART runs at half the system clock rate
	bus_clock = (DEFAULT_SYSTEM_CLOCK) / 2;
	divisor = bus_clock / (baud_rate * 16);
	UART2->BDH = UART_BDH_SBR(divisor >> 8);
	UART2->BDL = UART_BDL_SBR(divisor);
	
	UART2->C1 = 0;
	UART2->S2 = 0;
	UART2->C3 = 0;
	
	// Enable receiver
	UART2->C2 |= (UART_C2_RE_MASK);
	
	// Enable interrupts on UART2
	NVIC_SetPriority(UART2_IRQn,128);
	NVIC_ClearPendingIRQ(UART2_IRQn);
	NVIC_EnableIRQ(UART2_IRQn);
	
	// Enable RX interrupt when data ready
	UART2->C2 |= (UART_C2_RIE_MASK);
}

// Change state of robot according to data read by bluetooth
void UART2_IRQHandler(void) {
	NVIC_ClearPendingIRQ(UART2_IRQn);
	if (UART2->S1 & UART_S1_RDRF_MASK) {
		rxdata = UART2->D;
		osSemaphoreRelease(brainSem);
	}
	move(rxdata);
}

void tBrain (void *argument) {
	osSemaphoreAcquire(brainSem,osWaitForever);
	// If robot is connected
	switch (rxdata) {
		case CONNECTED:
			osSemaphoreRelease(ledSem);
			osSemaphoreRelease(soundSem);
			break;
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
		case STOP:
			move(STOP);
			break;
	}
	if (rxdata == CONNECTED) {
		// Release semaphore for led thread
		osSemaphoreRelease(ledSem);
		// Release semaphore for connected tone sequence
		osSemaphoreRelease(soundSem);
	}
	else if (rxdata == LEFT) {
		move(LEFT);
	}
	osSemaphoreRelease(brainSem);
}

