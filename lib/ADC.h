/* 
 * File:   ADC.h
 * Author: diasd1
 *
 * Created on 5. Dezember 2019, 08:02
 * 
 * Use this library to rapidly use any analog input on a PIC16F1787 (it is possible that some bits have to be manually adjusted when using another PIC)
 * 
 * ** FUNCTIONS **
 * 
 * § = Function is Necessary
 * 
 * § ADC init can be used in initialisation to initialise PORT A and the adc
 * 
 * ADC calc is a tool to calculate the measured value to a custom timescale
 * It isn't neccessary for the ADC to work
 * when calling ADC calc, you have to give the ADC read result, for this reason, it is recommended to use ADC calc like this:
 * ADC_calc(ADC_read(0), 12, 0.0, 99.0)
 * this returns a value on a scale from 0 and 99 depending on the 12 bit value measured by the ADC
 * 
 * 
 * ** Polling **
 * ADC poll read can be used to extract the measured value.
 * Please note that you could get stuck in ADC read if you didn't initialise the ADC and Ports properly
 * For this reason, it is recommended to use ADC init
 * 
 * ** Interrupts **
 * § ADC int start lets you start the conversion.
 * You can select a channel you want to convert from.
 * The function itself should be used when working with interrupts. If you're working with pulling, it is recommended to use ADC read
 * This function has to be combined with ADC readInt
 * 
 * ADC int read can be used to extract the measured value.
 * This function should be called either from the interrupt function itself or right after the interrupt
 * This function has to be combined with ADC start
 * 
 * ** ERRORS **
 * 
 * If ADC init gets a value, which isn't supported by the pic (every value except 10 and 12)
 * adc will be set to 12 bit resolution
 * 
 * If ADC calc gets a undefined value, it will return 0xFF
 */

#ifndef ADC_H
#define	ADC_H

#ifdef	__cplusplus
extern "C" {
#endif

#ifdef	__cplusplus
}
#endif

// *** GENERAL ***

/**
 * @brief               Initialisierung
 * @param resolution    10 for 10 bit value, 12 for 12 bit value, if another value is given, value will have 12 bits
 */
void ADC_init(char);

/**
 * @brief           rechnet die erhaltenen ADC werte auf eine benutzerdefinierte Skala um;
 * @param read      erhaltenes resultat
 * @param bitCount  anzahl bits (für auflösung)
 * @param val_min   minimum value of range
 * @param val_max   maximum value of range
 * @return          calculated value in range, returns 0xFF if function wasn't used properly
 */
float ADC_calc(int, char, float, float);

// *** POLLING ***

/**
 * @brief           read ADC, after starting the conversion, return it as a number < 4096 & > 0
 * @param input:    Input Channel Select as binary number
 * @return          digital value
 */
int ADC_poll_read(char);

// *** INTERRUPTS ***

/**
 * @brief Initialize the interrupt for the adc
 */
void ADC_int_init(void);

/**
 * @brief:          start conversion
 * @param input:    select channel to convert from
 */
void ADC_int_start(char);

/**
 * @brief           read ADC, after starting the conversion, return it as a number < 4096 & > 0
 * @return          digital value
 */
int ADC_int_read(void);

#endif	/* ADC_H */