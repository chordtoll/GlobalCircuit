
#ifndef _I2C_H    /* Guard against multiple inclusion */
#define _I2C_H

void I2cConfig(void);

char I2cSend(char addr, char* data, int size);

char I2cReceive(char addr, char* data, int size);

char I2cRead(void);
char I2cWrite(char data);
void I2cStart(void);
void I2cStop(void);
void I2cRestart(void);
void I2cAck(void);




#endif /* _I2C_H */

/* *****************************************************************************
 End of File
 */
