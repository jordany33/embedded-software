//The address here is the base address for system control registers + the offset for the clock gating of the timer
#define RCGCTIMER (*((volatile unsigned long *) 0x400FE604))
//The address here is the base address for core peripherals + the offset for NVIC register EN0
#define EN0 (*((volatile unsigned long *) 0x400FE604))
//The address here is the base address for timer 0/its GPTMCFG
#define GPTMCFG (*((volatile unsigned long *) 0x40030000))
//The address here is the base address for timer 0 + the offset for its GPTMCTL
#define GPTMCTL (*((volatile unsigned long *) 0x4003000C))
//The address here is the base address for timer 0 + the offset for its GPTMTAPR
#define GPTMTAPR (*((volatile unsigned long *) 0x40030038))
//The address here is the base address for timer 0 + the offset for its GPTMTAMR
#define GPTMTAMR (*((volatile unsigned long *) 0x40030004))
//The address here is the base address for timer 0 + the offset for its GPTMTAILR
#define GPTMTAILR (*((volatile unsigned long *) 0x40030028))
//The address here is the base address for timer 0 + the offset for its GPTMRIS
#define GPTMRIS (*((volatile unsigned long *) 0x4003001C))
//The address here is the base address for timer 0 + the offset for its GPTMICR
#define GPTMICR (*((volatile unsigned long *) 0x40030024))
//The address here is the base address for timer 0 + the offset for its GPTMIMR
#define GPTMIMR (*((volatile unsigned long *) 0x40030018))

//Setup GPIOIS and GPIOIEV and GPIOIM later, all use base address of GPIO port + offset, not sure which to use for now
//Should need two ports, 4 pins for output of matrix, 4 for inputs, represent the 4 by 4 matrix
//Same for GPIOPUR or GPIOPDR, don't know which one we want yet, one is pull down, other is pull up, sets default value to either 0 or 1 for pins if matrix
//button not pressed


/**
 * main.c
 */
int main(void)
{
	return 0;
}
