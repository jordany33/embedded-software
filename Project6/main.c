//The address here is the base address for system control registers + the offset for the GPIO port clock gating to control the peripheral for ports
#define RCGCGPIO (*((volatile unsigned long *) 0x400FE608))

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

//The address here is the base address for port E + the offset for the pins PE1-PE4
#define GPIODATA_E (*((volatile unsigned long *) 0x40024078))

//The address here is the base address for port A + the offset that enables digital operations on it's pins
#define GPIODENA (*((volatile unsigned long *) 0x4000451C))

//The address here is the base address for port A + the offset for it's corresponding GPIODIR
#define GPIODIRA (*((volatile unsigned long *) 0x40004400))

//The address here is the base address for port A + the offset for the pins PA2-PA4, Register Select, Read/Write, and Enable Input respectively
#define GPIODATA_A (*((volatile unsigned long *) 0x40004070))

//The address here is the base address for port A + the offset for its AFSEL
#define GPIOA_AFSEL (*((volatile unsigned long *) 0x40004420))

//The address here is the base address for port A + the offset for it's corresponding GPIOPCTL
#define GPIOAPCTL (*((volatile unsigned long *) 0x4000452C))

//We are using I2C1
//The address defined here is the base address for Port A + the offset for its GPIOODR, open drain select
#define GPIOODR (*((volatile unsigned long *) 0x4000450C))
//The address defined here is the base address for I2C1+ offset for I2CMCR/Master configuration
#define I2CMCR (*((volatile unsigned long *) 0x40021020))
//The address defined here is the base address for I2C1/Master slave address
#define I2CMSA (*((volatile unsigned long *) 0x40021000))

//The address defined here is the base address for I2C1+ offset for I2CMCS/Master Control/Status
#define I2CMCS (*((volatile unsigned long *) 0x40021004))

//The address defined here is the base address for I2C1+ offset for I2CMDR/Master Data
#define I2CMDR (*((volatile unsigned long *) 0x40021008))

//The address defined here is the base address for I2C1+ offset for I2CMTPR/Timer Period
#define I2CMTPR (*((volatile unsigned long *) 0x4002100C))

//The address here is the base address for port A + the offset for the pin PA6, which is our I2CSCL
#define I2CSCL (*((volatile unsigned long *) 0x40004100))

//The address here is the base address for port A + the offset for the pin PA7, which is our I2CSDA
#define I2CSDA (*((volatile unsigned long *) 0x40004200))

//The address here is the base address for system control registers + the offset for the I2C Clock Gating
#define RCGCI2C (*((volatile unsigned long *) 0x400FE620))

#define GPIOAPUR (*((volatile unsigned long *) 0x40004510))

#define slaveAdd 0x50;
#define slaveAddWr 0xA0;
#define slaveAddR 0xA1;
//#define I2CSCL 0x40;
//#define I2CSDA 0x80;

int curIndex = 0;

//0, 1, 2 possible values corresponding to play record playback

void waitDone(){
    while(I2CMCS & 0x1);
}

void writeChar(char toWrite){
    I2CMSA = slaveAddWr;
    //Told by TA to assume our curIndex never reaches the point where it needs the upper 4 bits
    I2CMDR = 0x00;
    //Enter master transmit
    I2CMCS = 3;
    waitDone();
    I2CMDR = ((curIndex));
    I2CMCS = 1;
    waitDone();
    I2CMDR = toWrite;
    I2CMCS = 5;
    waitDone();
    curIndex++;
}

char readChars(int address){
    I2CMSA = slaveAddWr;
    //What EEPROM address are we writing to, 4 bits don't care 4 upper bits of our address for first byte
    I2CMDR = 0x00;
    //Enter master transmit
    I2CMCS = 3;
    waitDone();
    I2CMDR = ((address));
    I2CMCS = 1;
    waitDone();
    I2CMSA = slaveAddR;
    I2CMCS = 0x7;
    waitDone();
    return I2CMDR;
}

int state = 0;
char getInput(){
    //n stands for non valid input
    char inp = 'n';
    int row;
    for (row = 0; row < 4; row++){
        if (row == 0){
            GPIODATA_C = 0x10;
            if ((GPIODATA_E & 0x2)){
                inp = '1';
            }
            else if ((GPIODATA_E & 0x4)){
                inp = '2';
            }
            else if ((GPIODATA_E & 0x8)){
                inp = '3';
            }
            else if ((GPIODATA_E & 0x10)){
                inp = 'A';
            }
            GPIODATA_C = 0;
            //means we found an input, can terminate early, same for the other row statements
            if (inp != 'n'){
                break;
            }
        }
        else if (row == 1){
            GPIODATA_C = 0x20;
            if ((GPIODATA_E & 0x2)){
                inp = '4';
            }
            else if ((GPIODATA_E & 0x4)){
                inp = '5';
            }
            else if ((GPIODATA_E & 0x8)){
                inp = '6';
            }
            else if ((GPIODATA_E & 0x10)){
                inp = 'B';
            }
            GPIODATA_C = 0;
            if (inp != 'n'){
                break;
            }
        }
        else if (row == 2){
            GPIODATA_C = 0x40;
            if ((GPIODATA_E & 0x2)){
                inp = '7';
            }
            else if ((GPIODATA_E & 0x4)){
                inp = '8';
            }
            else if ((GPIODATA_E & 0x10)){
                inp = 'C';
            }
            GPIODATA_C = 0;
            if (inp != 'n'){
                break;
            }
        }
        else if (row == 3){
            GPIODATA_C = 0x80;
            if ((GPIODATA_E & 0x10)){
                inp = 'D';
            }
            GPIODATA_C = 0;
            if (inp != 'n'){
                break;
            }
        }
    }
    buttonDelay();
    return inp;
}


void buttonDelay(){
    int n;
    for (n = 0; n<100000; n++){};
}

void delay(int n){
    int del = n;
    while(del--);
}
/**
 * main.c
 */
int main(void)
{
    //Us turning on clock gating for ports A, B, C, E
    RCGCGPIO |= 0x00000017;

    //This is us turning on the digital operation enabler for pin PA2-PA4
    GPIODENA |= 0x0000001C;
    //Setting PA2-PA4 to be output
    GPIODIRA |= 0x0000001C;

    //This is us turning on the digital operation enabler for pins PC4-PC7
    GPIODENC |= 0x000000F0;

    //This is us turning on the digital operation enabler for pins PE1-PE4
    GPIODENE |= 0x0000001E;

    //This is us setting our PC pins used to be outputs, and our PE pins used to be inputs
    GPIODIRC |= 0x000000F0;
    GPIODIRE &= 0xFFFFFFE1;

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
    //Stops it from making noise by having 100% duty cycle
    PWM0CMPA = 16000;
    //Settings to control what happens for each event, when our pwm counter equals our load value,
    //we want our pwm pin to go high, and when the counter matches our comparator A value while counting down, we want our pwm pin to go down.
    PWM0GENA |= (1<<2) | (1<<3) | (1<<7);
    //Enable the generator counter
    PWM0CTL |= (0x1);
    //Enable the pwm output for channel 0
    PWM0EN |= 0x1;
    GPIODATA_A = (1<<2);

    //Enable I2C 1, which is pa6 pa7
    RCGCI2C |= 0x2;

    I2CMCR |= 0x10;

    //Digitally enabling  PA6-7 for SSI for I2C
    GPIODENA |= 0x000000C0;

    //Setting PA6-PA7 to be alternate function pins
    GPIOA_AFSEL |= 0x000000C0;

    //Choosing alternate functions for I2C
    GPIOAPCTL |= 0x33000000;

    //Setting SDA (PA7) to open drain
    GPIOODR |= 0x00000080;

    //Setting pull up register
    GPIOAPUR |= 0x000000C0;

    //Setting the clock period
    I2CMTPR = 0x7;

    while(1){
        char inp = getInput();
        if (inp >= '1' && inp <= '8' && state<2){
            PWM0LOAD = 5000*(inp-'0');
            PWM0CMPA = 0.5*5000*(inp-'0');
            if (state == 1){
                writeChar(inp);
            }
            //Prevents multiple records.
            while(inp == getInput()){};

        }
        else if (inp == 'A' || inp == 'B' || inp == 'C'){
            state = inp-'A';
            if(state == 0){
                GPIODATA_A = (1<<2);
            }
            else if (state == 1){
                GPIODATA_A = (1<<3);
                curIndex = 0;
            }
            else if (state == 2){
                GPIODATA_A = (1<<4);
            }
        }
        if (inp == 'C'){
            //Array version
            int i;
            for (i = 0; i<curIndex; i++){
                char curInp = readChars(i);
                PWM0LOAD = 5000*(curInp-'0');
                PWM0CMPA = 0.5*5000*(curInp-'0');
                delay(2000000);
                PWM0CMPA = 0;
                delay(100000);
            }
            PWM0CMPA = 0;
        }
        PWM0CMPA = 0;
    }
    return 0;
}
