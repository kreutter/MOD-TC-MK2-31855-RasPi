#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "wiringPi.h"
#include "wiringPiI2C.h"

static int I2C_ADDR = 0x23;

int main (void)
{
    int fd ;
    unsigned int x ;
    unsigned char buf [5] ;
    
    wiringPiSetup () ;
    
    if ((fd = wiringPiI2CSetup (I2C_ADDR)) < 0)
    {
        fprintf (stderr, "Unable to open I2C device: %s\n", strerror (errno)) ;
        exit (-1) ;
    }
    
    wiringPiI2CWrite (fd, 0x01) ;
    wiringPiI2CWrite (fd, 0x01) ;	// what is the second parameter for?
    wiringPiI2CWrite (fd, 0x21) ;
    delay(100);
    
    x = read (fd, buf, 4) ;
    
    printf ("raw data (%d bytes): %02X %02X %02X %02X\n", x, buf[0], buf[1], buf[2], buf[3]) ;
    
    unsigned long int temperature = 0;
    
    for(int i = 0; i < 4; i++)
    {
        temperature |= buf[3-i];
        if(i!=3)
            temperature <<= 8;
    }
    
    unsigned long int open_circuit_mask = 0x00000001;
    unsigned long int gnd_short_mask 	= 0x00000002;
    unsigned long int vcc_short_mask 	= 0x00000004;
    unsigned long int fault_mask 		= 0x00010000;
    unsigned long int sign_int_mask		= 0x00008000;
    unsigned long int sign_ext_mask		= 0x80000000;
    unsigned long int foobar			= 0;
    
    float external = 0;
    float internal = 0;
    
    if(fault_mask & temperature)
    {
        if(open_circuit_mask & temperature)
        {
            fputs("Fault: open circuit!\n",stdout);
            exit(1);
        }
        if(gnd_short_mask & temperature)
        {
            fputs("Fault: GND short!\n",stdout);
            exit(1);
        }
        if(vcc_short_mask & temperature)
        {
            fputs("Fault: VCC short!\n",stdout);
            exit(1);
        }
        fputs("Fault: unknown error!\n",stdout);
        
    }
    else
    {
        /*PROCESS INTERNAL TEMPERATURE*/
        foobar = temperature;
        foobar &= 0x0000FFFF;
        foobar >>= 4;
        
        internal = (foobar & 0x00F) * 0.0625 + ((foobar >> 4) & 0x7F);
        
        /*CHECK SIGN*/
        if(sign_int_mask & temperature)
        {
            /*NEGATIVE TEMPERATURE*/
            printf("INTERNAL TEMPERATURE: %.4f C\n",  -128+internal);
        }
        else
        {
            /*POSITIVE TEMPERATURE*/
            
            printf("INTERNAL TEMPERATURE: %c%.4f C\n", '+', internal);
        }
        
        
        /*PROCESS EXTERNAL TEMPERATURE*/
        foobar = temperature;
        foobar >>= 16;
        foobar >>= 2;
        
        external = (foobar & 0x0003) * 0.25 + ((foobar >> 2) & 0x7FF);
        
        if(sign_ext_mask & temperature)
        {
            /*NEGATIVE TEMPERATURE*/
            printf("EXTERNAL TEMPERATURE: %.2f C\n",  -2048+external);
        }
        else
        {
            /*POSITIVE TEMPERATURE*/
            
            printf("EXTERNAL TEMPERATURE: %c%.2f C\n", '+', external);
        }
        
    }
    
    return 0 ;
}