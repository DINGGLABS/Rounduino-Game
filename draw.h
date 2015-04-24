#ifndef __DRAW_H_
#define __DRAW_H_

/** ===========================================================
 *  example game draw
 *
 * \file       draw
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

/* Global module type declaration ----------------------------- */

/* Global module data declaration ----------------------------- */

/* Global module procedure declaration ------------------------ */
extern void drawMinion(struct Minion m);
extern void drawBoss(struct Boss b);
extern void drawShield(struct Shield s);
 
 
 #endif /* __DRAW_H_ */