#include <CEGUI/CEGUI.h>
#include <G2Core/Entity.h>
 
class GameConsoleWindow : public G2::Entity
{
    public:
       GameConsoleWindow();                   // Constructor
       void setVisible(bool visible);         // Hide or show the console
       bool isVisible();                      // return true if console is visible, false if is hidden
 
    private:
       void createCEGUIWindow(std::string const& layoutName);                                  // The function which will load in the CEGUI Window and register event handlers
       void registerHandlers();                                   // Register our handler functions
       bool onTextSubmitted(const CEGUI::EventArgs &e);      // Handle when we press Enter after typing
       bool onSendButtonPressed(const CEGUI::EventArgs &e);  // Handle when we press the Send button         
       void parseText(CEGUI::String inMsg);                       // Parse the text the user submitted.
       void outputText(CEGUI::String inMsg,                       // Post the message to the ChatHistory listbox.
              CEGUI::Colour colour = CEGUI::Colour( 0xFFFFFFFF)); //   with a white color default
 
       CEGUI::Window* mConsoleWindow;                            // This will be the prefix name we give the layout
       bool m_bConsole;
};