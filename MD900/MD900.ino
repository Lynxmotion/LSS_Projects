/* Author:		rbscharette
** Date:		2019-03-01
** Description:	Broadcasts relative move in 1/10 ° (MD) to a LSS bus.
**				x12 in one direction then x12 in the opposite.
** 				One command per ~500 ms. Loops infinitely.
*/

void setup()
{
	Serial.begin(115200);
	delay(1000);
	Serial.print("#254D0\r");
	delay(1000);
}

#define countMax	12

int val = 300;
int count = 0;

void loop()
{
	count += 1;
	if(count > countMax)
	{
		count = 1;
		val = -val;		
	}
	Serial.print("#254MD"); Serial.print(val, DEC); Serial.print("\r");
	delay(500);
}