#include "utils.h"

#include <complex.h>
#include <stdio.h>
#include <stdlib.h>

int Smallest_Num_Position_In_Array(const int* _arr, int _size)
{
    int pos = 0;
    int min = _arr[0];
    for (int i = 1; i < _size; i++)
    {
        if (_arr[i] < min)
        {
            min = _arr[i];
            pos = i;
        }
    }

    return pos;
}

int Biggest_Num_Position_In_Array(const int* _arr, int _size)
{
    int pos = 0;
    int max = _arr[0];
    for (int i = 1; i < _size; i++)
    {
        if (_arr[i] > max)
        {
            max = _arr[i];
            pos = i;
        }
    }

    return pos;
}

char Get_User_Character_Input(void)
{
    char c;
    int  scan_count = scanf(" %c", &c);
    if (scan_count != 1)
    {
        return ' ';
    }
    return c;
}

void Fatal_Error(const char* _msg)
{
    printf("FATAL ERROR: \n");
    printf("%s \n", _msg);
    printf("Exiting execution... \n");
    exit(EXIT_FAILURE);
}

void Log_Error(const char* _msg)
{
    printf("<<< An error was caught with the description: \"%s\" >>>\n", _msg);
}
