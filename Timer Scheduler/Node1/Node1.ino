/*
	Board:  ESP32
    CPU Speed: 240Mhz
    All other settings to whatever they are in board manager

 */

#include <esp_now.h>
#include <WiFi.h>
#include "esp32-hal.h"
#include "GPOEspNow.h"

#define WIFI_CHANNEL 1

//ESPNOW COnfig
//This Node
uint8_t localCustomMac[] = {0x36, 0x33, 0x33, 0x33, 0x33, 0x01};
//Next Node
uint8_t nextNodeMac[] = {0x36, 0x33, 0x33, 0x33, 0x33, 0x02};
//Previous Node
uint8_t prevNodeMac[] = {0x36, 0x33, 0x33, 0x33, 0x33, 0x00};
GPOEspNow GPObject;

//Animation Que
byte animationIndex=0, totalAnimations=3, dataRcvd=0;
unsigned short int queLength = 0;
int16_t** animationQue;
byte isConfigured=0;

//Timers
long globalTimer[3], bootTimer[3], syncTimer[3];

void setup()
{
  Serial.begin(115200);
  Serial.printf("\r\n\r\n\r\n");

  //Set up WIFI & ESPNOW
  esp_base_mac_addr_set(localCustomMac);
  WiFi.mode(WIFI_AP_STA);
  Serial.println(WiFi.macAddress());
  //Init ESPNOW
  esp_now_init();
  delay(10);
  //Set up Slave & add to espnow system
  GPObject.init(nextNodeMac, prevNodeMac);  
  //set up reception callback function
  esp_now_register_recv_cb(OnDataRecv);
  //Sart a 10 second timer from boot to wait for config message
  if(!isConfigured)
  {
    startTimer(10000, bootTimer);
  }
}

byte hasTimedOut(long *timerObject)
{
  timerObject[1] = millis();
  if (timerObject[2] < timerObject[1] - timerObject[0])
  {
    return 1;
  }
  return 0;
}
void startTimer(unsigned long durationInMillis, long *timerObject)
{
  timerObject[0] = millis();
  timerObject[2] = durationInMillis;
}

void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len)
{
  unsigned short int configIndex, dataIndex;
  
  GPObject.processPacket(mac_addr, data, data_len);
  if( GPObject.currentActionType == SYNC_MESAGE )
  {
    Serial.printf("\r\nReceived SYNC Data\t%d", GPObject.dataReceived[4]);
    //Sets animation index from data recievd via espnow in array location[4] from que server
    animationIndex = GPObject.dataReceived[4];
    //Set global data received flag
    dataRcvd = 1;
  }
  else if( GPObject.currentActionType == CONFIG_MESSAGE )
  {
  	//check if config mesage is for this node only
  	if( (GPObject.dataReceived[2] == GPObject.nodeID || GPObject.dataReceived[2] == 255) && isConfigured==0)
  	{
  		Serial.printf("\r\nReceived CONFIGURATION Data...");
  		//set animation que length
  		queLength = GPObject.dataReceived[3];
  		dataIndex=4;
  		//create animation config array
  		animationQue = new int16_t*[queLength];
  		for(configIndex=0; configIndex<queLength; configIndex++)
  		{
  		  animationQue[configIndex] = new int16_t[2];
  		  animationQue[configIndex][0] = GPObject.dataReceived[dataIndex];
  		  animationQue[configIndex][1] = GPObject.dataReceived[dataIndex+1];
  		  dataIndex+=2;
  		}
     isConfigured=1;
     GPObject.clearRXBuffer();
  	}
  }
}

void loop()
{
  if(!isConfigured)
  {
    nonConfiguredLoop();
    //Check if 10s boot timer has expired and no config has arrived
    if( hasTimedOut(bootTimer) )
    {
      Serial.printf("\r\nI have no config, asking for config from server...");
      GPObject.clearTXBuffer();
      //Ask for a config message
      GPObject.dataToSend[1] = CONFIG_REQUEST;
      //Address this messsage to node 0
      GPObject.dataToSend[2] = 0;
      //Set return address to this node
      GPObject.dataToSend[3] = GPObject.nodeID;
      GPObject.unicast(0);
      //Restart timer with a 10s wait 
      startTimer(10000, bootTimer);
    } 
  }
  else
  {
    Serial.printf("\r\nLaunch animation\t%d\tfor\t%d\t seconds\tQue Index\t%d", animationQue[animationIndex][0], animationQue[animationIndex][1], animationIndex);
    //Launch animation on this node
    runAnimation(animationQue[animationIndex][0], animationQue[animationIndex][1]*1000);
    //Once time expires incremend to next animation in sequence
    //Reset data received flag
    if(dataRcvd)
    {  
      dataRcvd = 0;
    }
    else
    {
      animationIndex = (animationIndex+1)%queLength;
    }
  }
}

void nonConfiguredLoop()
{
  switch(animationIndex)
  {
    case  0:    Animation1();    
                break;
    case  1:    Animation2();    
                break;
    case  2:    Animation3();    
                break;    
  
    default:    break;
  }
  //Reset data received flag
  dataRcvd = 0;
}

void runAnimation(unsigned short int animationIndex, unsigned short int animationDuration)
{
  switch( animationIndex )
  {
    case  0:    startTimer(animationDuration);
                Animation1();    
                break;
    case  1:    startTimer(animationDuration);
                Animation2();    
                break;
    case  2:    startTimer(animationDuration);
                Animation3();    
                break;    
                
    default:    break;
  }
}


//-----------------ANIMATIONS---------------------

void Animation1()
{  
  while(true)
  {
    //Time Scheduler
    //check if new que command was received
    if(isConfigured)
    {
      if(hasTimedOut(globalTimer) || dataRcvd)
      {return;}
    }
    else if(dataRcvd){return;}
    //this is your animation loop
	//Each timeyou render a frame you need to check if the timer has timed out
  }
}
void Animation2()
{  
  while(true)
  {
    //Time Scheduler
    //check if new que command was received
    if(isConfigured)
    {
      if(hasTimedOut(globalTimer) || dataRcvd)
      {return;}
    }
    else if(dataRcvd){return;}
    //this is your animation loop
	//Each timeyou render a frame you need to check if the timer has timed out
  }
}
void Animation3()
{  
  while(true)
  {
    //Time Scheduler
    //check if new que command was received
    if(isConfigured)
    {
      if(hasTimedOut(globalTimer) || dataRcvd)
      {return;}
    }
    else if(dataRcvd){return;}
    //this is your animation loop
	//Each timeyou render a frame you need to check if the timer has timed out
  }
}
