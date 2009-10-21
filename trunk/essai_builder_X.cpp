#include "opengl/principal.h"

int main(int argc, char *argv[])
{
        /* Load SDL dynamic link library */
/*	if ( SDL_Init(0) < 0 ) {
                ShowError("WinMain() error", SDL_GetError());
                return(FALSE);
        }
        atexit(SDL_Quit);

        // Create and register our class, then run main code
        if ( SDL_RegisterApp( appname, CS_BYTEALIGNCLIENT
                            , GetModuleHandle(NULL)) < 0 ) {
                ShowError("WinMain() error", SDL_GetError());
                exit(1);
        }  */
        //SDL_main(argc, argv);

        principal prog;
        prog.demarrer( 0 );

        /* Exit cleanly, calling atexit() functions */
        exit(0);
        return 0;
}
