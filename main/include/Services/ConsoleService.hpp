#pragma once
#include "Interfaces/IConsoleService.hpp"

namespace ConsoleColors
{
    const WORD WHITE = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
    const WORD CYAN = FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE;
    const WORD GREEN = FOREGROUND_INTENSITY | FOREGROUND_GREEN;
    const WORD YELLOW = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN;
    const WORD RED = FOREGROUND_INTENSITY | FOREGROUND_RED;
}

class ConsoleService : public IConsoleService
{
private:
    HANDLE hConsole;
    void SetColor( WORD color ) const;
public:
    ConsoleService( );
    void PrintMessage( const std::string& message, WORD color ) const override;
    void PrintPrompt( const std::string& message ) const override;
    int GetIntegerInput( int min, int max ) const override;
    HWND SelectWindow( const std::vector<WindowInfo>& windows ) const override;
    int CaptureKey( ) const override;
    ActivationMode SelectActivationMode( ) const override;
    std::string VirtualKeyToString( int vkCode ) const override;
    void ClearScreen( ) const override;
    void DrawHeader( ) const override;

    ClickingMode SelectClickingMode( ) const override;
    RECT DefineClickArea( ) const override;
    void PrintSettings( const AppSettings& settings, const std::string& windowTitle ) const override;
};