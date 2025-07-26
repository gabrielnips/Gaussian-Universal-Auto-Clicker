#include "Stdafx.cpp"
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
	if ( activationKey == 0 )
	{
		return false;
	}

	switch ( mode )
	{
		case ActivationMode::HOLD:
		{
			return ( GetAsyncKeyState( activationKey ) & 0x8000 ) != 0;
		}

		case ActivationMode::TOGGLE:
		{
			if ( ( GetAsyncKeyState( activationKey ) & 1 ) != 0 )
			{
				toggledOn = !toggledOn;
			}
			return toggledOn;
		}

		default:
		{
			return false;
		}
	}
}
