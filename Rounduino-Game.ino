/** ===========================================================
 *  example game
 *
 * \file       example_game
 * \author     jh, nk
 * \date       xx.04.2015  
 *
 * \controller ATmega32U4
 * \f_clk      ext. 8MHz
 *
 * \brief      example SW to control the DINGGLABS Rounduino
 *
 * @{
 ============================================================== */

/* Configurable defines --------------------------------------- */
#define MAX_DURATION_SHORTCLICK    1000    // max. duration in ms a SHORTCLICK will be recognized
#define MAX_DURATION_FEEDBACK       500    // max. duration in ms a acustical feedback will be given

/* ADC */
#define V_SCAN_MAX    1023
#define V_SCAN_OFFSET  700
  
/* Includes --------------------------------------------------- */
#include "Rounduino_lib.h"
#include "game.h"
#include <SPI.h>
#include <Wire.h>

/* State machine ---------------------------------------------- */
/* states */
#define STATE_MENU             0
#define STATE_GAME             1
#define STATE_CONFIG           2
#define STATE_CONFIG_SELECTION 3

/* events */
#define EVENT_NONE             0
#define EVENT_SHORTCLICK       1
#define EVENT_LONGCLICK        2

/* General defines -------------------------------------------- */
#define FOREVER      true
#define UP           true
#define DOWN         false

/* Menu position defines -------------------------------------- */
#define MENU_POS_X        	  35
#define MENU_POS_Y1						14
#define MENU_POS_Y2						31
#define MENU_POS_Y3						48
#define MENU_POS_Y4						65
#define MENU_POS_Y5						82
#define MENU_POS_Y6						99

#define CURSOR_CHARACTER		 '>'
#define CURSOR_POS_X (MENU_POS_X - FONTWIDTH)

/* Global variables ------------------------------------------- */
/* defaults state machine */
byte state = STATE_MENU;          // first state

/* menu selection enumerations */
enum Selection1 {PLAY = 0, CONFIG, EXIT};
enum Selection2 {LIVES = 0, PATHS, STEPS, MINIONS, MAX_STEP_TIME, BACK};

/* menu selection variables */
char menuSelection = PLAY;
char configSelection = LIVES;

/* timing reference variables */
unsigned long startButton1 = 0;   // in ms
unsigned long startButton2 = 0;
unsigned long startButton3 = 0;

/* struct for the config values */
struct Config config;

/** ===========================================================
 * \fn      setup
 * \brief   standard Arduino setup-function with all
 *          initializations
 *
 * \param   -
 * \return  -
 ============================================================== */
void setup()
{
//  Serial.begin(115200);
//  while(!Serial);
//  Serial.println("-------------------------");
//  Serial.println("setup");
    
  initRounduino();

  /* init default config values for the Rounduino game */
  config = getDefaultConfigValues();
  
  clearSymbolList();
  clearDisplay();
}

/** ===========================================================
 * \fn      loop
 * \brief   standard Arduino forever loop-function with the
 *          state machine
 *
 * \param   -
 * \return  -
 ============================================================== */
void loop()
{ 
  /* check if a butten has been clicked (short, long or not) */
  byte buttonEvent1 = getButtonEvent1(MAX_DURATION_SHORTCLICK, MAX_DURATION_FEEDBACK);
  byte buttonEvent2 = getButtonEvent2(MAX_DURATION_SHORTCLICK, MAX_DURATION_FEEDBACK);
  byte buttonEvent3 = getButtonEvent3(MAX_DURATION_SHORTCLICK, MAX_DURATION_FEEDBACK);  

  /* state machine -------------------------------------------- */
  switch (state)
  {
    /* STATE 1 .................................................. */
    case STATE_MENU:
    {
      /* draw menu */
      drawMenu(menuSelection);
      drawBattery(80, 10, MAX_BRIGHTNESS);

      /* handle selection */
      if (buttonEvent1 >= EVENT_SHORTCLICK)
      {
        menuSelection++;
        if (menuSelection > EXIT) menuSelection = PLAY;
      }
      if (buttonEvent3 >= EVENT_SHORTCLICK)
      {
        menuSelection--;
        if (menuSelection < 0) menuSelection = EXIT;
      }

      if (buttonEvent2 >= EVENT_SHORTCLICK)
      {
        switch (menuSelection)
        {
          case PLAY: state = STATE_GAME;
          break;

          case CONFIG: state = STATE_CONFIG;
          break;

          case EXIT: 
					{
						menuSelection = PLAY;
						turnOff();
						break;	
					}

          default: error();
        }

        clearDisplay();
      }

      break;
    }
  

    /* STATE 2 .................................................. */
    case STATE_GAME:
    {
      playGame(config);

      /* finished playing */
      state = STATE_MENU;
      clearDisplay();

      break;
    }

    /* STATE 3 .................................................. */
    case STATE_CONFIG:
    {
      /* draw config */
      drawConfig(configSelection);

      /* handle selection */
      if (buttonEvent1 >= EVENT_SHORTCLICK)
      {
        configSelection++;
        if (configSelection > BACK) configSelection = LIVES;
      }
      if (buttonEvent3 >= EVENT_SHORTCLICK)
      {
        configSelection--;
        if (configSelection < LIVES) configSelection = BACK;
      }

      if (buttonEvent2 >= EVENT_SHORTCLICK)
      {
        if (configSelection == BACK)
        {
        	configSelection = LIVES;
        	state = STATE_MENU;
				}
				else state = STATE_CONFIG_SELECTION;

				clearDisplay();
			}

      break;
    }


    /* STATE 4 .................................................. */
    case STATE_CONFIG_SELECTION:
    {
      /* draw config selection */
      drawConfigSelection(configSelection);

      /* handle selection */
      if (buttonEvent1 >= EVENT_SHORTCLICK)
      {
      	switch (configSelection)
      	{
      		case LIVES: if (config.lives < MAX_LIVES) config.lives++; break;
      		case PATHS: if (config.numberOfPaths < MAX_NUMBER_PATHS) config.numberOfPaths++; break;
      		case STEPS: if (config.numberOfSteps < MAX_NUMBER_STEPS) config.numberOfSteps++; break;
      		case MINIONS: if (config.numberOfMinions < MAX_NUMBER_MINIONS) config.numberOfMinions++; break;
      		case MAX_STEP_TIME: if (config.maxStepTime < MAX_SPEED) config.maxStepTime++; break;
      	}
      }
      if (buttonEvent3 >= EVENT_SHORTCLICK)
      {
      	switch (configSelection)
      	{
      		case LIVES: if (config.lives > MIN_LIVES) config.lives--; break;
      		case PATHS: if (config.numberOfPaths > MIN_NUMBER_PATHS) config.numberOfPaths--; break;
      		case STEPS: if (config.numberOfSteps > MIN_NUMBER_STEPS) config.numberOfSteps--; break;
      		case MINIONS: if (config.numberOfMinions > MIN_NUMBER_MINIONS) config.numberOfMinions--; break;
      		case MAX_STEP_TIME: if (config.maxStepTime > MIN_SPEED) config.maxStepTime--; break;
      		default: error();
      	}
      }

      if (buttonEvent2 >= EVENT_SHORTCLICK)
      {
      	state = STATE_CONFIG;
      	clearDisplay();
			}

      break;
    }

    /* DEFAULT .................................................. */
    default: error();
  } 
  /* end of state machine ------------------------------------- */
}

/** ===========================================================
 * \fn      drawMenu
 * \brief   draw menu options and current cursor
 *
 * \param   (char) current menu selection
 * \return  -
 ============================================================== */
void drawMenu(char selection)
{
	byte cy;

  /* draw menu options */
	const unsigned char playStr[]={"Play"};
	const unsigned char configStr[]={"Config"};
	const unsigned char exitStr[]={"Exit"};
	drawString(playStr, MENU_POS_X, MENU_POS_Y3, MAX_BRIGHTNESS);
	drawString(configStr, MENU_POS_X, MENU_POS_Y4, MAX_BRIGHTNESS);
	drawString(exitStr, MENU_POS_X, MENU_POS_Y5, MAX_BRIGHTNESS);
  clearSymbolList();

  /* draw cursor */
  switch (selection)
  {
    case PLAY: cy = MENU_POS_Y3;
    	break;

    case CONFIG: cy = MENU_POS_Y4;
    	break;

    case EXIT: cy = MENU_POS_Y5;
    	break;

    default: error();
  }

  drawCursor(cy);
}

/** ===========================================================
 * \fn      drawConfig
 * \brief   draw config options and current cursor
 *
 * \param   (char) current config selection
 * \return  -
 ============================================================== */
void drawConfig(char selection)
{
	byte cy;

  /* draw menu options */
	const unsigned char livesStr[]={"Lives"};
	const unsigned char pathsStr[]={"Paths"};
	const unsigned char stepsStr[]={"Steps"};
	const unsigned char minionsStr[]={"Minions"};
	const unsigned char speedStr[]={"Speed"};
	const unsigned char backStr[]={"Back"};
	drawString(livesStr, MENU_POS_X, MENU_POS_Y1, MAX_BRIGHTNESS);
	drawString(pathsStr, MENU_POS_X, MENU_POS_Y2, MAX_BRIGHTNESS);
	drawString(stepsStr, MENU_POS_X, MENU_POS_Y3, MAX_BRIGHTNESS);
	drawString(minionsStr, MENU_POS_X, MENU_POS_Y4, MAX_BRIGHTNESS);
	drawString(speedStr, MENU_POS_X, MENU_POS_Y5, MAX_BRIGHTNESS);
	drawString(backStr, MENU_POS_X, MENU_POS_Y6, MAX_BRIGHTNESS);
  clearSymbolList();

  /* draw cursor */
	switch (selection)
  {
    case LIVES: cy = MENU_POS_Y1;
    	break;

    case PATHS: cy = MENU_POS_Y2;
    	break;
      
    case STEPS: cy = MENU_POS_Y3;
    	break;

    case MINIONS: cy = MENU_POS_Y4;
    	break;

    case MAX_STEP_TIME:	cy = MENU_POS_Y5;  // aka speed
    	break;

    case BACK: cy = MENU_POS_Y6;
    	break;

    default: error();
  }

  drawCursor(cy);
}

/** ===========================================================
 * \fn      drawCursor
 * \brief   draw cursor in front of current menu position
 *
 * \param   (byte) y coordinate of current menu position
 * \return  -
 ============================================================== */
void drawCursor(byte cy)
{
	/* clear cursor */
	createCharSymbol(' ', CURSOR_POS_X, MENU_POS_Y1, MAX_BRIGHTNESS);
	createCharSymbol(' ', CURSOR_POS_X, MENU_POS_Y2, MAX_BRIGHTNESS);
	createCharSymbol(' ', CURSOR_POS_X, MENU_POS_Y3, MAX_BRIGHTNESS);
	createCharSymbol(' ', CURSOR_POS_X, MENU_POS_Y4, MAX_BRIGHTNESS);
	createCharSymbol(' ', CURSOR_POS_X, MENU_POS_Y5, MAX_BRIGHTNESS);
	createCharSymbol(' ', CURSOR_POS_X, MENU_POS_Y6, MAX_BRIGHTNESS);

	/* set cursor */
	createCharSymbol(CURSOR_CHARACTER, CURSOR_POS_X, cy, MAX_BRIGHTNESS);
	drawSymbols();
	
	clearSymbolList();
}

/** ===========================================================
 * \fn      drawConfigSelection
 * \brief   draw config selection and it's current value
 *
 * \param   (char) current config selection
 * \return  -
 ============================================================== */
void drawConfigSelection(char selection)
{
	char str[5];

	/* draw '+' and '-' */
	createCharSymbol('+', 83, MENU_POS_Y4, MAX_BRIGHTNESS);
	createCharSymbol('-', 23, MENU_POS_Y4, MAX_BRIGHTNESS);
	drawSymbols();

	switch (selection)
  {
    case LIVES:
    {
			const unsigned char livesStr[]={"Lives"};
			drawString(livesStr, 23, MENU_POS_Y3, MAX_BRIGHTNESS);

			itoa(config.lives, str, 10);
			if (config.lives < 10)
			{
				createCharSymbol('0', 47, MENU_POS_Y4, MAX_BRIGHTNESS);
				drawSymbol(tail);
				drawString((unsigned char*) str, 47+FONTWIDTH-CHAR_SPACING_OFFSET, MENU_POS_Y4, MAX_BRIGHTNESS);
			}
			else drawString((unsigned char*) str, 47, MENU_POS_Y4, MAX_BRIGHTNESS);
			break;
    }

    case PATHS:
    {
			const unsigned char pathsStr[]={"Paths"};
			drawString(pathsStr, 23, MENU_POS_Y3, MAX_BRIGHTNESS);

			itoa(config.numberOfPaths, str, 10);
			if (config.numberOfPaths < 10)
			{
				createCharSymbol('0', 47, MENU_POS_Y4, MAX_BRIGHTNESS);
				drawSymbol(tail);
				drawString((unsigned char*) str, 47+FONTWIDTH-CHAR_SPACING_OFFSET, MENU_POS_Y4, MAX_BRIGHTNESS);
			}
			else drawString((unsigned char*) str, 47, MENU_POS_Y4, MAX_BRIGHTNESS);
			break;
    }
      
    case STEPS:
    {
			const unsigned char stepsStr[]={"Steps"};
			drawString(stepsStr, 23, MENU_POS_Y3, MAX_BRIGHTNESS);
    
			itoa(config.numberOfSteps, str, 10);
			if (config.numberOfSteps < 10)
			{
				createCharSymbol('0', 47, MENU_POS_Y4, MAX_BRIGHTNESS);
				drawSymbol(tail);
				drawString((unsigned char*) str, 47+FONTWIDTH-CHAR_SPACING_OFFSET, MENU_POS_Y4, MAX_BRIGHTNESS);
			}
			else drawString((unsigned char*) str, 47, MENU_POS_Y4, MAX_BRIGHTNESS);
			break;
    }

    case MINIONS:
    {
			const unsigned char minionsStr[]={"Minions"};
			drawString(minionsStr, 23, MENU_POS_Y3, MAX_BRIGHTNESS);

			itoa(config.numberOfMinions, str, 10);
			if (config.numberOfMinions < 10)
			{
				createCharSymbol('0', 47, MENU_POS_Y4, MAX_BRIGHTNESS);
				drawSymbol(tail);
				drawString((unsigned char*) str, 47+FONTWIDTH-CHAR_SPACING_OFFSET, MENU_POS_Y4, MAX_BRIGHTNESS);
			}
			else drawString((unsigned char*) str, 47, MENU_POS_Y4, MAX_BRIGHTNESS);
			break;
    }

    case MAX_STEP_TIME:	// aka speed
    {
			const unsigned char speedStr[]={"Speed"};
			drawString(speedStr, 23, MENU_POS_Y3, MAX_BRIGHTNESS);

			itoa(config.maxStepTime, str, 10);
			if (config.maxStepTime < 10)
			{
				createCharSymbol('0', 47, MENU_POS_Y4, MAX_BRIGHTNESS);
				drawSymbol(tail);
				drawString((unsigned char*) str, 47+FONTWIDTH-CHAR_SPACING_OFFSET, MENU_POS_Y4, MAX_BRIGHTNESS);
			}
			else drawString((unsigned char*) str, 47, MENU_POS_Y4, MAX_BRIGHTNESS);
			break;
    }

    default: error();
  }

  clearSymbolList();
}

/** ===========================================================
 * \fn      getButtonEventX
 * \brief   returns given values on base of current button 
 *          state
 *
 * \requ    bool function: getButtonStateX()
 *          define: EVENT_NONE
 *          define: EVENT_SHORTCLICK
 *          define: EVENT_LONGCLICK
 *
 * \param   (uint) max. duration in ms a short click will be
 *                 recognized
 *          (uint) max. duration in ms an acustical feedback
 *                 will be given
 *
 * \return  (byte) 0 = no event
 *                 1 = short button click
 *                 2 = long button click
 ============================================================== */
byte getButtonEvent1(unsigned int max_duration_shortclick, unsigned int max_duration_feedback)
{
  static boolean button1WasPressed = false;
  byte buttonEvent = EVENT_NONE;
  
  piezoOn = false;

  /* check if pressed */
  if (getButtonState1())
  {
    if (!button1WasPressed)
    {
      startButton1 = millis();  // get reference if first time pressed
      button1WasPressed = true;
    }
    
    /* calculate past time since pushed to give a feedback when a long click have been recognized */
    if ((millis() - startButton1) > max_duration_shortclick &&
        (millis() - startButton1) < (max_duration_feedback + max_duration_shortclick))
    {
      piezoFrequencyDivisor = LOW_PIEZO_FREQUENCY_DIVISOR;
      piezoOn = true;
    }
  }
  /* button have been released */
  else if (button1WasPressed)
  {
    button1WasPressed = false;
    
    /* calculate past time since pushed and set button event */
    if ((millis() - startButton1) <= max_duration_shortclick)
    {
      /* short click: */
      buttonEvent = EVENT_SHORTCLICK;
      piezoFrequencyDivisor = STD_PIEZO_FREQUENCY_DIVISOR;
      piezoOn = true;
    }
    else
    {
      /* long click: */
      buttonEvent = EVENT_LONGCLICK;
    }
  }
  
  return buttonEvent;
}

byte getButtonEvent2(unsigned int max_duration_shortclick, unsigned int max_duration_feedback)
{
  static boolean button2WasPressed = false;
  byte buttonEvent = EVENT_NONE;

  /* check if pressed */
  if (getButtonState2())
  {
    if (!button2WasPressed)
    {
      startButton2 = millis();  // get reference if first time pressed
      button2WasPressed = true;
    }
    
    /* calculate past time since pushed to give a feedback when a long click have been recognized */
    if ((millis() - startButton2) > max_duration_shortclick &&
        (millis() - startButton2) < (max_duration_feedback + max_duration_shortclick))
    {
      piezoFrequencyDivisor = LOW_PIEZO_FREQUENCY_DIVISOR;
      piezoOn = true;
    }
  }
  else if (button2WasPressed)
  {
    button2WasPressed = false;
    
    /* calculate past time since pushed and set button event */
    if ((millis() - startButton2) <= max_duration_shortclick) 
    {
      /* short click: */
      buttonEvent = EVENT_SHORTCLICK;
      piezoFrequencyDivisor = STD_PIEZO_FREQUENCY_DIVISOR;
      piezoOn = true;
    }
    else
    {
      /* long click: */
      buttonEvent = EVENT_LONGCLICK;
    }
  }
  
  return buttonEvent;
}

byte getButtonEvent3(unsigned int max_duration_shortclick, unsigned int max_duration_feedback)
{
  static boolean button3WasPressed = false;
  byte buttonEvent = EVENT_NONE;

  /* check if pressed */
  if (getButtonState3())
  {
    if (!button3WasPressed)
    {
      startButton3 = millis();  // get reference if first time pressed
      button3WasPressed = true;
    }
    
    /* calculate past time since pushed to give a feedback when a long click have been recognized */
    if ((millis() - startButton3) > max_duration_shortclick &&
        (millis() - startButton3) < (max_duration_feedback + max_duration_shortclick))
    {
      piezoFrequencyDivisor = LOW_PIEZO_FREQUENCY_DIVISOR;
      piezoOn = true;
    }
  }
  else if (button3WasPressed)
  {
    button3WasPressed = false;
    
    /* calculate past time since pushed and set button event */
    if ((millis() - startButton3) <= max_duration_shortclick) 
    {
      /* short click: */
      buttonEvent = EVENT_SHORTCLICK;
      piezoFrequencyDivisor = STD_PIEZO_FREQUENCY_DIVISOR;
      piezoOn = true;
    }
    else
    {
      /* long click: */
      buttonEvent = EVENT_LONGCLICK;
    }
  }
  
  return buttonEvent;
}

/** ===========================================================
 * \fn      drawBattery
 * \brief   draws the battery symbol on given coordinates and
 *          brightness
 *          this function have to be called from time to time
 *          to update current battery state
 *          (resets the buffer)
 *
 * \param   (byte) x and y coordinate of the battery symbol
 *          (byte) brightness of the battery symbol
 *
 * \return  -
 ============================================================== */
void drawBattery(byte x, byte y, byte b)
{
  clearCustomSymbol();
  
  addRectangle(0, 0, 15, 7);  
  addVLine(15, 2, 3);
  addVLine(16, 2, 3);
  
  /* get battery voltage and draw battery content */
  unsigned int v = (readBatteryVoltage() - V_SCAN_OFFSET);
  addFilledRectangle(1, 1, 13 * v / (V_SCAN_MAX - V_SCAN_OFFSET), 5);

  createCustomSymbol(x, y, b);
  drawCustomSymbol();
  
  clearCustomSymbol();
  clearSymbolList();
}

/** ===========================================================
 * \fn      error
 * \brief   displays an error-message and hangs up the program
 *
 * \param   -
 * \return  -
 ============================================================== */
void error()
{
  const unsigned char errorStr[] = {"ERROR"};

  clearSymbolList();
  clearDisplay();
  
  while (FOREVER)
  {
    drawString(errorStr, 23, 53, MAX_BRIGHTNESS/2);
  }
}

/**
 * @}
 */
