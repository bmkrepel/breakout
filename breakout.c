//
// breakout.c
//
// Computer Science 50
// Problem Set 4
//

// standard libraries
#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Stanford Portable Library
#include "gevents.h"
#include "gobjects.h"
#include "gwindow.h"

// height and width of game's window in pixels
#define HEIGHT 600
#define WIDTH 400

// number of rows of bricks
#define ROWS 5

// number of columns of bricks
#define COLS 10

// radius of ball in pixels
#define RADIUS 10

// lives
#define LIVES 3

// width and height and y coordinate of paddle in pixels
#define PWIDTH 50
#define PHEIGHT 10
int pycoord = HEIGHT - 20;

// prototypes
void initBricks(GWindow window);
GOval initBall(GWindow window);
GRect initPaddle(GWindow window);
GLabel initScoreboard(GWindow window);
void updateScoreboard(GWindow window, GLabel label, int points);
GObject detectCollision(GWindow window, GOval ball);

int main(void)
{
    // seed pseudorandom number generator
    srand48(time(NULL));

    // instantiate window
    GWindow window = newGWindow(WIDTH, HEIGHT);

    // instantiate bricks
    initBricks(window);

    // instantiate ball, centered in middle of window
    GOval ball = initBall(window);

    // instantiate paddle, centered at bottom of window
    GRect paddle = initPaddle(window);

    // instantiate scoreboard, centered in middle of window, just above ball
    GLabel label = initScoreboard(window);

    // number of bricks initially
    int bricks = COLS * ROWS;

    // number of lives initially
    int lives = LIVES;

    // number of points initially
    int points = 0;
    
    // initial ball velocities
    double xvelocity = drand48() + 1.0;
    double yvelocity = drand48() + 1.0;
            
    // keep playing until game over
    while (lives > 0 && bricks > 0)
    {
        // paddle follow mouse in horizontal domain
        
        // check for mouse event
        GEvent event = getNextEvent(MOUSE_EVENT);
        
        // if mouse moves
        if (event != NULL)
        {
            // if the event was movement
            if (getEventType(event) == MOUSE_MOVED)
            {
                // find center of paddle and x coordinate of mouse
                double center = getWidth(paddle) / 2;
                double x = getX(event);
                
                // have paddle center follow x coordinate of cursor if on screen
                if ((x > center) && (x < WIDTH - center))
                {
                    setLocation(paddle, x - center, pycoord);
                }
            }
        }
        
        // move ball
        move(ball, xvelocity, yvelocity);
        
        // bounce off right and left edge of window
        if ((getX(ball) + getWidth(ball) >= WIDTH) ||
            (getX(ball) <= 0))
        {
            xvelocity = -xvelocity;
        }

        // bounce off top & bottom edge of window
        else if ((getY(ball) <= 0) ||
                (getY(ball) + getWidth(ball) >= HEIGHT))
        {
            yvelocity = -yvelocity;
        }
        
        // linger before moving again
        pause(10);
        
        // detect collisons
        GObject object = detectCollision(window, ball);
        
        // if ball bounces off paddle
        if (object == paddle)
        {
            // bounce up
            yvelocity = -yvelocity;
        }
        
    }

    // wait for click before exiting
    waitForClick();

    // game over
    closeGWindow(window);
    return 0;
}

/**
 * Initializes window with a grid of bricks.
 */
void initBricks(GWindow window)
{
    // determine width of bricks based on width of window and number of columns    
    int BWIDTH = WIDTH / COLS - COLS;
    
    // for each row
    for (int i = 0; i < ROWS; i++)
    {
        // draw number of rectangles as cols to serve as bricks        
        for (int j = 0; j < COLS; j++)
        {
            GRect brick = newGRect((COLS / 2 + j * WIDTH / COLS), (20 + i * ROWS * 2), BWIDTH, 5);
            setFilled(brick, true);
            setColor(brick, "YELLOW");
            add(window, brick);
        }
    }    
}

/**
 * Instantiates ball in center of window.  Returns ball.
 */
GOval initBall(GWindow window)
{
    // call newGOval to create ball
    GOval ball = newGOval(WIDTH / 2 - RADIUS, HEIGHT / 2 - RADIUS, 2 * RADIUS, 2 * RADIUS);
    setFilled(ball, true);
    setColor(ball, "Black");

    // place on screen
    add(window, ball);
    return ball;
}

/**
 * Instantiates paddle in bottom-middle of window.
 */
GRect initPaddle(GWindow window)
{
    // create rectangular paddle aligned in bottom-middle of window
    
    GRect paddle = newGRect((WIDTH / 2) - (PWIDTH / 2), pycoord, PWIDTH, PHEIGHT);
    setFilled(paddle, true);
    setColor(paddle, "BLUE");
    add(window, paddle);
    return paddle;
}

/**
 * Instantiates, configures, and returns label for scoreboard.
 */
GLabel initScoreboard(GWindow window)
{
    // TODO
    return NULL;
}

/**
 * Updates scoreboard's label, keeping it centered in window.
 */
void updateScoreboard(GWindow window, GLabel label, int points)
{
    // update label
    char s[12];
    sprintf(s, "%i", points);
    setLabel(label, s);

    // center label in window
    double x = (getWidth(window) - getWidth(label)) / 2;
    double y = (getHeight(window) - getHeight(label)) / 2;
    setLocation(label, x, y);
}

/**
 * Detects whether ball has collided with some object in window
 * by checking the four corners of its bounding box (which are
 * outside the ball's GOval, and so the ball can't collide with
 * itself).  Returns object if so, else NULL.
 */
GObject detectCollision(GWindow window, GOval ball)
{
    // ball's location
    double x = getX(ball);
    double y = getY(ball);

    // for checking for collisions
    GObject object;

    // check for collision at ball's top-left corner
    object = getGObjectAt(window, x, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's top-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-left corner
    object = getGObjectAt(window, x, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // no collision
    return NULL;
}
