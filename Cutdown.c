#include "GPIO.h"
#include "Cutdown.h"
#include "Timing.c"

#define CDFLAG_SUCCESS 1     //cutdown was a success
#define CDFLAG_INPROGRESS 2  //cutdown in progress
#define CDFLAG_NORSP16 3     //cutdown failed due to no response from PIC16
#define CDFLAG_NORSPCD 4     //cutdown failed due to no response from cutdown apparatus
#define CDFLAG_BADRSP 5      //cutdown failed due to a bad response from cutdown apparatus

char cutdown_ip = 0;         //flag indicating whether or not the cutdown procedure has been completed

void InitiateCutdown() {
    SendString_GPIO("CUT");           //send a cutdown command to the PIC16
    StartKickTimer();
    while(!IN_TxEnable && !KICKED){}             //wait for the PIC16 to respond
    if(!KICKED && ExchangeChar_GPIO(0,0) == '?') //if a confirmation was received
        cutdown_ip = 1;               //set cutdown sequence flag
    else                              //if no or wrong confirmation was received
        cutdown_ip = 2;               //indicate cutdown failed starting
    StopKickTimer();
}

uint8_t GetCutdownStatus() {
    if(cutdown_rq)                               //if cutdown has been requested
    {
        ++cutdown_rq;                            //increment cutdown request counter
        if(cutdown_rq == 8)                      //if the window for confirmation was missed
        {
            cutdown_rq = 0;                      //clear the cutdown request counter
            return 0;                            //clear the cutdown flag
        }
        else                                     //if currently waiting for confirmation
            return 0xAC;                         //send cutdown acknowledge
    }
    else if(cutdown_ip == 1)      //check to see if cutdown was started previously
    {
        switch(ExchangeChar_GPIO(0,0))   //read in the status character from the PIC16
        {
            case 'K':                    //if character was 'K' (success)
                cutdown_ip = 0;          //cutdown sequence complete
                return CDFLAG_SUCCESS;   //return successful code
                break;

            case 'U':                    //if character was 'U' (unexpected response)
                cutdown_ip = 0;          //cutdown sequence complete
                return CDFLAG_BADRSP;    //return unexpected response code
                break;

            case 'N':                    //if character was 'N' (no response)
                cutdown_ip = 0;          //cutdown sequence complete
                return CDFLAG_NORSPCD;   //return no response from cutdown code
                break;

            default:                     //if none of the above characters were received
                cutdown_ip =1;           //cutdown sequence continues
                return CDFLAG_INPROGRESS;//return in progress code
                break;
        }
    }
    else if(cutdown_ip == 2)  //if cutdown was attempted, but PIC16 gave no response
    {
        cutdown_ip = 0;       //end cutdown sequence
        return CDFLAG_NORSP16;//send no response from PIC16 code
    }
    return 0;                 //if cutdown is not currently in progress, return 0
}
