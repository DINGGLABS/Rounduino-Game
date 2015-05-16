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
struct Game initGameStructure(struct Config *c);
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
	struct Game g = initGameStructure(&c);

	/* Rounduino game: */
	// while (playing(&g) && !getButtonState2())	//blup
	while (!getButtonState2())	//blup
	{
		// Serial.println("playing...");	//blup

		/* collect game informations and control events */
		controlGame(&g);

		/* draw all game objects */
		drawGame(&g);
		
		/* wait a defined time in ms */
		// delay(c.maxStepTime / 5);	//blup
	}
}

/** ===========================================================
 * \fn      initGameStructure
 * \brief   initializes the game according to the config values
 *
 * \param   (struct) config structure pointer
 * \return  (struct) game structure
 ============================================================== */
struct Game initGameStructure(struct Config *c)
{
	struct Game g;
	char tmpN = c->numberOfMinions;

	/* config inits */
	g.c = *c;

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
	piezoOn = false;	// reset piezo every pass

	/* update structur variables */
	controlShield(g);
	controlBoss(g);
	controlMinion(g);
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
	if (g->s.path < 0) g->s.path = g->c.numberOfPaths -1;

	/* control shield lives */
	for (byte n = 0; n < g->c.numberOfMinions; n++)
	{
		/* check if the minion's alive and has reached the edge */
		if (g->m[n].alive && g->m[n].step > g->c.numberOfSteps)
		{
			g->m[n].alive = false;
			g->numberOfMinionsAlive--;
			g->s.numberOfLivesLeft--;

			/* make some sound */
			piezoFrequencyDivisor = LOW_PIEZO_FREQUENCY_DIVISOR;
			piezoOn = true;
		}
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
	/* check if there're minions alive */
	if (g->numberOfMinionsAlive > 0)
	{
		/* check every minion */
		for (byte n = 0; n < g->c.numberOfMinions; n++)
		{
			if (g->m[n].alive)
			{
				/* control minion state */
				/* check if a minion have been hit */
				if (g->m[n].step == g->c.numberOfSteps && g->m[n].path == g->s.path)
				{
					g->m[n].alive = false;
					g->numberOfMinionsAlive--;

					/* make some sound */
					piezoFrequencyDivisor = STD_PIEZO_FREQUENCY_DIVISOR;
	      			piezoOn = true;
				}

	//			  /* control minion path */
	//			  g->m[n].path = 

				/* control minion step */
				/* check if it's time to make a step */
				if ((millis() - g->m[n].stepTimingReference) >= g->c.maxStepTime)
				{
					g->m[n].step++;

					/* update timing reference */
					g->m[n].stepTimingReference = millis();
				}
				
	//			  /* control minion stepTimingReference */
	//			  g->m[n].stepTimingReference = 
			}

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
