//The address here is the base address for system control registers + the offset for the GPIO port clock gating to control the peripheral for ports
#define RCGCGPIO (*((volatile unsigned long *) 0x400FE608))

//The address here is the base address for port A + the offset that enables digital operations on it's pins
#define GPIODENA (*((volatile unsigned long *) 0x4000451C))

//The address here is the base address for port A + the offset for it's corresponding GPIODIR
#define GPIODIRA (*((volatile unsigned long *) 0x40004400))

//The address here is the base address for port A + the offset for the pins PA3-PA5, Register Select, Read/Write, and Enable Input respectively
#define GPIODATA_A (*((volatile unsigned long *) 0x400040E0))

//The address here is the base address for port B + the offset that enables digital operations on it's pins
#define GPIODENB (*((volatile unsigned long *) 0x4000551C))

//The address here is the base address for port B + the offset for it's corresponding GPIODIR
#define GPIODIRB (*((volatile unsigned long *) 0x40005400))

//The address here is the base address for port B + the offset for the pins PB0-PB7
#define GPIODATA_B (*((volatile unsigned long *) 0x400053FC))

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

//mask
#define RS 0x8;
#define RW 0x10;
#define EN 0x20;
//A state is 0, B state is 1, Display state is 2, global so we can modify it in functions
//Might merge start/initial state
int state = 0;
//Tracking nums a and b and the current number of digits for the state
int a = 0;
int b = 0;
int curDigitNum = 0;

//Waits for previous function to finish first
void waitOnBusy(){
    while(GPIODATA_B & 0x80){}
}

void delay(){
    int i;
    for( i = 0; i < 25000;i++){}
}

void buttonDelay(){
    int n;
    for (n = 0; n<400000; n++){};
}

void lcdWriteIR(unsigned char data){
    //waitOnBusy();
    GPIODATA_B = data;
    delay();
    GPIODATA_A &= ~RS;
    delay();
    GPIODATA_A &= ~RW;
    //First set the RS values and RW values
    delay();
    GPIODATA_A |= EN;
    //Setting the enable input value
    //GPIODATA_A |= (1<<5);
    //Putting data in
    //Delay to match worst case timing diagrams, 3 dummy instructions
    delay();
    //Setting E low
    GPIODATA_A &= ~EN;
}

void lcdWriteDr(unsigned char data){
    //waitOnBusy();
    //First set the RS values and RW values
    GPIODATA_B = data;
    GPIODATA_A |= RS;
    GPIODATA_A &= ~RW;
    //GPIODATA_A |= (0x01<<4);
    //Setting the enable input value
    delay();
    GPIODATA_A |= EN;
    //Putting data in
    //Delay to match worst case timing diagrams, 3 dummy instructions
    delay();
    //Setting E low
    GPIODATA_A &= ~EN;
}

//cmd value
//#define Function_set_8bit 0x38
//#define Entry_mode        0x06

//DB0-DB7 values for functions
//RS RW 0 for all of these
//Clear display 00000001, clears display and resets cursor
//Return home 0000001X, resets cursor
//Init Function set 00111000
//Display on DB0-7 00001100
//Init Entry Mode set 00000110
void initLCD(){
    GPIODATA_A &= ~RS;
    GPIODATA_A &= ~RW;
    //Function Set
    lcdWriteIR(0x38);
    //Display settings
    //lcdWriteIR(0x0C);
    lcdWriteIR(0x0C);
    //Clear display
    lcdWriteIR(0x01);
    ////Entry mode set
    lcdWriteIR(0x06);
}

void clearRow(){
    int i = 0;
    for (i = 0; i<16; i++){
        lcdWriteDr(32);
    }
    lcdWriteIR(0x02);
}


//Changes to start state
void changeToStart(){
    lcdWriteIR(0x80);
    clearRow();
    lcdWriteIR(0x02);
    a = 0;
    b = 0;
    curDigitNum = 0;
    state = 0;
}

//goes to initial
void changeToInitial(){
    //Clear display then switch to start state
    lcdWriteIR(0x80);
    lcdWriteIR(0x01);
    lcdWriteIR(0x02);
    changeToStart();
}

//Switch to display state
void displayState(){
    //Clears display, moves to bottom, then writes in data
    lcdWriteIR(0x01);
    lcdWriteIR(0x02);
    lcdWriteIR(0xC0);
    long newNum = a*b;
    //Handles edge case if one of our values is zero, because our number to string conversion only works for at least 1 non zero digit.
    if (newNum == 0){
        lcdWriteDr('0');
        changeToStart();
        return;
    }
    char prod[20];
    int curDigit;
    for(curDigit = 0; newNum > 0; ++curDigit)
    {
      prod[curDigit] = newNum%10+'0';
      newNum/=10;
    }
    int priN;
    for (priN = curDigit; priN > 0; --priN){
      lcdWriteDr(prod[priN-1]);
    }
    changeToStart();
}

//Changes state accordingly based on 8 digits reached or */D for A and B respectively
void nextState(){
    if (state == 0){
        lcdWriteIR(0x02);
        clearRow();
        curDigitNum = 0;
        state = 1;
    }
    else if (state == 1){
        lcdWriteIR(0x01);
        lcdWriteIR(0x02);
        curDigitNum = 0;
        state = 2;
        displayState();
    }
}

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
            else if ((GPIODATA_E & 0x8)){
                inp = '9';
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
            if ((GPIODATA_E & 0x2)){
                inp = '*';
            }
            else if ((GPIODATA_E & 0x4)){
                inp = '0';
            }
            else if ((GPIODATA_E & 0x8)){
                inp = '#';
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

int main(void)
{
    //This is us turning on the clocking of port A, B, C, and E
    RCGCGPIO = 0x00000017;

    //This is us turning on the digital operation enabler for pin PA3-PA5
    GPIODENA |= 0x00000038;

    //This is us turning on the digital operation enabler for pin PB0-PB7
    GPIODENB |= 0x000000FF;

    //This is us turning on the digital operation enabler for pins PC4-PC7
    GPIODENC |= 0x000000F0;

    //This is us turning on the digital operation enabler for pins PE1-PE4
    GPIODENE |= 0x0000001E;

    //This is us setting the PA3-PA5 to be output
    GPIODIRA |= 0x00000038;

    //This is us setting the PB0-PB7 to be output
    GPIODIRB |= 0x000000FF;

    //This is us setting our PC pins used to be outputs, and our PE pins used to be inputs
    GPIODIRC |= 0x000000F0;
    GPIODIRE &= 0xFFFFFFE1;
    initLCD();
    changeToInitial();
    lcdWriteIR(0x02);
    while(1)
    {
        char inp = getInput();
        if (inp >= '0' && inp <= '9' && state != 2){
            lcdWriteDr(inp);
            if (state == 0){
                a *= 10;
                a += (inp-'0');
            }
            else if (state == 1){
                b *= 10;
                b += (inp-'0');
            }
            curDigitNum ++;
            if (curDigitNum == 8){
                nextState();
            }
        }
        else if (inp == 'C'){
            changeToInitial();
        }
        else if (inp == '*' && state == 0){
            nextState();
        }
        else if (inp == '#'){
            displayState();
        }
    }
	return 0;
}
