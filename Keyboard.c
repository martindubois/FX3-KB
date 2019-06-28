
// Author   KMS - Martin Dubois, ing.
// Product  FX3-KB
// File     Keyboard.c

// Includes
/////////////////////////////////////////////////////////////////////////////

#include "Component.h"

#include <cyu3dma.h>
#include <cyu3usb.h>
#include <cyu3utils.h>

#include "HID.h"
#include "USB.h"

#include "Keyboard.h"

// Constants
/////////////////////////////////////////////////////////////////////////////

#define EP_INTR_IN  0x81

#define THREAD_STACK     (0x1000)
#define THREAD_PRIORITY  (8)

// Static variables
/////////////////////////////////////////////////////////////////////////////

static CyBool_t        sActive = CyFalse;
static CyU3PDmaChannel sChHandleIntrCPU2U;
static CyU3PThread     sThread;

// Static function declarations
/////////////////////////////////////////////////////////////////////////////

static void Key     ( uint8_t aModifier, uint8_t aKey );
static void Key_Down( uint8_t aModifier, uint8_t aKey );
static void Key_Up  ( uint8_t aModifier );

static void SendReport( uint8_t aModifier, uint8_t aKey );

static void Start( void );
static void Stop ( void );

static void ThreadEntry( uint32_t aInput );

// Functions
/////////////////////////////////////////////////////////////////////////////

CyBool_t Keyboard_ClearFeature( uint16_t aIndex )
{
	 if ( ( aIndex == EP_INTR_IN ) && sActive )
	{
		CyU3PDmaChannelReset( & sChHandleIntrCPU2U );

		CyU3PUsbFlushEp( EP_INTR_IN );
		CyU3PUsbResetEp( EP_INTR_IN );

		return CyTrue;
	}

	return CyFalse;
}

void Keyboard_Define( void )
{
	void   * lPtr;
    uint32_t lRet;

    lPtr = CyU3PMemAlloc( THREAD_STACK );

    lRet = CyU3PThreadCreate( & sThread, "30:Keyboard Thread", ThreadEntry, 0, lPtr, THREAD_STACK, THREAD_PRIORITY, THREAD_PRIORITY, CYU3P_NO_TIME_SLICE, CYU3P_AUTO_START );
    ASSERT( 0 == lRet );
}

CyBool_t Keyboard_ProcessRequest( uint32_t aData0, uint32_t aData1 )
{
	uint8_t              lBuffer[ 16 ];
	uint16_t             lLength_byte;
	uint16_t             lInfo_byte  ;
    uint8_t              lRequest    ;
    CyBool_t             lResult     ;
    CyU3PReturnStatus_t  lStatus     ;

    lLength_byte = (uint16_t)( ( aData1 & USB_SETUP_LENGTH_MASK ) >> 16 );
    lRequest     = (uint8_t )( ( aData0 & USB_SETUP_REQ_MASK    ) >>  8 );

    switch ( lRequest )
    {
    case HID_REQUEST_SET_IDLE   :
		CyU3PUsbAckSetup();
    	lResult = true;
    	break;

    case HID_REQUEST_SET_REPORT :
    	ASSERT(                 0 <  lLength_byte );
    	ASSERT( sizeof( lBuffer ) >= lLength_byte );

    	lStatus = CyU3PUsbGetEP0Data( sizeof( lBuffer ), lBuffer, & lInfo_byte );
    	ASSERT( CY_U3P_SUCCESS == lStatus );

    	ASSERT(                 0 <  lInfo_byte );
    	ASSERT( sizeof( lBuffer ) >= lInfo_byte );

    	lResult = true;
    	break;

    default : ASSERT( false );
    }

    return lResult;
}

void Keyboard_Reset( void )
{
	if ( sActive )
	{
	    Stop();
	}
}

void Keyboard_SetConf( void )
{
	if ( sActive )
	{
	    Stop();
	}

    Start();
}

// Static functions
/////////////////////////////////////////////////////////////////////////////

void Key( uint8_t aModifier, uint8_t aKey )
{
	ASSERT( 0 != aKey );

	Key_Down( aModifier, aKey );
	Key_Up  ( aModifier );
}

void Key_Down( uint8_t aModifier, uint8_t aKey )
{
	ASSERT( 0 != aKey );

	SendReport( aModifier, aKey );
}

void Key_Up( uint8_t aModifier )
{
	SendReport( aModifier, 0 );
}

void SendReport( uint8_t aModifier, uint8_t aKey )
{
    CyU3PDmaBuffer_t    lOutBuf;
    CyU3PReturnStatus_t lStatus;

    lOutBuf.buffer = 0;
    lOutBuf.status = 0;
    lOutBuf.size   = 8;
    lOutBuf.count  = 8;

    lStatus = CyU3PDmaChannelGetBuffer( & sChHandleIntrCPU2U, & lOutBuf, 1000 );
    if ( CY_U3P_SUCCESS != lStatus )
    {
        CyU3PDmaChannelReset  ( & sChHandleIntrCPU2U );
        CyU3PDmaChannelSetXfer( & sChHandleIntrCPU2U, 0 );

        lStatus = CyU3PDmaChannelGetBuffer( & sChHandleIntrCPU2U, & lOutBuf, 1000 );
    }

    if ( CY_U3P_SUCCESS == lStatus )
    {
    	CyU3PBusyWait( 2000 );

    	lOutBuf.buffer[ 0 ] = aModifier;
    	lOutBuf.buffer[ 1 ] = 0;
    	lOutBuf.buffer[ 2 ] = aKey;
    	lOutBuf.buffer[ 3 ] = 0;
    	lOutBuf.buffer[ 4 ] = 0;
    	lOutBuf.buffer[ 5 ] = 0;
    	lOutBuf.buffer[ 6 ] = 0;
    	lOutBuf.buffer[ 7 ] = 0;

    	lStatus = CyU3PDmaChannelCommitBuffer( & sChHandleIntrCPU2U, 8, 0 );
    	if ( CY_U3P_SUCCESS != lStatus )
    	{
        	CyU3PDmaChannelReset  ( & sChHandleIntrCPU2U );
        	CyU3PDmaChannelSetXfer( & sChHandleIntrCPU2U, 0 );
    	}

    	CyU3PBusyWait( 2000 );
    }
}

void Start( void )
{
    CyU3PReturnStatus_t     lStatus = CY_U3P_SUCCESS;
    CyU3PDmaChannelConfig_t lDmaCfg;
    uint16_t                lSize_byte = 0x40;
    CyU3PEpConfig_t         lEpCfg;

    CyU3PMemSet( (uint8_t *)( & lEpCfg ), 0, sizeof( lEpCfg ) );

    lEpCfg.enable   = CyTrue;
    lEpCfg.epType   = CY_U3P_USB_EP_INTR;
    lEpCfg.burstLen = 1;
    lEpCfg.streams  = 0;
    lEpCfg.pcktSize = lSize_byte;

    lStatus = CyU3PSetEpConfig( EP_INTR_IN, & lEpCfg );
    ASSERT( CY_U3P_SUCCESS == lStatus );

    lDmaCfg.size           = lSize_byte;
    lDmaCfg.count          = 20;
    lDmaCfg.prodSckId      = CY_U3P_CPU_SOCKET_PROD;
    lDmaCfg.consSckId      = (CyU3PDmaSocketId_t)( CY_U3P_UIB_SOCKET_CONS_1 );
    lDmaCfg.dmaMode        = CY_U3P_DMA_MODE_BYTE;
    lDmaCfg.notification   = 0;
    lDmaCfg.cb             = NULL;
    lDmaCfg.prodHeader     = 0;
    lDmaCfg.prodFooter     = 0;
    lDmaCfg.consHeader     = 0;
    lDmaCfg.prodAvailCount = 0;

    lStatus = CyU3PDmaChannelCreate( & sChHandleIntrCPU2U, CY_U3P_DMA_TYPE_MANUAL_OUT, & lDmaCfg );
    ASSERT( CY_U3P_SUCCESS == lStatus );

    CyU3PDmaChannelSetXfer( & sChHandleIntrCPU2U, 0 );

    CyU3PUsbFlushEp( EP_INTR_IN );

    sActive = CyTrue;
}

void Stop( void )
{
    CyU3PReturnStatus_t lStatus = CY_U3P_SUCCESS;
    CyU3PEpConfig_t     lEpCfg;

    sActive = CyFalse;

    CyU3PUsbFlushEp( EP_INTR_IN );

    CyU3PDmaChannelDestroy( & sChHandleIntrCPU2U );

    CyU3PMemSet( ( uint8_t * )( & lEpCfg ), 0, sizeof( lEpCfg ) );

    lEpCfg.enable = CyFalse;

    lStatus = CyU3PSetEpConfig( EP_INTR_IN, & lEpCfg );
    ASSERT( CY_U3P_SUCCESS == lStatus );
}

void ThreadEntry( uint32_t aInput )
{
	uint8_t lModifier = 0;

    CyU3PThreadSleep( 10000 );

    for ( ; ; )
    {
        CyU3PThreadSleep( 10000 );

        lModifier = HID_LEFT_SHIFT;

        Key( lModifier, HID_KEY_LETTER( 'M' ) );

        lModifier = 0;

        Key( lModifier, HID_KEY_LETTER( 'A' ) );
        Key( lModifier, HID_KEY_LETTER( 'R' ) );
        Key( lModifier, HID_KEY_LETTER( 'T' ) );
        Key( lModifier, HID_KEY_LETTER( 'I' ) );
        Key( lModifier, HID_KEY_LETTER( 'N' ) );

        Key( lModifier, HID_KEY_ENTER );
    }
}
