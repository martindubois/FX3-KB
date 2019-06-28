
// Author   KMS - Martin Dubois, ing.
// Product  FX3-KB
// File     USB.h

#pragma once

// Constants
/////////////////////////////////////////////////////////////////////////////

#define USB_SET_INTF_REQ_TYPE    (uint8_t)( 0x01 )
#define USB_SET_INTERFACE_REQ    (uint8_t)( 0x0B )

#define USB_SETUP_REQ_TYPE_MASK  (uint32_t)( 0x000000FF )
#define USB_SETUP_REQ_MASK       (uint32_t)( 0x0000FF00 )
#define USB_SETUP_VALUE_MASK     (uint32_t)( 0xFFFF0000 )
#define USB_SETUP_INDEX_MASK     (uint32_t)( 0x0000FFFF )
#define USB_SETUP_LENGTH_MASK    (uint32_t)( 0xFFFF0000 )
