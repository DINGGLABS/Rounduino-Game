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
 * \fn      setMinion
 * \brief   Sets the minion m according to it's initializations
 *
 * \param   -
 * \return  -
 ============================================================== */
void setMinion(struct Minion *m) {
	/* Clear */
	clearCustomSymbol();
	/* Create */
	addFilledCircle(CUSTOM_SYMBOL_SIZE/2-1, CUSTOM_SYMBOL_WIDTH/2-1, R_MINION);	
	/* Set */
	double arc = PI*2/numberOfPaths * m->path;
	int hyp = R_PLAYGROUND/numberOfSteps * m->step;
	int dx = cos(arc) * hyp;
	int dy = sin(arc) * hyp;
	int cx = NUMBER_OF_PIXELS_PER_COLUMN/2 - CUSTOM_SYMBOL_SIZE/2;
	int cy = NUMBER_OF_PIXELS_PER_ROW/2 - CUSTOM_SYMBOL_WIDTH/2;
	createCustomSymbol(cx+dx, cy-dy, MAX_BRIGHTNESS);
}

/** ===========================================================
 * \fn      setBoss
 * \brief   Sets the boss b according to it's initializations
 *
 * \param   -
 * \return  -
 ============================================================== */
void setBoss(struct Boss *b) {
	/* Prepare */
	clearCustomSymbol();
	/* Create */
	addFilledCircle(CUSTOM_SYMBOL_SIZE/2-1, CUSTOM_SYMBOL_WIDTH/2-1, R_BOSS);
	/* Set */
	createCustomSymbol(NUMBER_OF_PIXELS_PER_COLUMN/2 - CUSTOM_SYMBOL_SIZE/2, NUMBER_OF_PIXELS_PER_ROW/2 - CUSTOM_SYMBOL_WIDTH/2, MAX_BRIGHTNESS);
}

/** ===========================================================
 * \fn      setShield
 * \brief   Sets the shield s according to it's initializations
 *
 * \param   -
 * \return  -
 ============================================================== */
void setShield(struct Shield *s) {
	//TODO
}

extern void drawGame(struct Game *g) {

}