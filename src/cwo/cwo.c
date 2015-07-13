#include <cwo/cwo.h>

#include "type.h"

SOEXPORT int
cwo_Init(void)
{
    return cwo_Type_bootstrap();
}

SOEXPORT void
cwo_Done(void)
{
    cwo_Type_cleanup();
}

