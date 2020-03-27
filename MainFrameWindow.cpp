#include <MathUtils.hpp>
#include "MainFrameWindow.hpp"
#include "MainApplication.hpp"
#include "RobotWorldCanvas.hpp"
#include "LogTextCtrl.hpp"
#include "WidgetDebugTraceFunction.hpp"
#include "StdOutDebugTraceFunction.hpp"
#include "Button.hpp"
#include "RobotWorld.hpp"
#include "Robot.hpp"
#include "Shape2DUtils.hpp"
#include <iostream>
#include "Thread.hpp"
#include "Logger.hpp"
#include "Client.hpp"
#include "Message.hpp"
#include "RobotWorld.hpp"
#include <string>

namespace Application
{
/**
 * IDs for the controls and the menu commands
 * If there are (default) wxWidget ID's: try to maintain
 * compatibility, especially wxID_ABOUT because on a Mac it is special
 */
enum
{
	ID_QUIT = wxID_EXIT,         	//!< ID_QUIT
	ID_OPTIONS = wxID_PROPERTIES,		//!< ID_OPTIONS
	ID_ABOUT = wxID_ABOUT,        	//!< ID_ABOUT
	ID_WIDGET_DEBUG_TRACE_FUNCTION, 	//!< ID_WIDGET_DEBUG_TRACE_FUNCTION
	ID_STDCOUT_DEBUG_TRACE_FUNCTION 	//!< ID_STDCOUT_DEBUG_TRACE_FUNCTION

};
/**
 *
 */
MainFrameWindow::MainFrameWindow(const std::string &aTitle) :
		Frame(nullptr, DEFAULT_ID, WXSTRING(aTitle), DefaultPosition,
				Size(500, 400)), clientPanel(nullptr), menuBar(nullptr), splitterWindow(
				nullptr), lhsPanel(nullptr), robotWorldCanvas(nullptr), rhsPanel(
				nullptr), logTextCtrl(nullptr), buttonPanel(nullptr), debugTraceFunction(
				nullptr)
{
	initialise();
}
/**
 *
 */
MainFrameWindow::~MainFrameWindow()
{
	if (debugTraceFunction)
	{
		delete debugTraceFunction;
	}
}
/**
 *
 */
Base::DebugTraceFunction& MainFrameWindow::getTraceFunction() const
{
	if (debugTraceFunction)
	{
		return *debugTraceFunction;
	}
	throw std::runtime_error(
			__PRETTY_FUNCTION__ + std::string(": no such debugTraceFunction"));
}
/**
 *
 */
void MainFrameWindow::initialise()
{
	SetMenuBar(initialiseMenuBar());

	GridBagSizer *sizer = new GridBagSizer(5, 5);

	sizer->Add(initialiseClientPanel(), GBPosition(0, 0), 	// row ,col
	GBSpan(1, 1), 		// row ,col
	EXPAND);

	sizer->AddGrowableCol(0);
	sizer->AddGrowableRow(0);

	sizer->SetSizeHints(this);
	SetMinSize(wxSize(500, 350));

	Bind(wxEVT_COMMAND_MENU_SELECTED, [this](CommandEvent &anEvent)
	{	this->OnQuit(anEvent);}, ID_QUIT);
	Bind(wxEVT_COMMAND_MENU_SELECTED, [this](CommandEvent &anEvent)
	{	this->OnWidgetDebugTraceFunction(anEvent);},
			ID_WIDGET_DEBUG_TRACE_FUNCTION);
	Bind(wxEVT_COMMAND_MENU_SELECTED, [this](CommandEvent &anEvent)
	{	this->OnStdOutDebugTraceFunction(anEvent);},
			ID_STDCOUT_DEBUG_TRACE_FUNCTION);
	Bind(wxEVT_COMMAND_MENU_SELECTED, [this](CommandEvent &anEvent)
	{	this->OnAbout(anEvent);}, ID_ABOUT);

	// By default we initialise the WidgetDebugTraceFunction
	// as we expect that this is what the user wants....
	debugTraceFunction = new Application::WidgetDebugTraceFunction(logTextCtrl);
}
/**
 *
 */
MenuBar* MainFrameWindow::initialiseMenuBar()
{
	Menu *fileMenu = new Menu;
	fileMenu->Append(ID_QUIT, WXSTRING("E&xit\tAlt-X"),
			WXSTRING("Exit the application"));

	Menu *debugMenu = new Menu;
	debugMenu->AppendRadioItem(ID_WIDGET_DEBUG_TRACE_FUNCTION,
			WXSTRING("Widget"), WXSTRING("Widget"));
	debugMenu->AppendRadioItem(ID_STDCOUT_DEBUG_TRACE_FUNCTION,
			WXSTRING("StdOut"), WXSTRING("StdOut"));

	Menu *helpMenu = new Menu;
	helpMenu->Append(ID_ABOUT, WXSTRING("&About...\tF1"),
			WXSTRING("Show about dialog"));

	menuBar = new wxMenuBar;
	menuBar->Append(fileMenu, WXSTRING("&File"));
	menuBar->Append(debugMenu, WXSTRING("&Debug"));
	menuBar->Append(helpMenu, WXSTRING("&Help"));

	return menuBar;
}
/**
 *
 */
Panel* MainFrameWindow::initialiseClientPanel()
{
	if (!clientPanel)
	{
		clientPanel = new Panel(this, DEFAULT_ID);

		GridBagSizer *sizer = new GridBagSizer();

		sizer->Add(5, 5, GBPosition(0, 0));

		sizer->Add(initialiseSplitterWindow(), GBPosition(1, 1), GBSpan(1, 1),
				EXPAND);
		sizer->AddGrowableRow(1);
		sizer->AddGrowableCol(1);

		sizer->Add(5, 5, GBPosition(2, 2));

		clientPanel->SetSizer(sizer);
		sizer->SetSizeHints(clientPanel);
	}
	return clientPanel;
}
/**
 *
 */
SplitterWindow* MainFrameWindow::initialiseSplitterWindow()
{
	if (!splitterWindow)
	{
		GridBagSizer *sizer = new GridBagSizer();

		splitterWindow = new SplitterWindow(clientPanel, DEFAULT_ID);

		splitterWindow->SplitVertically(initialiseLhsPanel(),
				initialiseRhsPanel());

		sizer->Add(5, 5, GBPosition(0, 0));

		sizer->Add(splitterWindow, GBPosition(1, 1), GBSpan(1, 1), EXPAND);
		sizer->AddGrowableRow(1);
		sizer->AddGrowableCol(1);

		sizer->Add(5, 5, GBPosition(2, 2));

		splitterWindow->SetSizer(sizer);
		sizer->SetSizeHints(splitterWindow);

	}
	return splitterWindow;
}
/**
 *
 */
Panel* MainFrameWindow::initialiseLhsPanel()
{
	if (!lhsPanel)
	{
		lhsPanel = new Panel(splitterWindow, DEFAULT_ID);

		GridBagSizer *sizer = new GridBagSizer();
		sizer->Add(5, 5, GBPosition(0, 0), GBSpan(1, 1), EXPAND);

		sizer->Add(robotWorldCanvas = new View::RobotWorldCanvas(lhsPanel),
				GBPosition(1, 1), GBSpan(1, 1), EXPAND);
		sizer->AddGrowableCol(1);
		sizer->AddGrowableRow(1);

		sizer->Add(5, 5, GBPosition(2, 2), GBSpan(1, 1), EXPAND);

		lhsPanel->SetSizer(sizer);
		sizer->SetSizeHints(lhsPanel);
	}
	return lhsPanel;
}
/**
 *
 */
Panel* MainFrameWindow::initialiseRhsPanel()
{
	if (!rhsPanel)
	{
		rhsPanel = new Panel(splitterWindow, DEFAULT_ID);

		GridBagSizer *sizer = new GridBagSizer();
		sizer->Add(5, 5, GBPosition(0, 0), GBSpan(1, 1), EXPAND);

		sizer->Add(
				logTextCtrl = new LogTextCtrl(rhsPanel, DEFAULT_ID,
						wxTE_MULTILINE | wxTE_DONTWRAP), GBPosition(1, 1),
				GBSpan(1, 1), EXPAND);
		sizer->AddGrowableCol(1);
		sizer->AddGrowableRow(1);
		logTextCtrl->SetMinSize(Size(500, 300));

		sizer->Add(buttonPanel = initialiseButtonPanel(), GBPosition(2, 1),
				GBSpan(1, 1), SHRINK);
		sizer->AddGrowableRow(2);

		sizer->Add(5, 5, GBPosition(2, 2), GBSpan(1, 1), EXPAND);

		rhsPanel->SetSizer(sizer);
		sizer->SetSizeHints(rhsPanel);
	}
	return rhsPanel;
}
/**
 *
 */
Panel* MainFrameWindow::initialiseButtonPanel()
{
	Panel *panel = new Panel(rhsPanel);

	GridBagSizer *sizer = new GridBagSizer();

	sizer->Add(makeButton(panel, "Populate", [this](CommandEvent &anEvent)
	{	this->OnPopulate(anEvent);}), GBPosition(0, 0), GBSpan(1, 1), EXPAND);
	sizer->Add(makeButton(panel, "Merge", [this](CommandEvent &anEvent)
	{	this->onMerge(anEvent);}), GBPosition(0, 2), GBSpan(1, 1), EXPAND);
	sizer->Add(makeButton(panel, "Unpopulate", [this](CommandEvent &anEvent)
	{	this->OnUnpopulate(anEvent);}), GBPosition(0, 1), GBSpan(1, 1), EXPAND);

	sizer->Add(makeButton(panel, "Start robot", [this](CommandEvent &anEvent)
	{	this->OnStartRobot(anEvent);}), GBPosition(1, 0), GBSpan(1, 1), EXPAND);
	sizer->Add(makeButton(panel, "Stop robot", [this](CommandEvent &anEvent)
	{	this->OnStopRobot(anEvent);}), GBPosition(1, 1), GBSpan(1, 1), EXPAND);

	sizer->Add(
			makeButton(panel, "Start listening", [this](CommandEvent &anEvent)
			{	this->OnStartListening(anEvent);}), GBPosition(2, 0),
			GBSpan(1, 1), EXPAND);
	sizer->Add(makeButton(panel, "Send message", [this](CommandEvent &anEvent)
	{	this->OnSendMessage(anEvent);}), GBPosition(2, 1), GBSpan(1, 1),
			EXPAND);
	sizer->Add(makeButton(panel, "Stop listening", [this](CommandEvent &anEvent)
	{	this->OnStopListening(anEvent);}), GBPosition(2, 2), GBSpan(1, 1),
			EXPAND);

	panel->SetSizerAndFit(sizer);

	return panel;
}
/**
 *
 */
void MainFrameWindow::OnQuit(CommandEvent&UNUSEDPARAM(anEvent))
{
	Close(true);
}
/**
 *
 */
void MainFrameWindow::OnWidgetDebugTraceFunction(
		CommandEvent&UNUSEDPARAM(anEvent))
{
	if (debugTraceFunction)
		delete debugTraceFunction;
	debugTraceFunction = new WidgetDebugTraceFunction(logTextCtrl);
}
/**
 *
 */
void MainFrameWindow::OnStdOutDebugTraceFunction(
		CommandEvent&UNUSEDPARAM(anEvent))
{
	if (debugTraceFunction)
		delete debugTraceFunction;
	debugTraceFunction = new Base::StdOutDebugTraceFunction();

}

/**
 *
 */
void MainFrameWindow::OnAbout(CommandEvent&UNUSEDPARAM(anEvent))
{
	wxMessageBox(WXSTRING("OOSE 2016 RobotWorld.\n"),
			WXSTRING("About RobotWorld"), wxOK | wxICON_INFORMATION, this);
}
/**
 *
 */
void MainFrameWindow::OnStartRobot(CommandEvent&UNUSEDPARAM(anEvent))
{
	Logger::log("Attempting to start Robot...");
	unsigned short RobotID = std::stoul(
			Application::MainApplication::getArg("-robot").value);

	Model::RobotPtr robot;
	if (RobotID == 1)
	{

		robot = Model::RobotWorld::getRobotWorld().getRobot("player1");
	}
	else if (RobotID == 2)
	{
		robot = Model::RobotWorld::getRobotWorld().getRobot("player2");
	}
	if (robot && !robot->isActing())
	{
		Application::Logger::log("start robot 1");
		robot->startActing();
	}
}
/**
 *
 */
void MainFrameWindow::OnStopRobot(CommandEvent&UNUSEDPARAM(anEvent))
{
	Logger::log("Attempting to stop Robot...");
	unsigned short RobotId = std::stoul(
			Application::MainApplication::getArg("-robot").value);
	Model::RobotPtr robot = Model::RobotWorld::getRobotWorld().getRobot(
			RobotId);
	if (robot && robot->isActing())
	{
		robot->stopActing();
	}
}
/**
 *
 */
void MainFrameWindow::OnPopulate(CommandEvent&UNUSEDPARAM(anEvent))
{
	robotWorldCanvas->populate(2);
}
/**
 *
 */
void MainFrameWindow::OnUnpopulate(CommandEvent&UNUSEDPARAM(anEvent))
{
	robotWorldCanvas->unpopulate();
}
/**
 *
 */
void MainFrameWindow::OnStartListening(CommandEvent&UNUSEDPARAM(anEvent))
{
	unsigned short RobotId = std::stoul(
			Application::MainApplication::getArg("-robot").value);
	Model::RobotPtr robot = Model::RobotWorld::getRobotWorld().getRobot(
			RobotId);
	if (robot)
	{
		robot->startCommunicating();
	}
}
/**
 *
 */
void MainFrameWindow::OnSendMessage(CommandEvent&UNUSEDPARAM(anEvent))
{
	Application::Logger::log("trying to send message");
	unsigned short RobotId = std::stoul(
			Application::MainApplication::getArg("-robot").value);
	Model::RobotPtr robot = Model::RobotWorld::getRobotWorld().getRobot(
			RobotId);
	if (robot)
	{
		std::string remoteIpAdres = "localhost";
		std::string remotePort = "12345";

		if (MainApplication::isArgGiven("-remote_ip"))
		{
			remoteIpAdres = MainApplication::getArg("-remote_ip").value;
		}
		if (MainApplication::isArgGiven("-remote_port"))
		{
			remotePort = MainApplication::getArg("-remote_port").value;
		}

		Application::Logger::log(remotePort);

		// We will request an echo message. The response will be "Hello World", if all goes OK,
		// "Goodbye cruel world!" if something went wrong.
		Messaging::Client c1ient(remoteIpAdres, remotePort, robot);
		Messaging::Message message(Model::Robot::MessageType::EchoRequest,
				"Hello world!");
		Application::Logger::log("trying to send message");
		c1ient.dispatchMessage(message);
	}
}
/**
 *
 */
void MainFrameWindow::OnStopListening(CommandEvent&UNUSEDPARAM(anEvent))
{
	unsigned short RobotId = std::stoul(
			Application::MainApplication::getArg("-robot").value);
	Model::RobotPtr robot = Model::RobotWorld::getRobotWorld().getRobot(
			RobotId);
	if (robot)
	{
		robot->stopCommunicating();
	}
}

void MainFrameWindow::onMerge(CommandEvent &anEvent)
{
	Model::RobotWorld::getRobotWorld().sendWalls(Model::Robot::MessageType::MergeRequest);
}

} // namespace Application
