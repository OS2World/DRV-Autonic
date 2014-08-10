/*
   HEADER.C (Header portion of driver, must be first in DATA SEG)
*/

#include "devdefs.h"
#include "devhdr.h"

// Ensure that the header is located at the beginning of the driver

#pragma data_seg ( "_HEADER", "DATA" ) ;

// Declare the device driver header

extern VOID Strategy();

DEVHEADER AUTONIC = {
      FENCE,
      DAW_CHARACTER | DAW_OPENCLOSE | DAW_LEVEL1,
      Strategy,
      0,
      "AUTO$   "
      "",
      CAP_NULL
};
