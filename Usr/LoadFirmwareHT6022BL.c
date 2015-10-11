#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "HT6022.h"
int main ()
{
    printf("Return value of HT6022_Init: %d\n", HT6022_Init());
    printf("Return value of HT6022_FirmwareUpload: %d\n", HT6022BL_FirmwareUpload());
    printf ("Uploading firmware... Type <enter> when the red light starts blinking.");
    HT6022_Exit();
    return 0;
}
