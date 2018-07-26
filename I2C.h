
#ifndef _I2C_H    /* Guard against multiple inclusion */
#define _I2C_H

void InitI2C(void);

char SendI2C(char addr, char* data, int size);

char ReceiveI2C(char addr, char* data, int size);

char ReadI2C(void);
char WriteI2C(char data);
void StartI2C(void);
void StopI2C(void);
void RestartI2C(void);
void AckI2C(void);
void NAckI2C(void);




#endif /* _I2C_H */

/* *****************************************************************************
 End of File
 */
