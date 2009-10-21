#ifdef __BORLANDC__
  #pragma argsused
#endif

#include <principal.h>

int main( int argc, char * argv[] )
{affichage_pointeurs *aff_ptr = new affichage_pointeurs(NULL);
 int tx = 1024
   , ty = 768
   , propriete = 0;

 principal prog(argv[0]);

if(argc >=3 )
 {tx = atoi(argv[1]);
  ty = atoi(argv[2]);
  if(argc >= 4)
   {if(!strcmp("FULL_SCREEN", argv[3])
     ||!strcmp("FULLSCREEN", argv[3])
     ||!strcmp("FS", argv[3])
     ||!strcmp("FULL", argv[3]) )
     propriete = SDL_FULLSCREEN;
   }
 }

 prog.demarrer(propriete, tx, ty);

 return 0;
}
