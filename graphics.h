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
#define R_BOSS      4
#define R_MINION    2

/* Global module type declaration ----------------------------- */

/* Global module data declaration ----------------------------- */

/* Global module procedure declaration ------------------------ */
extern void addMinion(struct Minion *m);
extern void addBoss(struct Boss *b);
extern void addShield(struct Shield *s);
extern void drawGame(struct Game *g);
 
 #endif /* __GRAPHICS_H_ */