#ifdef TLLT_WITH_WIRINGPI
#	include <wiringPi.h>
#endif

#include "tllt-driver.h"

void
tllt_driver_init(void)
{
#ifdef TLLT_WITH_WIRINGPI
	wiringPiSetup();
#endif
}
