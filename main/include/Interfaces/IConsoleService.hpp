#pragma once

#include "Stdafx.hpp"
#include "ITargetWindowService.hpp"

class IConsoleService
{
public:
    virtual ~IConsoleService( ) = default;
    virtual void PrintMessage( const std::string& message, WORD color ) const = 0;
    virtual void PrintPrompt( const std::string& message ) const = 0;
    virtual int GetIntegerInput( ) const = 0;
    virtual HWND SelectWindow( const std::vector<WindowInfo>& windows ) const = 0;
    virtual int CaptureKey( ) const = 0;
    virtual std::string VirtualKeyToString( int vkCode ) const = 0;
    virtual void ClearScreen( ) const = 0;
    virtual void DrawHeader( ) const = 0;
};