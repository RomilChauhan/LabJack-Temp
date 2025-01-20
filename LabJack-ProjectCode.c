/*
    Name: Romil Chauhan
    Date: 12/06/24
    Purpose: Code for HSI project.
*/

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include "C:\Program Files (x86)\LabJack\Drivers\LabJackUD.h"

//Function to clear the input buffer 
void clearBuffer(void);

int main()
{
    //Variables for LabJack operations and program control

    LJ_ERROR lj_error; //Error variable for LabJack functions
    LJ_HANDLE lj_handle = 0; //Handle to interact with the LabJack device
    double tempAIN1; //Variable to get analog input(AIN1 Channel)(Read Temperature Sensor)
    double thresholdLimit; //Variable to set the temperature threshold limit
    int exitChoice, runTime = 0, maxActivations = 0, buzzerOnCount = 0; //Variables for user choice and buzzer count
    int counter = 0, msCount = 0; //Variables for counters (sleep and millisecond count)

    //Open connection with LabJack U3 device
    lj_error = OpenLabJack(LJ_dtU3, LJ_ctUSB, "1", 1, &lj_handle);
    
    //Reset the device configuration to default
    lj_error = ePut(lj_handle, LJ_ioPIN_CONFIGURATION_RESET, 0, 0, 0);


    ////Setting up the configuration for the Output channel FIO4 for the buzzer
    lj_error = AddRequest(lj_handle, LJ_ioPUT_CONFIG, LJ_chTIMER_COUNTER_PIN_OFFSET, 4, 0, 0); //Set pin offset to 4 (output channel FIO4)
    lj_error = AddRequest(lj_handle, LJ_ioPUT_CONFIG, LJ_chTIMER_CLOCK_BASE, LJ_tc48MHZ_DIV, 0, 0); //Set TCB to 48MHz/Div 
    lj_error = AddRequest(lj_handle, LJ_ioPUT_CONFIG, LJ_chTIMER_CLOCK_DIVISOR, 38, 0, 0); //Set divisor to 38
    lj_error = AddRequest(lj_handle, LJ_ioPUT_CONFIG, LJ_chNUMBER_TIMERS_ENABLED, 1, 0, 0); //Set enable timers to 1
    lj_error = AddRequest(lj_handle, LJ_ioPUT_TIMER_MODE, 0, LJ_tmPWM8, 0, 0); //Set Mode to PWM8
    lj_error = Go(); //Apply Configuration

    //Set DAC1(Output) channel for LED (On when 0 and off when >=4.5)
    lj_error = AddRequest(lj_handle, LJ_ioPUT_DAC, 1, 0, 0, 0);
    lj_error = Go();

    printf("Welcome to the temperature monitoring program!\n");
    printf("\n");

    //Getting threshold value from user
    printf("Enter the threshold limit in Celsius: ");
    scanf_s("%lf", &thresholdLimit);
    clearBuffer();
    
    //Getting exit strategy from user
    printf("Choose exit strategy:\n1. Time-based \n2. Buzzer activation count\nYour choice: ");
    scanf_s("%d", &exitChoice);
    clearBuffer();

    //Additional user input based on their chosen exit strategy
    if (exitChoice == 1) 
    {
        //If choice is 1 getting program run time from user
        printf("Enter the duration of program in seconds: ");
        scanf_s("%d", &runTime);

        printf("\n");
        clearBuffer();
    }
    else if (exitChoice == 2) 
    {
        //If choice is 2, getting the total number of buzzer activations to end the program
        printf("Enter the number of buzzer activations: ");
        scanf_s("%d", &maxActivations);

        printf("\n");
        clearBuffer();
    }
    else 
    {
        //Warning! If the user provides an invalid choice, exiting the program
        printf("Invalid choice. Exiting.\n");
        return 1;
    }

    //Loop for temperature monitoring
    while (1) 
    {
        //Getting temperature sensor value from AIN1
        lj_error = AddRequest(lj_handle, LJ_ioGET_AIN, 1, 0, 0, 0);
        lj_error = Go();
        lj_error = GetResult(lj_handle, LJ_ioGET_AIN, 1, &tempAIN1);

        //Converting voltage reading to temperature value (assuming 1V = 100 Celsius)
        double temperature = tempAIN1 * 100; 
        printf("Current temperature: %.2lf C\n", temperature);

        //Checking if temperature exceeds threshold limit
        if (temperature > thresholdLimit) 
        {
            lj_error = AddRequest(lj_handle, LJ_ioPUT_DAC, 1, 4.5, 0, 0); //Turning LED off
            lj_error = AddRequest(lj_handle, LJ_ioPUT_TIMER_VALUE, 0, 32000, 0, 0); //Turning Buzzer on
            lj_error = Go();
            buzzerOnCount++; //Increment in buzzer count
            printf("Temperature is Above Threshold | Buzzer activated.\n");
        }

        //When temperature is below threshold limit
        else 
        {
            lj_error = AddRequest(lj_handle, LJ_ioPUT_DAC, 1, 0, 0, 0); //Turning LED on
            lj_error = AddRequest(lj_handle, LJ_ioPUT_TIMER_VALUE, 0, 0, 0, 0); //Turning buzzer off
            lj_error = Go();
            printf("Temperature is NORMAL. LED ON.\n");
        }

        //Incrementing the time counter
        msCount += 200; // Increment by delay duration (milliseconds)

        //Checking the exit conditions
        if ((exitChoice == 1 && msCount >= runTime * 1000) ||  (exitChoice == 2 && buzzerOnCount >= maxActivations)) 
        {  
            printf("Program ended.\n");
            break; //Breaking the loop
        }

        Sleep(200); //Delay for 200 milliseconds
    }

    //Turning both LED and Buzzer off
    lj_error = AddRequest(lj_handle, LJ_ioPUT_DAC, 1, 4.5, 0, 0); 
    lj_error = AddRequest(lj_handle, LJ_ioPUT_TIMER_VALUE, 0, 0, 0, 0);
    lj_error = Go();

    //Reset the device configuration to default
    lj_error = ePut(lj_handle, LJ_ioPIN_CONFIGURATION_RESET, 0, 0, 0);

    return 0;
}

//Function to clear input buffer
void clearBuffer() 
{
    char c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/* 
Output:

Welcome to the temperature monitoring program!

Enter the threshold limit in Celsius: 25
Choose exit strategy:
1. Time-based
2. Buzzer activation count
Your choice: 1
Enter the duration of program in seconds: 1

Current temperature: 23.21 C
Temperature is NORMAL. LED ON.
Current temperature: 22.15 C
Temperature is NORMAL. LED ON.
Current temperature: 23.05 C
Temperature is NORMAL. LED ON.
Current temperature: 25.32 C
Temperature is Above Threshold | Buzzer activated.
Current temperature: 26.84 C
Temperature is Above Threshold | Buzzer activated.
Program ended.


*/
