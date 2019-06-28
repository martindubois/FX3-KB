
#ifndef _INCLUDED_CYFXUVCAPP_H_
#define _INCLUDED_CYFXUVCAPP_H_

#include <cyu3types.h>
#include <cyu3externcstart.h>

#define CY_FX_EP_VIDEO_CONS_SOCKET      0x03    /* USB Consumer socket 3 is used for video data. */
#define CY_FX_EP_CONTROL_STATUS_SOCKET  0x02    /* USB Consumer socket 2 is used for the status pipe. */

#define CY_FX_EP_IN_TYPE                0x80    /* USB IN end points have MSB set */
#define CY_FX_EP_BULK_VIDEO             (CY_FX_EP_VIDEO_CONS_SOCKET | CY_FX_EP_IN_TYPE)         /* EP 3 IN */
#define CY_FX_EP_CONTROL_STATUS         (CY_FX_EP_CONTROL_STATUS_SOCKET | CY_FX_EP_IN_TYPE)     /* EP 2 IN */

#define CY_FX_INTF_ASSN_DSCR_TYPE       (0x0B)          /* Type code for Interface Association Descriptor (IAD) */

#define CY_FX_SS_EP_COMPN_DSCR_TYPE             (48)

#define CY_FX_WIRELESS_USB_CAPB_TYPE            (1)
#define CY_FX_CONTAINER_ID_CAPBD_TYPE           (4)

#include <cyu3externcend.h>

#endif /* _INCLUDED_CYFXUVCAPP_H_ */
