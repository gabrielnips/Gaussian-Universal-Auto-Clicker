#include "Services/TargetWindowService.hpp"

BOOL CALLBACK TargetWindowService::EnumWindowsProc( HWND hwnd, LPARAM lParam )
{
    auto* windows = reinterpret_cast<std::vector<WindowInfo>*>( lParam );
    char windowTitle[256];
    if ( IsWindowVisible( hwnd ) && GetWindowTextLength( hwnd ) > 0 )
    {
        GetWindowTextA( hwnd, windowTitle, sizeof( windowTitle ) );
        windows->push_back( { hwnd, std::string( windowTitle ) } );
    }
    return TRUE;
}

std::vector<WindowInfo> TargetWindowService::FindTopLevelWindows( )
{
    std::vector<WindowInfo> windows;
    EnumWindows( EnumWindowsProc, reinterpret_cast<LPARAM>( &windows ) );
    return windows;
}

void TargetWindowService::SetTarget( HWND handle )
{
    targetWindow = handle;
}

bool TargetWindowService::IsTargetWindowActive( ) const
{
    return targetWindow != nullptr && GetForegroundWindow( ) == targetWindow;
}