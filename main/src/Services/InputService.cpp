#include "Services/InputService.hpp"

void InputService::SetActivationKey( int vkCode )
{
    activationKey = vkCode;
}

bool InputService::IsActivationKeyDown( ) const
{
    if ( activationKey == 0 ) return false;
    return ( GetAsyncKeyState( activationKey ) & 0x8000 ) != 0;
}