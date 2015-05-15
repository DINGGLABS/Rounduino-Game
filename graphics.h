#ifndef __GRAPHICS_H_
#define __GRAPHICS_H_

/** ===========================================================
 *  example game graphics
 *
 * \file       graphics
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

/* Global imports --------------------------------------------- */
#include "game.h"
#include <Arduino.h>

/* Global module constant declaration ------------------------- */
/* radii */
#define R_PLAYGROUND		   60
#define R_BOSS      		 	4
#define R_MINION    	 	 	2
#define R_SHIELD    	 	 	5

/* display times */
#define DISPLAY_TIME_WON	 2000
#define DISPLAY_TIME_LOST	 2000

/* Global module type declaration ----------------------------- */

/* Global module data declaration ----------------------------- */

/* Global module procedure declaration ------------------------ */
extern void drawGame(struct Game *g);
extern void displayStartGame();
extern void displayWon();
extern void displayLost();
 
 #endif /* __GRAPHICS_H_ */