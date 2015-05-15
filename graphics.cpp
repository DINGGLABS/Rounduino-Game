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
void drawShield(struct Shield *s, byte numberOfPaths);
void drawBoss(struct Boss *b);
void drawMinion(struct Minion *m, byte numberOfPaths, byte numberOfSteps);

/** ===========================================================
 * \fn      drawGame
 * \brief   draws all game elements
 *
 * \param   (struct) game structure
 * \return  -
 ============================================================== */
void drawGame(struct Game *g) {
	drawShield(&(g->s), g->c.numberOfPaths);
	drawBoss(&(g->b));

	for (byte n = 0; n < g->c.numberOfMinions; n++) {
		if (g->m[n].alive) drawMinion(&(g->m[n]), g->c.numberOfPaths, g->c.numberOfSteps);
	}

	clearDisplay();
	clearSymbolList();
}

/** ===========================================================
 * \fn      drawShield
 * \brief   draws the shield s according to it's 
 *			initializations
 *
 * \param   (struct) shield structure
 *          (byte)   total number of paths in the game
 * \return  -
 ============================================================== */
void drawShield(struct Shield *s, byte numberOfPaths) {
	/* Prepare */
	clearCustomSymbol();

	/* Add */
	double arc = PI*2/numberOfPaths * s->path;
	int dx = -sin(arc) * R_SHIELD;
	int dy = cos(arc) * R_SHIELD;
	int cx = CUSTOM_SYMBOL_WIDTH/2-1;
	int cy = CUSTOM_SYMBOL_SIZE/2-1;
	addPixel(cx, cy);
	addLine(cx-dx, cy+dy, cx+dx, cy-dy);

	/* add live pixels */
	dx = -sin(arc) * s->numberOfLivesLeft *2;	//blup
	dy = cos(arc) * s->numberOfLivesLeft *2;
	addLine(cx, cy, cx-dy, cy-dx);

	/* Create */
	cx = NUMBER_OF_PIXELS_PER_COLUMN/2 - CUSTOM_SYMBOL_WIDTH/2;
	cy = NUMBER_OF_PIXELS_PER_ROW/2 - CUSTOM_SYMBOL_SIZE/2;
	dx = cos(arc) * R_PLAYGROUND;
	dy = sin(arc) * R_PLAYGROUND;

	if (dx < -cx) dx = -cx;
	if (dy > cy) dy = cy;

	createCustomSymbol(cx+dx, cy-dy, MAX_BRIGHTNESS);

	/* Draw */
	drawSymbol(tail);
}

/** ===========================================================
 * \fn      drawBoss
 * \brief   draws the boss b according to it's initializations
 *
 * \param   (struct) boss structure
 * \return  -
 ============================================================== */
void drawBoss(struct Boss *b) {
	/* Prepare */
	clearCustomSymbol();

	/* Add */
	addFilledCircle(CUSTOM_SYMBOL_SIZE/2-1, CUSTOM_SYMBOL_WIDTH/2-1, R_BOSS, true);

	/* Create */
	createCustomSymbol(NUMBER_OF_PIXELS_PER_COLUMN/2 - CUSTOM_SYMBOL_SIZE/2, NUMBER_OF_PIXELS_PER_ROW/2 - CUSTOM_SYMBOL_WIDTH/2, MAX_BRIGHTNESS);
	
	/* Draw */
	drawSymbol(tail);
}

/** ===========================================================
 * \fn      drawMinion
 * \brief   draws a minion m according to it's initializations
 *
 * \param   (struct) minion structure
 *          (byte)   total number of paths in the game
 *          (byte)   number of steps on the path
 * \return  -
 ============================================================== */
void drawMinion(struct Minion *m, byte numberOfPaths, byte numberOfSteps) {
	/* Prepare */
	clearCustomSymbol();

	/* Add */
	addFilledCircle(CUSTOM_SYMBOL_SIZE/2-1, CUSTOM_SYMBOL_WIDTH/2-1, R_MINION, true);

	/* Create */
	double arc = PI*2/numberOfPaths * m->path;
	int hyp = R_PLAYGROUND/numberOfSteps * m->step;
	int cx = NUMBER_OF_PIXELS_PER_COLUMN/2 - CUSTOM_SYMBOL_SIZE/2;
	int cy = NUMBER_OF_PIXELS_PER_ROW/2 - CUSTOM_SYMBOL_WIDTH/2;
	int dx = cos(arc) * hyp;
	int dy = sin(arc) * hyp;
	
	if (dx < -cx) dx = -cx;
	if (dy > cy) dy = cy;
	
	createCustomSymbol(cx+dx, cy-dy, MAX_BRIGHTNESS);

	/* Draw */
	drawSymbol(tail);
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

/** ===========================================================
 * \fn      displayWon
 * \brief   displays something when you've won
 *
 * \param   -
 * \return  -
 ============================================================== */
void displayWon() {
	clearDisplay();
	const unsigned char str1[] = {"You've"};
	const unsigned char str2[] = {"WON!"};
	drawString(str1, 27, 55, MAX_BRIGHTNESS);
	drawString(str2, 39, 71, MAX_BRIGHTNESS);
	delay(DISPLAY_TIME_WON);

	clearSymbolList();
	clearDisplay();
}

/** ===========================================================
 * \fn      displayLost
 * \brief   displays something when you've lost
 *
 * \param   -
 * \return  -
 ============================================================== */
void displayLost() {
	clearDisplay();
	const unsigned char str1[] = {"You've"};
	const unsigned char str2[] = {"LOST!"};
	drawString(str1, 27, 55, MAX_BRIGHTNESS);
	drawString(str2, 33, 71, MAX_BRIGHTNESS);
	delay(DISPLAY_TIME_LOST);

	clearSymbolList();
	clearDisplay();
}