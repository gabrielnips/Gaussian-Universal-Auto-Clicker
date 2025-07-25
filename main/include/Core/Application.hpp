#pragma once

#include "Interfaces/IConsoleService.hpp"
#include "Interfaces/ITargetWindowService.hpp"
#include "Interfaces/IInputService.hpp"
#include "Interfaces/IClickerService.hpp"
#include "Stdafx.hpp"

class Application
{
private:
    std::shared_ptr<IConsoleService> console;
    std::shared_ptr<ITargetWindowService> windowService;
    std::shared_ptr<IInputService> inputService;
    std::unique_ptr<IClickerService> clickerService;

public:
    Application(
        std::shared_ptr<IConsoleService> cs,
        std::shared_ptr<ITargetWindowService> ws,
        std::shared_ptr<IInputService> is );

    void Start( );
};