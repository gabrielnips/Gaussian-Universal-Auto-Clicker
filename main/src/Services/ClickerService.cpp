#include "Services/ClickerService.hpp"
#include "Globals.hpp"

ClickerService::ClickerService( double min, double max, int jitter, std::shared_ptr<IInputService> input, std::shared_ptr<ITargetWindowService> window )
    : minCPS( min ), maxCPS( max ), jitterIntensity( jitter ), inputService( input ), windowService( window ), gen( std::random_device {}( ) )
{
    if ( minCPS <= 0 || maxCPS <= 0 || minCPS > maxCPS )
    {
        throw std::invalid_argument( "Invalid CPS range. Values must be positive and min <= max." );
    }
    double meanCPS = ( minCPS + maxCPS ) / 2.0;
    double stdDevCPS = ( maxCPS - minCPS ) / 4.0;
    dist = std::normal_distribution<>( meanCPS, stdDevCPS );
}

void ClickerService::PerformClick( )
{
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
    if ( jitterIntensity <= 0 ) return;

    std::uniform_int_distribution<> jitterDist( -jitterIntensity, jitterIntensity );
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

void ClickerService::Run( )
{
    while ( true )
    {
        bool shouldClick = inputService->IsActive( ) && windowService->IsTargetWindowActive( );
        UpdateStatus( shouldClick );

        if ( shouldClick )
        {
            double currentCPS = std::clamp( dist( gen ), minCPS, maxCPS );
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