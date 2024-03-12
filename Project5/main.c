//The address here is the base address for system control registers + the offset for the GPIO port clock gating to control the peripheral for ports
#define RCGCGPIO (*((volatile unsigned long *) 0x400FE608))

//The address here is the base address for system control registers + the offset for the ADC clock
#define RCGCADC (*((volatile unsigned long *) 0x400FE638))

//The address here is the the ADC0 active sample sequencer
#define ADC0ACTSS (*((volatile unsigned long *) 0x40038000))

//Base address for the the ADC0 active sample sequencer + offset for its ADCSSCTL3
#define ADC0SSCTL3 (*((volatile unsigned long *) 0x400380A4))

//Base address for ADC0 + the offset for its sample sequence input multiplexer select 3
#define ADC0SSMUX (*((volatile unsigned long *) 0x400380A0))

//Base address for ADC0 + the offset for its event multiplexer select
#define ADC0EMUX (*((volatile unsigned long *) 0x40038014))

//Base address for ADC0 + the offset for its ADCRIS/completion flags
#define ADC0RIS (*((volatile unsigned long *) 0x40038004))

//Base address for ADC0 + the offset for its ADCISC/interrupt status clear
#define ADC0ISC (*((volatile unsigned long *) 0x4003800C))

//Base address for ADC0 + the offset for its ADCSSFIFOn/data result
#define ADC0SSFIFO3 (*((volatile unsigned long *) 0x400380A8))

//Base address for ADC0 + the offset for its ADCPSSI
#define ADC0PSSI (*((volatile unsigned long *) 0x40038028))

//The address here is the base address for system control registers + the offset for the PWM clock gating
#define RCGCPWM (*((volatile unsigned long *) 0x400FE640))

//Base address for PWM0 + the offset for its output enable
#define PWM0EN (*((volatile unsigned long *) 0x40028008))

//Base address for PWM0 + offset for PWM0CTL, not sure if this one or other
#define PWM0CTL (*((volatile unsigned long *) 0x40028040))

//Base address for PWM0 + the offset for its PWM0LOAD
#define PWM0LOAD (*((volatile unsigned long *) 0x40028050))

//Base address for PWM0 + the offset for its PWM0COUNT
#define PWM0COUNT (*((volatile unsigned long *) 0x40028054))

//Base address for PWM0 + the offset for its PWM0CMPA
#define PWM0CMPA (*((volatile unsigned long *) 0x40028058))

//Base address for PWM0 + the offset for its PWM0CMPB
#define PWM0CMPB (*((volatile unsigned long *) 0x4002805C))

//Base address for PWM0 + the offset for its PWM0GENA/its signal generator operation
#define PWM0GENA (*((volatile unsigned long *) 0x40028060))

//The address here is the base address for port E + the offset that enables digital operations on it's pins
#define GPIODENE (*((volatile unsigned long *) 0x4002451C))

//The address here is the base address for port E + the offset for the pins PE3
#define GPIODATA_E (*((volatile unsigned long *) 0x40024020))

//The address here is the base address for port E + the offset for its AFSEL
#define GPIOE_AFSEL (*((volatile unsigned long *) 0x40024420))

//The address here is the base address for port E + the offset for its AMSEL
#define GPIOE_AMSEL (*((volatile unsigned long *) 0x40024528))

//The address here is the base address for port B + the offset that enables digital operations on it's pins
#define GPIODENB (*((volatile unsigned long *) 0x4000551C))

//The address here is the base address for port B + the offset for the pin PB6
#define GPIODATA_B (*((volatile unsigned long *) 0x40005100))

//The address here is the base address for port B + the offset for its AFSEL
#define GPIOB_AFSEL (*((volatile unsigned long *) 0x40005420))

//The address here is the base address for port B + the offset for it's corresponding GPIOPCTL
#define GPIOBPCTL (*((volatile unsigned long *) 0x4000552C))

//Base address for system control registers + offset for run mode clock configuration
#define RCC (*((volatile unsigned long *) 0x400FE060))

void delay(long num){
    int i;
    for( i = num; i > 0;i--){}
}
/**
 * main.c
 */
int main(void)
{
    //Us turning on clock gating for ports E and B
    RCGCGPIO |= 0x00000012;
    //Us turning on clock gating for PWM0
    RCGCADC |= 0x00000001;
    //This is us setting the bit corresponding to PE3 to be alternate
    GPIOE_AFSEL |= (1<<3);
    //This is us making PE3 analog
    GPIODENE &= (~(1<<3));
    //This is us enabling the analog function of PE3
    GPIOE_AMSEL |= (1<<3);
    //This is us disabling sample sequencer 3 as we adjust it's settings
    ADC0ACTSS &= (~(1<<3));
    //This is us setting the event multiplexer for SS3 to be processor
    ADC0EMUX &= (~(0xF000));
    //This is us configuring the input source for ADCSSMUX3
    ADC0SSMUX = 0x0;
    //This is us configuring sample sequencer 3 settings to enable the interrupt when conversion is done bit/interrupt signal that lets it read values 2 and to set the end of sequence bit 1
    ADC0SSCTL3 |= (1<<1) | (1<<2);
    //This is us choosing sample sequencer 3
    ADC0ACTSS |= (1<<3);
    //Turning on clock gating for PWM0
    RCGCPWM = 0x1;
    //Making sure we dont use the divider, Harris said probably don't need this but we did it in case
    RCC &= (~(1<<20));
    //Making PB6 alternate function pin
    GPIOB_AFSEL |= (1<<6);
    //Setting the alternate function for PB6 to be the one corresponding to PWM0Channel0
    GPIOBPCTL = 0x04000000;
    //Digitally enable PB6
    GPIODENB |= (1<<6);
    //Disable the PWM counter as we configure it
    PWM0CTL &= (~(1));
    //Down count mode
    PWM0CTL &= (~(1<<1));
    //Setting pwm0Load to be 16000
    PWM0LOAD = 16000;
    //Duty cycle 60% initially (our min)
    PWM0CMPA = 9600;
    //Settings to control what happens for each event, we When our pwm counter equals our load value,
    //we want our pwm pin to go high, and when the counter matches our comparator A value while counting down, we want our pwm pin to go down.
    PWM0GENA |= (1<<2) | (1<<3) | (1<<7);
    //Enable the generator counter
    PWM0CTL |= (0x1);
    //Enable the pwm output for channel 0
    PWM0EN |= 0x1;
    long adcVal;
    while(1){
        //Us starting sampling on SS3
        ADC0PSSI |= (1<<3);
        //Wait if busy
        while((ADC0RIS & 0x8) == 0x0){}
        adcVal = ADC0SSFIFO3;
        ADC0ISC |= (1<<3);
        //Find what percent our adcVal is out of the max it can be, used later for the duty cycle
        double perc = (double)adcVal/4096;
        PWM0CMPA = (9600+(perc*6400));
        delay(1000000);
    }
    return 0;
}
