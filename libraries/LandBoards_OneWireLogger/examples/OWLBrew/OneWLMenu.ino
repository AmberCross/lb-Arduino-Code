////////////////////////////////////////////////////////////////////////////////////
// OneWLMenu() - Display the One Wire Menu.
// Hardware required are five buttons (Up, Down, Left, Right and Select) plus an LCD.
// The five buttons can be a 5-way switch.
// The menu operations are typically to have up/down move through the displayed items.
// Right or select can call submenus.
// Left can return to the previous menu.
// Select can execute the selected item.
// Use left and right arrows to indicate submenu availability.
// This menu system is fairly lightweight and is strongly geared towards LCDs.
// This menu system allows for submenus to be called from the first menu.
// There are corresponding ENUM values for each of the xxx_MENU_PTR values.
// The ENUMs need to be ordered in the same order as they appear in the menu structure.
////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////
// Menu structure - The format of the menu system
// Memory size could be improved if the menuString was variable size and 
//  used pointers rather than a fixed array but that would increase complexity.
////////////////////////////////////////////////////////////////////////////////////

struct menuStruc                  // The ENUMs for the *_MENU_PTR values must preceed this
{
  uint8_t CURRENT_MENU_PTR;          // The current menu item
  char    menuString[LCD_COLUMNS+1]; // The string that gets displayed
  uint8_t rowNumber;                 // The row that the string gets displayed on the LCD
  uint8_t UP_MENU_PTR;               // What menu gets highlighted when the up arrow is pressed
  uint8_t DOWN_MENU_PTR;             // What menu gets highlighted when the down arrow is pressed
  uint8_t LEFT_MENU_PTR;             // What menu gets highlighted when the left arrow is pressed
  uint8_t RIGHT_MENU_PTR;            // What menu gets highlighted when the right arrow is pressed
  void (*pt2Function)(void);      // Name of the function which gets called when select is pressed
  uint8_t SEL_MENU_PTR;              // Menu to display after the selected function complets
};

const char * menuFooter;

////////////////////////////////////////////////////////////////////////////////////
// Menu structure - used to generate the navigation menus. Format is
// CURR_MENU,MENU_STRING,DISPLAY_ROW,UP_MENU,DOWN_MENU,LEFT_MENU,RIGHT_MENU,select-execute-fcn-name,RETURN_MENU
// CURR_MENU needs to be ordered in the enums to match the order on the screen
////////////////////////////////////////////////////////////////////////////////////

menuStruc menus[] = 
{
  STEEP_MENU,      "Steep Temps",         2, STEEP_MENU,      BOIL_MENU,        STEEP_MENU,      STEEP_MENU,     &steepTemps,    STEEP_MENU,
  BOIL_MENU,       "Boil Temps",          3, STEEP_MENU,      INITOW_MENU,      BOIL_MENU,       BOIL_MENU,      &boilTemps,     BOIL_MENU,  
  INITOW_MENU,     "Initialize Limits",   4, BOIL_MENU,       INITOW_MENU,      INITOW_MENU,     INITOW_MENU,    &InitLimits,    INITOW_MENU,
};

////////////////////////////////////////////////////////////////////////////////////
// Refresh the menu 
// The special thing about this is that it prints the lines above the selected line first
// then it prints the lines below the selected lines. For some reason this feels natural.
////////////////////////////////////////////////////////////////////////////////////

void menuRefresh(void)
{
  uint8_t nextLine, lastLine;
  clearDisplay();
  setBannerTextColor();                // Sets color of the banner at the top line of the screen
  tft.print("* Brew Thermomether *");  // Banner string is always reprinted with every refresh
  setUnselectedTextColor();            // Return to "normal" text color
  setDisplayCursor(TFT_HEIGHT-1,0);    // Set the cursor to the bottom line of the screen
  tft.print(__DATE__);                 // Print the code compilation date and time
  tft.print(" ");
  tft.print(__TIME__);
// The following lines are helpful to debug the menu but use serial port resources so beware
#ifdef SERIAL_DEBUG
  Serial.print(F("menuState = "));     // Menu state helps for debugging if the menu is stuck
  Serial.print(menuState);
  Serial.print(F(",String="));
  Serial.print(menus[menuState].menuString);
  Serial.print(F(",RowNumber = "));
  Serial.println(menus[menuState].rowNumber);
#endif
// Print the currently selected line
  setDisplayCursor((menus[menuState].rowNumber)-1,0);
  setSelectedTextColor();    
  tft.print(menus[menuState].menuString);     
// Return to "normal" text color
  setUnselectedTextColor();
  lastLine = menuState;
  // display the lines above the selected line first
  while ((menus[lastLine].UP_MENU_PTR != menus[lastLine].CURRENT_MENU_PTR) && (menus[menuState].rowNumber != 1))
  {
    lastLine = menus[lastLine].UP_MENU_PTR;
    setDisplayCursor(menus[lastLine].rowNumber-1,0);
    tft.print(menus[lastLine].menuString);
#ifdef SERIAL_DEBUG
    Serial.print(F("UP-1"));
    Serial.println(menus[lastLine].menuString);
#endif
  }
  nextLine = menuState;
  // next display the lines below the selected line
  while (menus[nextLine].DOWN_MENU_PTR != menus[nextLine].CURRENT_MENU_PTR)
  {
    nextLine = menus[nextLine].DOWN_MENU_PTR;
    setDisplayCursor(menus[nextLine].rowNumber-1,0);
    tft.print(menus[nextLine].menuString);
#ifdef SERIAL_DEBUG
    Serial.print(F("Down,"));
    Serial.print(menus[nextLine].menuString);
    Serial.print(F(",nextLine="));
    Serial.println(nextLine);
#endif
  }
}

////////////////////////////////////////////////////////////////////////////////////
// menuNav - Uses the joystick switch to move around the menu
////////////////////////////////////////////////////////////////////////////////////

void menuNav(void)
{
  uint8_t keyState;
#ifdef SERIAL_OUT
  Serial.println("Waiting for a keypress");
#endif
  keyState = myOneWireLogger.waitKeyPressed();
#ifdef SERIAL_OUT
  Serial.print("menuNav: got");
  Serial.println(keyState);
#endif
  switch(keyState)
  {
  case UP:
    menuState = menus[menuState].UP_MENU_PTR;
    break;
  case DOWN:
    menuState = menus[menuState].DOWN_MENU_PTR;
    break;
  case LEFT:
    menuState = menus[menuState].LEFT_MENU_PTR;
    break;
  case RIGHT:
    menuState = menus[menuState].RIGHT_MENU_PTR;
    break;
  case SELECT:
    // run the selected function
    void (*foo)(void);
    foo = menus[menuState].pt2Function;
    foo();
    menuState = menus[menuState].SEL_MENU_PTR;
    break;
  }
}

//////////////////////////////////////////////////////////////////////////////
// void nullFcn(void)
//////////////////////////////////////////////////////////////////////////////

void nullFcn(void) {  }

