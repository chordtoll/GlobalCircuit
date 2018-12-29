
#ifndef _I2C_H    /* Guard against multiple inclusion */
#define _I2C_H

#include <stdint.h>

//Initialize I2C module
void InitI2C(void);

//Read I2C line and return the result
uint8_t ReadI2C(void);

//Write to the I2C line and return acknowledge status
uint8_t WriteI2C(uint8_t data);

//Initiate start sequence of I2C line
void StartI2C(void);

//Initiate stop sequence of I2C line
void StopI2C(void);

//Initiate restart sequence of I2C line
void RestartI2C(void);

//Send an acknowledge on the I2C line
void AckI2C(void);

//Send a NotAcknowledge on the I2C line
void NAckI2C(void);




#endif /* _I2C_H */

/* *****************************************************************************
 End of File
 */
