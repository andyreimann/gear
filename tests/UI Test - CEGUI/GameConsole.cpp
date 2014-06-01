#include "GameConsole.h"

int GameConsoleWindow::iInstanceNumber;            // Don't forget this declaration
 
GameConsoleWindow::GameConsoleWindow()
{
   m_ConsoleWindow = NULL;       // Always good practice to initialize a pointer to a NULL value, helps when switching to Release Builds.
   iInstanceNumber = 0;
   sNamePrefix = "";
   CreateCEGUIWindow("GameConsole.layout");
   setVisible(false);
   m_bConsole = false;
}

void GameConsoleWindow::CreateCEGUIWindow(std::string const& inLayoutName)
{
	// Get a local pointer to the CEGUI Window Manager, Purely for convenience to reduce typing
	CEGUI::WindowManager *pWindowManager = CEGUI::WindowManager::getSingletonPtr();
 
		// Now before we load anything, lets increase our instance number to ensure no conflicts.  
		// I like the format #_ConsoleRoot so thats how i'm gonna make the prefix.  This simply
		// Increments the iInstanceNumber then puts it + a _ into the sNamePrefix string. 
		sNamePrefix = ++iInstanceNumber + "_";
 
		// Note : for CEGUI 0.8 
		m_ConsoleWindow = pWindowManager->loadLayoutFromFile(inLayoutName);
 
		// Being a good programmer, its a good idea to ensure that we got a valid window back. 
		if (m_ConsoleWindow)
		{
			// Lets add our new window to the Root GUI Window
			CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->addChild(m_ConsoleWindow);
			// Now register the handlers for the events (Clicking, typing, etc)
			(this)->RegisterHandlers();
		}
		else
		{
			// Something bad happened and we didn't successfully create the window lets output the information
			CEGUI::Logger::getSingleton().logEvent("Error: Unable to load the ConsoleWindow from .layout");
		}
}

void GameConsoleWindow::RegisterHandlers()
{
    //// Alright now we need to register the handlers.  We mentioned above we want to acknowledge when the user presses Enter, and 
    //  // when they click the 'Send' button.  So we need to register each of those events
 
    //// First lets register the Send button.  Our buttons name is "ConsoleRoot/SendButton", but don't forget we prepended a name to      
    //  // all the windows which were loaded.  So we need to take that into account here.
    //m_ConsoleWindow->getChild(sNamePrefix + "SendButton")->subscribeEvent(
    //                    CEGUI::PushButton::EventClicked,    // If we recall our button was of type CEGUI::PushButton in the .scheme
    //                                                        // and we want to acknowledge the EventClicked action.
    //                    CEGUI::Event::Subscriber(           // What function to call when this is clicked.  Remember, all functions 
    //                                                        // are contained within (this) class.
    //                    &GameConsoleWindow::Handle_SendButtonPressed,  // Call Handle_SendButtonPressed member of GameConsoleWindow
    //                    this));                             // Using (this) instance we're in right now
 
    //// Now for the TextSubmitted, we will be registering the event on the edit box, which is where the users cursor will be when   
    //  //they press Enter.  I'm not going to break this down as much, because I believe that is very ugly to read, but was a good  
    //  //way of expressing it.  Here is the function call.
    ////m_ConsoleWindow->getChild(sNamePrefix + "EditBox")->subscribeEvent(CEGUI::Editbox::EventTextAccepted,
    ////                    CEGUI::Event::Subscriber(&GameConsoleWindow::Handle_TextSubmitted,this));
}

bool GameConsoleWindow::Handle_TextSubmitted(const CEGUI::EventArgs &e)
{
    //// The following line of code is not really needed in this particular example, but is good to show.  The EventArgs by itself 
    // // only has limited uses. You will find it more useful to cast this to another type of Event.  In this case WindowEventArgs
    // // could be much more useful as we are dealing with a CEGUI::Window.  Notably, this will allow you access to the .window
    // // member of the argument, which will have a pointer to the window which called the event.  You can imagine that would be
    // // useful!
    //const CEGUI::WindowEventArgs* args = static_cast<const CEGUI::WindowEventArgs*>(&e);
 
    //// Now we need to get the text that is in the edit box right now.
    //CEGUI::String Msg = m_ConsoleWindow->getChild(sNamePrefix + "EditBox")->getText();
 
    //// Since we have that string, lets send it to the TextParser which will handle it from here
    //(this)->ParseText(Msg);
 
    //// Now that we've finished with the text, we need to ensure that we clear out the EditBox.  This is what we would expect
    //  // To happen after we press enter
    //m_ConsoleWindow->getChild(sNamePrefix + "EditBox")->setText("");
 
    return true;
}

bool GameConsoleWindow::Handle_SendButtonPressed(const CEGUI::EventArgs &e)
{
    //CEGUI::String Msg = m_ConsoleWindow->getChild(sNamePrefix + "EditBox")->getText();
    //(this)->ParseText(Msg);
    //m_ConsoleWindow->getChild(sNamePrefix + "EditBox")->setText("");
 
    return true;
}

void GameConsoleWindow::ParseText(CEGUI::String inMsg)
{
 //
 //      // I personally like working with std::string. So i'm going to convert it here.
 //      std::string inString = inMsg.c_str();
 //
	//if (inString.length() >= 1) // Be sure we got a string longer than 0
	//{
	//	if (inString.at(0) == '/') // Check if the first letter is a 'command'
	//	{
	//		std::string::size_type commandEnd = inString.find(" ", 1);
	//		std::string command = inString.substr(1, commandEnd - 1);
	//		std::string commandArgs = inString.substr(commandEnd + 1, inString.length() - (commandEnd + 1));
	//		//convert command to lower case
	//		for(std::string::size_type i=0; i < command.length(); i++)
	//		{
	//			command[i] = tolower(command[i]);
	//		}
 //
	//		// Begin processing
 //
	//		if (command == "say")
	//		{
	//			std::string outString = "You:" + inString; // Append our 'name' to the message we'll display in the list
 //                        	OutputText(outString);
	//		}
	//		else if (command == "quit")
	//		{
	//			// do a /quit 
	//		}
	//		else if (command == "help")
	//		{
	//			// do a /help
	//		}
	//		else
	//		{
	//			std::string outString = "<" + inString + "> is an invalid command.";
	//			(this)->OutputText(outString,CEGUI::Colour(1.0f,0.0f,0.0f)); // With red ANGRY colors!
	//		}
	//	} // End if /
	//	else
	//	{
	// (this)->OutputText(inString); // no commands, just output what they wrote
	//	}
	//} 
}

void GameConsoleWindow::OutputText(CEGUI::String inMsg, CEGUI::Colour colour)
{
 //
	//// Get a pointer to the ChatBox so we don't have to use this ugly getChild function everytime.
	//CEGUI::Listbox *outputWindow = static_cast<CEGUI::Listbox*>(m_ConsoleWindow->getChild(sNamePrefix + "ChatBox"));
 //
	//CEGUI::ListboxTextItem* newItem=0; // This will hold the actual text and will be the listbox segment / item
 //
	//newItem = new CEGUI::ListboxTextItem(inMsg); // instance new item
 //       newItem->setTextColours(colour); // Set the text color
	//outputWindow->addItem(newItem); // Add the new ListBoxTextItem to the ListBox
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
    return m_ConsoleWindow->isVisible();
}