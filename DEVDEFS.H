#include <os2def.h>
#include <bsedos.h>

typedef VOID NEAR * NPVOID;

typedef struct _RESULTCODES
 {
   USHORT  codeTerminate;
   USHORT  codeResult;
 } RESULTCODES;

typedef RESULTCODES FAR *PRESULTCODES;

#define EXEC_SYNC 0

/***    DosExecPgm - Execute a program
 *
 *      Allows a program to request another program be executed as a
 *      child process.  The requestor's process may optionally continue
 *      to execute asynchronous to the new program
 */

USHORT APIENTRY DosExecPgm(
        PCHAR pchFailName,              /* Object name buffer (address)       */
        SHORT cbFailName,               /* Object name buffer length          */
        USHORT fExecFlag,               /* 0=synchronous, 1=asynchronous with */
                                        /* return code discarded, 2=async     */
                                        /* with return code saved, 3=trace    */
                                        /* 4=asynch detached, 5=loaded but    */
                                        /* not executed                       */
        PSZ pszArgs,                    /* Address of argument strings        */
        PSZ pszEnv,                     /* Address of environment strings     */
        PRESULTCODES prescResults,      /* Address to put return codes        */
        PSZ pszPgmName);                /* Address of program filename        */

/* DosExecPgm functions */
