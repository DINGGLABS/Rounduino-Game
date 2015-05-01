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
 
/* Imports ---------------------------------------------------- */
#include "game.h"
#include "graphics.h"

/* Module constant declaration -------------------------------- */

/* Module type declaration ------------------------------------ */

/* Module data declaration ------------------------------------ */
byte numberOfPaths = 12;          /*!< Num. of total paths      */
byte numberOfSteps = 10;          /*!< Num. of steps per path   */
byte maxSpeed = 1;                /*!< Max. minion speed        */
byte numberOfMinionsAlive = 0;    /*!< Num. of minions on paths */

/* Module procedure declaration ------------------------------- */
// void displayStartGame();
struct Game initGame();
boolean playing(struct Game g);
void controlGame(struct Game g);
void drawGame(struct Game g);

/** ===========================================================
 * \fn      playGame
 * \brief   starts and stays in the Rounduino game until you've
 *					won or lost
 *
 * \param   -
 * \return  -
 ============================================================== */
void playGame()
{
	/* display start game text */
//	displayStartGame();

	/* init game structure */
	struct Game g;
	g = initGame();

	/* game */
	while (playing(g))
	{
		controlGame(g);

		drawGame(g);
	}
}

/** ===========================================================
 * \fn      initGame
 * \brief   initializes the game according to the config values
 *
 * \param   -
 * \return  (struct) game structure
 ============================================================== */
struct Game initGame()
{
	struct Game g;

	/* shield inits */
	g.s.path = numberOfPaths;
	g.s.numberOfLivesLeft = 3;	//blup

	/* boss inits */
	g.b.numberOfMinionsLeft = 10;	//blup

	return g;
}

/** ===========================================================
 * \fn      playing
 * \brief   checks if you've won or lost and returns true if
 *					you're still playing
 *
 * \param   -
 * \return  (bool) true = still playing, false = not
 ============================================================== */
boolean playing(struct Game g)
{
	boolean playing = true;

	if (g.b.numberOfMinionsLeft == 0 && numberOfMinionsAlive == 0)
	{
//		displayWon();
		playing = false;
	}
	else if (g.s.numberOfLivesLeft == 0)
	{
//		displayLost();
		playing = false;
	}

	return playing;
}

/** ===========================================================
 * \fn      controlShield
 * \brief   controls the shield
 *
 * \param   -
 * \return  (struct) shield structure
 ============================================================== */
void controlShield(struct Shield s)
{

}

/** ===========================================================
 * \fn      controlBoss
 * \brief   controls the boss
 *
 * \param   -
 * \return  (struct) boss structure
 ============================================================== */
void controlBoss(struct Boss b)
{

}

/** ===========================================================
 * \fn      controlMinion
 * \brief   controls the minions
 *
 * \param   -
 * \return  (struct) minion structure array
 ============================================================== */
void controlMinion(struct Minion m[MAX_NUMBER_MINIONS])
{

}

/** ===========================================================
 * \fn      controlGame
 * \brief   collects game informations and recognizes events
 *
 * \param   -
 * \return  -
 ============================================================== */
void controlGame(struct Game g)
{
	controlShield(g.s);
	controlBoss(g.b);
	controlMinion(g.m);

	/* check events */
	
}
