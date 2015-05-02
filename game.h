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
#define R_PLAYGROUND    				50	// playground radius

#define MAX_NUMBER_MINIONS				50
#define DEFAULT_SPAWN_TIME			  3000

/* default config defines */
#define DEFAULT_LIVES  			 	 	 3
#define DEFAULT_NUMBER_OF_PATHS			12
#define DEFAULT_NUMBER_OF_STEPS		 	 8
#define DEFAULT_NUMBER_OF_MINIONS 		10
#define DEFAULT_SPEED				 	 1

/* Global module type declaration ----------------------------- */
/*! \struct Config */
struct Config
{
	char lives;    				       /*!< Num. of total lives      */
	byte numberOfPaths;          /*!< Num. of total paths      */
	byte numberOfSteps;          /*!< Num. of steps per path   */
	byte numberOfMinions;        /*!< Num. of total minions    */
	byte maxSpeed;               /*!< Max. minion speed        */
};

/*! \struct Shield */
struct Shield
{
	char path;                 /*!< ID of the path              */
	char numberOfLivesLeft;    /*!< Number of lives left        */
};

/*! \struct Boss */
struct Boss
{
	char numberOfMinionsLeft;  /*!< Num. of minions left inside */
};

/*! \struct Minion */
struct Minion
{
	byte id;                   /*!< ID of the minion            */
	byte path;                 /*!< ID of the path              */
	byte step;                 /*!< Position on the path        */
	unsigned long speed;       /*!< Speed of the minion         */
};

/*! \struct Game */
struct Game
{
	struct Config c;
	struct Shield s;
	struct Boss b;
	struct Minion m[MAX_NUMBER_MINIONS];
};

/* Global module data declaration ----------------------------- */

/* Global module procedure declaration ------------------------ */
extern struct Config getDefaultConfigValues();
extern void playGame(struct Config *config);

#endif /* __GAME_H_ */