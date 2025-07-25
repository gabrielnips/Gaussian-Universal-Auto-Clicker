#pragma once

#include "Interfaces/IConsoleService.hpp"
#include "Interfaces/ITargetWindowService.hpp"
#include "Interfaces/IInputService.hpp"
#include "Interfaces/IClickerService.hpp"

#include "Services/ClickerService.hpp"
#include "Services/ConsoleService.hpp"
#include "Core/AppSettings.hpp"

class Application
{
	AppSettings settings;
	std::string targetWindowTitle;

	std::shared_ptr<IConsoleService> console;
	std::shared_ptr<ITargetWindowService> windowService;
	std::shared_ptr<IInputService> inputService;

	std::unique_ptr<IClickerService> clickerService;
	std::thread clickerThread;

	void InitialSetup( );
	void MainLoop( );
	void CreateAndRunClickerThread( );
	void ExportConfigToClipboard( ) const;
	void ImportConfigFromClipboard( );

	std::string SerializeSettings( ) const;
	bool DeserializeSettings( const std::string& data );

public:
	Application(
		std::shared_ptr<IConsoleService> cs,
		std::shared_ptr<ITargetWindowService> ws,
		std::shared_ptr<IInputService> is );

	~Application( );

	void Start( );
};
