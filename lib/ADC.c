#include <xc.h>

#define _XTAL_FREQ 8000000      // Oszillatorfrequenz PBA6: 8MHz (ohne PLL) // for delay

#define ACQUISITION_TIME_us (2) // time to wait after changing adc channel and before starting the process

// *** GENERAL ***

/**
 * @brief               Initialisierung
 * @param resolution    10 for 10 bit value, 12 for 12 bit value, if another value is given, value will have 12 bits
 */
void ADC_init(char resolution)
{
    if (resolution == 10)
        resolution = 1;
    else
        resolution = 0;
    
	TRISA |= 0b00000011;                                                               // Port A0 & A1 as Input
    ANSELA |= 0b00000011;                                                              // Port A0 & A1 as analog

	ADCON2 = 0x0F;                                                              // negative Reference is equal to Vref neg (single signal input)
    ADCON1 = 0b11010000;                                                        // 2's complement format, Fosc/16, Vref is Vdd / Vss
    ADCON0 = 0b00000101 + (resolution << 7);                                                        // 12 Bit, input AN0 (= A0), Go/Done Bit (must be set later on), Enable 
}

/**
 * @brief           rechnet die erhaltenen ADC werte auf eine benutzerdefinierte Skala um;
 * @param read      erhaltenes resultat
 * @param bitCount  anzahl bits (für auflösung)
 * @param val_min   minimum value of range
 * @param val_max   maximum value of range
 * @return          calculated value in range, returns 0xFF if function wasn't used properly
 */
float ADC_calc(int read, char bitCount, float val_min, float val_max)
{
    if (val_min > val_max) // error
        return(0xFF); // 0xFF für Fehler

    /* Werte setzen */
    float resolution = 0.0; // standardwert, nur für deklaration
    
    if (bitCount == 12) // 12 bit
        resolution = 4095.0; // maximaler wert mit 12 bit
    else if (bitCount == 10) // 10 bit
        resolution = 1023.0; // maximaler wert mit 10 bit
    else if (bitCount == 8)
        resolution = 255.0;
    else // error
        return(0xFF); // 0xFF für Fehler
    
    /* Berechnen */  
    float range = val_max - val_min; // range berrechnen
    
    /* Zurückgeben */ 
    return((read / resolution) * range + val_min); // berechnet wert auf benutzerdefinierter Skala
}

// *** POLLING ***

/**
 * @brief           read ADC, on channel input, return it as a number < 4096 & > 0
 * @param input:    Input Channel Select as binary number
 * @return          digital value
 */
int ADC_poll_read(char input)
{
    ADCON0bits.CHS = input;
    
    __delay_us(ACQUISITION_TIME_us); // acquisition time
    
    ADCON0bits.ADGO = 1;                                                        // start ad converter
    
    while (ADCON0bits.ADGO);
    
    /* PROCESSING */
    // Read result
    int result = (ADRESH << 8);                                                 // Bits 0b [0000] 0000 0000
    result += ADRESL;

    return((result));
}

// *** INTERRUPTS ***

/**
 * @brief Initialize the interrupt for the adc
 */
void ADC_int_init()
{
    ADIE = 1; // enable adc interrupt
    PEIE = 1; // enabeles all active peripheral interrupts
    GIE = 1; // Enables all active interrutps
}

/**
 * @brief:          start conversion
 * @param input:    select channel to convert from
 */
void ADC_int_start(char input)
{
    ADCON0bits.CHS = input;
    
    __delay_us(ACQUISITION_TIME_us); // acquisition time
    
    ADCON0bits.ADGO = 1;                                                        // start ad converter
}

/**
 * @brief           read ADC, on channel input, return it as a number < 4096 & > 0
 * @return          digital value
 */
int ADC_int_read()
{
    /* PROCESSING */
    // Read result
    int result = (ADRESH << 8);                                                 // Bits 0b [0000] 0000 0000
    result += ADRESL;                                                           // Bits 0b 0000 [0000 0000]

    return(result);
}