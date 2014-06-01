#include "GameConsole.h"
#include <G2Cegui/CeguiComponent.h>
 
GameConsoleWindow::GameConsoleWindow()
{
   mConsoleWindow = NULL;       // Always good practice to initialize a pointer to a NULL value, helps when switching to Release Builds.
   createCEGUIWindow("GameConsole.layout");
   setVisible(false);
   m_bConsole = false;
}

void GameConsoleWindow::createCEGUIWindow(std::string const& inLayoutName)
{ 
	auto* component = addComponent<G2::UI::CeguiComponent>(inLayoutName);
	mConsoleWindow = component->getWindow();
	// Being a good programmer, its a good idea to ensure that we got a valid window back. 
	if (mConsoleWindow)
	{
		// Lets add our new window to the Root GUI Window
		CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->addChild(mConsoleWindow);
		// Now register the handlers for the events (Clicking, typing, etc)
		registerHandlers();
	}
	else
	{
		// Something bad happened and we didn't successfully create the window lets output the information
		CEGUI::Logger::getSingleton().logEvent("Error: Unable to load the ConsoleWindow from .layout");
	}
}

void GameConsoleWindow::registerHandlers()
{
	// Alright now we need to register the handlers.  We mentioned above we want to acknowledge when the user presses Enter, and 
	// when they click the 'Send' button.  So we need to register each of those events.
	mConsoleWindow->getChild("SendButton")->subscribeEvent(
						CEGUI::PushButton::EventClicked,
						CEGUI::Event::Subscriber(
							&GameConsoleWindow::onSendButtonPressed,
							this
						)
	);
	mConsoleWindow->getChild("EditBox")->subscribeEvent(
		CEGUI::Editbox::EventTextAccepted,
		CEGUI::Event::Subscriber(
			&GameConsoleWindow::onTextSubmitted,
			this
		)
	);
}

bool GameConsoleWindow::onTextSubmitted(const CEGUI::EventArgs &e)
{
	// The following line of code is not really needed in this particular example, but is good to show.  The EventArgs by itself 
	// only has limited uses. You will find it more useful to cast this to another type of Event.  In this case WindowEventArgs
	// could be much more useful as we are dealing with a CEGUI::Window.  Notably, this will allow you access to the .window
	// member of the argument, which will have a pointer to the window which called the event.  You can imagine that would be
	// useful!
	const CEGUI::WindowEventArgs* args = static_cast<const CEGUI::WindowEventArgs*>(&e);
 
	// Now we need to get the text that is in the edit box right now.
	CEGUI::String Msg = mConsoleWindow->getChild("EditBox")->getText();
 
	// Since we have that string, lets send it to the TextParser which will handle it from here
	parseText(Msg);
 
	// Now that we've finished with the text, we need to ensure that we clear out the EditBox.  This is what we would expect
	// To happen after we press enter
	mConsoleWindow->getChild("EditBox")->setText("");
 
	return true;
}

bool GameConsoleWindow::onSendButtonPressed(const CEGUI::EventArgs &e)
{
	CEGUI::String Msg = mConsoleWindow->getChild("EditBox")->getText();
	parseText(Msg);
	mConsoleWindow->getChild("EditBox")->setText("");
 
	return true;
}

void GameConsoleWindow::parseText(CEGUI::String inMsg)
{
 
	// I personally like working with std::string. So i'm going to convert it here.
	std::string inString = inMsg.c_str();
	if (inString.length() >= 1) // Be sure we got a string longer than 0
	{
		if (inString.at(0) == '/') // Check if the first letter is a 'command'
		{
			std::string::size_type commandEnd = inString.find(" ", 1);
			std::string command = inString.substr(1, commandEnd - 1);
			std::string commandArgs = inString.substr(commandEnd + 1, inString.length() - (commandEnd + 1));
			//convert command to lower case
			for(std::string::size_type i=0; i < command.length(); i++)
			{
				command[i] = tolower(command[i]);
			}
			// Begin processing
			if (command == "say")
			{
				std::string outString = "You:" + inString; // Append our 'name' to the message we'll display in the list
				outputText(outString);
			}
			else if (command == "quit")
			{
				// do a /quit 
			}
			else if (command == "help")
			{
				// do a /help
			}
			else
			{
				std::string outString = "<" + inString + "> is an invalid command.";
				outputText(outString,CEGUI::Colour(1.0f,0.0f,0.0f)); // With red ANGRY colors!
			}
		} // End if /
		else
		{
			outputText(inString); // no commands, just output what they wrote
		}
	} 
}

void GameConsoleWindow::outputText(CEGUI::String inMsg, CEGUI::Colour colour)
{
	// Get a pointer to the ChatBox so we don't have to use this ugly getChild function everytime.
	CEGUI::Listbox *outputWindow = static_cast<CEGUI::Listbox*>(mConsoleWindow->getChild("ChatBox"));
 
	CEGUI::ListboxTextItem* newItem=0; // This will hold the actual text and will be the listbox segment / item
 
	newItem = new CEGUI::ListboxTextItem(inMsg); // instance new item
	newItem->setTextColours(colour); // Set the text color
	outputWindow->addItem(newItem); // Add the new ListBoxTextItem to the ListBox
}

void GameConsoleWindow::setVisible(bool visible)
{
	//m_ConsoleWindow->setVisible(visible);
	//m_bConsole = visible;
 
	//CEGUI::Window* editBox = m_ConsoleWindow->getChild("EditBox");
	//if(visible)
	//   editBox->activate();
	//else
	//   editBox->deactivate();
}
 
bool GameConsoleWindow::isVisible()
{
	return mConsoleWindow->isVisible();
}