#ifndef __HID_Reader_H__
#define __HID_Reader_H__


/*
	The response code
*/

#define HR_OK					0
#define HR_ParameterError		-1
#define HR_DeviceNoFound		-2
#define HR_ConnectError			-3
#define HR_UnConnected			-4
#define HR_TransmitError		-5
#define HR_NoPower				-6
#define HR_NoExist				-7
#define HR_SlotErrRegisterError		-8
#define HR_TimeExtensionRequested	-9


/****************************************************************************
 *	     API: HID_Reader_Open		 	                   					*
 *	Function: Open reader and be ready create communication                 *
 *--------------------------------------------------------------------------*
 * Parameter:																*
 *			 char *dev_Name:Device name 									*
 * 							The values£ºAUTO¡¢USBn[n is integer]		    *
 *--------------------------------------------------------------------------*
 *	  Return:															    *
 *				Successful: Return handle of device, 						*
							The device handle value is not less than 0  	*
 *			 	     Error: Return specific response code                   *
 ****************************************************************************/
long __stdcall HID_Reader_Open(char* dev_Name);


/****************************************************************************
 *		 API: HID_Reader_Close		 	                   					*
 *	Function: close session with specific reader                            *
 *--------------------------------------------------------------------------*
 * Parameter:																*
 *			 long readerHandle: The handle of reader  						*
 * 																		    *
 *--------------------------------------------------------------------------*
 *	  Return:															    *
 *				Successful: HR_OK										  	*
 *			 	     Error: Error code										*
 ****************************************************************************/
long __stdcall HID_Reader_Close(long readerHandle);


/****************************************************************************
 *		  API: HID_Reader_PowerOn											*
 *	 Function: Power on card, reader will do cold reset at first, if reset 	*
				faiure, then start  hot reset								*
 *	Parameter:																*	
 *			 long readerHandle: The handle of reader						*			
 *			 unsigned char Slot_No: The card slot number                	*
 *			 unsigned char* Atr: Return Card ATR [OUTPUT]            	    *
 *			 int* AtrLen: [OUTPUT,INPUT] The size of card ATR            	*
 *	  Return:                                                         	    *
 *			 Successful: HR_OK										        *
 *			      Error: Error code	                                        *
 ****************************************************************************/
long __stdcall HID_Reader_PowerOn(long readerHandle,unsigned char Slot_No,unsigned char* Atr,int* AtrLen);



/****************************************************************************
 *		  API: HID_Reader_PowerOff											*
 *	 Function: Power off card, reader will disconnect card              	*
 *	Parameter:																*	
 *			 long readerHandle: The handle of reader						*			
 *			 unsigned char Slot_No: The card slot number                	*
 *	  Return:                                                         	    *
 *			 Successful: HR_OK										        *
 *			      Error: Error code	                                        *
 ****************************************************************************/
long __stdcall HID_Reader_PowerOff(long readerHandle,unsigned char Slot_No);


/****************************************************************************
 *		  API: HID_Reader_GetStatus											*
 *	 Function: Get card slot status, and get the reader status,				*
			   Can using this API to check the card slot 		            *
			   status(absent, present)                            		    *
 *	Parameter:																*	
 *			 long readerHandle: The handle of reader						*			
 *			 unsigned char Slot_No: The card slot number                	*
 *	  Return:                                                         	    *
 *			 Successful: HR_OK (Card present and power ON)                  *
 *			      Error: Error code	                                        *
 ****************************************************************************/
long __stdcall HID_Reader_GetStatus(long readerHandle,unsigned char Slot_No);




/****************************************************************************
 *		  API: HID_Reader_Application									    *
 *	 Function: This API using transfer APDU to card and get response        *
 *	Parameter:																*	
 *			 long readerHandle: The handle of reader						*			
 *		 unsigned char Slot_No: The card slot number                	    *
 * unsigned char* Command_APDU: The APDU for send to card   				*
 * long Length_of_Command_APDU: The size of APDU command   				*
 *unsigned char* Response_APDU: Response data, include SW1 and SW2	      	*
 *long* Length_of_Response_APDU: The size of response data			        *
 *	   Return:                                                         	    *
 *			 Successful: The size of response data, value is not less than 0*  
 *			      Error: Error code	                                        *
 ****************************************************************************/
long __stdcall HID_Reader_Application(long readerHandle,unsigned char Slot_No,
										unsigned char* Command_APDU,long Length_of_Command_APDU,
										unsigned char* Response_APDU,long* Length_of_Response_APDU);

										
/****************************************************************************
 *		  API: HID_Reader_Escape											*
 *	 Function: This API using send escape command to reader					*
 *	Parameter:																*	
 *			 long readerHandle: The handle of reader						*			
 *		 unsigned char Slot_No: The card slot number                		*
 * unsigned char* Command_APDU: The APDU for send to card    		  		*
 * long Length_of_Command_APDU: The size of APDU command					*
 *unsigned char* Response_APDU: Response data, include SW1 and SW2		    *
 *long* Length_of_Response_APDU: The size of response data					*
 *	   Return:                                                         	    *
 *			 Successful: The size of response data, value is not less than 0*  
 *			      Error: Error code	                                        *
 ****************************************************************************/
long __stdcall HID_Reader_Escape(long readerHandle,unsigned char Slot_No,
									unsigned char* Command_APDU,long Length_of_Command_APDU,
									unsigned char* Response_APDU,long* Length_of_Response_APDU);

									
																			
/****************************************************************************
 *		  API: HID_Reader_GetName											*
 *	 Function: This API using to get reader information(Manufacturer + Product)
 *	Parameter:																*	
 *			 long readerHandle: The handle of reader						*			
 *	    	  char* readerName: Response data for reader information     	*
 *		   long readerName_Len: The size of reader information	            *
 *	   Return:                                                         	    *
 *			 Successful: The size of response data, value is not less than 0* 
 *			      Error: Error code	                                        *
 ****************************************************************************/
long __stdcall HID_Reader_GetName(long readerHandle,char* readerName,long readerName_Len);


/****************************************************************************
 *		  API: ICC_Reader_Libinfo											*
 *	 Function: This API using to get lib information						*
 *	Parameter:																*	
 *			 char *Info: Response data for lib information					*			
 *	   Return:                                                         	    *
 *			 Successful: HR_OK												* 
 *			      Error: Error code	                                        *
 ****************************************************************************/
 long __stdcall ICC_Reader_Libinfo(char *Info);
#endif