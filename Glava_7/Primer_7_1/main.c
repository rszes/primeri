#include <stdint.h>
#include "../pin_driver/pin.h"

int16_t main()
{
	pinInit(PORT_B, 5, OUTPUT);
	pinSetValue(PORT_B, 5, HIGH);

	while(1)
		;
}
