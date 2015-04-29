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
 
/* Global imports --------------------------------------------- */

#include <Arduino.h>


/* Global module constant declaration ------------------------- */

#define R_PLAYGROUND    50

/* Global module type declaration ----------------------------- */

 /*! \struct Shield */
struct Shield {
	byte path;                 /*!< ID of the path              */
	byte numberOfLivesLeft;    /*!< Number of lives left        */
};

 /*! \struct Minion */
struct Minion {
	byte path;                 /*!< ID of the path              */
	byte step;                 /*!< Position on the path        */
	byte id;                   /*!< ID of the minion            */
	byte speed;                /*!< Speed of the minion         */
};

 /*! \struct Boss */
struct Boss {
	byte numberOfMinionsLeft;  /*!< Num. of minions left inside */
};


/* Global module data declaration ----------------------------- */

extern byte numberOfPaths;
extern byte numberOfSteps;
extern byte maxSpeed;
extern byte numberOfMinionsAlive;


/* Global module procedure declaration ------------------------ */



#endif /* __GAME_H_ */