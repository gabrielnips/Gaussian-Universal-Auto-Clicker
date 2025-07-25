#pragma once

#include "Interfaces/IInputService.hpp"
#include "Stdafx.hpp"

class InputService : public IInputService
{
private:
    int activationKey = 0;
public:
    void SetActivationKey( int vkCode ) override;
    bool IsActivationKeyDown( ) const override;
};