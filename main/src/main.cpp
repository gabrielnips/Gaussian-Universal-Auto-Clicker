#include "Stdafx.cpp"

#include "Core/Application.hpp"
#include "Services/ConsoleService.hpp"
#include "Services/TargetWindowService.hpp"
#include "Services/InputService.hpp"

void ConfigureConsoleWindow( )
{
	HWND consoleWindow = GetConsoleWindow( );
	if ( consoleWindow == nullptr )
	{
		return;
	}

	LONG style = GetWindowLong( consoleWindow, GWL_STYLE );
	style      = style & ~( WS_MAXIMIZEBOX ) & ~( WS_THICKFRAME );
	SetWindowLong( consoleWindow, GWL_STYLE, style );

	HANDLE hConsole = GetStdHandle( STD_OUTPUT_HANDLE );
	if ( hConsole == INVALID_HANDLE_VALUE )
	{
		return;
	}

	constexpr short windowWidth  = 75;
	constexpr short windowHeight = 20;

	COORD bufferSize = { windowWidth, windowHeight };
	SetConsoleScreenBufferSize( hConsole, bufferSize );

	SMALL_RECT windowRect = { 0, 0, windowWidth - 1, windowHeight - 1 };
	SetConsoleWindowInfo( hConsole, TRUE, &windowRect );
}

int main( )
{
	ConfigureConsoleWindow( );

	auto consoleService = std::make_shared<ConsoleService>( );
	auto windowService  = std::make_shared<TargetWindowService>( );
	auto inputService   = std::make_shared<InputService>( );

	Application app( consoleService, windowService, inputService );
	app.Start( );

	return 0;
}
