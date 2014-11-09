
/* 
 * Example of using the ChainableRGB library for controlling a Grove RGB.
 * This code fades in an out colors in a strip of leds.
 */


#include <ChainableLED.h>
#include <SoftwareSerial.h>


#define NUM_LEDS  1
#define NUM_TRACKS 3
#define PIR_MOTION_SENSOR 5//Use pin 2 to receive the signal from the module 
#define TRACK_TIME 4000 //appx 4 seconds

ChainableLED leds(7, 8, NUM_LEDS);
SoftwareSerial mp3(2, 3);//modify this with the connector you are using.

void setup()
{
  	mp3.begin(9600);
	Serial.begin(9600); 
	delay(100);
        if (true == SetPlayMode(0x00))
        Serial.println("Set The Play Mode to 0x01, Single Loop Mode.");
        else
        Serial.println("Playmode Set Error");
        SetVolume(0x0E);
        //PauseOnOffCurrentMusic();
        pinMode(PIR_MOTION_SENSOR, INPUT);
        //SetMusicPlay(00,01);
}

float hue = 0.0;
boolean up = true;
byte power = 0;

void loop()
{
	if(isPeopleDetected())//if it detects the moving people?
		onMotion();
	else
		noMotion();
}

void onMotion()
{

  nextTrack();
  int playTime = 0;
  int delayInterval = 5;
  
  while (playTime < TRACK_TIME){
      for (byte i=0; i<NUM_LEDS; i++)
      {
        if (i%2 == 0)
          leds.setColorRGB(i, power, 0, 0);
        else
          leds.setColorRGB(i, 0, 255-power, 0);
      }  
      power+= 10;
      delay(delayInterval);
      playTime = playTime + delayInterval;
  } 
  
}


void noMotion()
{
  for (byte i=0; i<NUM_LEDS; i++)
    leds.setColorHSB(i, hue, 1.0, 0.5);
    
  delay(20);
    
  if (up)
    hue+= 0.005;
  else
    hue-= 0.005;
    
  if (hue>=1.0 && up)
    up = false;
  else if (hue<=0.0 && !up)
    up = true;
}

/***************************************************************/
/*Function: Detect whether anyone moves in it's detecting range*/
/*Return:-boolean, ture is someone detected.*/
boolean isPeopleDetected()
{
	int sensorValue = digitalRead(PIR_MOTION_SENSOR);
	if(sensorValue == HIGH)//if the sensor value is HIGH?
	{
		return true;//yes,return ture
	}
	else
	{
		return false;//no,return false
	}
}

//Set the music index to play, the index is decided by the input sequence
//of the music;
//hbyte: the high byte of the index;
//lbyte: the low byte of the index;
boolean SetMusicPlay(uint8_t hbyte,uint8_t lbyte)
{
	mp3.write(0x7E);
	mp3.write(0x04);
	mp3.write(0xA0);
	mp3.write(hbyte);
	mp3.write(lbyte);
	mp3.write(0x7E);
        delay(10);
	 while(mp3.available())
	{
	if (0xA0==mp3.read())
	return true;
	else 
	return false;
        }
}

// Pause on/off  the current music
boolean nextTrack(void)
{
 mp3.write(0x7E);
 mp3.write(0x02);
 mp3.write(0xA5);
 mp3.write(0x7E);
 delay(10);
 while(mp3.available())
 {
 if (0xA5==mp3.read())
 return true;
 else 
 return false;
 }
}

// Pause on/off  the current music
boolean PauseOnOffCurrentMusic(void)
{
 mp3.write(0x7E);
 mp3.write(0x02);
 mp3.write(0xA3);
 mp3.write(0x7E);
 delay(10);
 while(mp3.available())
 {
 if (0xA3==mp3.read())
 return true;
 else 
 return false;
 }
}

//Set the volume, the range is 0x00 to 0x1F
boolean SetVolume(uint8_t volume)
{
 mp3.write(0x7E);
 mp3.write(0x03);
 mp3.write(0xA7);
 mp3.write(volume);
 mp3.write(0x7E);
 delay(10);
 while(mp3.available())
 {
 if (0xA7==mp3.read())
 return true;
 else 
 return false;
 }
}

boolean SetPlayMode(uint8_t playmode)
{
  if (((playmode==0x00)|(playmode==0x01)|(playmode==0x02)|(playmode==0x03))==false)
  {
    Serial.println("PlayMode Parameter Error! ");
    return false;
  }
   mp3.write(0x7E);
 mp3.write(0x03);
 mp3.write(0xA9);
 mp3.write(playmode);
 mp3.write(0x7E);
 delay(10);
 while(mp3.available())
 {
 if (0xA9==mp3.read())
 return true;
 else 
 return false;
 }
}
