#pragma once

#include "Interfaces/IClickerService.hpp"
#include "Interfaces/IInputService.hpp"
#include "Interfaces/ITargetWindowService.hpp"
#include "Core/AppSettings.hpp"

class ClickerService : public IClickerService
{
private:
    AppSettings settings;
    std::shared_ptr<IInputService> inputService;
    std::shared_ptr<ITargetWindowService> windowService;
    mutable std::mt19937 gen;
    std::normal_distribution<> dist;
    bool wasClicking = false;
    std::atomic<bool> stopRequested;

    void PerformClick( );
    void PerformJitter( );
    void UpdateStatus( bool isClicking );

public:
    ClickerService( const AppSettings& appSettings, std::shared_ptr<IInputService> input, std::shared_ptr<ITargetWindowService> window );
    void Run( ) override;
    void Stop( ) override;
};