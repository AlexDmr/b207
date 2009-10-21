/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997, 1998, 1999  Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    slouken@devolution.com
*/

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: SDL_joystick.h,v 1.1.2.8 2000/03/03 20:12:44 hercules Exp $";
#endif

/* Include file for SDL joystick event handling */

#ifndef _SDL_joystick_h
#define _SDL_joystick_h

#include "SDL_types.h"

#include "begin_code.h"
/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

/* In order to use these functions, SDL_Init() must have been called
   with the SDL_INIT_JOYSTICK flag.  This causes SDL to scan the system
   for joysticks, and load appropriate drivers.
*/

/* The joystick structure used to identify an SDL joystick */
struct _SDL_Joystick;
typedef struct _SDL_Joystick SDL_Joystick;


/* Function prototypes */
/*
 * Count the number of joysticks attached to the system
 */
extern DECLSPEC int SDL_NumJoysticks(void);

/*
 * Get the implementation dependent name of a joystick.
 * This can be called before any joysticks are opened.
 * If no name can be found, this function returns NULL.
 */
extern DECLSPEC const char *SDL_JoystickName(int index);

/*
 * Open a joystick for use - the index passed as an argument refers to
 * the N'th joystick on the system.  This index is the value which will
 * identify this joystick in future joystick events.
 *
 * This function returns a joystick identifier, or NULL if an error occurred.
 */
extern DECLSPEC SDL_Joystick *SDL_JoystickOpen(int index);

/*
 * Returns 1 if the joystick has been opened, or 0 if it has not.
 */
extern DECLSPEC int SDL_JoystickOpened(int index);

/*
 * Get the device index of an opened joystick.
 */
extern DECLSPEC int SDL_JoystickIndex(SDL_Joystick *joystick);

/*
 * Get the number of general axis controls on a joystick
 */
extern DECLSPEC int SDL_JoystickNumAxes(SDL_Joystick *joystick);

/*
 * Get the number of trackballs on a joystick
 * Joystick trackballs have only relative motion events associated
 * with them and their state cannot be polled.
 */
extern DECLSPEC int SDL_JoystickNumBalls(SDL_Joystick *joystick);

/*
 * Get the number of POV hats on a joystick
 */
extern DECLSPEC int SDL_JoystickNumHats(SDL_Joystick *joystick);

/*
 * Get the number of buttons on a joystick
 */
extern DECLSPEC int SDL_JoystickNumButtons(SDL_Joystick *joystick);

/*
 * Update the current state of the open joysticks.
 * This is called automatically by the event loop if any joystick
 * events are enabled.
 */
extern DECLSPEC void SDL_JoystickUpdate(void);

/*
 * Enable/disable joystick event polling.
 * If joystick events are disabled, you must call SDL_JoystickUpdate()
 * yourself and check the state of the joystick when you want joystick
 * information.
 * The state can be one of SDL_QUERY, SDL_ENABLE or SDL_IGNORE.
 */
int SDL_JoystickEventState(int state);

/*
 * Get the current state of an axis control on a joystick
 * The state is a value ranging from -32768 to 32767.
 * The axis indices start at index 0.
 */
extern DECLSPEC Sint16 SDL_JoystickGetAxis(SDL_Joystick *joystick, int axis);

/*
 * Get the current state of a POV hat on a joystick
 * The return value is one of the following positions:
		8   1   2
		7   0   3
		6   5   4
   with zero meaning the hat is centered.
 * The hat indices start at index 0.
 */
extern DECLSPEC Uint8 SDL_JoystickGetHat(SDL_Joystick *joystick, int hat);

/*
 * Get the ball axis change since the last poll
 * This returns 0, or -1 if you passed it invalid parameters.
 * The ball indices start at index 0.
 */
extern DECLSPEC int SDL_JoystickGetBall(SDL_Joystick *joystick, int ball, int *dx, int *dy);

/*
 * Get the current state of a button on a joystick
 * The button indices start at index 0.
 */
extern DECLSPEC Uint8 SDL_JoystickGetButton(SDL_Joystick *joystick, int button);

/*
 * Close a joystick previously opened with SDL_JoystickOpen()
 */
extern DECLSPEC void SDL_JoystickClose(SDL_Joystick *joystick);


/* Ends C function definitions when using C++ */
#ifdef __cplusplus
};
#endif
#include "close_code.h"

#endif /* _SDL_joystick_h */
