#include <pic18f4550.h>
#include <xc.h>

#pragma config WDT = OFF
#pragma config WDTPS = 1024

void main(void) {
    ////////////////////////////////////////////////////////////////////////////
    // SETUP DE PUERTOS
    ////////////////////////////////////////////////////////////////////////////
    
    ADCON1 = 0b00001111;    // todos los pines son digitales [AN0-AN12]
    TRISA = 0b0000001;      // A0 es una entrada, el resto salidas [A1-A6] 
    TRISB = 0b10000000;     // B7 es entrada, B0-B6 salidas
    LATAbits.LA1 = 0;       // La salida de las luces está apagada al principio
    
    ////////////////////////////////////////////////////////////////////////////
    // SETUP GENERAL DE INTERRUPCIONES
    ////////////////////////////////////////////////////////////////////////////
    RCONbits.IPEN = 0;         // Disable Interrupt Priority
    INTCONbits.GIEH = 1;       // Enable de Interrupciones no enmascaradas
    INTCONbits.GIEL = 0;       // Deshabilita Interrupciones de Perifericos
     
    //  SETUP DE INT0: usado para detectar cuando apretan el boton.
    // INT0 se encarga de encender o apagar el LED según sea necesario
    INTCONbits.INT0IF   = 0;   // Se limpia la bandera de INT0
    INTCON2bits.INTEDG0 = 1;   // INT0 en flanco positivo
    INTCONbits.INT0IE   = 1;   // Enable de las luces
    
    // SETUP DE INT1
    // INT1 se encarga de desabilitar INT0
    INTCON3bits.INT1IF  = 0;   // Se limpia la bandera de INT1
    INTCON2bits.INTEDG1 = 1;   // INT1 con flanco positivo
    INTCON3bits.INT1IP  = 0;   // Baja prioridad a la INT1
    INTCON3bits.INT1IE  = 0;   // Enable del botón del modo de luces es 0
    
    ////////////////////////////////////////////////////////////////////////////
    // SETUP DE TIMER
    ////////////////////////////////////////////////////////////////////////////
    INTCONbits.TMR0IE = 0; // Deshabilita la interrupción por timer
    
    T0CONbits.T08BIT = 1; // Timer 0 a 8 bit timer
    T0CONbits.T0CS   = 0; // Usa Clock Interno
    T0CONbits.T0SE   = 0; // Flanco positivo para la patilla T0CKl
    T0CONbits.PSA    = 0; // Usa pre escala
    
    T0CONbits.T0PS2 = 0;
    T0CONbits.T0PS1 = 0;  // 1:4 Preescala de 4 XXXXXXXXXXXXXXXX
    T0CONbits.T0PS0 = 1;
    
    T0CONbits.TMR0ON = 1; // Empieza el timer
    
    // Ciclo Principal
    while(1){}
    return;
}

void __interrupt() luces(void){
    
    // Handler particular para la interrupción INT0
    if (INTCONbits.INT0IF == 1 & INTCONbits.INT0IE == 1){        
        
        LATAbits.LA1 = ~LATAbits.LA1; 
        INTCON3bits.INT1IE = ~INTCON3bits.INT1IE;
        
        if(INTCON3bits.INT1IE == 0){
            LATAbits.LA1 = 0;
        }
        //Si la luz está encendida y la apago no importa en qué modo estoy
        if (LATAbits.LA1 == 0){
            INTCONbits.TMR0IE = 0;  
        }
        
        INTCONbits.TMR0IF = 0;
        INTCONbits.INT0IF = 0;
        INTCON3bits.INT1IF = 0;
    }
    
    //HANDLER PARA LA INTERRUPCIÓN INT1
    if (INTCON3bits.INT1IF == 1 & INTCON3bits.INT1IE == 1){
                       
        // Habilita la interrupción por timer al tocar el boton de parpadeo
        INTCONbits.TMR0IE = ~INTCONbits.TMR0IE;
        if (INTCONbits.TMR0IE == 0){
            LATAbits.LA1 = 1;
        }
        
        INTCONbits.TMR0IF = 0;
        INTCONbits.INT0IF = 0;
        INTCON3bits.INT1IF = 0;
    }
    
    //Handler para la interrupción del CLK
    if(INTCONbits.TMR0IF & INTCONbits.TMR0IE) {
            
        // Enciende y apaga el led
        LATAbits.LA1 = ~LATAbits.LA1;
        
        // Se reinicia la Interrupcion del clock
        INTCONbits.TMR0IF = 0;
        INTCONbits.INT0IF = 0;
        INTCON3bits.INT1IF = 0;   
    }  
}
    