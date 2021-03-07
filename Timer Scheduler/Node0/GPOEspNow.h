#ifndef GPOEspNow_h
#define GPOEspNow_h
#include "Arduino.h"
#include <esp_now.h>

#define BROADCAST_PACKET 0
#define UNICAST_PACKET 1
#define CONFIG_MESSAGE 0
#define SYNC_MESAGE 1
#define CONFIG_REQUEST 2
#define WIFI_CHANNEL 1
    
class GPOEspNow
{
  public:
    GPOEspNow();
  	void init(uint8_t *nextNode);
  	void processPacket(const uint8_t *mac_addr, const uint8_t *data, int data_len);
  	void broadcast();
  	void unicast();
  	void processBroadcast(const uint8_t *mac_addr, const uint8_t *data, unsigned short int data_len);
  	void processUnicast(const uint8_t *mac_addr, const uint8_t *data, unsigned short int data_len);
  	void clearLocalPacket();
	
	  //Vars
    uint8_t currentPacketType;
    uint8_t currentActionType;
    const uint8_t maxDataFrameSize=250;    
    uint8_t *dataToSend;
    esp_now_peer_info_t slave;
    const esp_now_peer_info_t *peer = &slave;
    uint8_t nodeID;
    uint8_t cnt;
    
    private:
};

#endif
