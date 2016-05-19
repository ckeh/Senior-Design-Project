#ifndef READBATTERY_H
#define READBATTERY_H

void ADCInit(void);
void GPIOInit(void);
uint32_t ReadADC(void);
void FlashWrite(uint32_t data);
uint32_t FlashRead(int flag);
void openGate(void);
void closeGate(void);
uint8_t convertBat(int bat);


int buttonCheck();

#endif /* READBATTERY_H */
