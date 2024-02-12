//The address here is the base address for system control registers + the offset for the GPIO port clock gating to control the peripheral for ports
#define RCGCGPIO (*((volatile unsigned long *) 0x400FE608))

//The address here is the base address for system control registers + the offset for the UART clock gating to enable the clock for UART
#define RCGCUART (*((volatile unsigned long *) 0x400FE618))

//The address here is the base address for port A + the offset that enables digital operations on it's pins
#define GPIODENA (*((volatile unsigned long *) 0x4000451C))

//The address here is the base address for port A + the offset for it's corresponding GPIODIR
#define GPIODIRA (*((volatile unsigned long *) 0x40004400))

//The address here is the base address for port A + the offset for it's corresponding GPIOPCTL
#define GPIOPCTL (*((volatile unsigned long *) 0x4000452C))

//The address here is the base address for port A + the offset for it's corresponding GPIOAFSEL
#define GPIOAFSEL (*((volatile unsigned long *) 0x40004420))

//The address here is the base address for port A + the offset for the pins PA2 and PA5
#define GPIODATA_A (*((volatile unsigned long *) 0x40004090))

//The address here is the base address for port C + the offset that enables digital operations on it's pins
#define GPIODENC (*((volatile unsigned long *) 0x4000651C))

//The address here is the base address for port C + the offset for it's corresponding GPIODIR
#define GPIODIRC (*((volatile unsigned long *) 0x40006400))

//The address here is the base address for port C + the offset for the pins PC4-PC7
#define GPIODATA_C (*((volatile unsigned long *) 0x400063C0))

//The address here is the base address for port E + the offset that enables digital operations on it's pins
#define GPIODENE (*((volatile unsigned long *) 0x4002451C))

//The address here is the base address for port E + the offset for it's corresponding GPIODIR
#define GPIODIRE (*((volatile unsigned long *) 0x40024400))

//The address here is the base address for port E + the offset for it's corresponding GPIOPDR, as we are using it as the input columns for the keypad.
#define GPIOPDR (*((volatile unsigned long *) 0x40024514))

//The address here is the base address for port E + the offset for the pins PC4-PC7
#define GPIODATA_E (*((volatile unsigned long *) 0x40024078))

//The address here is the base address for port A + the offset for it's corresponding GPIOIM
#define GPIOIM (*((volatile unsigned long *) 0x40004410))

//The address here is the base address for port A + the offset for it's corresponding GPIOIS
#define GPIOIS (*((volatile unsigned long *) 0x40004404))

//The address here is the base address for port A + the offset for it's corresponding GPIOIEV
#define GPIOIEV (*((volatile unsigned long *) 0x4000440C))

//The address here is the base address for port A + the offset for it's corresponding GPIOICR
//#define GPIOICR (*((volatile unsigned long *) 0x4000441C)) didn't end up using it

//The address here is the base address for UART0//UART0DR (the data we send or receive)
#define UART0DR (*((volatile unsigned long *) 0x4000C000))

//The address here is the base address for UART0 + the offset for UART0FR, the flag register which contains transmit/receive status of UART0
#define UART0FR (*((volatile unsigned long *) 0x4000C018))

//The address here is the base address for UART0 + the offset for it's UARTIBRD
#define UART0IBRD (*((volatile unsigned long *) 0x4000C024))

//The address here is the base address for UART0 + the offset for it's UARTFBRD
#define UART0FBRD (*((volatile unsigned long *) 0x4000C028))

//The address here is the base address for UART0 + the offset for it's UARTCTL
#define UART0CTL (*((volatile unsigned long *) 0x4000C030))

//The address here is the base address for UART0 + the offset for it's UART0LCRH
#define UART0LCRH (*((volatile unsigned long *) 0x4000C02C))

//The address here is the base address for system control registers + the offset for the clock gating of the timer
#define RCGCTIMER (*((volatile unsigned long *) 0x400FE604))

//The address here is the base address for core peripherals + the offset for NVIC register EN0
#define EN0 (*((volatile unsigned long *) 0xE000E100))

//The address here is the base address for timer 0/its GPTMCFG, used to set bitwidth of timer
#define GPTMCFG (*((volatile unsigned long *) 0x40030000))

//The address here is the base address for timer 0 + the offset for its GPTMCTL, used to enable timer
#define GPTMCTL (*((volatile unsigned long *) 0x4003000C))

//The address here is the base address for timer 0 + the offset for its GPTMTAPR, didn't end up needing it
//#define GPTMTAPR (*((volatile unsigned long *) 0x40030038))

//The address here is the base address for timer 0 + the offset for its GPTMTAMR or Timer mode, count down periodic
#define GPTMTAMR (*((volatile unsigned long *) 0x40030004))

//The address here is the base address for timer 0 + the offset for its GPTMTAILR our init cnt for timer
#define GPTMTAILR (*((volatile unsigned long *) 0x40030028))

//The address here is the base address for timer 0 + the offset for its GPTMRIS, don't actually need it for code to work
//#define GPTMRIS (*((volatile unsigned long *) 0x4003001C))

//The address here is the base address for timer 0 + the offset for its GPTMICR, used to clear flags in gptmris
#define GPTMICR (*((volatile unsigned long *) 0x40030024))

//The address here is the base address for timer 0 + the offset for its GPTMIMR or its interrupt mask
#define GPTMIMR (*((volatile unsigned long *) 0x40030018))


void UART_Tx (char data) {
    while((UART0FR & 0x20) == 1);
    UART0DR = data;
    //Slight delay so a single button press doesn't result in dozens of print outs which was happening
    int n;
    for (n = 0; n<800000; n++){};
}

void timerLED()
{
    GPIODATA_A |= 0x20;
    int i;
    for (i = 0; i<100000; i++){};
    GPIODATA_A &= 0xFFFFFFDF;
    GPTMICR |= 0x1;
}

void buttonInterr(){
    UART_Tx('$');
    //GPIOICR |= 0x4;  //this was testing icr, was having some issues
}

/**
 * main.c
 */
int main(void)
{
    //Bit 0 to control the clock to UART0, done before configuring/using UART
    RCGCUART = 0x00000001;

    //This is us turning on the clocking of port A, C, and E
    RCGCGPIO = 0x00000015;

    //This is us turning on the digital operation enabler for pin PA2 PA5 PA0
    GPIODENA |= 0x00000026;

    //This is us turning on the digital operation enabler for pins PC4-PC7
    GPIODENC |= 0x000000F0;

    //This is us turning on the digital operation enabler for pins PE1-PE4
    GPIODENE |= 0x0000001E;

    //This is us setting the PA5 pin's direction to be output and PA2 pin's direction to be input
    GPIODIRA |= 0x00000020;
    GPIODIRA &= 0xFFFFFFFB;

    //This is us setting our PC pins used to be outputs, and our PE pins used to be inputs
    GPIODIRC |= 0x000000F0;
    GPIODIRE &= 0xFFFFFFE1;

    //Clearing the UARTEN
    UART0CTL &= 0xFFFFFFFE;

    //This is us setting the bit to be alternate
    GPIOAFSEL |= 0x00000003;

    //This is us setting the alternate function to be the UART0 ones
    GPIOPCTL |= 0x00000011;

    //Setting PA2 to be level sensitive
    GPIOIS = 0x00000004;

    //This is us setting PA2 to make the interrupt event sense high
    GPIOIEV = 0x00000004;

    //This is us turning on the bit to allow interrupts from pin PA2
    GPIOIM |= 0x00000004;

    //The integer of the result of 1MHz/9600 (baud rate), which was 104.166667
    UART0IBRD = 104;

    //The fractional calculation, which is 0.166667 * 64 + 0.5
    UART0FBRD = 11;

    //Setting UART0LCRH to match our command shell console, data frame
    UART0LCRH = 0x60;

    //Setting the reception/transmission bits alongside the UARTEN to be 1 to enable
    UART0CTL |= 0x301;

    //Turns on clock gating for timer 0 by setting bit 0 high
    RCGCTIMER |= 0x1;

    //For choosing the 32 bit timer configuration for the 16/32 bit timer under TA recommendation so that no prescalar is needed
    //GPTMCFG = 0x4;16bit
    GPTMCFG = 0x0;

    //Periodic and down count for our timer
    GPTMTAMR = 0x2;

    //Setting 19th bit high on EN0 to enable the timer 0A interrupt and 1st bit for port A interrupt
    EN0 |= 0x80001;

    //Setting TATOIM/bit 0 to 1 to unmask the interrupt
    GPTMIMR |= 0x1;

    //Setting bit0/tatocint to 1 clear the timer A flag in GPTMRIS
    GPTMICR |= 0x1;

    //Give initcnt to the timer by setting gptmtailr to be 1second/clock cycle
    GPTMTAILR = 16000000;

    //Setting bit 0 high to enable timer 0
    GPTMCTL |= 0x1;

    //This is us turning on the pull down select for PE1-PE4 since we are using them for the columns of the matrix
    GPIOPDR |= 0x0000001E;

    while(1)
    {
        int row;
        for (row = 0; row < 4; row++){
            if (row == 0){
                GPIODATA_C |= 0x10;
                if ((GPIODATA_E & 0x2)){
                    UART_Tx ('1');
                }
                else if ((GPIODATA_E & 0x4)){
                    UART_Tx('2');
                }
                else if ((GPIODATA_E & 0x8)){
                    UART_Tx('3');
                }
                else if ((GPIODATA_E & 0x10)){
                    UART_Tx('A');
                }
                GPIODATA_C &= 0xFFFFFFEF;
            }
            else if (row == 1){
                GPIODATA_C |= 0x20;
                if ((GPIODATA_E & 0x2)){
                    UART_Tx ('4');
                }
                else if ((GPIODATA_E & 0x4)){
                    UART_Tx('5');
                }
                else if ((GPIODATA_E & 0x8)){
                    UART_Tx('6');
                }
                else if ((GPIODATA_E & 0x10)){
                    UART_Tx('B');
                }
                GPIODATA_C &= 0xFFFFFFDF;
            }
            else if (row == 2){
                GPIODATA_C |= 0x40;
                if ((GPIODATA_E & 0x2)){
                    UART_Tx ('7');
                }
                else if ((GPIODATA_E & 0x4)){
                    UART_Tx('8');
                }
                else if ((GPIODATA_E & 0x8)){
                    UART_Tx('9');
                }
                else if ((GPIODATA_E & 0x10)){
                    UART_Tx('C');
                }
                GPIODATA_C &= 0xFFFFFFBF;
            }
            else if (row == 3){
                GPIODATA_C |= 0x80;
                if ((GPIODATA_E & 0x2)){
                    UART_Tx ('*');
                }
                else if ((GPIODATA_E & 0x4)){
                    UART_Tx('0');
                }
                else if ((GPIODATA_E & 0x8)){
                    UART_Tx('#');
                }
                else if ((GPIODATA_E & 0x10)){
                    UART_Tx('D');
                }
                GPIODATA_C &= 0xFFFFFF7F;
            }
        }
    }
	return 0;
}