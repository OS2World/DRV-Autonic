/*
   STRATINI.C (Discardable portion of driver)
*/
#include "devdefs.h"
#include <i86.h>
#include <conio.h>
#include "devreqp.h"
#include "devaux.h"

// Ensure that the Initialization code located at the end of the driver

#pragma code_seg ( "_INITCODE" ) ;
#pragma data_seg ( "_INITDATA", "INITDATA" ) ;


extern USHORT  OffFinalCS;
extern USHORT  OffFinalDS;

static char CopyRightMsg[] = {
    "\r\nAUTO NIC - Version 1.0\r\n"
    "(c) Serge Sterck 2002.\r\n\n"
};

extern VOID StratInit( REQP_INIT FAR *rp )
{
    USHORT      ret;
    RESULTCODES res_code;

    DosWrite( 1, STRV( CopyRightMsg ), &ret );
    DevHlp = rp->in.devhlp;                         // save far pointer to DevHlp
    ret = DosExecPgm(NULL,0,EXEC_SYNC,NULL,NULL,&res_code,"\\OS2\\SNIFPCI.EXE");
    rp->out.finalcs = FP_OFF( &OffFinalCS );    // set pointers to
    rp->out.finalds = FP_OFF( &OffFinalDS );    //discardable code/data
    rp->header.status |=  RPDONE;
}
