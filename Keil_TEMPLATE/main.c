#include "TM4C123.h"                    // Device header


int main(void){
	SYSCTL->RCGC2 |= 1<<5;
	while(1){
		
	}
}
