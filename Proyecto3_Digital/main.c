
// Universidad del Valle de Guatemala
// Electronica digital 2
// Proyecto 3
// Maria Andrea Lopez
// Enma Lopez

// Librerias
#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "inc/hw_gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/systick.h"
#include "driverlib/uart.h"

#ifdef DEBUG
void
__error_(char *pcfilename, uint32_t ui32Line)
{
    while(1);

}
#endif

#define XTAL 16000000

uint32_t push1;
uint32_t push2;
uint32_t push3;
uint32_t push4;

uint32_t bandera1;
uint32_t bandera2;
uint32_t bandera3;
uint32_t bandera4;

uint32_t suma;
uint8_t indicadorP = 0;

char banderac;
void display(void);
void InitUART(void);
void parqueo(void);
void initGPIOF(void);

int main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ); // Reloj de 40MHz
    InitUART(); // Configuracion de UART5
    initGPIOF(); //configuracion de puertos

    while (1)
    {
        parqueo();
        display();
        UARTCharPut(UART5_BASE, bandera1);
        UARTCharPut(UART5_BASE, bandera2);
        UARTCharPut(UART5_BASE, bandera3);
        UARTCharPut(UART5_BASE, bandera4);
    }
}

void InitUART(void) // Funcion para inicializar el UART5
{
    /*Enable the GPIO Port A*/
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

    /*Enable the peripheral UART Module 0*/
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART5);

    /* Make the UART pins be peripheral controlled. */
    GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5);

    /* Sets the configuration of a UART. */
    UARTConfigSetExpClk(
            UART5_BASE, SysCtlClockGet(), 115200,
            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

    //UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);
}



void parqueo(void) // Funcion para encender leds correspondientes segun push presionado
{
    //PRIMER PAR DE LEDS
    push1 = GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_4);
    if (((push1 & GPIO_PIN_4) == 0))
    {
        GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, GPIO_PIN_1); //rojo-ocupado
        GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4, 0);
        bandera1 = 0;
        indicadorP = indicadorP - 1;
    }
    if ((push1 & GPIO_PIN_4) != 0)
    {
        GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4, GPIO_PIN_4);  //verde-libre
        GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, 0);
        bandera1 = 1;
        indicadorP = indicadorP + 1;
    }
    //SEGUNDO PAR DE LEDS
    push2 = GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_7);
    if (((push2 & GPIO_PIN_7) == 0))
    {
        GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, GPIO_PIN_2); //rojo-ocupado
        GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_3, 0);
        bandera2 = 0;
        indicadorP = indicadorP - 2;
    }
    if ((push2 & GPIO_PIN_7) != 0)
    {
        GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_3, GPIO_PIN_3);  //verde-libre
        GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, 0);
        bandera2 = 1;
        indicadorP = indicadorP + 2;
    }
    //TERCER PAR DE LEDS
    push3 = GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_6);
    if (((push3 & GPIO_PIN_6) == 0))
    {
        GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0, GPIO_PIN_0); //rojo-ocupado
        GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_6, 0);
        bandera3 = 0;
        indicadorP = indicadorP - 4;
    }
    if ((push3 & GPIO_PIN_6) != 0)
    {
        GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_6, GPIO_PIN_6);  //verde-libre
        GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0, 0);
        bandera3 = 1;
        indicadorP = indicadorP + 4;
    }
    //CUARTO PAR DE LEDS
    push4 = GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_5);
    if (((push4 & GPIO_PIN_5) == 0))
    {
        GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, GPIO_PIN_1); //rojo-ocupado
        GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, 0);
        bandera4 = 0;
        indicadorP = indicadorP - 8;
    }
    if ((push4 & GPIO_PIN_5) != 0)
    {
        GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, GPIO_PIN_2);  //verde-libre
        GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, 0);
        bandera4 = 1;
        indicadorP = indicadorP + 8;
    }
    suma = (bandera1 + bandera2 + bandera3 + bandera4); // Suma para saber cantidad de parqueos ocupados

}

void display(void) // Funcion para mostrar el valor correspondiente a parqueos disponibles
{//MOSTRAR 4
    if (suma == 0)
    {
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0, GPIO_PIN_0);
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, GPIO_PIN_1);
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, GPIO_PIN_2);
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, GPIO_PIN_3);
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4, GPIO_PIN_4);
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5, GPIO_PIN_5);
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_6, GPIO_PIN_6);
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_7, 0);
    }
    //MOSTRAR 3
    if (suma == 1)
    {
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0, 0);
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, GPIO_PIN_1);
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, 0);
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, 0);
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4, GPIO_PIN_4);
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5, 0);
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_6, 0);
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_7, 0);
    }
    //MOSTRAR 2
    if (suma == 2)
    {
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0, GPIO_PIN_0);
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, GPIO_PIN_1);
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, GPIO_PIN_2);
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, GPIO_PIN_3);
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4, 0);
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5, 0);
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_6, GPIO_PIN_6);
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_7, GPIO_PIN_7);
    }
    //MOSTRAR 1
    if (suma == 3)
    {
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0, GPIO_PIN_0);
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, GPIO_PIN_1);
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, GPIO_PIN_2);
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, GPIO_PIN_3);
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4, GPIO_PIN_4);
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5, 0);
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_6, 0);
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_7, GPIO_PIN_7);
    }
    //MOSTRAR 0
    if (suma == 4)
    {
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0, 0);
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, GPIO_PIN_1);
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, 0);
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, 0);
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4, GPIO_PIN_4);
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5, GPIO_PIN_5);
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_6, 0);
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_7, GPIO_PIN_7);

    }
}


//******************* INICIALIZA PUERTOS, LEDS OUTPUTS Y PUSH INPUTS *******************
void initGPIOF(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); // Habilitar reloj para puertos
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3); // Pines como inputs o outputs
    GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
    GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_4 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_6 | GPIO_PIN_7);
    GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_4 );
    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_3);

    GPIOPadConfigSet(GPIO_PORTA_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, // Push para pull up
                     GPIO_STRENGTH_4MA,GPIO_PIN_TYPE_STD_WPU);

}
