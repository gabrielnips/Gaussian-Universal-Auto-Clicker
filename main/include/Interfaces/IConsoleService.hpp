#pragma once

#include "ITargetWindowService.hpp"
#include "Core/AppSettings.hpp"

class IConsoleService
{
public:
    virtual ~IConsoleService( ) = default;
    virtual void PrintMessage( const std::string& message, WORD color ) const = 0;
    virtual void PrintPrompt( const std::string& message ) const = 0;
    virtual int GetIntegerInput( int min = 0, int max = 2147483647 ) const = 0;
    virtual HWND SelectWindow( const std::vector<WindowInfo>& windows ) const = 0;
    virtual int CaptureKey( ) const = 0;
    virtual ActivationMode SelectActivationMode( ) const = 0;
    virtual ClickingMode SelectClickingMode( ) const = 0;
    virtual RECT DefineClickArea( ) const = 0;
    virtual std::string VirtualKeyToString( int vkCode ) const = 0;
    virtual void ClearScreen( ) const = 0;
    virtual void DrawHeader( ) const = 0;
    virtual void PrintSettings( const AppSettings& settings, const std::string& windowTitle ) const = 0;
};