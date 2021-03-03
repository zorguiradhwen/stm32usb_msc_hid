/*
 * usbd_msc_hid_core.c
 *
 *  Created on: Mar 2, 2021
 *      Author: radhwen.zorgui
 */

#include "usbd_msc.h"
#include "usbd_customhid.h"
#include "usbd_msc_hid_core.h"

#include "usbd_custom_hid_if.h"
#include "usbd_storage_if.h"

USBD_StorageHidTypeDef USBD_MSC_HID_fops_FS =
{
		&USBD_CustomHID_fops_FS,
		&USBD_Storage_Interface_fops_FS
};



uint8_t  USBD_MSC_HID_EP0_RxReady (USBD_HandleTypeDef  *pdev);

uint8_t  USBD_MSC_HID_Init (USBD_HandleTypeDef *pdev, uint8_t cfgidx);

uint8_t  USBD_MSC_HID_DeInit (USBD_HandleTypeDef *pdev, uint8_t cfgidx);

uint8_t  USBD_MSC_HID_Setup (USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req);

uint8_t  USBD_MSC_HID_DataIn (USBD_HandleTypeDef *pdev, uint8_t epnum);

uint8_t  USBD_MSC_HID_DataOut (USBD_HandleTypeDef *pdev, uint8_t epnum);

uint8_t *USBD_MSC_HID_GetFSCfgDesc (uint16_t *length);

uint8_t *USBD_MSC_HID_GetDeviceQualifierDescriptor (uint16_t *length);


USBD_ClassTypeDef  USBD_MSC_HID_cb =
{
  USBD_MSC_HID_Init,
  USBD_MSC_HID_DeInit,
  USBD_MSC_HID_Setup,
  NULL, /*EP0_TxSent*/
  USBD_MSC_HID_EP0_RxReady, /*EP0_RxReady*/
  USBD_MSC_HID_DataIn, /*DataIn*/
  USBD_MSC_HID_DataOut, /*DataOut*/
  NULL, /*SOF */
  NULL,
  NULL,
  NULL,
  USBD_MSC_HID_GetFSCfgDesc,
  NULL,
  USBD_MSC_HID_GetDeviceQualifierDescriptor,

};

__ALIGN_BEGIN static uint8_t USBD_MSC_HID_CfgDesc[USB_MSC_HID_CONFIG_DESC_SIZ] __ALIGN_END =
{
	/***************Configuration Descriptor***********************/
	0x09, /* bLength: Configuration Descriptor size */
	USB_DESC_TYPE_CONFIGURATION, /* bDescriptorType: Configuration */
	USB_MSC_HID_CONFIG_DESC_SIZ,
	/* wTotalLength: Bytes returned */
	0x00,
	0x02,         /*bNumInterfaces: 2 interface*/
	0x01,         /*bConfigurationValue: Configuration value*/
	0x00,         /*iConfiguration: Index of string descriptor describing
	the configuration*/
	0xC2,         /*bmAttributes: bus powered and Support Remote Wake-up */
	0x32,         /*MaxPower 100 mA: this current is used for detecting Vbus*/
	  /* 09 */
	/********************  Mass Storage interface ********************/
	0x09,   /* bLength: Interface Descriptor size */
	0x04,   /* bDescriptorType: */
	0x00,   /* bInterfaceNumber: Number of Interface */
	0x00,   /* bAlternateSetting: Alternate setting */
	0x02,   /* bNumEndpoints*/
	0x08,   /* bInterfaceClass: MSC Class */
	0x06,   /* bInterfaceSubClass : SCSI transparent*/
	0x50,   /* nInterfaceProtocol */
	0x05,          /* iInterface: */
	/********************  Mass Storage Endpoints ********************/
	  /* 18 */
	0x07,   /*Endpoint descriptor length = 7*/
	0x05,   /*Endpoint descriptor type */
	MSC_EPIN_ADDR,   /*Endpoint address (IN, address 1) */
	0x02,   /*Bulk endpoint type */
	LOBYTE(MSC_MAX_FS_PACKET),
	HIBYTE(MSC_MAX_FS_PACKET),
	0x00,   /*Polling interval in milliseconds */

	0x07,   /*Endpoint descriptor length = 7 */
	0x05,   /*Endpoint descriptor type */
	MSC_EPOUT_ADDR,   /*Endpoint address (OUT, address 1) */
	0x02,   /*Bulk endpoint type */
	LOBYTE(MSC_MAX_FS_PACKET),
	HIBYTE(MSC_MAX_FS_PACKET),
	0x00,     /*Polling interval in milliseconds*/

	/*---------------------------------------------------------------------------*/
	//IAD Interface Association Descriptor
//	0x08           ,//bLenght:Interface Descriptor size
//	0x0B     ,//bDescriptorType:IAD
//	0x00,//bFirstInterface
//	0x01,//bInterfaceCount
//	0x03,//bFunctionClass:CDC
//	0x00,//bFunctionSubClass
//	0x00,//bInterfaceProtocol
//	0x02,//iFunction
	/************** Descriptor of CUSTOM HID interface ****************/
	/* 09 */
	0x09,         /*bLength: Interface Descriptor size*/
	USB_DESC_TYPE_INTERFACE,/*bDescriptorType: Interface descriptor type*/
	0x01,         /*bInterfaceNumber: Number of Interface*/
	0x00,         /*bAlternateSetting: Alternate setting*/
	0x02,         /*bNumEndpoints*/
	0x03,         /*bInterfaceClass: CUSTOM_HID*/
	0x00,         /*bInterfaceSubClass : 1=BOOT, 0=no boot*/
	0x00,         /*nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse*/
	0,            /*iInterface: Index of string descriptor*/
	/******************** Descriptor of CUSTOM_HID *************************/
	/* 18 */
	0x09,         /*bLength: CUSTOM_HID Descriptor size*/
	CUSTOM_HID_DESCRIPTOR_TYPE, /*bDescriptorType: CUSTOM_HID*/
	0x11,         /*bCUSTOM_HIDUSTOM_HID: CUSTOM_HID Class Spec release number*/
	0x01,
	0x00,         /*bCountryCode: Hardware target country*/
	0x01,         /*bNumDescriptors: Number of CUSTOM_HID class descriptors to follow*/
	0x22,         /*bDescriptorType*/
	USBD_CUSTOM_HID_REPORT_DESC_SIZE,/*wItemLength: Total length of Report descriptor*/
	0x00,
	/******************** Descriptor of Custom HID endpoints ********************/
	/* 27 */
	0x07,          /*bLength: Endpoint Descriptor size*/
	USB_DESC_TYPE_ENDPOINT, /*bDescriptorType:*/

	CUSTOM_HID_EPIN_ADDR,     /*bEndpointAddress: Endpoint Address (IN)*/
	0x03,          /*bmAttributes: Interrupt endpoint*/
	CUSTOM_HID_EPIN_SIZE, /*wMaxPacketSize: 2 Byte max */
	0x00,
	CUSTOM_HID_FS_BINTERVAL,          /*bInterval: Polling Interval */
	/* 34 */

	0x07,	         /* bLength: Endpoint Descriptor size */
	USB_DESC_TYPE_ENDPOINT,	/* bDescriptorType: */
	CUSTOM_HID_EPOUT_ADDR,  /*bEndpointAddress: Endpoint Address (OUT)*/
	0x03,	/* bmAttributes: Interrupt endpoint */
	CUSTOM_HID_EPOUT_SIZE,	/* wMaxPacketSize: 2 Bytes max  */
	0x00,
	CUSTOM_HID_FS_BINTERVAL,	/* bInterval: Polling Interval */
	/* 41 */
	/*---------------------------------------------------------------------------*/
//	//IAD Interface Association Descriptor
//	0x08           ,//bLenght:Interface Descriptor size
//	0x0B     ,//bDescriptorType:IAD
//	0x01,//bFirstInterface
//	0x01,//bInterfaceCount
//	0x08,//bFunctionClass:MSC
//	0x06,//bFunctionSubClass
//	0x50,//bInterfaceProtocol
//	0x05,//iFunction

} ;

/* USB Standard Device Descriptor */
__ALIGN_BEGIN static uint8_t USBD_MSC_HID_DeviceQualifierDesc[USB_LEN_DEV_QUALIFIER_DESC] __ALIGN_END =
{
  USB_LEN_DEV_QUALIFIER_DESC,
  USB_DESC_TYPE_DEVICE_QUALIFIER,
  0x00,
  0x02,
  0x00,
  0x00,
  0x00,
  0x40,
  0x01,
  0x00,
};



uint8_t  USBD_MSC_HID_RegisterUserData  (USBD_HandleTypeDef   *pdev,
										USBD_StorageHidTypeDef* fops )
{
  uint8_t  ret = USBD_FAIL;

  if(fops != NULL)
  {
    pdev->pUserData= fops;
    ret = USBD_OK;
  }

  return ret;
}

uint8_t  USBD_MSC_HID_EP0_RxReady (USBD_HandleTypeDef  *pdev)
{
	return USBD_CUSTOM_HID_EP0_RxReady(pdev);
}

uint8_t  USBD_MSC_HID_Init (USBD_HandleTypeDef *pdev, uint8_t cfgidx)
{
	uint8_t ret = 0U;
	ret |= USBD_MSC_Init(pdev, cfgidx);
	ret |= USBD_CUSTOM_HID_Init(pdev, cfgidx);
	return ret;
}

uint8_t  USBD_MSC_HID_DeInit (USBD_HandleTypeDef *pdev, uint8_t cfgidx)
{
	uint8_t ret = 0U;
	ret |= USBD_MSC_DeInit(pdev, cfgidx);
	ret |= USBD_CUSTOM_HID_DeInit(pdev, cfgidx);
	return ret;
}

uint8_t  USBD_MSC_HID_Setup (USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req)
{
	uint8_t ret = 0U;
	ret |= USBD_MSC_Setup(pdev, req);
	ret |= USBD_CUSTOM_HID_Setup(pdev, req);
	return ret;

}

uint8_t  USBD_MSC_HID_DataIn (USBD_HandleTypeDef *pdev, uint8_t epnum)
{
	uint8_t ret = 0U;
	ret |= USBD_MSC_DataIn(pdev, epnum);
	ret |= USBD_CUSTOM_HID_DataIn(pdev, epnum);
	return ret;
}

uint8_t  USBD_MSC_HID_DataOut (USBD_HandleTypeDef *pdev, uint8_t epnum)
{
	uint8_t ret = 0U;
	ret |= USBD_MSC_DataOut(pdev, epnum);
	ret |= USBD_CUSTOM_HID_DataOut(pdev, epnum);
	return ret;
}



uint8_t *USBD_MSC_HID_GetFSCfgDesc (uint16_t *length)
{
	*length = sizeof (USBD_MSC_HID_CfgDesc);
	return USBD_MSC_HID_CfgDesc;

}

uint8_t *USBD_MSC_HID_GetDeviceQualifierDescriptor (uint16_t *length)
{
	*length = sizeof (USBD_MSC_HID_DeviceQualifierDesc);
	return USBD_MSC_HID_DeviceQualifierDesc;
}




