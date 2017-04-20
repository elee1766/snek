#include <ncurses.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

int main (int argc, char *argv[])
{
  int highscore = 0;
  int lastscore = 0;
  int frame = 1;

  int x = 5, y = 5;
  int max_y = 0, max_x = 0;
  int next_x = 0;
  int next_y = 0;

  int xlast;
  int ylast;

  int xdirection = 0;

  int ydirection = 0;

  int yhist[250];
  int xhist[250];

  int xbody[250];
  int ybody[250];

  int xcherry;
  int ycherry;

  int loss = 0;
  int eaten = 1;

  int length = 4;
  int lastinput;

  char str[1000];
  char fr[1000];
  char frames[1000];

  double scale = 1;
  int DELAY = 40000;

  initscr ();
  curs_set (FALSE);
  // Global var `stdscr` is created by the call to `initscr()`

  getmaxyx (stdscr, max_y, max_x);

  nodelay (stdscr, TRUE);

  keypad (stdscr, TRUE);

  start_color();
  init_color(COLOR_BLACK, 8, 8, 8);
  init_pair(1, COLOR_RED, COLOR_BLACK);
  init_pair(2, COLOR_YELLOW, COLOR_BLACK);
  init_pair(3, COLOR_BLUE, COLOR_BLACK);

  echo ();

  //this stupid piece of code makes string str equal to an int
  //sprintf(str,"%d",lastinput);


  while (true)
    {
      length = 4;
      memset (xhist, 0, sizeof (xhist));
      memset (yhist, 0, sizeof (yhist));
      memset (xbody, 0, sizeof (xbody));
      memset (ybody, 0, sizeof (ybody));
      xdirection = 1;
      ydirection = 0;
      x = 5;
      y = 5;
      scale = 1;
      frame = 1;

      while (loss == 0)
	{
	  clear ();

	  //frame tells which frame the game is on, or how long the game has been played for
	  frame++;

	  //debug code to display values

	  sprintf (str, "high score: %d", highscore);
	  sprintf (fr, "current score: %d", length);
      sprintf(frames,"Frames Elapsed: %d %f",frame,scale);

      attron(COLOR_PAIR(3));
	  mvprintw (0, 0, str);
	  mvprintw (0, max_x - 20, fr);
      mvprintw (0, max_x/2 - 15, frames);




      attroff(COLOR_PAIR(3));
	  //head of the snake
      attron(COLOR_PAIR(1));
	  mvprintw (y, x, "O");
      attroff(COLOR_PAIR(1));

      //body of the snake
	  if (length > 1)
	    {
	      for (int a = 1; a < length; a++)
		{
		  mvprintw (yhist[a], xhist[a], "o");
		}
	    }
      srand(time(NULL));
	  //power cherry! there is only one at any given time. It will only generate a new cherry if the cherry has been eaten!
	  if (eaten == 1)
	    {
	      xcherry = rand () % (max_x - 3);
	      ycherry = rand () % (max_y - 4) + 2;
	      eaten = 0;
	    }
      attron(COLOR_PAIR(2));
	  mvprintw (ycherry, xcherry, "@");
      attroff(COLOR_PAIR(2));

	  refresh ();
	  usleep (DELAY);

	  lastinput = getch ();

      xlast=xdirection;
      ylast=ydirection;

	  if (lastinput == 119)
	    {
	      ydirection = -1;
	      xdirection = 0;
	    }
	  if (lastinput == 97)
	    {
	      ydirection = 0;
	      xdirection = -1;
	    }
	  if (lastinput == 115)
	    {
	      ydirection = 1;
	      xdirection = 0;
	    }
	  if (lastinput == 100)
	    {
	      ydirection = 0;
	      xdirection = 1;
	    }

      //we need to stop you from killing yourself. if your direction is the opposite of your card, then cancel the move

      if(xdirection*-1 == xlast && ydirection * -1 == ylast){
      xdirection *= -1;
      ydirection *= -1;
      }

	  next_x = x + xdirection;
	  next_y = y + ydirection;

	  //checks if you have lost the game or not
	  if (next_x >= max_x || next_x < 0)
	    {
	      loss = 1;
	    }
	  if (next_y >= max_y || next_y < 0)
	    {
	       loss = 1;
	    }
	  //sets the new x and y direction
	  x += xdirection;
	  y += ydirection;

	  //allows me to increase the SPEEEED of the game
	  if (ydirection == 0)
	    {
	      DELAY = 40000 * scale;
	    }
	  if (ydirection != 0)
	    {
	      DELAY = 66666 * scale;
	    }

	  //shifts all the array one space to the right
	  for (int k = 250; k > 0; k--)
	    {
	      xhist[k] = xhist[k - 1];
	      yhist[k] = yhist[k - 1];
	    }

	  //this code sets the head position to the 0 part of the array
	  xhist[0] = x;
	  yhist[0] = y;
	  //this code sets the xy of the body

	  for (int a = 0; a < length; a++)
	    {
	      xbody[a] = xhist[a];
	      ybody[a] = yhist[a];
	    }

	  //now we check if the snake has hit itself, if it has, we send the loss signal
	  if (frame > 50)
	    {
	      for (int i = 1; i < length; i++)
		{
		  if (x == xbody[i] & y == ybody[i])
		    {
		      loss = 1;
		    }
		}
	    }
	  //now we check if we have eaten the cherry. If we have, we increase the length/score by 1!!
	  if (x == xcherry && y == ycherry)
	    {
	      length++;
	      eaten = 1;
	    }

      scale = 1 - 7*log(8*length)*0.01 + 0.2;
	  lastscore = length;
	  //this is one iteration of the game
	}


      if (lastscore > highscore)
	{
	  highscore = lastscore;
	}
      clear ();

      attron(COLOR_PAIR(2));

      mvprintw (0, 0, "You have lost! press any key to restart your game!");
      sprintf (str, "your last score was %d. your highscore is %d", lastscore,
	       highscore);

      mvprintw (1, 0, str);

      attroff(COLOR_PAIR(2));
      refresh ();

      getchar ();
      loss = 0;

////leaving space so i dont mess shit up
//
//
//
//
//
//
    }
  endwin ();
}
