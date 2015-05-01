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
#define STATE_MENU           0
#define STATE_GAME           1
#define STATE_CONFIG         2

/* events */
#define EVENT_NONE           0
#define EVENT_SHORTCLICK     1
#define EVENT_LONGCLICK      2

/* General defines -------------------------------------------- */
#define FOREVER      true
#define UP           true
#define DOWN         false

/* Global variables ------------------------------------------- */
/* menu selection enumerations */
enum Selection1 {PLAY = 0, CONFIG, EXIT};
enum Selection2 {LIVES = 0, PATHS, STEPS, STEP_FREQUENCY, BACK};

/* defaults state machine */
byte state = STATE_MENU;          // first state

/* timing reference variables */
unsigned long startSecond = 0;    // in ms
unsigned long startButton1 = 0;
unsigned long startButton2 = 0;
unsigned long startButton3 = 0;

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
      static char menuSelection = PLAY;

      if (buttonEvent1 >= EVENT_SHORTCLICK)
      {
        menuSelection--;
        if (menuSelection < 0) menuSelection = EXIT;
      }
      if (buttonEvent3 >= EVENT_SHORTCLICK)
      {
        menuSelection++;
        if (menuSelection > EXIT) menuSelection = PLAY;
      }

      if (buttonEvent2 >= EVENT_SHORTCLICK)
      {
        switch (menuSelection)
        {
          case PLAY: state = STATE_GAME;
          break;

          case CONFIG: state = STATE_CONFIG;
          break;

          case EXIT: turnOff();
          break;

          default: error();
        }
      }

      /* draw menu */
      drawMenu(menuSelection);
      drawBattery(80, 10, MAX_BRIGHTNESS);

      break;
    }
  

    /* STATE 2 .................................................. */
    case STATE_GAME:
    {
      playGame();

      break;
    }

    /* STATE 3 .................................................. */
    case STATE_CONFIG:
    {
      static char configSelection = LIVES;

      if (buttonEvent1 >= EVENT_SHORTCLICK)
      {
        configSelection--;
        if (configSelection < LIVES) configSelection = BACK;
      }
      if (buttonEvent3 >= EVENT_SHORTCLICK)
      {
        configSelection++;
        if (configSelection > BACK) configSelection = LIVES;
      }

      if (buttonEvent2 >= EVENT_SHORTCLICK)
      {
        switch (configSelection)
        {
          case LIVES:
          {


            break;
          }

          case PATHS:
          {


            break;
          }
            
          case STEPS:
          {


            break;
          }

          case STEP_FREQUENCY:
          {


            break;
          }

          case BACK: state = STATE_MENU;
          break;

          default: error();
        }
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
 * \param   (int) current menu selection
 * \return  -
 ============================================================== */
void drawMenu(int selection)
{
  /* draw menu options */

  /* draw cursor */
  switch (selection)
  {
    case PLAY: 
    break;

    case CONFIG: 
    break;

    case EXIT: 
    break;

    default: error();
  }
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
  
  drawRectangle(0, 0, 15, 7);  
  drawVLine(15, 2, 3);
  drawVLine(16, 2, 3);
  
  /* get battery voltage and draw battery content */
  unsigned int v = (readBatteryVoltage() - V_SCAN_OFFSET);
  drawFilledRectangle(1, 1, 13 * v / (V_SCAN_MAX - V_SCAN_OFFSET), 5);

  setCustomSymbol(x, y, b);
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
  const char str[] = {"ERROR"};

  clearSymbolList();
  clearDisplay();
  
  while (FOREVER)
  {
    drawString(str, 23, 53, MAX_BRIGHTNESS/2);
  }
}

/**
 * @}
 */
