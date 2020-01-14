#include "GPIO.h"
#include "Cutdown.h"
#include "Timing.h"

void InitiateCutdown() {
    SendString_GPIO("CUT");           //send a cutdown command to the PIC16
    StartKickTimer();
    while(!IN_TxEnable && !KICKED){}             //wait for the PIC16 to respond
    if(!KICKED && ExchangeChar_GPIO(0,0) == '?') //if a confirmation was received
        cut_state = CUT_INPROGRESS;
    else                              //if no or wrong confirmation was received
        cut_state = CUT_NORSP16;
    StopKickTimer();
}

void UpdateCutdownStatus() {
    if(cut_state == CUT_INPROGRESS)      //check to see if cutdown was started previously
    {
        switch(ExchangeChar_GPIO(0,0))   //read in the status character from the PIC16
        {
            case 'K':                    //if character was 'K' (success)
                cut_state = CUT_SUCCESS;
                break;

            case 'U':                    //if character was 'U' (unexpected response)
                cut_state = CUT_BADRSP;       //return unexpected response code
                break;

            case 'N':                    //if character was 'N' (no response)
                cut_state = CUT_NORSPCD; //return no response from cutdown code
                break;
        }
    }
}
