#pragma once

enum class ActivationMode
{
    HOLD,
    TOGGLE
};

class IInputService
{
public:
    virtual ~IInputService( ) = default;
    virtual void SetActivationKey( int vkCode ) = 0;
    virtual void SetActivationMode( ActivationMode mode ) = 0;
    virtual bool IsActive( ) = 0;
};