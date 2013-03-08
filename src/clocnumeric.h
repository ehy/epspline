/**********************************************************************\
  *     stack (auto) object to temporarily set LC_NUMERIC to         *
  *     "C" for textual real number formatting and scanning.         *
\**********************************************************************/

#ifndef __C_LOCAL_NUMERIC_TEMP__
#define __C_LOCAL_NUMERIC_TEMP__

#include "clocall.h"

typedef C_loc_Temp<LC_NUMERIC> C_Numeric_Temp;

// For brevity where desired:
typedef C_Numeric_Temp cnumtmp;

#endif // __C_LOCAL_NUMERIC_TEMP__
