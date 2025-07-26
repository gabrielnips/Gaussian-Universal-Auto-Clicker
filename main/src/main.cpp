#include "Stdafx.cpp"

#include "Core/Application.hpp"
#include "Services/ConsoleService.hpp"
#include "Services/TargetWindowService.hpp"
#include "Services/InputService.hpp"

bool ConfigureConsoleWindow( )
{
	constexpr short width  = 75;
	constexpr short height = 20;

	const HWND consoleWindow = GetConsoleWindow( );
	if ( consoleWindow == nullptr )
	{
		return false;
	}

	const LONG currentStyle = GetWindowLong( consoleWindow, GWL_STYLE );
	SetWindowLong( consoleWindow, GWL_STYLE, currentStyle & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME );

	const HANDLE hConsoleOutput = GetStdHandle( STD_OUTPUT_HANDLE );
	if ( hConsoleOutput == INVALID_HANDLE_VALUE )
	{
		return false;
	}

	constexpr COORD bufferSize = { width, height };
	if ( !SetConsoleScreenBufferSize( hConsoleOutput, bufferSize ) )
	{
		return false;
	}

	constexpr SMALL_RECT windowRect = { 0, 0, width - 1, height - 1 };
	if ( !SetConsoleWindowInfo( hConsoleOutput, TRUE, &windowRect ) )
	{
		return false;
	}

	const HANDLE hConsoleInput = GetStdHandle( STD_INPUT_HANDLE );
	if ( hConsoleInput == INVALID_HANDLE_VALUE )
	{
		return false;
	}

	DWORD consoleMode;
	if ( !GetConsoleMode( hConsoleInput, &consoleMode ) )
	{
		return false;
	}

	consoleMode &= ~ENABLE_QUICK_EDIT_MODE;
	consoleMode |= ENABLE_EXTENDED_FLAGS;

	if ( !SetConsoleMode( hConsoleInput, consoleMode ) )
	{
		return false;
	}

	return true;
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
