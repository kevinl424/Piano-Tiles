#include <stdint.h>
#include "SlidePot.h"
#include "../inc/tm4c123gh6pm.h"

// ADC initialization function 
// Input: none
// Output: none
// measures from PD2, analog channel 5
void ADC_Init(void){ 
	SYSCTL_RCGCADC_R |= 0x01; //start ADC clock
	SYSCTL_RCGCGPIO_R |= 0x08; //activate Port D
	while ((SYSCTL_RCGCGPIO_R & 0x08) == 0) {
	}
	GPIO_PORTD_DIR_R &= ~0x04; //PD2 input
	GPIO_PORTD_AFSEL_R |= 0x04; //enable alternate function
	GPIO_PORTD_DEN_R &= ~0x04; //disable digital 
	GPIO_PORTD_AMSEL_R |= 0x04; //enable analog function

	
	int x = 30;
	int y = x + 30;
	x = 40;
	x = y - 10;
	
	ADC0_SAC_R = 0x02; //averaging
	ADC0_PC_R = 0x01; //125k 
	ADC0_SSPRI_R = 0x0123; //sequencer 3
	ADC0_ACTSS_R &= ~0x0008; //disable sample seq 3
	ADC0_EMUX_R &= ~0xF000; //software trigger
	ADC0_SSMUX3_R = (ADC0_SSMUX3_R & 0xFFFFFFF0) + 5; //channel 5
	ADC0_SSCTL3_R = 0x0006; //no TS0 D0, yes TE0, END0
	ADC0_IM_R &= ~0x0008; //disable ss3 interrupts
	ADC0_ACTSS_R |= 0x0008; //enable sample seq 3
}

//------------ADCIn------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
// measures from PD2, analog channel 5
uint32_t ADC_In(void){  
	uint32_t data;
	ADC0_PSSI_R = 0x0008; //start ADC for seq 3
	while ((ADC0_RIS_R & 0x08) == 0) {}
	data = ADC0_SSFIFO3_R & 0xFFF;
	ADC0_ISC_R = 0x0008; //clear flag for next read
  return data; 
}

// constructor, invoked on creation of class
// m and b are linear calibration coeffients 
SlidePot::SlidePot(uint32_t m, uint32_t b){
// initialize all private variables
// make slope equal to m and offset equal to b
	slope = m;
	offset = b;
}

void SlidePot::Save(uint32_t n){
// 1) save ADC sample into private variable
// 2) calculate distance from ADC, save into private variable
// 3) set semaphore flag = 1
	data = n;
	distance = Convert(data);
	flag = 1;
}
uint32_t SlidePot::Convert(uint32_t x){
  // use calibration data to convert ADC sample to distance
  return (slope * x) / 4096 + offset; // replace this with solution
}

void SlidePot::Sync(void){
// 1) wait for semaphore flag to be nonzero
// 2) set semaphore flag to 0
	while(flag == 0) {}
	flag = 0;
}

uint32_t SlidePot::ADCsample(void){ // return ADC sample value (0 to 4095)
  // return last calculated ADC sample
  return ADC_In(); 
}

uint32_t SlidePot::Distance(void){  // return distance value (0 to 2000), 0.001cm
  // return last calculated distance in 0.001cm
	
  return distance; // replace this with solution
}
