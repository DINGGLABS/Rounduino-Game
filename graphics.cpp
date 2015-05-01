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

/* Imports ---------------------------------------------------- */
#include "graphics.h"
#include "Rounduino_lib.h"

/* Module constant declaration -------------------------------- */

/* Module type declaration ------------------------------------ */

/* Module data declaration ------------------------------------ */

/* Module procedure  declaration ------------------------------ */

/** ===========================================================
 * \fn      addMinion
 * \brief   adds the minion m according to it's initializations
 *
 * \param   -
 * \return  -
 ============================================================== */
void addMinion(struct Minion *m, byte numberOfPaths, byte numberOfSteps) {
	/* Prepare */
	clearCustomSymbol();
	/* Add */
	addFilledCircle(CUSTOM_SYMBOL_SIZE/2-1, CUSTOM_SYMBOL_WIDTH/2-1, R_MINION, true);
	/* Create */
	double arc = PI*2/numberOfPaths * m->path;
	int hyp = R_PLAYGROUND/numberOfSteps * m->step;
	int dx = cos(arc) * hyp;
	int dy = sin(arc) * hyp;
	int cx = NUMBER_OF_PIXELS_PER_COLUMN/2 - CUSTOM_SYMBOL_SIZE/2;
	int cy = NUMBER_OF_PIXELS_PER_ROW/2 - CUSTOM_SYMBOL_WIDTH/2;
	createCustomSymbol(cx+dx, cy-dy, MAX_BRIGHTNESS);
}

/** ===========================================================
 * \fn      addBoss
 * \brief   adds the boss b according to it's initializations
 *
 * \param   -
 * \return  -
 ============================================================== */
void addBoss(struct Boss *b) {
	/* Prepare */
	clearCustomSymbol();
	/* Add */
	addFilledCircle(CUSTOM_SYMBOL_SIZE/2-1, CUSTOM_SYMBOL_WIDTH/2-1, R_BOSS, true);
	/* Create */
	createCustomSymbol(NUMBER_OF_PIXELS_PER_COLUMN/2 - CUSTOM_SYMBOL_SIZE/2, NUMBER_OF_PIXELS_PER_ROW/2 - CUSTOM_SYMBOL_WIDTH/2, MAX_BRIGHTNESS);
}

/** ===========================================================
 * \fn      addShield
 * \brief   adds the shield s according to it's initializations
 *
 * \param   -
 * \return  -
 ============================================================== */
void addShield(struct Shield *s) {
	//TODO
}

extern void drawGame(struct Game *g) {
	addShield(&(g->s));
	addBoss(&(g->b));
	for (byte n = 0; n < sizeof(g->m); n++) addMinion(&(g->m[n]), g->c.numberOfPaths, g->c.numberOfSteps);

	drawSymbols();
}