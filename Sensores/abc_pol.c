/*!
\file   LCD.c
\date   2024-07-05
\author Daniel Rengifo <danielrengifor@unicauca.edu.co>
\brief  LCD 16x2 mode 4 bits.
*/
#include    <xc.h>
#include    <pic16f887.h>
#include    "config.h"
#include    <stdio.h>
#include "LCD.h"
#include    <string.h>
#define beta 4090

void read_temperatura(void);
void read_Luz(void);
void read_Sonido(void);
void read_Potenciometro(void);

float temperatura;
unsigned char Potenciometro;
unsigned char Luz;
unsigned char Sonido;
//-----------------------------------------------
void main(void) {
    //OSCCON = 0b01110001;
    OSCCON = 0x71;
    TRISA0=1;
    TRISE1=1;
    TRISB1=1;
    TRISB5=1;
    ANSEL = 0x041;//  Configura el Puerto como Entrada Analógica.
    ANSELH = 0x24;
    TRISC1=0;
    TRISC2=0;
    TRISC3=0;
            
    ADCON1bits.ADFM = 0; //  Justificación Izquierda (modo-8bits).
    ADCON1bits.VCFG0 = 0; //  Selecciona Voltajes de Referencia (5v-0v).
    ADCON1bits.VCFG1 = 0; //  Selecciona Voltajes de Referencia (5v-0v). 
    ADCON0bits.ADCS = 0b01; //  Tiempo de Conversión Fosc/8.
   
    
    LCD_Init();
    LCD_String_xy(0,0,"HOLA");
    __delay_ms(2000);

    while (1) {
        
        LCD_Clear();
        
        read_temperatura();
        read_Luz();
        read_Sonido();
        read_Potenciometro();
        
        
        //--------------------------------------
        char bufferT [8];
        sprintf(bufferT,"T; %.2f",temperatura);
        
        char bufferS [8];
        sprintf(bufferS,"S; %d",Sonido);
        
        char bufferTS[16];
        strcpy (bufferTS,bufferT);
        strcat (bufferTS,bufferS);
        
        LCD_String_xy(0,0,bufferTS);
        //--------------------------------------
        char bufferL [8];
        sprintf(bufferL,"L; %d",Luz);
        
        char bufferP [8];
        sprintf(bufferP,"P; %d",Potenciometro);
        
        char bufferLP[16];
        strcpy (bufferLP,bufferL);
        strcat (bufferLP,bufferP);
        
        LCD_String_xy(2,0,bufferLP);
        //--------------------------------------
        PORTCbits.RC1= 0;
        PORTCbits.RC2= 0;
        PORTCbits.RC3= 0;
        //-------------------------------------
        if(temperatura>20){

        PORTCbits.RC1= 1; //rojo
        
        }else if((temperatura<20) && (Luz>100)) {
        
        PORTCbits.RC2= 1; //azul 
        
        }else{
        
        PORTCbits.RC3= 1;//verde
        }
        //---------------------------------------
         __delay_ms(3000);
           
       
        
    }
}
//----------------------------------------------------------------------------


/*---------------------------------------------------------------------------
 NAME: read_temperatura
----------------------------------------------------------------------------
PARAMS:   
dat: none
return: none
----------------------------------------------------------------------------
PURPOSE:interpretar el sensor analogico de temperatura y 
hacer una conversion matematica y asignarlo a la variable "temperatura"
----------------------------------------------------------------------------*/

void read_temperatura(void){
    ADCON0bits.CHS = 0b0000;
    ADCON0bits.ADON = 1; //  Habilita el Módulo AD.
        __delay_us(30);
        ADCON0bits.GO = 1; //  Inicia la COnversió AD.
        while (ADCON0bits.GO); //  Espera a que termine la conversión AD.
        ADCON0bits.ADON = 0; //  Habilita el Módulo AD.
        temperatura = 1023-(ADRESH*4);
        temperatura=(float)(temperatura*0.04058);
        __delay_ms(500);
 
}



/*---------------------------------------------------------------------------
 NAME: read_Luz
----------------------------------------------------------------------------
PARAMS:   
dat: none
return: none
----------------------------------------------------------------------------
PURPOSE:interpretar el sensor analogico de Luz y asignarlo a la variable "Luz"
----------------------------------------------------------------------------*/
void read_Luz(void){
    ADCON0bits.CHS = 0b0110;
    ADCON0bits.ADON = 1; //  Habilita el Módulo AD.
        __delay_us(30);
        ADCON0bits.GO = 1; //  Inicia la COnversió AD.
        while (ADCON0bits.GO); //  Espera a que termine la conversión AD.
        ADCON0bits.ADON = 0; //  Habilita el Módulo AD.
        Luz = ADRESH;
        __delay_ms(500);
}



/*---------------------------------------------------------------------------
 NAME: read_Sonido
----------------------------------------------------------------------------
PARAMS:   
dat: none
return: none
----------------------------------------------------------------------------
PURPOSE:interpretar el sensor analogico de sonido y asignarlo a la variable "Sonido"
----------------------------------------------------------------------------*/
void read_Sonido(void){
    ADCON0bits.CHS = 0b1010;
    ADCON0bits.ADON = 1; //  Habilita el Módulo AD.
        __delay_us(30);
        ADCON0bits.GO = 1; //  Inicia la COnversió AD.
        while (ADCON0bits.GO); //  Espera a que termine la conversión AD.
        ADCON0bits.ADON = 0; //  Habilita el Módulo AD.
        Sonido = ADRESH;
        __delay_ms(500);
}



/*---------------------------------------------------------------------------
 NAME: read_Potenciometro
----------------------------------------------------------------------------
PARAMS:   
dat: none
return: none
----------------------------------------------------------------------------
PURPOSE:interpretar el valor analogico que tiene el potenciometro y asignarlo a la variable "Potenciometro"
----------------------------------------------------------------------------*/
void read_Potenciometro(void){
    ADCON0bits.CHS = 0b1101;
    ADCON0bits.ADON = 1; //  Habilita el Módulo AD.
        __delay_us(30);
        ADCON0bits.GO = 1; //  Inicia la COnversió AD.
        while (ADCON0bits.GO); //  Espera a que termine la conversión AD.
        ADCON0bits.ADON = 0; //  Habilita el Módulo AD.
        Potenciometro = ADRESH;
        __delay_ms(500);
}
