#include <REG52.H>
char xdata key[11] = {'.','t','i','e','5','R','o','n','a','l','\0'};
unsigned short xdata UserA[9] = {0,0,0,0,0,0,0,0,0};
unsigned short xdata UserB[9] = {0,0,0,0,0,0,0,0,0};
unsigned short xdata time1[9] = {0,0,0,0,0,0,0,0,0};
unsigned char data filledB _at_ 0x44 ;
unsigned char data trialCounter _at_ 0x48;
char data chr _at_ 0x4C;
unsigned short data integer _at_ 0x50;
unsigned short data time _at_ 0x52;
float data da _at_ 0x58;
float data db _at_ 0x65 ;
sbit switchUser = P0^0;
sbit switchMode = P0^1;
sbit switchToUser = P0^2;
sbit switchToTest = P0^3;
sbit rightAnswer = P0^4;
sbit wrongLetter = P0^5;
sbit switchToUserA = P0^6;
sbit switchToUserB = P0^7;
double sqrt(double square)
{
    double root=square/3;
    int i;
    if (square <= 0) return 0;
    for (i=0; i<32; i++)
        root = (root + square / root) / 2;
    return root;
}
void T0_ISR(void) interrupt 1 {
	time ++;
	TH0 = 0xFF;
	TL0 = 0x00;
	TF0 = 0;
}
void main (void) {
	SCON = 0x50;
	TMOD |= 0x20;
	TH1 = 0xFF;
	TR1 = 1;
	TI = 1;
	integer = 0;
	filledB = 0;
	ET0 = 1;
	EA = 1;
	TMOD = (TMOD & 0xF0) | 0x09;
	while(1){
		if(switchMode==1){
			switchToTest = 1;
			switchToUser = 0;
			wrongLetter = 0;
			rightAnswer = 0;
			for(integer = 9; integer>0;integer--){
				UserA[integer-1] /=5;
				UserB[integer-1] /=5;
			}
			break;
		}
		if(filledB == 2){
			switchToTest = ~switchToTest;
			continue;
		}
		else
			if(trialCounter == 5 && switchUser == 1){
				filledB ++;
				if(filledB == 2) continue;
				trialCounter = 0;
				switchToUser = 1;
			}
			else
				if(trialCounter == 5 && switchUser == 0){
					switchToUser = ~switchToUser;
					continue;
				}
		chr = _getkey();
		wrongLetter = 0;
		rightAnswer = 0;
		TR0 = 0;
		if(chr != key[integer]){
			wrongLetter = 1;
			integer = 0;
			continue;
		}
		else
			integer++;
		if(integer >=2){
			time1[integer-2] = time;
		}	
		if(integer == 10){
			trialCounter ++;
			while(integer >0){
				integer --;
				if(switchUser == 0)
					UserA[integer-1] += time1[integer-1];
				else
					UserB[integer-1] += time1[integer-1];
			}
			integer = 0;
			rightAnswer = 1;
		}
		time = 0;
		TH0 = 0xFF;
		TL0 = 0x00;
		TR0 = 1;
	}
	while(1){
		while(1){
			chr = _getkey();
			switchToUserA=0; switchToUserB = 0;
			wrongLetter = 0;
			rightAnswer = 0;
			TR0 = 0;
			if(chr != key[integer]){
				wrongLetter = 1;
				integer = 0;
				continue;
			}
			else
				integer++;
			if(integer >=2){
				time1[integer-2] = time;
			}	
				if(integer == 10){
					da,db = 0.0;
					while(integer >0){
						integer --;
						da += ((short)(time1[integer-1]-UserA[integer-1])*(short)(time1[integer-1]-UserA[integer-1]));
						db += ((short)(time1[integer-1]-UserB[integer-1])*(short)(time1[integer-1]-UserB[integer-1]));
					}
				integer = 0;
				rightAnswer = 1;
				break;
			}
			time = 0;
			TH0 = 0xFF;
			TL0 = 0x00;
			TR0 = 1;	
		}
		da = sqrt(da);
		db = sqrt(db);
		if(da < db)
			switchToUserA = 1;
		else
			switchToUserB = 1;
	}
}
