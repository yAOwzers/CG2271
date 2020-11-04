/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/
 
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"
#include "led.c"
#include "uart.c"

#define BAUD_RATE 9600
#define PTB0_Pin 0
#define PTB1_Pin 1

#define MASK(x) (1 << (x))

osMutexId_t myMutex;
osEventFlagsId_t redLed_flag;
osThreadId_t redLed_Id, greenLed_Id, blueLed_Id, control_Id;

void initPWM(void) {
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;
	
	// Configure PTB0 and PTB1 to alternative 3 for PWM pin operation
	PORTB->PCR[PTB0_Pin] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[PTB0_Pin] |= PORT_PCR_MUX(3);
	
	PORTB->PCR[PTB1_Pin] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[PTB1_Pin] |= PORT_PCR_MUX(3);
	
	SIM->SCGC6 |= SIM_SCGC6_TPM1_MASK;
	
	SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);
	
	TPM1->MOD = 7500;
	
	TPM1->SC &= ((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));
	TPM1->SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(7));
	TPM1->SC &= ~(TPM_SC_CPWMS_MASK);
	
	TPM1_C0SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	TPM1_C0SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
}

/*----------------------------------------------------------------------------
 * Application main thread
 *---------------------------------------------------------------------------*/
/*void app_main (void *argument) {

  // ...
  for (;;) {
		led_control(RED_LED);
		osDelay(1000);
		offLed();
		osDelay(1000);
	}
}
*/

/*----------------------------------------------------------------------------
 * Application control thread
 *---------------------------------------------------------------------------*/
void control_thread (void *argument) {

  // ...
  for (;;) {
		osThreadFlagsSet(redLed_Id, 0x00000001);
		osDelay(1000);
		osThreadFlagsSet(greenLed_Id, 0x00000001);
		osDelay(1000);
		osThreadFlagsSet(blueLed_Id, 0x00000001);
		osDelay(1000);
	}
}

int main (void) {
 
  // System Initialization
  SystemCoreClockUpdate();
	initPWM();
	initLED();
	initUART(BAUD_RATE);
	offLed();
  // ...
	
  osKernelInitialize();                 // Initialize CMSIS-RTOS
	myMutex = osMutexNew(NULL);
  
	control_Id = osThreadNew(control_thread, NULL, NULL);
  osKernelStart();                      // Start thread execution
  for (;;) {}
}
