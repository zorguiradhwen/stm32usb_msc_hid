/*
 * usbd_msc_hid_core.c
 *
 *  Created on: Mar 2, 2021
 *      Author: radhwen.zorgui
 */

#include "usbd_msc.h"
#include "usbd_customhid.h"

USBD_Class_cb_TypeDef  USBD_MSC_HID_cb =
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
  USBD_MSC_HID_GetCfgDesc,
#ifdef USB_OTG_HS_CORE
  USBD_MSC_HID_GetCfgDesc, /* use same config as per FS */
#endif
};


__ALIGN_BEGIN static uint8_t USBD_MSC_HID_CfgDesc[USB_MSC_HID_CONFIG_DESC_SIZ] __ALIGN_END =
{
  /***************Configuration Descriptor***********************/
  0x09, /* bLength: Configuration Descriptor size */
  USB_CONFIGURATION_DESCRIPTOR_TYPE, /* bDescriptorType: Configuration */
  USB_MSC_HID_CONFIG_DESC_SIZ,
  /* wTotalLength: Bytes returned */
  0x00,
  0x02,         /*bNumInterfaces: 2 interface*/
  0x01,         /*bConfigurationValue: Configuration value*/
  0x00,         /*iConfiguration: Index of string descriptor describing
  the configuration*/
  0xC2,         /*bmAttributes: bus powered and Support Remote Wake-up */
  0x32,         /*MaxPower 100 mA: this current is used for detecting Vbus*/


  /*---------------------------------------------------------------------------*/
  //IAD Interface Association Descriptor
 USBD_IAD_DESC_SIZE           ,//bLenght:Interface Descriptor size
 USBD_IAD_DESCRIPTOR_TYPE     ,//bDescriptorType:IAD
 HID_INTERFACE,//bFirstInterface
 0x01,//bInterfaceCount
 0x03,//bFunctionClass:CDC
 0x00,//bFunctionSubClass
 0x00,//bInterfaceProtocol
 0x02,//iFunction
  /************** Descriptor of Custom HID interface ****************/
  /* 09 */
  0x09,         /*bLength: Interface Descriptor size*/
  USB_INTERFACE_DESCRIPTOR_TYPE,/*bDescriptorType: Interface descriptor type*///Interface descriptor number
  HID_INTERFACE,         /*bInterfaceNumber: Number of Interface*/  //The number of the interface The first number is 0
  0x00,         /*bAlternateSetting: Alternate setting*/            //Alternate interface number
  0x02,         /*bNumEndpoints*/                                   //The number of non-zero endpoints. The interface has 2 bulk endpoints
  0x03,         /*bInterfaceClass: HID*/                            //The class used by the interface.
  0x00,         /*bInterfaceSubClass : 1=BOOT, 0=no boot*/
  0x00,         /*nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse*/
  0,            /*iInterface: Index of string descriptor*/
  /******************** Descriptor of Custom HID ********************/
  /* 18 */
  0x09,         /*bLength: HID Descriptor size*/
  CUSTOM_HID_DESCRIPTOR_TYPE, /*bDescriptorType: HID*/
  0x11,         /*bcdHID: HID Class Spec release number*/
  0x01,
  0x00,         /*bCountryCode: Hardware target country*/
  0x01,         /*bNumDescriptors: Number of HID class descriptors to follow*/
  0x22,         /*bDescriptorType*/
  USBD_CUSTOM_HID_REPORT_DESC_SIZE,/*wItemLength: Total length of Report descriptor*/
  0x00,
  /******************** Descriptor of HID endpoint ********************/
  /* 27 */
  0x07,          /*bLength: Endpoint Descriptor size*/
  USB_ENDPOINT_DESCRIPTOR_TYPE, /*bDescriptorType:*/

  HID_IN_EP,     /*bEndpointAddress: Endpoint Address (IN)*/
  0x03,          /*bmAttributes: Interrupt endpoint*/
  HID_IN_PACKET, /*wMaxPacketSize: 4 Byte max */
  0x00,
  0x0A,          /*bInterval: Polling Interval (10 ms)*/
  /* 34 */
    0x07,          /*bLength: Endpoint Descriptor size*/
  USB_ENDPOINT_DESCRIPTOR_TYPE, /*bDescriptorType:*/

  HID_OUT_EP,     /*bEndpointAddress: Endpoint Address (IN)*/
  0x03,          /*bmAttributes: Interrupt endpoint*/
  HID_OUT_PACKET, /*wMaxPacketSize: 4 Byte max */
  0x00,
  0x10,          /* bInterval: Polling Interval (31 ms) */
  /* 34 */
  /*---------------------------------------------------------------------------*/
 //IAD Interface Association Descriptor
  USBD_IAD_DESC_SIZE           ,//bLenght:Interface Descriptor size
  USBD_IAD_DESCRIPTOR_TYPE     ,//bDescriptorType:IAD
  MSC_INTERFACE,//bFirstInterface
  0x01,//bInterfaceCount
  0x08,//bFunctionClass:MSC
  0x06,//bFunctionSubClass
  0x50,//bInterfaceProtocol
  0x05,//iFunction
  /********************  Mass Storage interface ********************/
  0x09,   /* bLength: Interface Descriptor size */
  USB_INTERFACE_DESCRIPTOR_TYPE,   /* bDescriptorType: */
  MSC_INTERFACE,   /* bInterfaceNumber: Number of Interface */  //The number of the interface The second interface number is 1
  0x00,   /* bAlternateSetting: Alternate setting */            //Alternate number
  0x02,   /* bNumEndpoints*/
  0x08,   /* bInterfaceClass: MSC Class */                      //The class used by this interface mass storage device 0x08
  0x06,   /* bInterfaceSubClass : SCSI transparent command set*///The subclass code of SCSI transparent command set is0x06。
  0x50,   /* nInterfaceProtocol */                              //The protocol is batch transmission only, and the code is 0x50.
  0x05,          /* iInterface: */                              //String index value of the interface
  /********************  Mass Storage Endpoints ********************/
  0x07,   /*Endpoint descriptor length = 7*/
  USB_ENDPOINT_DESCRIPTOR_TYPE,   /*Endpoint descriptor type */
  MSC_IN_EP,   /*Endpoint address (IN, address 1) */
  0x02,   /*Bulk endpoint type */
  0x40,
  0x00,
  0x00,   /*Polling interval in milliseconds */

  0x07,   /*Endpoint descriptor length = 7 */
  0x05,   /*Endpoint descriptor type */
  MSC_OUT_EP,   /*Endpoint address (OUT, address 1) */
  0x02,   /*Bulk endpoint type */
  0x40,
  0x00,
  0x00     /*Polling interval in milliseconds*/
} ;
