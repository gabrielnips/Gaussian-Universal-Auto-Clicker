#pragma once

class IClickerService
{
public:
    virtual ~IClickerService( ) = default;
    virtual void Run( ) = 0;
    virtual void Stop( ) = 0; 
};