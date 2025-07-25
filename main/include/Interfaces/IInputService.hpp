#pragma once

class IInputService
{
public:
    virtual ~IInputService( ) = default;
    virtual void SetActivationKey( int vkCode ) = 0;
    virtual bool IsActivationKeyDown( ) const = 0;
};