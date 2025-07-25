#pragma once

struct WindowInfo
{
    HWND handle;
    std::string title;
};

class ITargetWindowService
{
public:
    virtual ~ITargetWindowService( ) = default;
    virtual std::vector<WindowInfo> FindTopLevelWindows( ) = 0;
    virtual void SetTarget( HWND handle ) = 0;
    virtual bool IsTargetWindowActive( ) const = 0;
};