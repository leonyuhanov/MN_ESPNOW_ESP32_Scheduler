#include "GPOEspNow.h"

GPOEspNow::GPOEspNow()
{
  dataToSend = new uint8_t[maxDataFrameSize];
  dataReceived = new uint8_t[maxDataFrameSize];
}

void GPOEspNow::init(uint8_t *nextNode, uint8_t *prevNode)
{
	//Set Node id
  nodeID = nextNode[5]-1;
	//add next node 
	memcpy( nextSlave.peer_addr, nextNode, 6 );
	nextSlave.channel = WIFI_CHANNEL;
	nextSlave.encrypt = 0;
	esp_now_add_peer(nextPeer);
  
 //add prev node
  memcpy( prevSlave.peer_addr, prevNode, 6 );
  prevSlave.channel = WIFI_CHANNEL;
  prevSlave.encrypt = 0;
  esp_now_add_peer(prevPeer);
	clearTXBuffer();
  clearRXBuffer();
}

void GPOEspNow::processPacket(const uint8_t *mac_addr, const uint8_t *data, int data_len)
{
	//store Received data into local buffer
	memcpy(dataReceived, data, maxDataFrameSize);
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
										
		default:						      break;
	}
}


void GPOEspNow::broadcast()
{
	//Set Msssage TYPE
	dataToSend[0] = BROADCAST_PACKET;;
	esp_now_send(nextSlave.peer_addr, dataToSend, maxDataFrameSize);
}

void GPOEspNow::unicast(uint8_t txDirection)
{
  //Set Msssage TYPE
  dataToSend[0] = UNICAST_PACKET;
	if(txDirection==1)
  {
	  esp_now_send(nextSlave.peer_addr, dataToSend, maxDataFrameSize);
  }
  else if(txDirection==0)
  {
    esp_now_send(prevSlave.peer_addr, dataToSend, maxDataFrameSize);
  }
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
									
		default:					break;
	}
  memcpy(dataToSend, data, data_len);
  broadcast();
  clearTXBuffer();
}

void GPOEspNow::processUnicast(const uint8_t *mac_addr, const uint8_t *data, unsigned short int data_len)
{
	//If the packet is not meant for this node return
	if(data[2]!=nodeID)
	{
		if( data[2]>nodeID )
    {
      //addressed to next node
      memcpy(dataToSend, data, maxDataFrameSize);
      unicast(1);
      clearTXBuffer();
    }
    else if( data[2]<nodeID )
    {
      //addressed to previous node
      memcpy(dataToSend, data, maxDataFrameSize);
      unicast(0);
      clearTXBuffer();
    }
		return;
	}
 else
 {
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
}

void GPOEspNow::clearTXBuffer()
{
	for(cnt=0; cnt<maxDataFrameSize; cnt++)
	{
		dataToSend[cnt] = 0;
	}
}

void GPOEspNow::clearRXBuffer()
{
  for(cnt=0; cnt<maxDataFrameSize; cnt++)
  {
    dataReceived[cnt] = 0;
  }
}
