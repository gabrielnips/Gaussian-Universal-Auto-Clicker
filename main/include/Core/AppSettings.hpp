#pragma once

#include "Interfaces/IInputService.hpp"

enum class ClickingMode
{
    AT_CURSOR,
    IN_AREA
};

struct AppSettings
{
    int activationKey = VK_LBUTTON;
    ActivationMode activationMode = ActivationMode::HOLD;
    ClickingMode clickingMode = ClickingMode::AT_CURSOR;
    int minCPS = 10;
    int maxCPS = 15;
    int jitterIntensity = 0;
    RECT clickArea = { 0, 0, 0, 0 };
};