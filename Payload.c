
// Author   KMS - Martin Dubois, ing.
// Product  FX3-KB
// File     Payload.c

// CODE REVIEW  2019-07-01  KMS - Martin Dubois, ing.

// CONFIG  _EMPTY_
// #define _EMPTY_

// CONFIG  _HELLO_WORLD_
// #define _HELLO_WORLD_

// CONFIG  _SLEEP_WAKE_LOGIN_
#define _SLEEP_WAKE_LOGIN_

// Includes
/////////////////////////////////////////////////////////////////////////////

#include "Component.h"

#include <cyu3os.h>
#include <cyu3usb.h>

#include "HID.h"
#include "Keyboard.h"

// Entry point
/////////////////////////////////////////////////////////////////////////////

#ifdef _EMPTY_

    void Payload()
    {
        for ( ; ; )
        {
            CyU3PThreadSleep( 10000 );
        }
    }

#endif

#ifdef _HELLO_WORLD_

    void Payload()
    {
        for ( ; ; )
        {
            Key_String( "Hello, world!\n" );
            CyU3PThreadSleep( 10000 );
        }
    }

#endif

#ifdef _SLEEP_WAKE_LOGIN_

    void Payload()
    {
        for ( ; ; )
        {
            // 0 s - Put the computer in sleep
            Key_Sleep();

            CyU3PThreadSleep( 19000 );

            // 19 s - Wakeup the computer
            CyU3PUsbDoRemoteWakeup();
            CyU3PThreadSleep( 10000 );

            // 29 s - Exit the black screen
            Key( 0, HID_KEY_ESCAPE );
            CyU3PThreadSleep( 1000 );

            // 30 s - Enter the login screen
            Key( 0, HID_KEY_ESCAPE );
            CyU3PThreadSleep( 1000 );

            // 31 s - Enter the password
            Key_String( "alazar" );
            CyU3PThreadSleep( 1000 );

            // 32 s - Login!
            Key( 0, HID_KEY_ENTER );
            CyU3PThreadSleep( 10000 );

            // 42 s
        }
    }

#endif
