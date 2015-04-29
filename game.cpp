#ifndef __GAME_H_
#define __GAME_H_

/** ===========================================================
 *  example game
 *
 * \file       game
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
 
/* Imports ---------------------------------------------------- */

#include "game.h"
#include "graphics.h"

/* Module constant declaration -------------------------------- */

/* Module type declaration ------------------------------------ */

/* Module data declaration ------------------------------------ */

byte numberOfPaths = 12;          /*!< Num. of total paths      */
byte numberOfSteps = 10;          /*!< Num. of steps per path   */
byte maxSpeed = 1;                /*!< Max. minion speed        */
byte numberOfMinionsAlive = 0;    /*!< Num. of minions on paths */

/* Module procedure declaration ------------------------------- */


#endif /* __GAME_H_ */