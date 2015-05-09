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
 * \param   (struct) minion structure
 *			(byte)   number of paths
 *			(byte)   number of steps
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
 * \param   (struct) boss structure
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
 * \param   (struct) shield structure
 * \return  -
 ============================================================== */
void addShield(struct Shield *s) {
	//TODO
}

/** ===========================================================
 * \fn      drawGame
 * \brief   draws all game elements
 *
 * \param   (struct) game structure
 * \return  -
 ============================================================== */
void drawGame(struct Game *g) {
	addShield(&(g->s));
	addBoss(&(g->b));
	//blup (numberOfMinionsAlive)
	for (byte n = 0; n < numberOfMinionsAlive; n++) addMinion(&(g->m[n]), g->c.numberOfPaths, g->c.numberOfSteps);

	drawSymbols();
}

/** ===========================================================
 * \fn      displayStartGame
 * \brief   displays something before the game starts
 *
 * \param   -
 * \return  -
 ============================================================== */
void displayStartGame() {
	const unsigned char str1[] = {"READY"};
	const unsigned char str2[] = {"SET"};
	const unsigned char str3[] = {"GO!"};
	drawString(str1, 33, 55, MAX_BRIGHTNESS);
	delay(1000);
	clearDisplay();
	drawString(str2, 45, 55, MAX_BRIGHTNESS);
	delay(1000);
	clearDisplay();
	drawString(str3, 45, 55, MAX_BRIGHTNESS);
	delay(1000);

	clearSymbolList();
	clearDisplay();
}