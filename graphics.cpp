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
void setMinion(struct Minion m) {
	//TODO
}

/** ===========================================================
 * \fn      setBoss
 * \brief   Sets the boss b according to it's initializations
 *
 * \param   -
 * \return  -
 ============================================================== */
void setBoss(struct Boss b) {
	// Prepare
	clearCustomSymbol();
	// Create
	drawFilledCircleSymbol(CUSTOM_SYMBOL_SIZE/2-1, CUSTOM_SYMBOL_WIDTH/2-1, R_BOSS);
	// Set
	setCustomSymbol(NUMBER_OF_PIXELS_PER_COLUMN/2 - CUSTOM_SYMBOL_SIZE/2, NUMBER_OF_PIXELS_PER_ROW/2 - CUSTOM_SYMBOL_WIDTH/2, MAX_BRIGHTNESS);
}

/** ===========================================================
 * \fn      setShield
 * \brief   Sets the shield s according to it's initializations
 *
 * \param   -
 * \return  -
 ============================================================== */
void setShield(struct Shield s) {
	//TODO
}