#include <RpmCounter.h>

RpmCounter rpmCounter1;
RpmCounter rpmCounter2;

void setup()
{
	rpmCounter1.setCountCallback(onCounter1);
	rpmCounter2.setCountCallback(onCounter2);

	rpmCounter1.setRpmCallback(onRpm1);
	rpmCounter2.setRpmCallback(onRpm2);

	rpmCounter1.begin(3, 1, 400, 4, 10);
	rpmCounter2.begin(2, 1, 400, 4, 10);
}

void loop()
{
	rpmCounter1.update();
	rpmCounter2.update();
}

void onRpm1(float val)
{
	Serial.print("Sensor 1: ");
	Serial.println(val);
}

void onRpm2(float val)
{
	Serial.print("Sensor 2: ");
	Serial.println(val);
}

void onCounter1()
{
	rpmCounter1.count();
}

void onCounter2()
{
	rpmCounter2.count();
}