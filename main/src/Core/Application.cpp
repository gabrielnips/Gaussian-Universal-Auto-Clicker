#include "Stdafx.cpp"
#include "Core/Application.hpp"

using json = nlohmann::json;

Application::Application(
	std::shared_ptr<IConsoleService> cs,
	std::shared_ptr<ITargetWindowService> ws,
	std::shared_ptr<IInputService> is )
	: console( cs ),
	  windowService( ws ),
	  inputService( is )
{
}

Application::~Application( )
{
	if ( clickerService )
	{
		clickerService->Stop( );
	}
	if ( clickerThread.joinable( ) )
	{
		clickerThread.join( );
	}
}

void Application::MainLoop( )
{
	console->PrintSettings( settings, targetWindowTitle );

	while ( GetAsyncKeyState( VK_RETURN ) & 0x8000 )
	{
		std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
	}

	while ( true )
	{
		if ( GetAsyncKeyState( VK_F6 ) & 1 )
		{
			settings.clickArea = console->DefineClickArea( );
			console->PrintSettings( settings, targetWindowTitle );
		}
		if ( GetAsyncKeyState( VK_F7 ) & 1 )
		{
			ImportConfigFromClipboard( );
			CreateAndRunClickerThread( );
			console->PrintSettings( settings, targetWindowTitle );
		}
		if ( GetAsyncKeyState( VK_F8 ) & 1 )
		{
			ExportConfigToClipboard( );
		}
		if ( GetAsyncKeyState( VK_RETURN ) & 1 )
		{
			break;
		}
		std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
	}
}

void Application::CreateAndRunClickerThread( )
{
	if ( clickerService )
	{
		clickerService->Stop( );
	}
	if ( clickerThread.joinable( ) )
	{
		clickerThread.join( );
	}
	clickerService = std::make_unique<ClickerService>( settings, inputService, windowService );
	clickerThread  = std::thread( &IClickerService::Run, clickerService.get( ) );
}

void Application::Start( )
{
	SetConsoleTitleA( AppGlobals::APP_NAME.c_str( ) );
	InitialSetup( );
	CreateAndRunClickerThread( );
	MainLoop( );
}

void Application::InitialSetup( )
{
	console->ClearScreen( );
	console->DrawHeader( );
	auto windows = windowService->FindTopLevelWindows( );
	if ( windows.empty( ) )
	{
		console->PrintMessage( "No visible windows found. Exiting.", ConsoleColors::RED );
		exit( 0 );
	}
	HWND target = console->SelectWindow( windows );
	windowService->SetTarget( target );
	for ( const auto& win : windows )
	{
		if ( win.handle == target )
		{
			targetWindowTitle = win.title;
			break;
		}
	}
	console->ClearScreen( );
	console->DrawHeader( );
	settings.activationKey = console->CaptureKey( );
	inputService->SetActivationKey( settings.activationKey );
	settings.activationMode = console->SelectActivationMode( );
	inputService->SetActivationMode( settings.activationMode );
	settings.clickingMode = console->SelectClickingMode( );
	if ( settings.clickingMode == ClickingMode::IN_AREA )
	{
		settings.clickArea = console->DefineClickArea( );
	}
	console->ClearScreen( );
	console->DrawHeader( );
	console->PrintPrompt( "Enter minimum CPS >> " );
	settings.minCPS = console->GetIntegerInput( 1, 1000 );
	console->PrintPrompt( "Enter maximum CPS >> " );
	settings.maxCPS = console->GetIntegerInput( settings.minCPS, 1000 );
	console->PrintPrompt( "Enter jitter intensity (0-10, 0 to disable) >> " );
	settings.jitterIntensity = console->GetIntegerInput( 0, 10 );
}

void Application::ExportConfigToClipboard( ) const
{
	std::string configStr = SerializeSettings( );
	if ( OpenClipboard( nullptr ) )
	{
		EmptyClipboard( );
		HGLOBAL hg = GlobalAlloc( GMEM_MOVEABLE, configStr.size( ) + 1 );
		if ( hg )
		{
			memcpy( GlobalLock( hg ), configStr.c_str( ), configStr.size( ) + 1 );
			GlobalUnlock( hg );
			SetClipboardData( CF_TEXT, hg );
		}
		CloseClipboard( );
		console->PrintMessage( "\nConfig exported to clipboard!", ConsoleColors::GREEN );
		std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
		console->PrintSettings( settings, targetWindowTitle );
	}
}

void Application::ImportConfigFromClipboard( )
{
	if ( OpenClipboard( nullptr ) )
	{
		HANDLE hData = GetClipboardData( CF_TEXT );
		if ( hData )
		{
			auto pszText = static_cast<char*>( GlobalLock( hData ) );
			if ( pszText )
			{
				if ( DeserializeSettings( std::string( pszText ) ) )
				{
					console->PrintMessage( "\nConfig imported successfully!", ConsoleColors::GREEN );
				}
				else
				{
					console->PrintMessage( "\nFailed to import config from clipboard. Invalid format.", ConsoleColors::RED );
				}
				GlobalUnlock( hData );
			}
		}
		CloseClipboard( );
		std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
	}
}

std::string Application::SerializeSettings( ) const
{
	json j;
	j[ "activationKey" ]   = settings.activationKey;
	j[ "activationMode" ]  = settings.activationMode;
	j[ "clickingMode" ]    = settings.clickingMode;
	j[ "minCPS" ]          = settings.minCPS;
	j[ "maxCPS" ]          = settings.maxCPS;
	j[ "jitterIntensity" ] = settings.jitterIntensity;
	j[ "clickArea" ]       = {
		{ "left", settings.clickArea.left },
		{ "top", settings.clickArea.top },
		{ "right", settings.clickArea.right },
		{ "bottom", settings.clickArea.bottom }
	};
	return j.dump( 4 );
}

bool Application::DeserializeSettings( const std::string& data )
{
	try
	{
		json j = json::parse( data );

		settings.activationKey   = j.at( "activationKey" ).get<int>( );
		settings.activationMode  = j.at( "activationMode" ).get<ActivationMode>( );
		settings.clickingMode    = j.at( "clickingMode" ).get<ClickingMode>( );
		settings.minCPS          = j.at( "minCPS" ).get<int>( );
		settings.maxCPS          = j.at( "maxCPS" ).get<int>( );
		settings.jitterIntensity = j.at( "jitterIntensity" ).get<int>( );

		json area                 = j.at( "clickArea" );
		settings.clickArea.left   = area.at( "left" ).get<long>( );
		settings.clickArea.top    = area.at( "top" ).get<long>( );
		settings.clickArea.right  = area.at( "right" ).get<long>( );
		settings.clickArea.bottom = area.at( "bottom" ).get<long>( );
	}
	catch ( const json::exception& e )
	{
		return false;
	}
	return true;
}
