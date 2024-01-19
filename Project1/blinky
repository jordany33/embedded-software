
#define RCGCGPIO (*((volatile unsigned long *) 0x400FE608))
//The address here is the base address for system control registers + the offset for the GPIO port clock gating to control the peripheral for ports

#define GPIODEN (*((volatile unsigned long *) 0x4002551C))
//The address here is the base address for port f + the offset that enables digital operations on it's pins
#define GPIODIR (*((volatile unsigned long *) 0x40025400))
//The address here is the base address for port f + the offset for it's corresponding GPIODIR

#define GPIODATA_F (*((volatile unsigned long *) 0x40025010))
//The address here is the base address for port f + the offset for it's corresponding pins that we want to turn high while we mask the rest

/**
 * main.c
 */
int main(void)
{
    RCGCGPIO = 0x00000020;
    //This is us turning on the clocking of port f

    GPIODEN = 0x00000004;
    //This is us turning on the digital operation enabler for pin PF2

    GPIODIR = 0x00000004;
    //This is us setting the PF2 pin's direction to be output

    while (1) //Loop that lets us blink the led
    {
        GPIODATA_F = 0x00000004;
        //Turning on the led by setting the PF2 value to be 1

        int i;
        for (i = 0; i < 1000000; i++) {}
        //Delay

        GPIODATA_F = 0x00000000;
        //Turning off the led by setting the PF2 value to be 1

        for (i = 0; i < 1000000; i++) {}
        //Delay
    }
	return 0;
}
