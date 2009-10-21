//code by zZz/KCN aka Anatoliy Samara. feb2003

#include "zflash.h"
#include "zflash_flash.h"

#include <stdio.h>

#include <ATLBASE.H>
CComModule _Module;
#include <atlwin.h>


#include <atlcom.h>
#include <atlhost.h>

HRESULT tmp;

extern void __Error(char *message,const char *file,int line);
IShockwaveFlash *iflash=NULL;
IShockwaveFlash *try_flash;
IUnknown *unk=NULL;
CAxWindow cw;
IViewObjectEx *viewobject;

int bm_width=0,bm_height=0;
HDC hdcCompatible = NULL;
HBITMAP hBitmap = NULL;
void *frame_buffer;
RECTL rectl;
HINSTANCE hInst;

BEGIN_OBJECT_MAP(ObjectMap)
END_OBJECT_MAP()

HWND hwnd;
                         
namespace ZFLASH{

LRESULT CALLBACK default_window_proc(HWND hwnd,UINT msg,WPARAM wparam,LPARAM lparam)
{
 return (DefWindowProc(hwnd,msg,wparam,lparam));
}

int Init(int version_needed){

   hInst = GetModuleHandle(NULL);
   WNDCLASS window_class;

   //Fill in all the fields for the WNDCLASS structure.  Window classes
   //are a sort of template for window creation.  You could create many
   //windows using the same window class.
   window_class.style          = CS_OWNDC;
   window_class.cbClsExtra     = 0;
   window_class.cbWndExtra     = 0;
   window_class.hInstance      = hInst;
   window_class.hIcon          = LoadIcon(NULL,IDI_APPLICATION);
   window_class.hCursor        = LoadCursor(NULL,IDC_ARROW);
   window_class.hbrBackground  = (HBRUSH)GetStockObject(BLACK_BRUSH);
   window_class.lpszMenuName   = NULL;
   window_class.lpszClassName  = "DH Class";
   //Here we provide our default window handler, all windows messages
   //will be sent to this function.
   window_class.lpfnWndProc    = default_window_proc;


   //Register the class with windows
   if(!RegisterClass(&window_class))
     exit(50);

        hwnd = CreateWindow( "DH Class", "", WS_OVERLAPPED
                           , 0, 0, 256, 256, NULL, NULL
                           , hInst, NULL );
       // ALEX
        _Module.Init(ObjectMap, hInst);

        cw = hwnd;
        //_Module.Init(ObjectMap, hInstance);
       // FIN ALEX 

	int player_ok=1;

	OleInitialize(NULL);
	CoCreateInstance(CLSID_ShockwaveFlash,NULL,CLSCTX_INPROC_SERVER,IID_IShockwaveFlash,(void **)&try_flash);
	if (!try_flash){
		player_ok=0;
	}
	else{
		long version;
		try_flash->FlashVersion(&version);
		if (version<version_needed)
			player_ok=0;
		//try_flash->Release();
	}

	if (!player_ok){
		char temp[1024];
		sprintf(temp,"Flash Player v%d.%02d is not registered on this system!\nFlash Player is required to run this programm.\n\nRegister it now?",version_needed>>16,version_needed&0xffff);
		int res=MessageBox(NULL,temp,"Requirements",MB_ICONQUESTION|MB_YESNO);
		if (res==IDYES){
			HMODULE hlib=LoadLibrary("swflash.ocx");
			if (!hlib)
				__Error("File swflash.ocx not found", "toto", 0);
			void (__stdcall *DllRegisterServer)(void);
			DllRegisterServer=(void (__stdcall *)(void))GetProcAddress(hlib,"DllRegisterServer");
			DllRegisterServer();
		}
		else{
			return 0;
		}
	}
	return 1;
}

void ReleaseInterfaces()
{
	if (unk) {
		unk->Release();
		unk=NULL;
	}
	if (iflash){
		iflash->Release();
		iflash=NULL;
	}
}

void ReleaseGDI(){
	if (hBitmap){
		DeleteObject(hBitmap);
	}

	if (hdcCompatible){
		DeleteDC(hdcCompatible);
	}
}

void Exit(){
	ReleaseInterfaces();
	ReleaseGDI();
}
			
void* CreateBuffer(int width,int height){
	ReleaseGDI();

	HDC desktop_dc=GetDC(GetDesktopWindow());
	hdcCompatible = CreateCompatibleDC(desktop_dc);	

	BITMAPINFO binfo;
	memset(&binfo,0,sizeof(binfo));
	binfo.bmiHeader.biSize=sizeof(binfo);
	binfo.bmiHeader.biPlanes=1;
	binfo.bmiHeader.biBitCount=32;
	binfo.bmiHeader.biCompression=BI_RGB;
	binfo.bmiHeader.biHeight=height;
	binfo.bmiHeader.biWidth=width;
	
	hBitmap=CreateDIBSection(hdcCompatible,&binfo,DIB_RGB_COLORS,&frame_buffer,NULL,0);
	SelectObject(hdcCompatible, hBitmap);
	bm_width=width;
	bm_height=height;

	RECT rc={0,0,width,height};
	AdjustWindowRect(&rc,cw.GetWindowLong(GWL_STYLE),FALSE);

	cw.SetWindowPos(NULL,0,0,rc.right-rc.left,rc.bottom-rc.top,SWP_NOMOVE|SWP_NOREDRAW|SWP_DEFERERASE|
	                                      SWP_NOACTIVATE|SWP_NOCOPYBITS|SWP_NOOWNERZORDER|SWP_NOSENDCHANGING|
										  SWP_NOZORDER);

	rectl.left=0;
	rectl.right=bm_width;
	rectl.top=0;
	rectl.bottom=bm_height;

	return frame_buffer;
}

void LoadMovie(char *movie_name){
	
	ReleaseInterfaces();
	ReleaseGDI();

	AtlAxWinInit();
	if (cw.IsWindow())
		cw.DestroyWindow();

	RECT rc={0,0,400,400};
	AdjustWindowRect(&rc,0,FALSE);
	cw.Create(NULL,&rc,flash_clsid_c,0);

	cw.QueryControl(&unk);
	if (!unk) __Error("NULL IUnknown", "toto", 0);

	HRESULT hr=unk->QueryInterface(IID_IShockwaveFlash,(void **)&iflash);
	if (!iflash) __Error("Unable to query IFlash", "toto", 0);

	char buf[256];
	if (movie_name[0]=='.'){
		GetCurrentDirectory(256,buf);
		char last_ch=buf[strlen(buf)-1];
		if (last_ch!='\\'&&last_ch!='/'){
			strcat(buf,"/");
		}
		strcat(buf,movie_name);
	}
	else{
		strcpy(buf,movie_name);
	}

	wchar_t real_name[256];
	MultiByteToWideChar(CP_ACP,0,buf,-1,real_name,256);
	
	//iflash->put_Playing(FALSE);
	tmp = iflash->LoadMovie(0,real_name);
        tmp = try_flash->LoadMovie(0,real_name);
	//iflash->put_Quality(5);

	tmp = iflash->QueryInterface(IID_IViewObjectEx,(void **)&viewobject);

	//tmp = iflash->SetZoomRect(0,0,256,256);

}

void Lire()
{
/*
 BSTR *nom;
 tmp = iflash->get_Movie(nom);

 long num_trame;
 tmp = iflash->put_FrameNum(10);
 tmp = iflash->get_FrameNum(&num_trame);

 tmp = iflash->get_WMode(nom);

 tmp = iflash->get_Base(nom);

 tmp = iflash->Play();
 tmp = iflash->put_Playing(TRUE);

 tmp = iflash->GotoFrame(10);
 viewobject->Draw(DVASPECT_CONTENT, -1, NULL, NULL, NULL, hdcCompatible, &rectl, &rectl, NULL, NULL);
 
 long nb_trames = 0;
 tmp = iflash->get_TotalFrames(&nb_trames);

 VARIANT_BOOL j = 1;
 tmp = iflash->IsPlaying(&j);
*/ 
}

int DrawFrame(){
	static long lastframe=-1;
	long frame = 0;
	tmp = iflash->CurrentFrame(&frame);
	
	if (frame!=lastframe){
		lastframe=frame;
		viewobject->Draw(DVASPECT_CONTENT, -1, NULL, NULL, NULL, hdcCompatible, &rectl, &rectl, NULL, NULL); 
		return 1;
	}
	return 0;
}

}//namespace