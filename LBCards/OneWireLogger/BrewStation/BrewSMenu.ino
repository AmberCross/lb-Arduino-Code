////////////////////////////////////////////////////////////////////////////////////
// DisplayTestMenu() - Display the Test Menu.
////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////
// Menu structure
////////////////////////////////////////////////////////////////////////////////////

struct menuStruc
{
  int CURRENT_MENU_PTR;
  char menuString[21];
  int rowNumber;
  int UP_MENU_PTR;
  int DOWN_MENU_PTR;
  int LEFT_MENU_PTR;
  int RIGHT_MENU_PTR;
  void (*pt2Function)(void);
  int SEL_MENU_PTR;
};

const char * menuHeader     = "*** Brew Station  ***";
const char * menuFooter     = "";

////////////////////////////////////////////////////////////////////////////////////
// Menu structure - used to generate the navigation menus. Format is
// CURR_MENU,MENU_STRING,DISPLAY_ROW,UP_MENU,DOWN_MENU,LEFT_MENU,RIGHT_MENU,select-execute-fcn-name,RETURN_MENU
// CURR_MENU needs to be ordered in the enums to match the order on the screen
////////////////////////////////////////////////////////////////////////////////////

menuStruc menus[] =
{
  STEEP_MENU,       "Steep",               2, STEEP_MENU,   BOIL_MENU,      STEEP_MENU,    STEEP_MENU,    &steep,       STEEP_MENU,
  BOIL_MENU,        "Boil",                3, STEEP_MENU,   THERMO_MENU,    BOIL_MENU,     BOIL_MENU,     &boil,        BOIL_MENU,
  THERMO_MENU,      "Thermometer",         4, BOIL_MENU,    SERLOG_MENU,    THERMO_MENU,   THERMO_MENU,   &do1Wire,     THERMO_MENU,
  SERLOG_MENU,      "Serial Logging",      5, THERMO_MENU,  SETTIME_MENU,   SERLOG_MENU,   SERLOG_MENU,   &serLog,      SERLOG_MENU,
  SETTIME_MENU,     "Set Time",            6, SERLOG_MENU,  SETBKLT_MENU,   SETTIME_MENU,  SETTIME_MENU,  &setTime,     SETTIME_MENU,
  SETBKLT_MENU,     "Set Backlight Level", 7, SETTIME_MENU, CFG18B20_MENU,  SETBKLT_MENU,  SETBKLT_MENU,  &setBLt,      SETBKLT_MENU,
  CFG18B20_MENU,    "Configure DS18B20",   8, SETBKLT_MENU, CFG18B20_MENU,  CFG18B20_MENU, CFG18B20_MENU, &cfg18b20,    CFG18B20_MENU,
};

////////////////////////////////////////////////////////////////////////////////////
// Refresh the menu
////////////////////////////////////////////////////////////////////////////////////

int menuRefresh(void)
{
  int nextLine, lastLine;
  tft.fillRect(0,0,127,90,ST7735_BLACK);
  setCursorTFT(0,0);
  tft.setTextColor(ST7735_WHITE,ST7735_RED);
  tft.print(menuHeader);
  textWhiteOnBlack();
  setCursorTFT(15,0);
  tft.print(__DATE__);
  tft.print(" ");
  tft.print(__TIME__);
#ifdef SERIAL_DEBUG
  Serial.print(F("menuState = "));
  Serial.print(menuState);
  Serial.print(F(",String="));
  Serial.print(menus[menuState].menuString);
  Serial.print(F(",RowNumber = "));
  Serial.println(menus[menuState].rowNumber);
#endif
  setCursorTFT((menus[menuState].rowNumber)-1,0);
  tft.setTextColor(ST7735_WHITE,ST7735_BLUE);
  tft.print(menus[menuState].menuString);
  textWhiteOnBlack();
  lastLine = menuState;
  while ((menus[lastLine].UP_MENU_PTR != menus[lastLine].CURRENT_MENU_PTR) && (menus[menuState].rowNumber != 1))
  {
    lastLine = menus[lastLine].UP_MENU_PTR;
    setCursorTFT(menus[lastLine].rowNumber-1,0);
    tft.print(menus[lastLine].menuString);
#ifdef SERIAL_DEBUG
    Serial.print(F("UP-1"));
    Serial.println(menus[lastLine].menuString);
#endif
  }
  nextLine = menuState;
  while (menus[nextLine].DOWN_MENU_PTR != menus[nextLine].CURRENT_MENU_PTR)
  {
    nextLine = menus[nextLine].DOWN_MENU_PTR;
    setCursorTFT(menus[nextLine].rowNumber-1,0);
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

int menuNav(void)
{
  signed char keyState;
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

////////////////////////////////////////////////////////////////////////////////////
// displayMenuLine
////////////////////////////////////////////////////////////////////////////////////

void displayMenuLine(int row, int fontColor, int backgroundColor, char * menuString, int selected)
{
  setCursorTFT(row,0);
  tft.setTextColor(fontColor,backgroundColor);
  tft.print(menuString);
  return;
}

//////////////////////////////////////////////////////////////////////////////
// void nullFcn(void)
//////////////////////////////////////////////////////////////////////////////

void nullFcn(void) {  }
