#pragma once

#include "Interfaces/ITargetWindowService.hpp"
#include "Stdafx.hpp"

class TargetWindowService : public ITargetWindowService
{
private:
    HWND targetWindow = nullptr;
    static BOOL CALLBACK EnumWindowsProc( HWND hwnd, LPARAM lParam );
public:
    std::vector<WindowInfo> FindTopLevelWindows( ) override;
    void SetTarget( HWND handle ) override;
    bool IsTargetWindowActive( ) const override;
};