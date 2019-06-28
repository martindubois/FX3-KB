
// Author   KMS - Martin Dubois, ing.
// Product  FX3-KB
// File     Device.c

// Includes
/////////////////////////////////////////////////////////////////////////////

#include "Component.h"

#include <cyu3gpio.h>
#include <cyu3pib.h>
#include <cyu3usb.h>

#include "Desc.h"
#include "Keyboard.h"
#include "USB.h"

// Constants
/////////////////////////////////////////////////////////////////////////////

#define THREAD_STACK     (0x0800)
#define THREAD_PRIORITY  (8)

// Static variables
/////////////////////////////////////////////////////////////////////////////

static CyBool_t  sConfigured = CyFalse;
static CyBool_t  sConnected  = CyFalse;

static CyU3PUSBSpeed_t sSpeed = CY_U3P_NOT_CONNECTED;
static CyU3PThread     sThread;

// Static function declarations
/////////////////////////////////////////////////////////////////////////////

static void Init( void );

static void     OnEvent     ( CyU3PUsbEventType_t aEvType, uint16_t aEvData );
static CyBool_t OnLPMRequest( CyU3PUsbLinkPowerMode aLinkMode );
static CyBool_t OnSetup     ( uint32_t aData0, uint32_t aData1 );

static CyBool_t ProcessDeviceRequest   ( uint32_t aData0, uint32_t aData1 );
static CyBool_t ProcessEndPointRequest ( uint32_t aData0, uint32_t aData1 );
static CyBool_t ProcessInterfaceRequest( uint32_t aData0, uint32_t aData1 );

static void ThreadEntry( uint32_t aInput );

// Entry points
/////////////////////////////////////////////////////////////////////////////

/* void _close ( void )        { assert( false ); }
void _exit  ( int aStatus ) { for ( ;; ); }
void _fstat ( void )        { assert( false ); }
void _getpid( void )        { assert( false ); }
void _isatty( void )        { assert( false ); }
void _kill  ( void )        { assert( false ); }
void _lseek ( void )        { assert( false ); }
void _read  ( void )        { assert( false ); }
void _sbrk  ( void )        { assert( false ); }
void _write ( void )        { assert( false ); } */

int main( void )
{
    CyU3PIoMatrixConfig_t lIoCfg ;
    CyU3PReturnStatus_t   lStatus;

    lStatus = CyU3PDeviceInit( 0 );
    ASSERT( CY_U3P_SUCCESS == lStatus );

    lStatus = CyU3PDeviceCacheControl( CyTrue, CyFalse, CyFalse );
    ASSERT( CY_U3P_SUCCESS == lStatus );

    lIoCfg.isDQ32Bit        = CyFalse;
    lIoCfg.s0Mode           = CY_U3P_SPORT_4BIT;
    lIoCfg.s1Mode           = CY_U3P_SPORT_4BIT;
    lIoCfg.gpioSimpleEn [0] = 0;
    lIoCfg.gpioSimpleEn [1] = 0x02102800; // IOs 43, 45, 52 and 57 are chosen as GPIO.
    lIoCfg.gpioComplexEn[0] = 0;
    lIoCfg.gpioComplexEn[1] = 0;
    lIoCfg.useI2C           = CyFalse;
    lIoCfg.useI2S           = CyFalse;
    lIoCfg.useUart          = CyFalse;
    lIoCfg.useSpi           = CyFalse;
    lIoCfg.lppMode          = CY_U3P_IO_MATRIX_LPP_UART_ONLY;

    lStatus = CyU3PDeviceConfigureIOMatrix( & lIoCfg );
    ASSERT( CY_U3P_SUCCESS == lStatus );

    CyU3PKernelEntry ();

    // Dummy return to make the compiler happy
    return 0;
}

void CyFxApplicationDefine( void )
{
    void   * lPtr;
    uint32_t lRet;

    lPtr = CyU3PMemAlloc( THREAD_STACK );
    ASSERT( NULL != lPtr );

    lRet = CyU3PThreadCreate( & sThread, "31:EP0 Thread", ThreadEntry, 0, lPtr, THREAD_STACK, THREAD_PRIORITY, THREAD_PRIORITY, CYU3P_NO_TIME_SLICE, CYU3P_AUTO_START );
    ASSERT( 0 == lRet );

    Keyboard_Define();
}

// Static functions
/////////////////////////////////////////////////////////////////////////////

void Init( void )
{
    CyU3PReturnStatus_t  lStatus   ;
    CyU3PGpioClock_t     lGpioClock;
    CyU3PPibClock_t      lPibClock ;

    sConnected = CyFalse;

    lGpioClock.fastClkDiv = 2;
    lGpioClock.slowClkDiv = 2;
    lGpioClock.simpleDiv  = CY_U3P_GPIO_SIMPLE_DIV_BY_2;
    lGpioClock.clkSrc     = CY_U3P_SYS_CLK;
    lGpioClock.halfDiv    = 0;

    lStatus = CyU3PGpioInit( & lGpioClock, NULL );
    ASSERT( CY_U3P_SUCCESS == lStatus );

    lPibClock.clkDiv      = 2;
    lPibClock.clkSrc      = CY_U3P_SYS_CLK;
    lPibClock.isDllEnable = CyFalse;
    lPibClock.isHalfDiv   = CyFalse;

    lStatus = CyU3PPibInit( CyTrue, & lPibClock );
    ASSERT( CY_U3P_SUCCESS == lStatus );

    lStatus = CyU3PUsbStart();
    ASSERT( CY_U3P_SUCCESS == lStatus );

    CyU3PUsbRegisterSetupCallback( OnSetup, CyFalse );
    CyU3PUsbRegisterEventCallback( OnEvent );

    CyU3PUsbRegisterLPMRequestCallback( OnLPMRequest );

    CyU3PUsbSetDesc( CY_U3P_USB_SET_HS_DEVICE_DESCR, 0, (uint8_t *)( Desc_Device_HS ) );
    CyU3PUsbSetDesc( CY_U3P_USB_SET_SS_DEVICE_DESCR, 0, (uint8_t *)( Desc_Device_SS ) );

    CyU3PUsbSetDesc( CY_U3P_USB_SET_DEVQUAL_DESCR, 0, (uint8_t *)( Desc_Device_Qual ) );
    CyU3PUsbSetDesc( CY_U3P_USB_SET_SS_BOS_DESCR , 0, (uint8_t *)( Desc_BOS         ) );

    CyU3PUsbSetDesc( CY_U3P_USB_SET_HS_CONFIG_DESCR, 0, (uint8_t *)( Desc_Config_HS ) );
    CyU3PUsbSetDesc( CY_U3P_USB_SET_FS_CONFIG_DESCR, 0, (uint8_t *)( Desc_Config_FS ) );
    CyU3PUsbSetDesc( CY_U3P_USB_SET_SS_CONFIG_DESCR, 0, (uint8_t *)( Desc_Config_SS ) );

    CyU3PUsbSetDesc( CY_U3P_USB_SET_STRING_DESCR, 0, (uint8_t *)( Desc_LangId              ) );
    CyU3PUsbSetDesc( CY_U3P_USB_SET_STRING_DESCR, 1, (uint8_t *)( Desc_String_Manufacturer ) );
    CyU3PUsbSetDesc( CY_U3P_USB_SET_STRING_DESCR, 2, (uint8_t *)( Desc_String_Product      ) );

    lStatus = CyU3PConnectState( CyTrue, CyTrue );
    ASSERT( CY_U3P_SUCCESS == lStatus );
}

void OnEvent( CyU3PUsbEventType_t aEvType, uint16_t aEvData )
{
    switch ( aEvType )
    {
    case CY_U3P_USB_EVENT_RESET:
        sConfigured = CyFalse;

        Keyboard_Reset();
        break;

    case CY_U3P_USB_EVENT_SUSPEND:
        break;

    case CY_U3P_USB_EVENT_DISCONNECT:
        sConfigured = CyFalse;
        sConnected  = CyFalse;
        break;

    case CY_U3P_USB_EVENT_SETCONF:
        if ( aEvData == 0 )
        {
            sConfigured = CyFalse;
            break;
        }

        if ( sConfigured == CyFalse )
        {
            sConfigured = CyTrue;
        }

        Keyboard_SetConf();
        break;

    default:
    	break;
    }
}

CyBool_t OnLPMRequest( CyU3PUsbLinkPowerMode aLinkMode )
{
    return CyTrue;
}

CyBool_t OnSetup( uint32_t aData0, uint32_t aData1 )
{
    uint8_t  lReqType;
    CyBool_t lResult = CyFalse;
    uint8_t  lTarget ;

    lReqType = ( aData0   & CY_U3P_USB_REQUEST_TYPE_MASK );
    lTarget  = ( lReqType & CY_U3P_USB_TARGET_MASK       );

    switch ( lTarget )
    {
    case CY_U3P_USB_TARGET_DEVICE : lResult = ProcessDeviceRequest   ( aData0, aData1 ); break;
    case CY_U3P_USB_TARGET_ENDPT  : lResult = ProcessEndPointRequest ( aData0, aData1 ); break;
    case CY_U3P_USB_TARGET_INTF   : lResult = ProcessInterfaceRequest( aData0, aData1 ); break;

    case CY_U3P_USB_TARGET_OTHER : CyU3PUsbStall( 0, CyTrue, CyFalse ); break;

    default : ASSERT( false );
    }

    return lResult;
}

CyBool_t ProcessDeviceRequest( uint32_t aData0, uint32_t aData1 )
{
    uint16_t  lIndex  ;
    uint8_t   lRequest;
    CyBool_t  lResult = CyFalse;
    uint16_t  lValue  ;

    lIndex   = (uint16_t)(   aData1 & USB_SETUP_INDEX_MASK          );
    lRequest = (uint8_t )( ( aData0 & USB_SETUP_REQ_MASK    ) >>  8 );
    lValue   = (uint16_t)( ( aData0 & USB_SETUP_VALUE_MASK  ) >> 16 );

	switch ( lRequest )
	{
	case CY_U3P_USB_SC_GET_DESCRIPTOR    :
	case CY_U3P_USB_SC_SET_CONFIGURATION :
		break;

	default : ASSERT( false );
    }

	return lResult;
}

CyBool_t ProcessEndPointRequest( uint32_t aData0, uint32_t aData1 )
{
    uint16_t  lIndex  ;
    uint8_t   lRequest;
    CyBool_t  lResult ;
    uint16_t  lValue  ;

    lIndex   = (uint16_t)(   aData1 & USB_SETUP_INDEX_MASK          );
    lRequest = (uint8_t )( ( aData0 & USB_SETUP_REQ_MASK    ) >>  8 );
    lValue   = (uint16_t)( ( aData0 & USB_SETUP_VALUE_MASK  ) >> 16 );

    switch ( lRequest )
	{
    case CY_U3P_USB_SC_CLEAR_FEATURE:
		if ( lValue == CY_U3P_USBX_FS_EP_HALT )
		{
			lResult = Keyboard_ClearFeature( lIndex );
			if ( lResult )
			{
				CyU3PUsbStall( lIndex, CyFalse, CyTrue );
				CyU3PUsbAckSetup();
			}
		}
		break;

    default: ASSERT( false );
	}

	return lResult;
}

CyBool_t ProcessInterfaceRequest( uint32_t aData0, uint32_t aData1 )
{
    uint16_t  lIndex  ;
    uint8_t   lRequest = CyFalse;
    uint8_t   lReqType;
    CyBool_t  lResult ;
    uint8_t   lType   ;
    uint16_t  lValue  ;

    lIndex   = (uint16_t)(   aData1 & USB_SETUP_INDEX_MASK          );
    lReqType =           (   aData0 & CY_U3P_USB_REQUEST_TYPE_MASK  );
    lRequest = (uint8_t )( ( aData0 & USB_SETUP_REQ_MASK    ) >>  8 );
    lType    =           ( lReqType & CY_U3P_USB_TYPE_MASK          );
    lValue   = (uint16_t)( ( aData0 & USB_SETUP_VALUE_MASK  ) >> 16 );

    switch ( lType )
    {
	case CY_U3P_USB_CLASS_RQT : lResult = Keyboard_ProcessRequest( aData0, aData1 ); break;

    case CY_U3P_USB_STANDARD_RQT :
		switch ( lRequest )
		{
		case CY_U3P_USB_SC_GET_DESCRIPTOR :
			if ( ( lValue >> 8 ) == 0x22 )
			{
			    uint32_t lStatus;

				lResult = CyTrue;

				switch ( lIndex )
				{
				case 0 : lStatus = CyU3PUsbSendEP0Data( DESC_REPORT_KEYBOARD_SIZE_byte, (uint8_t *)( Desc_Report_Keyboard ) ); break;

				#ifdef _CONSUMER_CONTROL_
	        		case 1 : lStatus = CyU3PUsbSendEP0Data( DESC_REPORT_CONSUMER_CONTROL_SIZE_byte, (uint8_t *)( Desc_Report_Consumer_Control ) ); break;
				#endif

	        	default : lStatus = CY_U3P_ERROR_NOT_SUPPORTED;
				}

				if ( lStatus != CY_U3P_SUCCESS )
				{
					CyU3PUsbStall( 0, CyTrue, CyFalse );
				}
			}
			break;

		case CY_U3P_USB_SC_SET_FEATURE :
			if ( ( lValue == 0 ) && ( lIndex == 1 ) )
			{
				if ( sConfigured )
				{
					CyU3PUsbAckSetup();
				}
				else
				{
					CyU3PUsbStall( 0, CyTrue, CyFalse );
				}

				lResult = CyTrue;
			}
			break;

		case CY_U3P_USB_SC_SET_INTERFACE :
			CyU3PUsbStall( 0, CyTrue, CyFalse );
			break;

		default : ASSERT( false );
		}
		break;

	default : ASSERT( false );
    }

	return lResult;
}

void ThreadEntry( uint32_t aInput )
{
    Init();

    for (;;)
    {
        if ( ! sConnected )
        {
            sSpeed = CyU3PUsbGetSpeed();
            if ( CY_U3P_NOT_CONNECTED != sSpeed )
            {
                sConnected = CyTrue;
            }
        }

        CyU3PThreadRelinquish ();
    }
}
