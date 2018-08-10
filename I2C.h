
#ifndef _I2C_H    /* Guard against multiple inclusion */
#define _I2C_H

#include <stdint.h>

void InitI2C(void);
uint8_t ReadI2C(void);
uint8_t WriteI2C(uint8_t data);
void StartI2C(void);
void StopI2C(void);
void RestartI2C(void);
void AckI2C(void);
void NAckI2C(void);




#endif /* _I2C_H */

/* *****************************************************************************
 End of File
 */
