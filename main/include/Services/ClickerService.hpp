#pragma once

#include "Interfaces/IClickerService.hpp"
#include "Interfaces/IInputService.hpp"
#include "Interfaces/ITargetWindowService.hpp"
#include "Stdafx.hpp"

class ClickerService : public IClickerService
{
private:
    double minCPS, maxCPS;
    std::shared_ptr<IInputService> inputService;
    std::shared_ptr<ITargetWindowService> windowService;
    std::mt19937 gen;
    std::normal_distribution<> dist;
    bool wasClicking = false;

    void PerformClick( ) const;
    void UpdateStatus( bool isClicking );

public:
    ClickerService( double min, double max, std::shared_ptr<IInputService> input, std::shared_ptr<ITargetWindowService> window );
    void Run( ) override;
};