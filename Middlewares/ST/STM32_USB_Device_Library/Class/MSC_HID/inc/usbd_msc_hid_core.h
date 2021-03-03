/*
 * usbd_msc_hid_core.h
 *
 *  Created on: Mar 2, 2021
 *      Author: radhwen.zorgui
 */

#ifndef ST_STM32_USB_DEVICE_LIBRARY_CLASS_MSC_HID_INC_USBD_MSC_HID_CORE_H_
#define ST_STM32_USB_DEVICE_LIBRARY_CLASS_MSC_HID_INC_USBD_MSC_HID_CORE_H_

#include "usbd_customhid.h"
#include "usbd_msc.h"

#define USB_MSC_HID_CONFIG_DESC_SIZ 64u


extern USBD_ClassTypeDef  USBD_MSC_HID_cb;



typedef struct
{
	USBD_CUSTOM_HID_ItfTypeDef *hid;
	USBD_StorageTypeDef *msc;
}USBD_StorageHidTypeDef;




extern USBD_StorageHidTypeDef USBD_MSC_HID_fops_FS ;


#endif /* ST_STM32_USB_DEVICE_LIBRARY_CLASS_MSC_HID_INC_USBD_MSC_HID_CORE_H_ */
