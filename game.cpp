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
#include "Rounduino_lib.h"
#include "game.h"
#include "graphics.h"

/* Module constant declaration -------------------------------- */

/* Module type declaration ------------------------------------ */

/* Module data declaration ------------------------------------ */
byte currentSpeedDivider = 1;
unsigned int spawnTime = DEFAULT_SPAWN_TIME;
unsigned long spawnTimeReference = millis();

/* Module procedure declaration ------------------------------- */
// void displayStartGame();
struct Game initGame(struct Config *c);
boolean playing(struct Game *g);
void controlGame(struct Game *g);
void controlShield(struct Game *g);
void controlBoss(struct Game *g);
void controlMinion(struct Game *g);
void drawGame(struct Game *g);

/** ===========================================================
 * \fn      playGame
 * \brief   starts and stays in the Rounduino game until you've
 *					won or lost
 *
 * \param   (struct) config structure
 * \return  -
 ============================================================== */
void playGame(struct Config c)
{
	/* display start game text */
	displayStartGame();

	/* init game structure */
	struct Game g = initGame(&c);

	/* Rounduino game: */
	while (playing(&g) && !getButtonState2())	//blup
	{
		//blup
		// const unsigned char s[] = {"Playing"};
		// drawString(s, 21, 55, MAX_BRIGHTNESS);
		// clearSymbolList();

		/* collect game informations and control events */
		controlGame(&g);

		/* draw all game objects */
		drawGame(&g);
		
		/* wait a defined time in ms */
		delay(c.maxStepTime / 2);	//blup
	}
}

/** ===========================================================
 * \fn      initGame
 * \brief   initializes the game according to the config values
 *
 * \param   (struct) config structure pointer
 * \return  (struct) game structure
 ============================================================== */
struct Game initGame(struct Config *c)
{
	struct Game g;
	char tmpN = c->numberOfMinions;

	/* shield inits */
	g.s.path = 0;
	g.s.numberOfLivesLeft = c->lives;
	
	/* boss inits */
	g.b.numberOfMinionsLeft = tmpN;

	/* minion inits */
	for (byte n = 0; n < tmpN; n++) g.m[n].alive = false;

	/* global inits */
	g.numberOfMinionsAlive = 0;

	return g;
}

/** ===========================================================
 * \fn      playing
 * \brief   checks if you've won or lost and returns true if
 *					you're still playing
 *
 * \param   (struct) game structure pointer
 * \return  (bool)   true = still playing, false = not
 ============================================================== */
boolean playing(struct Game *g)
{
	boolean playing = true;

	if (g->b.numberOfMinionsLeft <= 0 && g->numberOfMinionsAlive <= 0)
	{
		displayWon();
		playing = false;
	}
	else if (g->s.numberOfLivesLeft <= 0)
	{
		displayLost();
		playing = false;
	}

	return playing;
}

/** ===========================================================
 * \fn      controlGame
 * \brief   collects game informations and recognizes events
 *
 * \param   (struct) game structure pointer
 * \return  -
 ============================================================== */
void controlGame(struct Game *g)
{
	/* update structur variables */
	controlShield(g);
	controlBoss(g);
	controlMinion(g);

	/* check events */

}

/** ===========================================================
 * \fn      controlShield
 * \brief   controls the shield
 *
 * \param   (struct) game structure pointer
 * \return  -
 ============================================================== */
void controlShield(struct Game *g)
{
	/* control shield path */
	if (getButtonState1()) g->s.path++;
	else if (getButtonState3()) g->s.path--;

	if (g->s.path >= g->c.numberOfPaths) g->s.path = 0;

	/* control shield lives */
	for (byte n = 0; n < g->c.numberOfMinions; n++)
	{
		/* check if the minion's alive and has reached the edge */
		if (g->m[n].alive && g->m[n].step > g->c.numberOfSteps) g->s.numberOfLivesLeft--;
	}
}

/** ===========================================================
 * \fn      controlBoss
 * \brief   controls the boss
 *
 * \param   (struct) game structure pointer
 * \return  -
 ============================================================== */
void controlBoss(struct Game *g)
{
	/* control number of minions left */
	/* check if there are minions left to spawn */
	if (g->b.numberOfMinionsLeft > 0)
	{
		/* check if it's time to spawn a minion */
		if ((millis() - spawnTimeReference) >= spawnTime)
		{
			struct Minion newM;
			byte nextMinionArrayPosition = 0;

			/* get next minion array position */
			nextMinionArrayPosition = g->c.numberOfMinions - g->b.numberOfMinionsLeft;

			/* init new minion */
			newM.alive = true;
			newM.path = random(g->c.numberOfPaths);
			newM.step = 0;
			newM.stepTimingReference = (g->c.maxStepTime) / currentSpeedDivider;

			/* add minion to the game */
			g->m[nextMinionArrayPosition] = newM;
			g->numberOfMinionsAlive++;
			g->b.numberOfMinionsLeft--;

			/* update timing reference */
			spawnTimeReference = millis();
		}
	}
}

/** ===========================================================
 * \fn      controlMinion
 * \brief   controls the minions
 *
 * \param   (struct) game structure pointer
 * \return  -
 ============================================================== */
void controlMinion(struct Game *g)
{
	byte currentMinion;

	/* check if there're minions alive */
	if (g->numberOfMinionsAlive > 0)
	{
		/* check every minion */
		for (currentMinion = 0; currentMinion < g->c.numberOfMinions; currentMinion++)
		{
			/* control minion state */
			/* check if a minion have been hit */
			if (g->m[currentMinion].step == g->c.numberOfSteps && g->m[currentMinion].path == g->s.path)
			{
				g->m[currentMinion].alive = false;
				g->numberOfMinionsAlive--;
			}

//			  /* control minion path */
//			  g->m[currentMinion].path = 

			/* control minion step */
			/* check if it's time to make a step */
			if ((millis() - g->m[currentMinion].stepTimingReference) >= g->c.maxStepTime)
			{
				g->m[currentMinion].step++;

				/* update timing reference */
				g->m[currentMinion].stepTimingReference = millis();
			}
			
//			  /* control minion stepTimingReference */
//			  g->m[currentMinion].stepTimingReference = 

		}
	}
}

/** ===========================================================
 * \fn      getDefaultConfigValues
 * \brief   returns the default configuration values for the
 *					Rounduino game
 *
 * \param   -
 * \return  (struct) config structure
 ============================================================== */
struct Config getDefaultConfigValues()
{
	struct Config c;

	c.lives = DEFAULT_LIVES;
	c.numberOfPaths = DEFAULT_NUMBER_OF_PATHS;
	c.numberOfSteps = DEFAULT_NUMBER_OF_STEPS;
	c.numberOfMinions = DEFAULT_NUMBER_OF_MINIONS;
	c.maxStepTime = millis();

	return c;
}
