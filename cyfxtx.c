
// Author   KMS - Martin Dubois, ing.
// Product  FX3-KB
// File     cyfxtx.c

// Includes
/////////////////////////////////////////////////////////////////////////////

#include "Component.h"

#include <cyu3os.h>
#include <cyu3error.h>

// Constants
/////////////////////////////////////////////////////////////////////////////

#define MEM_HEAP_BASE       ( (uint8_t *) 0x40038000 )
#define MEM_HEAP_SIZE_byte  (             0x8000     )

#define SYS_MEM_TOP  ( 0x40080000 )

#define BUFFER_HEAP_BASE       ( (uint32_t)( MEM_HEAP_BASE ) + MEM_HEAP_SIZE_byte )
#define BUFFER_HEAP_SIZE_byte  ( SYS_MEM_TOP - BUFFER_HEAP_BASE )

#define ALLOC_TIMEOUT  ( 10 )

// Macro
/////////////////////////////////////////////////////////////////////////////

#define MIN(a,b)  ( ( ( a ) < ( b ) ) ? ( a ) : ( b ) )

// Static variables
/////////////////////////////////////////////////////////////////////////////

static CyBool_t          sMemPool_Init = CyFalse;
static CyU3PBytePool     sMemPool_byte;
static CyU3PDmaBufMgr_t  sBufferManager = {{0}, 0, 0, 0, 0, 0};

// Static function declarations
/////////////////////////////////////////////////////////////////////////////

static void DmaBufMgr_SetStatus( uint32_t aStartPos, uint32_t aNumBits, CyBool_t aValue );

// Functions
/////////////////////////////////////////////////////////////////////////////

void CyU3PUndefinedHandler( void )
{
    for (;;);
}

void CyU3PPrefetchHandler( void )
{
    for (;;);
}

void CyU3PAbortHandler( void )
{
    for (;;);
}

void tx_application_define( void * aUnusedMem )
{
    (void)( aUnusedMem );

    CyU3PApplicationDefine();
}

void CyU3PMemInit( void )
{
    if ( ! sMemPool_Init )
    {
    	sMemPool_Init = CyTrue;

    	CyU3PBytePoolCreate( & sMemPool_byte, MEM_HEAP_BASE, MEM_HEAP_SIZE_byte );
    }
}

void * CyU3PMemAlloc( uint32_t aSize_byte )
{
    void   * lResult;
    uint32_t status;

    if ( CyU3PThreadIdentify() )
    {
        status = CyU3PByteAlloc ( & sMemPool_byte, (void **)( & lResult ), aSize_byte, ALLOC_TIMEOUT );
    }
    else
    {
        status = CyU3PByteAlloc ( & sMemPool_byte, (void **)( & lResult ), aSize_byte, CYU3P_NO_WAIT );
    }

    if( status != CY_U3P_SUCCESS )
    {
        lResult = NULL;
    }

    return lResult;
}

void CyU3PMemFree( void * aMem )
{
    CyU3PByteFree( aMem );
}

void CyU3PMemSet( uint8_t * aPtr, uint8_t aData, uint32_t aCount )
{
    while ( aCount >> 3 )
    {
        aPtr[ 0 ] = aData;
        aPtr[ 1 ] = aData;
        aPtr[ 2 ] = aData;
        aPtr[ 3 ] = aData;
        aPtr[ 4 ] = aData;
        aPtr[ 5 ] = aData;
        aPtr[ 6 ] = aData;
        aPtr[ 7 ] = aData;

        aCount -= 8;
        aPtr   += 8;
    }

    while ( aCount -- )
    {
        * aPtr = aData;

        aPtr++;
    }
}

void CyU3PMemCopy( uint8_t * aDest, uint8_t * aSrc, uint32_t aCount )
{
    while ( aCount >> 3 )
    {
        aDest[ 0 ] = aSrc[ 0 ];
        aDest[ 1 ] = aSrc[ 1 ];
        aDest[ 2 ] = aSrc[ 2 ];
        aDest[ 3 ] = aSrc[ 3 ];
        aDest[ 4 ] = aSrc[ 4 ];
        aDest[ 5 ] = aSrc[ 5 ];
        aDest[ 6 ] = aSrc[ 6 ];
        aDest[ 7 ] = aSrc[ 7 ];

        aCount -= 8;
        aDest  += 8;
        aSrc   += 8;
    }

    while ( aCount -- )
    {
        * aDest = * aSrc;

        aDest ++;
        aSrc  ++;
    }
}

int32_t CyU3PMemCmp( const void * aS1, const void * aS2, uint32_t aN )
{
    const uint8_t * aPtr1 = aS1;
    const uint8_t * aPtr2 = aS2;

    while( aN -- )
    {
        if( * aPtr1 != * aPtr2 )
        {
            return ( * aPtr1 - * aPtr2 );
        }
        
        aPtr1 ++;
        aPtr2 ++;
    }

    return 0;
}

void CyU3PDmaBufferInit( void )
{
    uint32_t lStatus;
    uint32_t lSize_byte;
    uint32_t lTmp;

    if ( ( sBufferManager.startAddr != 0 ) && ( sBufferManager.regionSize != 0 ) )
    {
        return;
    }

    lStatus = CyU3PMutexCreate ( & sBufferManager.lock, CYU3P_NO_INHERIT );
    if ( lStatus != CY_U3P_SUCCESS )
    {
        return;
    }

    lSize_byte = ( ( BUFFER_HEAP_SIZE_byte / 32 ) + 31 ) / 32;
    sBufferManager.usedStatus = (uint32_t *)( CyU3PMemAlloc( lSize_byte * 4 ) );
    if ( sBufferManager.usedStatus == 0 )
    {
        CyU3PMutexDestroy( & sBufferManager.lock );
        return;
    }

    CyU3PMemSet( (uint8_t *)( sBufferManager.usedStatus ), 0, ( lSize_byte * 4 ) );
    if ( ( BUFFER_HEAP_SIZE_byte / 32 ) & 31 )
    {
        lTmp = 32 - ( ( BUFFER_HEAP_SIZE_byte / 32 ) & 31 );
        sBufferManager.usedStatus[ lSize_byte - 1 ] = ~ ( ( 1 << lTmp ) - 1 );
    }

    sBufferManager.startAddr  = BUFFER_HEAP_BASE;
    sBufferManager.regionSize = BUFFER_HEAP_SIZE_byte;
    sBufferManager.statusSize = lSize_byte;
    sBufferManager.searchPos  = 0;
}

void CyU3PDmaBufferDeInit( void )
{
    uint32_t lStatus;

    if ( CyU3PThreadIdentify() )
    {
        lStatus = CyU3PMutexGet( & sBufferManager.lock, CYU3P_WAIT_FOREVER );
    }
    else
    {
        lStatus = CyU3PMutexGet( & sBufferManager.lock, CYU3P_NO_WAIT );
    }

    if ( lStatus != CY_U3P_SUCCESS )
    {
        return;
    }

    CyU3PMemFree( sBufferManager.usedStatus );

    sBufferManager.usedStatus = 0;
    sBufferManager.startAddr  = 0;
    sBufferManager.regionSize = 0;
    sBufferManager.statusSize = 0;

    CyU3PMutexPut    ( & sBufferManager.lock);
    CyU3PMutexDestroy( & sBufferManager.lock);
}

void * CyU3PDmaBufferAlloc( uint16_t aSize_byte )
{
    uint32_t lBitnum;
    uint32_t lCount;
    void   * lPtr   = 0;
    uint32_t lStart = 0;
    uint32_t lTmp;
    uint32_t lWordnum;

    if ( CyU3PThreadIdentify() )
    {
        lTmp = CyU3PMutexGet( & sBufferManager.lock, ALLOC_TIMEOUT );
    }
    else
    {
        lTmp = CyU3PMutexGet( & sBufferManager.lock, CYU3P_NO_WAIT );
    }

    if ( lTmp != CY_U3P_SUCCESS )
    {
        return lPtr;
    }

    if ( ( sBufferManager.startAddr == 0 ) || ( sBufferManager.regionSize == 0 ) )
    {
        CyU3PMutexPut( & sBufferManager.lock );
        return lPtr;
    }

    aSize_byte = ( aSize_byte <= 32 ) ? 2 : ( aSize_byte + 31 ) / 32;

    lWordnum = sBufferManager.searchPos;
    lBitnum  = 0;
    lCount   = 0;
    lTmp     = 0;

    while ( lTmp < sBufferManager.statusSize )
    {
        if ( ( sBufferManager.usedStatus[ lWordnum ] & ( 1 << lBitnum ) ) == 0 )
        {
            if ( lCount == 0 )
            {
                lStart = ( lWordnum << 5 ) + lBitnum + 1;
            }
            lCount++;
            if ( lCount == ( aSize_byte + 1 ) )
            {
                sBufferManager.searchPos = lWordnum;
                break;
            }
        }
        else
        {
            lCount = 0;
        }

        lBitnum++;
        if ( lBitnum == 32 )
        {
            lBitnum = 0;
            lWordnum ++;
            lTmp     ++;
            if ( lWordnum == sBufferManager.statusSize )
            {
                lWordnum = 0;
                lCount   = 0;
            }
        }
    }

    if ( lCount == ( aSize_byte + 1 ) )
    {
        DmaBufMgr_SetStatus( lStart, aSize_byte - 1, CyTrue );
        lPtr = (void *)( sBufferManager.startAddr + ( lStart << 5 ) );
    }

    CyU3PMutexPut( & sBufferManager.lock );

    return lPtr;
}

int CyU3PDmaBufferFree( void * aBuffer )
{
    uint32_t lBitnum;
    uint32_t lCount;
    int      lResult = -1;
    uint32_t lStart;
    uint32_t lStatus;
    uint32_t lWordnum;

    if ( CyU3PThreadIdentify() )
    {
        lStatus = CyU3PMutexGet( & sBufferManager.lock, ALLOC_TIMEOUT );
    }
    else
    {
        lStatus = CyU3PMutexGet( & sBufferManager.lock, CYU3P_NO_WAIT );
    }

    if ( lStatus != CY_U3P_SUCCESS )
    {
        return lResult;
    }

    lStart = (uint32_t)( aBuffer );
    if ( ( lStart > sBufferManager.startAddr ) && ( lStart < ( sBufferManager.startAddr + sBufferManager.regionSize ) ) )
    {
        lStart = ( ( lStart - sBufferManager.startAddr ) >> 5 );

        lWordnum = ( lStart >> 5    );
        lBitnum  = ( lStart &  0x1F );
        lCount   = 0;

        while ( ( lWordnum < sBufferManager.statusSize ) && ( ( sBufferManager.usedStatus[ lWordnum] & ( 1 << lBitnum ) ) != 0 ) )
        {
            lCount  ++;
            lBitnum ++;
            if ( lBitnum == 32 )
            {
                lBitnum = 0;
                lWordnum++;
            }
        }

        DmaBufMgr_SetStatus( lStart, lCount, CyFalse );

        sBufferManager.searchPos = 0;

        lResult = 0;
    }

    CyU3PMutexPut( & sBufferManager.lock );
    return lResult;
}

void CyU3PFreeHeaps( void )
{
    CyU3PDmaBufferDeInit();
    CyU3PBytePoolDestroy( & sMemPool_byte );
    sMemPool_Init = CyFalse;
}

// Static functions
/////////////////////////////////////////////////////////////////////////////

static void DmaBufMgr_SetStatus( uint32_t aStartPos, uint32_t aNumBits, CyBool_t aValue )
{
    uint32_t lEndbit;
    uint32_t lMask;
    uint32_t lStartbit;
    uint32_t lWordnum  = ( aStartPos >> 5 );

    lStartbit = ( aStartPos & 31 );
    lEndbit   = MIN( 32, lStartbit + aNumBits );

    lMask  = ( lEndbit == 32 ) ? 0xFFFFFFFFU : ( (uint32_t)( 1 << lEndbit ) - 1 );
    lMask -= ( ( 1 << lStartbit ) - 1 );

    while ( aNumBits )
    {
        if ( aValue )
        {
            sBufferManager.usedStatus[ lWordnum ] |=   lMask;
        }
        else
        {
            sBufferManager.usedStatus[ lWordnum ] &= ~ lMask;
        }

        lWordnum ++;
        aNumBits -= ( lEndbit - lStartbit );
        if ( aNumBits >= 32 )
        {
            lStartbit = 0;
            lEndbit   = 32;
            lMask     = 0xFFFFFFFFU;
        }
        else
        {
            lStartbit = 0;
            lEndbit   = aNumBits;
            lMask     = ( (uint32_t)( 1 << aNumBits ) - 1 );
        }
    }
}
