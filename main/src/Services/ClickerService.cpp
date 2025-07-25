#include "Stdafx.cpp"
#include "Services/ClickerService.hpp"


ClickerService::ClickerService( const AppSettings& appSettings, std::shared_ptr<IInputService> input, std::shared_ptr<ITargetWindowService> window )
    : settings( appSettings ), inputService( input ), windowService( window ), gen( std::random_device {}( ) ), stopRequested( false )
{
    if ( settings.minCPS <= 0 || settings.maxCPS <= 0 || settings.minCPS > settings.maxCPS )
    {
        throw std::invalid_argument( "Invalid CPS range. Values must be positive and min <= max." );
    }
    double meanCPS = ( settings.minCPS + settings.maxCPS ) / 2.0;
    double stdDevCPS = ( settings.maxCPS - settings.minCPS ) / 4.0;
    dist = std::normal_distribution<>( meanCPS, stdDevCPS );
}

void ClickerService::Stop( )
{
    stopRequested.store( true );
}

void ClickerService::Run( )
{
    while ( !stopRequested )
    { 
        bool shouldClick = inputService->IsActive( ) && windowService->IsTargetWindowActive( );
        UpdateStatus( shouldClick );

        if ( shouldClick )
        {
            double currentCPS = std::clamp( dist( gen ), static_cast<double>( settings.minCPS ), static_cast<double>( settings.maxCPS ) );
            int delay_ms = static_cast<int>( 1000.0 / currentCPS );

            PerformClick( );
            PerformJitter( );

            if ( delay_ms > 0 )
            {
                std::this_thread::sleep_for( std::chrono::milliseconds( delay_ms ) );
            }
        }
        else
        {
            std::this_thread::sleep_for( std::chrono::milliseconds( 20 ) );
        }
    }
}

void ClickerService::PerformClick( )
{
    if ( settings.clickingMode == ClickingMode::IN_AREA )
    {
        std::uniform_int_distribution<> distribX( settings.clickArea.left, settings.clickArea.right );
        std::uniform_int_distribution<> distribY( settings.clickArea.top, settings.clickArea.bottom );
        SetCursorPos( distribX( gen ), distribY( gen ) );
        std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
    }
    INPUT inputs[2] = {};
    ZeroMemory( inputs, sizeof( inputs ) );
    inputs[0].type = INPUT_MOUSE;
    inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    inputs[1].type = INPUT_MOUSE;
    inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;
    SendInput( 2, inputs, sizeof( INPUT ) );
}
void ClickerService::PerformJitter( )
{
    if ( settings.jitterIntensity <= 0 ) return;
    std::uniform_int_distribution<> jitterDist( -settings.jitterIntensity, settings.jitterIntensity );
    int dx = jitterDist( gen );
    int dy = jitterDist( gen );
    INPUT input = {};
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_MOVE;
    input.mi.dx = dx;
    input.mi.dy = dy;
    SendInput( 1, &input, sizeof( INPUT ) );
}
void ClickerService::UpdateStatus( bool isClicking )
{
    if ( isClicking == wasClicking ) return;
    std::string title = AppGlobals::APP_NAME + " | Status: ";
    title += isClicking ? "CLICKING!" : "Idle";
    SetConsoleTitleA( title.c_str( ) );
    wasClicking = isClicking;
}