#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "HT6022.h"
int main ()
{
    printf("Return value of HT6022_Init: %d\n", HT6022_Init());
    printf ("Uploading firmware...\n");
    printf("Return value of HT6022_FirmwareUpload: %d\n", HT6022BL_FirmwareUpload());
    printf ("Firmware Installed please wait until the red ligth start to blinking...\n");
    HT6022_Exit();
    return 0;
}
