#pragma once
#include "Interfaces/IInputService.hpp"

class InputService : public IInputService
{
private:
    int activationKey = 0;
    ActivationMode mode = ActivationMode::HOLD;
    bool toggledOn = false;
    bool keyWasDown = false;

public:
    void SetActivationKey( int vkCode ) override;
    void SetActivationMode( ActivationMode mode ) override;
    bool IsActive( ) override;
};