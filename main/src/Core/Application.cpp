#include "Core/Application.hpp"
#include "Services/ClickerService.hpp"
#include "Globals.hpp"
#include "Services/ConsoleService.hpp"

Application::Application(
    std::shared_ptr<IConsoleService> cs,
    std::shared_ptr<ITargetWindowService> ws,
    std::shared_ptr<IInputService> is )
    : console( cs ), windowService( ws ), inputService( is )
{
}

void Application::Start( )
{
    SetConsoleTitleA( AppGlobals::APP_NAME.c_str( ) );
    console->ClearScreen( );
    console->DrawHeader( );

    auto windows = windowService->FindTopLevelWindows( );
    if ( windows.empty( ) )
    {
        console->PrintMessage( "No visible windows found. Exiting.", ConsoleColors::RED );
        return;
    }
    HWND target = console->SelectWindow( windows );
    windowService->SetTarget( target );

    console->ClearScreen( );
    console->DrawHeader( );

    int activationKey = console->CaptureKey( );
    inputService->SetActivationKey( activationKey );

    console->ClearScreen( );
    console->DrawHeader( );
    console->PrintMessage( "Activation key set to: " + console->VirtualKeyToString( activationKey ), ConsoleColors::GREEN );

    try
    {
        console->PrintPrompt( "\nEnter minimum CPS >> " );
        int minCPS = console->GetIntegerInput( );

        console->PrintPrompt( "Enter maximum CPS >> " );
        int maxCPS = console->GetIntegerInput( );

        clickerService = std::make_unique<ClickerService>( minCPS, maxCPS, inputService, windowService );

        console->ClearScreen( );
        console->DrawHeader( );
        console->PrintMessage( "Configuration set. Clicker is active.", ConsoleColors::GREEN );
        console->PrintMessage( "Hold [" + console->VirtualKeyToString( activationKey ) + "] in the target window to click.", ConsoleColors::YELLOW );
        console->PrintMessage( "\nPress ENTER in this console to exit.", ConsoleColors::WHITE );

        std::thread clickerThread( &IClickerService::Run, clickerService.get( ) );
        clickerThread.detach( );

        std::string dummy;
        std::getline( std::cin, dummy );

    }
    catch ( const std::invalid_argument& e )
    {
        console->PrintMessage( "Error: " + std::string( e.what( ) ), ConsoleColors::RED );
        console->PrintMessage( "Press ENTER to exit.", ConsoleColors::WHITE );
        std::string dummy;
        std::getline( std::cin, dummy );
    }
}