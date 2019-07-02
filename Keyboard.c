
// Author   KMS - Martin Dubois, ing.
// Product  FX3-KB
// File     Keyboard.c

// CODE REVIEW  2019-07-01  KMS - Martin Dubois, ing.

// Includes
/////////////////////////////////////////////////////////////////////////////

#include "Component.h"

#include <cyu3dma.h>
#include <cyu3usb.h>
#include <cyu3utils.h>

#include "Desc.h"
#include "HID.h"
#include "USB.h"

#include "Keyboard.h"

// Constants
/////////////////////////////////////////////////////////////////////////////

#define EP_INTR_IN  0x81

#define THREAD_STACK_byte  (0x1000)
#define THREAD_PRIORITY    (8)

// Static variables
/////////////////////////////////////////////////////////////////////////////

static CyBool_t        sActive = CyFalse;
static CyU3PDmaChannel sChHandleIntrCPU2U;
static CyU3PThread     sThread;

// Static function declarations
/////////////////////////////////////////////////////////////////////////////

static void                Send_Complete();
static CyU3PReturnStatus_t Send_Prepare ( CyU3PDmaBuffer_t * aOutBuf );

static void SendReport_Control( uint8_t aBitMask );
static void SendReport_Key    ( uint8_t aModifier, uint8_t aKey );

static void Start( void );
static void Stop ( void );

static void ThreadEntry( uint32_t aInput );

// Functions
/////////////////////////////////////////////////////////////////////////////

// aModifier  See HID_MODIFIER_...
// aKey       See HID_KEY_...
void Key( uint8_t aModifier, uint8_t aKey )
{
    ASSERT( 0 != aKey );

    Key_Down( aModifier, aKey );
    Key_Up  ( aModifier );
}

// aC  The char to send
void Key_Char( char aC )
{
    ASSERT(    0 < aC );
    ASSERT( 0x80 > aC );

    uint8_t lKey     ;
    uint8_t lModifier;

    lKey = HID_KeyFromChar( aC, & lModifier );

    Key( lModifier, lKey );
}

// aModifier  See HID_MODIFIER_...
// aKey       See HID_KEY_...
void Key_Down( uint8_t aModifier, uint8_t aKey )
{
    ASSERT( 0 != aKey );

    SendReport_Key( aModifier, aKey );
}

void Key_Hibernate()
{
    SendReport_Control( DESC_CONTROL_HIBERNATE );
    SendReport_Control(                      0 );
}

void Key_PowerDown()
{
    SendReport_Control( DESC_CONTROL_POWER_DOWN );
    SendReport_Control(                       0 );
}

void Key_Sleep()
{
    SendReport_Control( DESC_CONTROL_SLEEP );
    SendReport_Control(                  0 );
}

// aStr  The string to send
void Key_String( const char * aStr )
{
    ASSERT( NULL != aStr );

    while ( '\0' != ( * aStr ) )
    {
        Key_Char( * aStr );

        aStr ++;
    }
}

// aModifier  See HID_MODIFIER_...
void Key_Up( uint8_t aModifier )
{
    SendReport_Key( aModifier, 0 );
}

void Key_Wakeup()
{
    SendReport_Control( DESC_CONTROL_WAKEUP );
    SendReport_Control(                   0 );
}

// aIndex  The index part of the setup packet. It contains the end point
//         address.
//
// Return
//  CyFalse  Not handled
//  CyTrue   Handled
CyBool_t Keyboard_ClearFeature( uint16_t aIndex )
{
     if ( ( aIndex == EP_INTR_IN ) && sActive )
    {
        CyU3PReturnStatus_t  lStatus;

        lStatus = CyU3PDmaChannelReset( & sChHandleIntrCPU2U );
        ASSERT( CY_U3P_SUCCESS == lStatus );

        lStatus = CyU3PUsbFlushEp( EP_INTR_IN );
        ASSERT( CY_U3P_SUCCESS == lStatus );

        lStatus = CyU3PUsbResetEp( EP_INTR_IN );
        ASSERT( CY_U3P_SUCCESS == lStatus );

        return CyTrue;
    }

    return CyFalse;
}

void Keyboard_Define( void )
{
    void   * lPtr   ;
    uint32_t lStatus;

    lPtr = CyU3PMemAlloc( THREAD_STACK_byte );
    ASSERT( NULL != lPtr );

    lStatus = CyU3PThreadCreate( & sThread, "30:Keyboard Thread", ThreadEntry, 0, lPtr, THREAD_STACK_byte, THREAD_PRIORITY, THREAD_PRIORITY, CYU3P_NO_TIME_SLICE, CYU3P_AUTO_START );
    ASSERT( CY_U3P_SUCCESS == lStatus );
}

// aData0  The first part of the setup packet
// aData1  The second part of the setup packet
//
// Return
//  CyFalse  Not handled
//  CyTrue   Handled
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

void Send_Complete()
{
    CyU3PReturnStatus_t lStatus;

    lStatus = CyU3PDmaChannelCommitBuffer( & sChHandleIntrCPU2U, 8, 0 );
    if ( CY_U3P_SUCCESS != lStatus )
    {
        lStatus = CyU3PDmaChannelReset  ( & sChHandleIntrCPU2U );
        ASSERT( CY_U3P_SUCCESS == lStatus );

        lStatus = CyU3PDmaChannelSetXfer( & sChHandleIntrCPU2U, 0 );
        ASSERT( CY_U3P_SUCCESS == lStatus );
    }

    CyU3PBusyWait( 2000 );
}

CyU3PReturnStatus_t Send_Prepare( CyU3PDmaBuffer_t * aOutBuf )
{
    CyU3PReturnStatus_t lResult;

    ASSERT( NULL != aOutBuf );

    aOutBuf->buffer =  0;
    aOutBuf->status =  0;
    aOutBuf->size   = 16;
    aOutBuf->count  =  8;

    lResult = CyU3PDmaChannelGetBuffer( & sChHandleIntrCPU2U, aOutBuf, 1000 );
    if ( CY_U3P_SUCCESS != lResult )
    {
        lResult = CyU3PDmaChannelReset( & sChHandleIntrCPU2U );
        // ASSERT( CY_U3P_SUCCESS == lResult );
        // CyU3PDmaChannelReset return CY_U3P_ERROR_BAD_ARGUMENT (64)

        lResult = CyU3PDmaChannelSetXfer( & sChHandleIntrCPU2U, 0 );
        ASSERT( CY_U3P_SUCCESS == lResult );

        lResult = CyU3PDmaChannelGetBuffer( & sChHandleIntrCPU2U, aOutBuf, 1000 );
    }

    if ( CY_U3P_SUCCESS == lResult )
    {
        unsigned int i;

        ASSERT( NULL != aOutBuf->buffer );

        CyU3PBusyWait( 2000 );

        for ( i = 0; i < 8; i ++ )
        {
            aOutBuf->buffer[ i ] = 0;
        }
    }

    return lResult;
}

void SendReport_Control( uint8_t aBitMask )
{
    CyU3PDmaBuffer_t    lOutBuf;
    CyU3PReturnStatus_t lStatus;

    lStatus = Send_Prepare( & lOutBuf );
    if ( CY_U3P_SUCCESS == lStatus )
    {
        lOutBuf.buffer[ 0 ] = DESC_REPORT_CONTROL;
        lOutBuf.buffer[ 1 ] = aBitMask;

        Send_Complete();
    }
}

void SendReport_Key( uint8_t aModifier, uint8_t aKey )
{
    CyU3PDmaBuffer_t    lOutBuf;
    CyU3PReturnStatus_t lStatus;

    lStatus = Send_Prepare( & lOutBuf );
    if ( CY_U3P_SUCCESS == lStatus )
    {
        lOutBuf.buffer[ 0 ] = DESC_REPORT_KEY;
        lOutBuf.buffer[ 1 ] = aModifier;
        lOutBuf.buffer[ 3 ] = aKey;

        Send_Complete();
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

    lStatus = CyU3PDmaChannelSetXfer( & sChHandleIntrCPU2U, 0 );
    ASSERT( CY_U3P_SUCCESS == lStatus );

    lStatus = CyU3PUsbFlushEp( EP_INTR_IN );
    ASSERT( CY_U3P_SUCCESS == lStatus );

    sActive = CyTrue;
}

void Stop( void )
{
    CyU3PReturnStatus_t lStatus = CY_U3P_SUCCESS;
    CyU3PEpConfig_t     lEpCfg;

    sActive = CyFalse;

    lStatus = CyU3PUsbFlushEp( EP_INTR_IN );
    ASSERT( CY_U3P_SUCCESS == lStatus );

    lStatus = CyU3PDmaChannelDestroy( & sChHandleIntrCPU2U );
    ASSERT( CY_U3P_SUCCESS == lStatus );

    CyU3PMemSet( ( uint8_t * )( & lEpCfg ), 0, sizeof( lEpCfg ) );

    lEpCfg.enable = CyFalse;

    lStatus = CyU3PSetEpConfig( EP_INTR_IN, & lEpCfg );
    ASSERT( CY_U3P_SUCCESS == lStatus );
}

void ThreadEntry( uint32_t aInput )
{
    uint32_t lStatus;

    // We let 4 seconds for the operating system to detect the device and
    // completely load the driver.
    lStatus = CyU3PThreadSleep( 4000 );
    ASSERT( CY_U3P_SUCCESS == lStatus );

    Payload();
}
