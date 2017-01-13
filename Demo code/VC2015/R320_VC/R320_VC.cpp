/*$T indentinput.cpp GC 1.140 01/13/17 13:14:44 */

/* R320_VC.cpp : Defines the entry point for the console application. */
#include "stdafx.h"
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>

/* include "../Lib/HID_Reader.h" */
typedef long (PASCAL *HID_Reader_Open_t) (char *dev_Name);
typedef long (PASCAL *HID_Reader_Close_t) (long readerHandle);
typedef long (PASCAL *HID_Reader_PowerOn_t) (long readerHandle, unsigned char Slot_No, unsigned char *Atr, int *AtrLen);
typedef long (PASCAL *HID_Reader_PowerOff_t) (long readerHandle, unsigned char Slot_No);
typedef long (PASCAL *HID_Reader_GetStatus_t) (long readerHandle, unsigned char Slot_No);
typedef long (PASCAL *HID_Reader_Application_t)
	(
		long readerHandle,
		unsigned char Slot_No,
		unsigned char *Command_APDU,
		long Length_of_Command_APDU,
		unsigned char *Response_APDU,
		long *Length_of_Response_APDU
	);
typedef long (PASCAL *HID_Reader_Escape_t)
	(
		long readerHandle,
		unsigned char Slot_No,
		unsigned char *Command_APDU,
		long Length_of_Command_APDU,
		unsigned char *Response_APDU,
		long *Length_of_Response_APDU
	);
typedef long (PASCAL *HID_Reader_GetName_t) (long readerHandle, char *readerName, long readerName_Len);

HMODULE						dllhandle = NULL;
HID_Reader_Open_t			HID_Reader_Open = NULL;
HID_Reader_Close_t			HID_Reader_Close = NULL;
HID_Reader_PowerOn_t		HID_Reader_PowerOn = NULL;
HID_Reader_PowerOff_t		HID_Reader_PowerOff = NULL;
HID_Reader_GetStatus_t		HID_Reader_GetStatus = NULL;
HID_Reader_Application_t	HID_Reader_Application = NULL;
HID_Reader_Escape_t			HID_Reader_Escape = NULL;
HID_Reader_GetName_t		HID_Reader_GetName = NULL;

/*
 =======================================================================================================================
 The API using to load DLL
 =======================================================================================================================
 */

void loadDll()
{
	dllhandle = LoadLibrary("../Lib/HID_Reader.dll");

	HID_Reader_Open = (HID_Reader_Open_t) GetProcAddress(dllhandle, "HID_Reader_Open");
	HID_Reader_Close = (HID_Reader_Close_t) GetProcAddress(dllhandle, "HID_Reader_Close");
	HID_Reader_PowerOn = (HID_Reader_PowerOn_t) GetProcAddress(dllhandle, "HID_Reader_PowerOn");
	HID_Reader_PowerOff = (HID_Reader_PowerOff_t) GetProcAddress(dllhandle, "HID_Reader_PowerOff");
	HID_Reader_GetStatus = (HID_Reader_GetStatus_t) GetProcAddress(dllhandle, "HID_Reader_GetStatus");
	HID_Reader_Application = (HID_Reader_Application_t) GetProcAddress(dllhandle, "HID_Reader_Application");
	HID_Reader_Escape = (HID_Reader_Escape_t) GetProcAddress(dllhandle, "HID_Reader_Escape");
	HID_Reader_GetName = (HID_Reader_GetName_t) GetProcAddress(dllhandle, "HID_Reader_GetName");
}

static char *errLog[30] =
{
	"Success",			/* 0 */
	"Parameter error",	/* 1 */
	"Cannot find device",		/* 2 */
	"Connection error",			/* 3 */
	"No connection",			/* 4 */
	"Transfer command error",	/* 5 */
	"Detected card,  but not power On to card", /* 6 */
	"Card not detect"	/* 7 */
};

/*
 =======================================================================================================================
 Show the log 
 =======================================================================================================================
 */

void printLog(char *funcName, long ret)
{
	if(ret >= 0)
	{
		printf("\n[%s] \nSuccess----------[%d]\n", funcName, ret);
	}
	else
	{
		printf("\n[%s] \nFailure----------[%d]\n%s\n", funcName, ret, errLog[abs(ret)]);
	}
}

/*
 =======================================================================================================================
 print out the buffer of communication
 =======================================================================================================================
 */
void printbuffer(char *msg, unsigned char *a, long b)
{
	printf("%s:\n\t", msg);
	for(int i = 0; i < b; i++)
	{
		if(i != 0 && i % 4 == 0)
		{
			printf("  ");
		}

		if(i != 0 && i % 8 == 0)
		{
			printf("\t");
		}

		if(i != 0 && i % 16 == 0)
		{
			printf("\n\t");
		}

		printf("%02x ", a[i]);
	}

	printf("\n");
}

long			readerHandle = 0;
unsigned char	Slot_No = 0;

/*
 =======================================================================================================================
 Open Card Reader
 =======================================================================================================================
 */

void hidOpen(char *devName = "USB0")
{
	printLog("HID_Reader_Open", readerHandle = HID_Reader_Open(devName));
}

/*
 =======================================================================================================================
 Close Card Reader
 =======================================================================================================================
 */
void hidClose(long r = readerHandle)
{
	printLog("HID_Reader_Close", HID_Reader_Close(r));
}

/*
 =======================================================================================================================
 Do power on to card
 =======================================================================================================================
 */
void hidPowerOn(long r = readerHandle, unsigned char s = Slot_No)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	long			ret;
	unsigned char	Response[32] = { 0 };
	int				ResponseLen = sizeof(Response);
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	ret = HID_Reader_PowerOn(r, s, Response, &ResponseLen);
	printLog("HID_Reader_PowerOn", ret);
	if(ret >= 0)
	{
		printbuffer("ATR", Response, ResponseLen);
	}
}

/*
 =======================================================================================================================
 Do power off of card
 =======================================================================================================================
 */
void hidPowerOff(long r = readerHandle, unsigned char s = Slot_No)
{
	printLog("HID_Reader_PowerOff", HID_Reader_PowerOff(r, s));
}

/*
 =======================================================================================================================
 Get card slot status
 =======================================================================================================================
 */
void hidGetStatus(long r = readerHandle, unsigned char s = Slot_No)
{
	printLog("HID_Reader_GetStatus", HID_Reader_GetStatus(r, s));
}

/*
 =======================================================================================================================
 Do communication with APDU commands
 =======================================================================================================================
 */
void hidApp(long r = readerHandle, unsigned char s = Slot_No, char *sendAPDU = "0084000008")
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	long			ret;
	unsigned char	recv[1024] = { 0 };
	long			recvLen = sizeof(recv);
	unsigned char	send[1024] = { 0 };
	long			sendLen = strlen(sendAPDU) / 2;
	char			*p = sendAPDU;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	for(unsigned int i = 0; i < strlen(sendAPDU) / 2; i++)
	{
		sscanf(p, "%02x", &send[i]);
		p += 2;
	}

	ret = HID_Reader_Application(r, s, send, sendLen, recv, &recvLen);
	printLog("HID_Reader_Application", ret);
	if(ret > 0)
	{
		printbuffer("APP RECV", recv, recvLen);
	}
}

/*
 =======================================================================================================================
 Using escape command to get specific information of reader
 =======================================================================================================================
 */
void hidEscape(long r = readerHandle, unsigned char s = Slot_No, char *sendAPDU = "a55a3730")
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	long			ret;
	unsigned char	recv[1024] = { 0 };
	long			recvLen = sizeof(recv);
	unsigned char	send[1024] = { 0 };
	long			sendLen = strlen(sendAPDU) / 2;
	char			*p = sendAPDU;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	for(unsigned int i = 0; i < strlen(sendAPDU) / 2; i++)
	{
		sscanf(p, "%02x", &send[i]);
		p += 2;
	}

	ret = HID_Reader_Escape(r, s, send, sendLen, recv, &recvLen);
	printLog("HID_Reader_Escape", ret);
	if(ret > 0)
	{
		printbuffer("APP RECV", recv, recvLen);
	}
}

/*
 =======================================================================================================================
 Get reader name
 =======================================================================================================================
 */
void hidGetName(long r = readerHandle)
{
	/*~~~~~~~~~~~~~~~~~~~~~~*/
	long	ret;
	char	name[128] = { 0 };
	/*~~~~~~~~~~~~~~~~~~~~~~*/

	ret = HID_Reader_GetName(r, name, sizeof(name));
	printLog("HID_Reader_GetName", ret);
	if(ret > 0)
	{
		printf("READER NAME:\"%s\"\n", name);
	}
}

/*
 =======================================================================================================================
 Do test with all API
 =======================================================================================================================
 */
int main(int argc, char *argv[])
{
	loadDll();

	/* Open Reader */
	hidOpen();

	/* get Reader name */
	hidGetName();

	/* do power on */
	hidPowerOn();

	/* get card slot status */
	hidGetStatus();

	/* Send command to card */
	hidApp(readerHandle, Slot_No, "0084000008");

	/* Test power off of card */
	hidPowerOff();

	/* Test escape command */
	hidEscape();

	/* close reader */
	hidClose();

	return 0;
}
