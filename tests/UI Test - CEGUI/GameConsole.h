#include <CEGUI/CEGUI.h>
 
class GameConsoleWindow
{
    public:
       GameConsoleWindow();                   // Constructor
       void setVisible(bool visible);         // Hide or show the console
       bool isVisible();                      // return true if console is visible, false if is hidden
 
    private:
       void CreateCEGUIWindow(std::string const& layoutName);                                  // The function which will load in the CEGUI Window and register event handlers
       void RegisterHandlers();                                   // Register our handler functions
       bool Handle_TextSubmitted(const CEGUI::EventArgs &e);      // Handle when we press Enter after typing
       bool Handle_SendButtonPressed(const CEGUI::EventArgs &e);  // Handle when we press the Send button         
       void ParseText(CEGUI::String inMsg);                       // Parse the text the user submitted.
       void OutputText(CEGUI::String inMsg,                       // Post the message to the ChatHistory listbox.
              CEGUI::Colour colour = CEGUI::Colour( 0xFFFFFFFF)); //   with a white color default
 
       CEGUI::Window *m_ConsoleWindow;                            // This will be a pointer to the ConsoleRoot window.
       CEGUI::String sNamePrefix;                                  // This will be the prefix name we give the layout
       static int iInstanceNumber;                                 // This will be the instance number for this class.
       bool m_bConsole;
};