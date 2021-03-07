#include "GPOEspNow.h"

GPOEspNow::GPOEspNow()
{
  dataToSend = new uint8_t[maxDataFrameSize];
}

void GPOEspNow::init(uint8_t *nextNode)
{
	memcpy( slave.peer_addr, nextNode, 6 );
	nodeID = nextNode[5]-1;
	slave.channel = WIFI_CHANNEL;
	slave.encrypt = 0;
	esp_now_add_peer(peer);
	clearLocalPacket();
}

void GPOEspNow::processPacket(const uint8_t *mac_addr, const uint8_t *data, int data_len)
{
	//store Received data into local buffer
	memcpy(dataToSend, data, data_len);
	//set current packet type value
	currentPacketType = data[0];
	//Choose what to do with received data packet
	switch(data[0])
	{
		case	BROADCAST_PACKET:		//Received a broadcast packet
										processBroadcast(mac_addr, data, data_len);
										break;
		case	UNICAST_PACKET:			//Recieved a unicast packet
										processUnicast(mac_addr, data, data_len);
										break;
										
		default:						break;
	}
}


void GPOEspNow::broadcast()
{
	//Set Msssage TYPE
	dataToSend[0] = BROADCAST_PACKET;;
	esp_now_send(slave.peer_addr, dataToSend, maxDataFrameSize);
	//clearLocalPacket();
}

void GPOEspNow::unicast()
{
	//Set Msssage TYPE
	dataToSend[0] = UNICAST_PACKET;
	esp_now_send(slave.peer_addr, dataToSend, maxDataFrameSize);
	//clearLocalPacket();
}

void GPOEspNow::processBroadcast(const uint8_t *mac_addr, const uint8_t *data, unsigned short int data_len)
{
	//Set current action type
	currentActionType = data[1];
	switch(data[1])
	{
		case	CONFIG_MESSAGE:		//Received a configuration packet
									break;
		case	SYNC_MESAGE:		//Received a SYNC Message
									break;									
    case  CONFIG_REQUEST:    //Received a SYNC Message
                  break;
                  
		default:					break;
	}
	broadcast();
}

void GPOEspNow::processUnicast(const uint8_t *mac_addr, const uint8_t *data, unsigned short int data_len)
{
	//If the packet is not meant for this node return
	if(data[2]!=nodeID)
	{
		return;
	}
	//Set current action type
	currentActionType = data[1];
	//Check what this packet type is
	switch(data[1])
	{
		case	CONFIG_MESSAGE:		//Received a configuration packet
									break;
		case	SYNC_MESAGE:		//Received a SYNC Message
									break;
									
		default:					break;
	}
	
}

void GPOEspNow::clearLocalPacket()
{
	for(cnt=0; cnt<maxDataFrameSize; cnt++)
	{
		dataToSend[cnt] = 0;
	}
}
