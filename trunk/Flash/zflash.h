//code by zZz/KCN aka Anatoliy Samara. feb2003
#include <stdlib.h>

void __Error(char *message,const char *file,int line) {}

namespace ZFLASH
{
 int Init(int version_needed);
 void Exit();
 void LoadMovie(char *movie_name);

 void* CreateBuffer(int width,int height);
 int DrawFrame();
 void Lire();
}//namespace
