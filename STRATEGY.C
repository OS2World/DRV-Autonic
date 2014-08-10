/*
        STRATEGY.C (Resident portion of driver)
                 (c) Serge Sterck 2002
*/

#include "devdefs.h"
#include <i86.h>
#include <conio.h>
#include "devreqp.h"
#include "devaux.h"

ULONG  DevHlp;  // DevHelp Interface Address


// Strategy entry point
//
// The strategy entry point must be declared according to the STRATEGY
// calling convention, which fetches arguments from the correct registers.


#pragma aux STRATEGY far parm [es bx];
#pragma aux (STRATEGY) Strategy;

VOID Strategy( REQP_ANY FAR *rp )
{

// Strategy routine for device set in header.c

    if( rp->header.command < RP_END ) {
        switch( rp->header.command ) {
        case RP_INIT:
            StratInit( rp );
            break;
        case RP_READ:
        case RP_READ_NO_WAIT:
        case RP_INPUT_STATUS:
        case RP_INPUT_FLUSH:
        case RP_WRITE:
        case RP_WRITE_VERIFY:
        case RP_OUTPUT_STATUS:
        case RP_OUTPUT_FLUSH:
        case RP_OPEN:
        case RP_CLOSE:
        default:
            rp->header.status = RPERR_COMMAND | RPDONE;
        }
    } else {
        rp->header.status = RPERR_COMMAND | RPDONE;
    }
}
