#include <msp430.h>

/* Chip select - P8.4
 * Reset - P3.5
 * AO (Register select) - P3.6
 * SCL - P8.1
 * SI - P8.2 */

void RES_state (unsigned int rs);
void CS_state (unsigned int cs);
void AO_state (unsigned int ao);
void SCL_state(unsigned int sck);
void SI_state(unsigned int sda);
void GLCD_GoTo (unsigned char col_addr, unsigned char pag_addr);
void GLCD_writeCommand (unsigned char cmd);
void GLCD_writeData (unsigned char data);
void GLCD_display(unsigned col_inc,unsigned page,unsigned char *buf);

static const unsigned char input[] = { //generated using "GLCD Font Creator"
                                           0x07, 0x00, 0xC0, 0xF8, 0x3F, 0x23, 0x3F, 0xF8, 0xC0,  // Code for char A <--- 0x07 changed
                                           0x07, 0x00, 0xFF, 0xFF, 0x89, 0x89, 0xFF, 0x76, 0x00,  // Code for char B
                                           0x07, 0x00, 0x3C, 0x7E, 0xC3, 0x81, 0xC3, 0x42, 0x00,  // Code for char C
                                           0x07, 0x00, 0xFF, 0xFF, 0x81, 0xC3, 0x7E, 0x3C, 0x00,  // Code for char D
                                           0x06, 0x00, 0xFF, 0xFF, 0x89, 0x89, 0x81, 0x00, 0x00,  // Code for char E
                                           0x06, 0x00, 0xFF, 0xFF, 0x09, 0x09, 0x01, 0x00, 0x00,  // Code for char F
                                           0x07, 0x00, 0x3C, 0x7E, 0xC3, 0x91, 0xF3, 0xF2, 0x00,  // Code for char G
                                           0x08, 0x00, 0xFF, 0xFF, 0x08, 0x08, 0x08, 0xFF, 0xFF,  // Code for char H
                                           0x03, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char I
                                           0x06, 0x00, 0x40, 0xC0, 0x80, 0xFF, 0x7F, 0x00, 0x00,  // Code for char J
                                           0x07, 0x00, 0xFF, 0xFF, 0x18, 0x7E, 0xE3, 0x81, 0x00,  // Code for char K
                                           0x06, 0x00, 0xFF, 0xFF, 0x80, 0x80, 0x80, 0x00, 0x00,  // Code for char L
                                           0x08, 0x00, 0xFF, 0xFF, 0x0E, 0x18, 0x0E, 0xFF, 0xFF,  // Code for char M
                                           0x07, 0x00, 0xFF, 0xFE, 0x0C, 0x30, 0xFF, 0xFF, 0x00,  // Code for char N
                                           0x08, 0x00, 0x3C, 0x7E, 0xC3, 0x81, 0xC3, 0x7E, 0x3C,  // Code for char O
                                           0x06, 0x00, 0xFF, 0xFF, 0x11, 0x1F, 0x0E, 0x00, 0x00,  // Code for char P
                                           0x08, 0x00, 0x3C, 0x7E, 0xC3, 0x81, 0xC3, 0x7E, 0x3C,  // Code for char Q
                                           0x08, 0x00, 0xFF, 0xFF, 0x11, 0x31, 0x7F, 0xCE, 0x80,  // Code for char R
                                           0x06, 0x00, 0x46, 0xCF, 0x99, 0xF3, 0x62, 0x00, 0x00,  // Code for char S
                                           0x07, 0x00, 0x01, 0x01, 0xFF, 0xFF, 0x01, 0x01, 0x00,  // Code for char T
                                           0x07, 0x00, 0x7F, 0xFF, 0x80, 0x80, 0xFF, 0x7F, 0x00,  // Code for char U
                                           0x08, 0x00, 0x01, 0x0F, 0x7C, 0xE0, 0x7C, 0x0F, 0x01,  // Code for char V
                                           0x08, 0x00, 0x01, 0x0F, 0x7E, 0xF0, 0x3E, 0x07, 0x3E,  // Code for char W
                                           0x08, 0x00, 0x81, 0xC3, 0x7E, 0x18, 0x7E, 0xC3, 0x81,  // Code for char X
                                           0x08, 0x00, 0x01, 0x07, 0x0E, 0xF8, 0xF8, 0x0E, 0x07,  // Code for char Y
                                           0x07, 0x00, 0x81, 0xE1, 0xB9, 0x9D, 0x87, 0x81, 0x00,  // Code for char Z
                                           };

void GLCD_initPorts(void)
{
    /*P1SEL &= ~(0x07);
    P3SEL &= 0x9F; //set pins P3.6 and P3.5 as GPIO
    P8SEL &= 0xE9; //set pins P8.1, P8.2 and P8.4 as GPIO

    P1DIR |= 0x07;
    P3DIR |= 0x60; //set pins P3.6 and P3.5 as output
    P8DIR |= 0x16; //set pins P8.1, P8.2 and P8.4 as output

    AO_state(1);
    RES_state(1);
    CS_state(0);
    SCL_state(0);
    SI_state(0);
*/

    P1DIR &= ~(0x07);
    P1OUT |=  (0x07);
    P1REN  |= (0x07);
    P1IES |= 0x07;
    P1IE  |= 0x07;

  //LCD display
   P8SEL &= 0x60;
   P3SEL &= 0x0c;
   P8DIR |= ~0x60;
   P3DIR |= ~0x0c;
   P8OUT |=  ~0x60;
   P3OUT |=  ~0x0c;
}
void GLCD_init(void)
{
    //12 o'clock (Top View)
    GLCD_writeCommand(0x40); // (2) display start line 0
    GLCD_writeCommand(0xA0); //(8) ADC normal
    GLCD_writeCommand(0xC8); //(15) COM63~COM0
    GLCD_writeCommand(0xA6); //(9) display NORMAL
    GLCD_writeCommand(0xA2); //(11) LCD bias set 1/9 (Duty 1/65)
    GLCD_writeCommand(0x2F); // (16) Booster, regulator & follower on <---- POWER ON?
    GLCD_writeCommand(0xF8); // (20) Booster radio set 4x
    GLCD_writeCommand(0x00); // (20) Booster radio set 4x
    GLCD_writeCommand(0x27); // (17) contrast set
    GLCD_writeCommand(0x81); // (18)  Electronic volume mode set
    GLCD_writeCommand(0x16); // (18)  Electronic volume register set
    GLCD_writeCommand(0xAC); // (19) static indicator off
    GLCD_writeCommand(0x00); //(19) static indicator mode (no flashing?)
    GLCD_writeCommand(0xAF); //(1) display ON
}
void GLCD_reset(void)
{
    volatile int k;
    AO_state(1);
    RES_state(1);
    CS_state(0);
    __no_operation();
    RES_state(0);
    for(k=0;k<2000;k++);
    RES_state(1);
    for(k=0;k<2000;k++);
}
void RES_state (unsigned int rs)  //P3.5
{
    if (rs==1)
            P3OUT |= 0x20;
    else if (rs==0)
        P3OUT &= 0xDF;
}

void CS_state (unsigned int cs) //P8.4
{
    if (cs==1)
        P3OUT |= 0x10; //CS_state enable
    else
        P3OUT &= 0xEF; //CS_state disable
}
void AO_state (unsigned int ao) //P3.6
{
    if (ao==1)
            P8OUT |= 0x40; //data register
    else
        P8OUT &= 0xBF; //command register
}
void SCL_state(unsigned int sck) //P8.1
{
    if (sck==1)
        P8OUT |= 0x02;
    else
        P8OUT &= 0xFD;
}
void SI_state(unsigned int sda) //P8.2
{
    if (sda==1)
        P8OUT |= 0x04; //send 1
    else
        P8OUT &= 0xFB; //send 0
}
void GLCD_clear_screen(void)
{
    int i = 0, q = 0;
    for (i = 0; i<8; i++) //page length = 8
    {
        for (q = 0; q<132; q++) //column 4 to 131
        {
            GLCD_GoTo(q,i);
            GLCD_writeData(0);
        }
    }
}
void GLCD_GoTo (unsigned char col_addr, unsigned char pag_addr)
{
    GLCD_writeCommand(0x10 | (col_addr>>4)) ; //first half nibble
    GLCD_writeCommand(0x00 | (col_addr&0x0F)) ; //second half nibble
    GLCD_writeCommand(0xB0 | pag_addr);
}
void GLCD_display(unsigned col_inc,unsigned page,unsigned char *buf)
{
    unsigned int yaddr, xaddr, i, n, pp, num, k;
    yaddr = col_inc;
    xaddr = page;
    i=0;
    while(buf[i]!='\0')
    {
        n=buf[i]-64;
        pp = 9*(n-1);
        num = 9*n;
        GLCD_GoTo(yaddr,xaddr);
        for(k=pp;k<num;k++)
        {
            GLCD_writeData(input[k]);
        }
        yaddr++;
        i++;

    }
}
void GLCD_writeCommand (unsigned char cmd)
{
    CS_state(0);
    __no_operation();
    AO_state(0);
    __no_operation();

    signed int i;
    for (i = 8; i>0; i--)
    {
        SCL_state(0);
        __no_operation();
        //MSB first
        if (cmd & (1<<7))
            SI_state(1);
        else
            SI_state(0);
        cmd= cmd <<1;
        SCL_state(1);
        __no_operation();
    }
    CS_state(1);
    __no_operation();
    AO_state(1);
}
void GLCD_writeData(unsigned char data)
{
    CS_state(0);
    __no_operation();
    AO_state(1);
    __no_operation();

    signed int i;
    for (i =8; i>0; i--)
    {
        SCL_state(0);
        __no_operation();
        //MSB first
        if (data & (1<<7))
            SI_state(1);
        else
            SI_state(0);
        data = data <<1;
        SCL_state(1);
        __no_operation();
    }
    CS_state(1);
    __no_operation();
    AO_state(0);
}
/**
 * main.c
 */

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    GLCD_initPorts();
    GLCD_reset();
    GLCD_init();
    GLCD_clear_screen();

    while(1)
    {
        GLCD_display(0x04,0xB0, "EDCBA");
    }

    //return 0;
}
