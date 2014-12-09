/*
PIC16F877A Code for i2c in master mode.
Developed By Arun Chettoor
Can be used freely in any project under GPL license.
E-mail : arunkrishnanc@gmail.com
*/


#include"i2c.h"

#define clockFreq 10 //in MHz
#define i2cClock 0 // 0 for 100KHz and 1 for 400KHz

void i2c_init(){
	char i2cFreq;
	TRISC3=1;           // set SCL and SDA pins as inputs
 	TRISC4=1;

 	SSPCON = 0x38;      // set I2C master mode
 	SSPCON2 = 0x00;

 	#if i2cClock==0
 		i2cFreq=100;
 	#else
 		i2cFreq=400;
 	#endif

 	SSPADD = (char)((clockFreq*1000)/i2cFreq)-i2cFreq; 

 	PSPIF=0;      // clear SSPIF interrupt flag
 	BCLIF=0;      // clear bus collision flag
}

/*********************************************
To wait until i2c is idle.
*********************************************/

void i2c_waitForIdle()
{
 while (( SSPCON2 & 0x1F ) | STAT_RW ) {}; // wait for idle and not writing
}

/********************************************
To sent the start sequence
**********************************************/

void i2c_start()
{
 i2c_waitForIdle();
 SEN=1;
}

/*********************************************
To sent a repeated start sequence
*********************************************/

void i2c_repStart()
{
 i2c_waitForIdle();
 RSEN=1;
}

/*********************************************
To sent a stop sequence
**********************************************/

void i2c_stop()
{
 i2c_waitForIdle();
 PEN=1;
}

/*********************************************
Read char from i2c buffer register and return the value
*********************************************/

unsigned char i2c_read( unsigned char ack )
{
 unsigned char i2cReadData;

 i2c_waitForIdle();

 RCEN=1;

 i2c_waitForIdle();

 i2cReadData = SSPBUF;

 i2c_waitForIdle();

 if ( ack )
  {
  ACKDT=0;
  }
 else
  {
  ACKDT=1;
  }
  ACKEN=1;               // send acknowledge sequence

 return( i2cReadData );
}

/*********************************************
Write char data to i2c buffer

*********************************************/

unsigned char i2c_write( unsigned char i2cWriteData )
{
 i2c_waitForIdle();
 SSPBUF = i2cWriteData;
 i2c_waitForIdle();
 return ( ! ACKSTAT  ); // function returns '1' if transmission is acknowledged
}

/******************************************************************************************/
