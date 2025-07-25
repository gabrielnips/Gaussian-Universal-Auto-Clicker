#include "Stdafx.cpp"
#include "Services/ConsoleService.hpp"

RECT ConsoleService::DefineClickArea( ) const
{
    RECT area;
    POINT p1, p2;

    ClearScreen( );
    DrawHeader( );
    PrintMessage( "DEFINE CLICK AREA: Step 1 of 2", ConsoleColors::YELLOW );
    PrintMessage( "Move your mouse to the TOP-LEFT corner of the desired area and press ENTER.", ConsoleColors::WHITE );
    std::cin.get( );
    GetCursorPos( &p1 );
    PrintMessage( "Top-left corner set!\n", ConsoleColors::GREEN );

    PrintMessage( "DEFINE CLICK AREA: Step 2 of 2", ConsoleColors::YELLOW );
    PrintMessage( "Now, move your mouse to the BOTTOM-RIGHT corner and press ENTER.", ConsoleColors::WHITE );
    std::cin.get( );
    GetCursorPos( &p2 );
    PrintMessage( "Bottom-right corner set!\n", ConsoleColors::GREEN );

    area.left = std::min( p1.x, p2.x );
    area.top = std::min( p1.y, p2.y );
    area.right = std::max( p1.x, p2.x );
    area.bottom = std::max( p1.y, p2.y );

    PrintMessage( "Click area has been defined.", ConsoleColors::GREEN );
    std::this_thread::sleep_for( std::chrono::seconds( 2 ) );
    return area;
}

ConsoleService::ConsoleService( ) : hConsole( GetStdHandle( STD_OUTPUT_HANDLE ) )
{
}
void ConsoleService::SetColor( WORD color ) const
{
    SetConsoleTextAttribute( hConsole, color );
}
void ConsoleService::ClearScreen( ) const
{
    system( "cls" );
}
void ConsoleService::DrawHeader( ) const
{
    SetColor( ConsoleColors::CYAN );
    std::cout << "--- " << AppGlobals::APP_NAME << " v" << AppGlobals::APP_VERSION << " ---\n\n";
    SetColor( ConsoleColors::WHITE );
}
void ConsoleService::PrintMessage( const std::string& message, WORD color ) const
{
    SetColor( color );
    std::cout << message << std::endl;
    SetColor( ConsoleColors::WHITE );
}
void ConsoleService::PrintPrompt( const std::string& message ) const
{
    SetColor( ConsoleColors::YELLOW );
    std::cout << message;
    SetColor( ConsoleColors::WHITE );
}
int ConsoleService::GetIntegerInput( int min, int max ) const
{
    int value;
    while ( true )
    {
        std::cin >> value;
        if ( std::cin.good( ) && value >= min && value <= max )
        {
            std::cin.ignore( std::numeric_limits<std::streamsize>::max( ), '\n' );
            return value;
        }
        std::cin.clear( );
        std::cin.ignore( std::numeric_limits<std::streamsize>::max( ), '\n' );
        PrintMessage( "Invalid input. Please enter a number between " + std::to_string( min ) + " and " + std::to_string( max ) + ".", ConsoleColors::RED );
        PrintPrompt( ">> " );
    }
}
ActivationMode ConsoleService::SelectActivationMode( ) const
{
    PrintMessage( "\nSelect activation mode:", ConsoleColors::CYAN );
    std::cout << "  [1] Hold (Click while key is pressed)\n";
    std::cout << "  [2] Toggle (Press key to turn on/off)\n";
    PrintPrompt( ">> " );
    int choice = GetIntegerInput( 1, 2 );
    return ( choice == 1 ) ? ActivationMode::HOLD : ActivationMode::TOGGLE;
}
ClickingMode ConsoleService::SelectClickingMode( ) const
{
    PrintMessage( "\nSelect clicking mode:", ConsoleColors::CYAN );
    std::cout << "  [1] At Cursor (Click at current mouse position)\n";
    std::cout << "  [2] In Area (Click within a defined area)\n";
    PrintPrompt( ">> " );
    int choice = GetIntegerInput( 1, 2 );
    return ( choice == 1 ) ? ClickingMode::AT_CURSOR : ClickingMode::IN_AREA;
}
void ConsoleService::PrintSettings( const AppSettings& settings, const std::string& windowTitle ) const
{
    ClearScreen( );
    DrawHeader( );
    PrintMessage( "Configuration loaded. Clicker is active.", ConsoleColors::GREEN );
    std::string activationModeStr = ( settings.activationMode == ActivationMode::HOLD ) ? "Hold" : "Toggle";
    std::string clickingModeStr = ( settings.clickingMode == ClickingMode::AT_CURSOR ) ? "At Cursor" : "In Defined Area";
    std::cout << "  - Target Window: " << windowTitle << "\n";
    std::cout << "  - Activation Key: [" << VirtualKeyToString( settings.activationKey ) << "]\n";
    std::cout << "  - Activation Mode: " << activationModeStr << "\n";
    std::cout << "  - Clicking Mode: " << clickingModeStr << "\n";
    std::cout << "  - CPS Range: " << settings.minCPS << " - " << settings.maxCPS << "\n";
    std::cout << "  - Jitter Intensity: " << settings.jitterIntensity << "\n";
    PrintMessage( "\n----- HOTKEYS -----", ConsoleColors::CYAN );
    std::cout << "  [F6] Define new click area\n";
    std::cout << "  [F7] Import config from clipboard\n";
    std::cout << "  [F8] Export config to clipboard\n";
    PrintMessage( "\nPress ENTER in this console to exit.", ConsoleColors::WHITE );
}
HWND ConsoleService::SelectWindow( const std::vector<WindowInfo>& windows ) const
{
    PrintMessage( "Available Windows:", ConsoleColors::CYAN );
    for ( size_t i = 0; i < windows.size( ); ++i )
    {
        std::cout << "  [" << i << "] " << windows[i].title << std::endl;
    }
    int choice = -1;
    while ( choice < 0 || static_cast<size_t>( choice ) >= windows.size( ) )
    {
        PrintPrompt( "\nSelect target window number >> " );
        choice = GetIntegerInput( 0, static_cast<int>( windows.size( ) - 1 ) );
    }
    return windows[choice].handle;
}
std::string ConsoleService::VirtualKeyToString( int vkCode ) const
{
    switch ( vkCode )
    {
    case VK_LBUTTON: return "Left Mouse Button"; case VK_RBUTTON: return "Right Mouse Button";
    case VK_MBUTTON: return "Middle Mouse Button"; case VK_XBUTTON1: return "Mouse 4";
    case VK_XBUTTON2: return "Mouse 5";
    default:
    UINT scanCode = MapVirtualKey( vkCode, MAPVK_VK_TO_VSC );
    char keyName[256];
    if ( GetKeyNameTextA( scanCode << 16, keyName, sizeof( keyName ) ) )
    {
        return std::string( keyName );
    }
    return "Unknown Key (Code: " + std::to_string( vkCode ) + ")";
    }
}
int ConsoleService::CaptureKey( ) const
{
    PrintMessage( "\nPress any key to use as the activator... (Listening)", ConsoleColors::YELLOW );
    while ( true )
    {
        bool anyKeyDown = false;
        for ( int i = 1; i < 255; i++ )
        {
            if ( GetAsyncKeyState( i ) & 0x8000 )
            {
                anyKeyDown = true; break;
            }
        }
        if ( !anyKeyDown ) break;
        std::this_thread::sleep_for( std::chrono::milliseconds( 50 ) );
    }
    while ( true )
    {
        for ( int i = 1; i < 255; i++ )
        {
            if ( GetAsyncKeyState( i ) & 0x8000 )
            {
                return i;
            }
        }
        std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
    }
}