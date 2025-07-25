#include "Services/ConsoleService.hpp"
#include "Globals.hpp"

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

    while ( true )
    {
        PrintPrompt( ">> " );
        int choice = GetIntegerInput( 1, 2 );
        if ( choice == 1 ) return ActivationMode::HOLD;
        if ( choice == 2 ) return ActivationMode::TOGGLE;
    }
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
        choice = GetIntegerInput( 0, windows.size( ) - 1 );
    }
    return windows[choice].handle;
}

std::string ConsoleService::VirtualKeyToString( int vkCode ) const
{
    switch ( vkCode )
    {
    case VK_LBUTTON: return "Left Mouse Button";
    case VK_RBUTTON: return "Right Mouse Button";
    case VK_MBUTTON: return "Middle Mouse Button";
    case VK_XBUTTON1: return "Mouse 4";
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