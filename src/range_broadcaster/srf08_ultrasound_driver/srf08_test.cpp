#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "srf08.h"

int main(int argc, char * argv[]){
    //printf("Entering program...\n");
    SRF08 srf08_1 = SRF08(1, 0x70); // Create an SRF08 object
    SRF08 srf08_2 = SRF08(1, 0x72);
    SRF08 srf08_3 = SRF08(1, 0x76);
    // printf("Successfully created object.\n");

    // Change the max range of all the sensors

    srf08_1.setRangeRegister(0x8C); // 0x8C = 6m
    srf08_2.setRangeRegister(0x8C); // 0x8C = 6m
    srf08_3.setRangeRegister(0x8C); // 0x8C = 6m

    int range;
    while(1){
        range = srf08_1.getRange();
        printf("Range: %d cm\n", range);
        range = srf08_2.getRange();
        printf("Range: %d cm\n", range);
        range = srf08_3.getRange();
        printf("Range: %d cm\n", range);
        usleep(100000);
    }

}