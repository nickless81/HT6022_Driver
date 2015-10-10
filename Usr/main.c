#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "HT6022.h"
int main ()
{
    char c;
    int i;
    unsigned char* CH1 = 0;
    unsigned char* CH2 = 0;

    HT6022_DeviceTypeDef Device; 
    CH1 = (unsigned char*) malloc (sizeof (unsigned char)*HT6022_1MB);
    CH2 = (unsigned char*) malloc (sizeof (unsigned char)*HT6022_1MB);
    printf("Return value of HT6022_Init: %d\n", HT6022_Init());
    printf("Return value of HT6022_FirmwareUpload: %d\n", HT6022_FirmwareUpload());
    do
    {
        printf ("Uploading firmware... Type <enter> when the red light starts blinking.");
        scanf ("%c", &c);
    } while (c != '\n');

    printf("Return value of HT6022_DeviceOpen: %d\n", HT6022_DeviceOpen(&Device));
    do {printf("Type <enter>"); scanf("%c", &c);} while(c != '\n');

    printf("Return value of HT6022_SetSR: %d\n", HT6022_SetSR (&Device, HT6022_100KSa));       /*100KSa per channel. Read  HT6022.h for more options*/
    do {printf("Type <enter>"); scanf("%c", &c);} while(c != '\n');

    printf("Return value of HT6022_SetCH1IR: %d\n", HT6022_SetCH1IR (&Device, HT6022_10V));   /*Channel 1 input range. Read HT6022.h for more information*/
    do {printf("Type <enter>"); scanf("%c", &c);} while(c != '\n');

    printf("Return value of HT6022_SetCH2IR: %d\n", HT6022_SetCH2IR (&Device, HT6022_10V));   /*Channel 2 input range. */
    do {printf("Type <enter>"); scanf("%c", &c);} while(c != '\n');

    printf("Return value of HT6022_ReadData: %d\n", HT6022_ReadData  (&Device, CH1, CH2, HT6022_1KB, 0)); /*read 1kb of data per channel*/ 
    do {printf("Type <enter>"); scanf("%c", &c);} while(c != '\n');

    printf("Data\n");
    printf("Sample    CH1     CH2\n");
    for (i = 0; i < 100; i++)
        printf("  %3d  |  %3d  |  %3d\n", i, CH1[i], CH2[i]);

    HT6022_DeviceClose(&Device);
    HT6022_Exit();
    return 0;
}
