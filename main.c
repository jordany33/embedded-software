//The address here is the base address for system control registers + the offset for the GPIO port clock gating to control the peripheral for ports
#define RCGCGPIO (*((volatile unsigned long *) 0x400FE608))

//The address here is the base address for system control registers + the offset for the UART clock gating to enable the clock for UART
#define RCGCUART (*((volatile unsigned long *) 0x400FE618))

//The address here is the base address for port A + the offset that enables digital operations on it's pins
#define GPIODEN (*((volatile unsigned long *) 0x4000451C))

//The address here is the base address for port A + the offset for it's corresponding GPIODIR
#define GPIODIR (*((volatile unsigned long *) 0x40004400))

//The address here is the base address for port A + the offset for the pins PA2 and PA5
#define GPIODATA_A (*((volatile unsigned long *) 0x40004090))

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

//The function here is the transmission function given during lecture
char UART_Rx () {
char data;
        while((UART0FR & 0x10));
        data = UART0DR;
        return ((unsigned char) data);
}

int main(void)
{
    //0 means positive 1 means negative, initially positive
    int state = 0;

    //Bit 0 to control the clock to UART0, done before configuring/using UART
    RCGCUART = 0x00000001;

    //This is us turning on the clocking of port A
    RCGCGPIO = 0x00000001;

    //This is us turning on the digital operation enabler for pin PA2 and PA5
    GPIODEN = 0x00000024;

    //This is us setting the PA5 pin's direction to be output and PA2 pin's direction to be input
    GPIODIR = 0x00000020;

    //The integer of the result of 1MHz/9600 (baud rate), which was 104.166667
    UART0IBRD = 104;

    //The fractional calculation, which is 0.166667 * 64 + 0.5
    UART0FBRD = 11;

    //Setting UART0LCRH to match our command shell console, data frame
    UART0LCRH = 0x60;

    //Clearing UART0CTL
    UART0CTL = 0;

    //Setting the reception bits
    UART0CTL |= 0x200;
    //Setting the UARTEN bits
    UART0CTL |= 0x1;

    while(1)
    {
        //Check if we received an input from the computer
        if(!(UART0FR & 0x10))
        {
            char data;
            data = UART0DR;
            //Update state to positive or negative accordingly
            if (data == 'p'){
                state = 0;
            }
            else if (data == 'n'){
                state = 1;
            }
        }
        //When state is positive, if button is pressed (its bit is 1) we light up, otherwise no light
        if (state == 0 ){
            if ((GPIODATA_A & 0x4)){
                GPIODATA_A |= 0x20;
            }
            else {
                GPIODATA_A &= 0xFFFFFFDF;
            }
        }
        //When state is negative, if button is pressed (its bit is 1) we don't light up, otherwise light up
        else if (state == 1){
            if ((GPIODATA_A & 0x4)){
                GPIODATA_A &= 0xFFFFFFDF;
            }
            else {
                GPIODATA_A |= 0x20;
            }
        }
    }
	return 0;
}
