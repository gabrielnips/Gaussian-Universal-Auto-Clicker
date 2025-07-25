#include "Services/InputService.hpp"

void InputService::SetActivationKey( int vkCode )
{
    activationKey = vkCode;
}

void InputService::SetActivationMode( ActivationMode newMode )
{
    mode = newMode;
}

bool InputService::IsActive( )
{
    if ( activationKey == 0 ) return false;

    bool isKeyDown = ( GetAsyncKeyState( activationKey ) & 0x8000 ) != 0;

    if ( mode == ActivationMode::HOLD )
    {
        return isKeyDown;
    }

    if ( isKeyDown && !keyWasDown )
    {
        toggledOn = !toggledOn;
    }
    keyWasDown = isKeyDown;
    return toggledOn;
}