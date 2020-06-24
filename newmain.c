#include <pic18f4550.h>
#include <xc.h>

void main(void) {
    ////////////////////////////////////////////////////////////////////////////
    // SETUP DE PUERTOS
    ////////////////////////////////////////////////////////////////////////////
    
    ADCON1 = 0b00001111;    // todos los pines son digitales [AN0-AN12]
    TRISA = 0b0000001;      // A0 es una entrada, el resto salidas [A1-A6] 
    TRISB = 0b10000000;     // B7 es entrada, B0-B6 salidas
    LATAbits.LA1 = 0;       // La salida A1 es 1 (estado inicial)
    
    ////////////////////////////////////////////////////////////////////////////
    // SETUP GENERAL DE INTERRUPCIONES
    ////////////////////////////////////////////////////////////////////////////
    RCONbits.IPEN = 0;         //  Disable Interrupt Priority
    INTCONbits.GIEH = 1;       // Enable de Interrupciones no enmascaradas
    INTCONbits.GIEL = 0;       // Desabilita Interrupciones de Perifericos
     
    //  SETUP DE INT0: usado para detectar cuando apretan el boton.
    // INT0 se encarga de encender o apagar el LED según sea necesario
    INTCONbits.INT0IF   = 0;   // Se limpia la bandera de INT0
    INTCON2bits.INTEDG0 = 1;   // INT0 en flanco positivo
    INTCONbits.INT0IE   = 1;   // Enable de INT0
    
    // SETUP DE INT1
    // INT1 se encarga de desabilitar INT0
    /*INTCON3bits.INT1IF  = 0;   // Se limpia la bandera de INT1
    INTCON2bits.INTEDG1 = 1;   // INT1 con flanco positivo
    INTCON3bits.INT1IP  = 0;   // Baja prioridad a la INT1
    INTCON3bits.INT1IE  = 1;   // Enable de INT1*/
    
    // Ciclo Principal
    while(1){}
    return;
}

void __interrupt() luces(void){
    // Handler particular para la interrupción INT0
    if (INTCONbits.INT0IF == 1){
        
        // Toggle a la Salida
        LATAbits.LA1 = ~LATAbits.LA1;
        
        // Se Limpia INT0IF
        INTCONbits.INT0IF = 0;
    }
}
    