#include <RpmCounter.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>

RpmCounter rpmCounter1;
RpmCounter rpmCounter2;

// GUItool: begin automatically generated code
AudioPlaySdWav           playWav1;       //xy=86,122
AudioSynthWaveformDc     dc1;            //xy=383,147
AudioSynthWaveformDc     dc2;            //xy=384,280
AudioEffectMultiply      multiply1;      //xy=500,124
AudioEffectMultiply      multiply2;      //xy=516,252
AudioOutputI2S           i2s1;           //xy=701,194
AudioConnection          patchCord1(playWav1, 0, multiply1, 0);
AudioConnection          patchCord2(playWav1, 1, multiply2, 0);
AudioConnection          patchCord3(dc1, 0, multiply1, 1);
AudioConnection          patchCord4(dc2, 0, multiply2, 1);
AudioConnection          patchCord5(multiply1, 0, i2s1, 0);
AudioConnection          patchCord6(multiply2, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;
// GUItool: end automatically generated code

#define SDCARD_CS_PIN    10
#define SDCARD_MOSI_PIN  7
#define SDCARD_SCK_PIN   14

// LEDS
const byte ledPin1 = 13;
const byte ledPin2 = 14;
// Sensors
const byte sensorPin1 = 5;
const byte sensorPin2 = 6;
// Sync
const byte syncPin = 3; 
const byte playPin =  4;
bool play = 0;

const char *tounes[] = {"toune1.wav", "toune2.wav", "toune3.wav", "toune4.wav"};
int nbTounes = sizeof(tounes)/sizeof(tounes[0]);
int currentSong = 0;

void setup()
{
	rpmCounter1.setCountCallback(onCounter1);
	rpmCounter2.setCountCallback(onCounter2);

	rpmCounter1.setRpmCallback(onRpm1);
	rpmCounter2.setRpmCallback(onRpm2);

	rpmCounter1.begin(sensorPin1, 4, 400, 4, 10);
	rpmCounter2.begin(sensorPin2, 4, 400, 4, 10);

	// LED
	pinMode(ledPin1, OUTPUT);
	pinMode(ledPin2, OUTPUT);
	digitalWrite(ledPin1, HIGH);
	digitalWrite(ledPin2, HIGH);

	// Sync and play
	pinMode(syncPin, OUTPUT);
	pinMode(playPin, INPUT);
	attachInterrupt(playPin, playTrigger, RISING);

	AudioMemory(8);

	sgtl5000_1.enable();
	sgtl5000_1.volume(0.5);

	SPI.setMOSI(SDCARD_MOSI_PIN);
	SPI.setSCK(SDCARD_SCK_PIN);
	if (!(SD.begin(SDCARD_CS_PIN))) {
		// stop here, but print a message repetitively
		while (1) {
			Serial.println("Unable to access the SD card");
			delay(500);
		}
	}
	dc1.amplitude(1.);
	dc2.amplitude(1.);

	// Start playback
	digitalWrite(syncPin, HIGH);
}

void loop()
{
	if (play)
		playFile(tounes[currentSong]);
}

void playFile(const char *filename)
{
	digitalWrite(syncPin, LOW);
	play = 0;
	
	Serial.print("Playing file: ");
	Serial.println(filename);

	// Start playing the file.  This sketch continues to
	// run while the file plays.
	playWav1.play(filename);

	// A brief delay for the library read WAV info
	delay(5);

	// Simply wait for the file to finish playing.
	while (playWav1.isPlaying()) {
		rpmCounter1.update();
		rpmCounter2.update();
		updateLed1();
		updateLed2();
		// uncomment these lines if you audio shield
		// has the optional volume pot soldered
		float vol = analogRead(15);
		vol = vol / 1024;
		sgtl5000_1.volume(vol);
	}

	// Increment next song
	currentSong++;
	if (currentSong >= nbTounes)
		currentSong = 0;

	// Set play flag to true for next song
	digitalWrite(syncPin, HIGH);
}

void playTrigger()
{		
	play = 1;
}

void onRpm1(float val)
{
	Serial.print("Sensor 1: ");
	//Serial.println(val);
	float vol = map(val, 0, 300, 0, 100) / 100.;
	Serial.println(val);
	dc1.amplitude(vol);

}

void onRpm2(float val)
{
	Serial.print("Sensor 2: ");
	//Serial.println(val);
	float vol = map(val, 0, 300, 0, 100) / 100.;
	Serial.println(val);
	dc2.amplitude(vol);
}

void onCounter1()
{
	rpmCounter1.count();
}

void onCounter2()
{
	rpmCounter2.count();
}

void updateLed1()
{
	if (digitalRead(sensorPin1) == HIGH) 
		digitalWrite(ledPin1, HIGH);
	else 
		digitalWrite(ledPin1, LOW);
}

void updateLed2()
{
	if (digitalRead(sensorPin2) == HIGH) 
		digitalWrite(ledPin2, HIGH);
	else 
		digitalWrite(ledPin2, LOW);
}

