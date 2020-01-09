#ifdef _WIN32
#include "windows.h"
#else
#include <stddef.h>
#include <sys/types.h>
#include <dirent.h>
#include "ctype.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SDL/SDL.h"
#include "SDL_mixer.h"
#include "SDL_image.h"

#include "fonts.h"
#include "list.h"
#include "auxiliar.h"

#include "tiles.h"
#include "maps.h"
#include "transball.h"

#include "encoder.h"

#define MAXLEVELS	64

extern int NLEVELS;
extern int SCREEN_X,SCREEN_Y;
extern int STATE,SUBSTATE,SUBSTATE2;
extern int level,timer;
extern int ship_type;
extern int fuelfactor[3];
extern int used_fuel,remaining_fuel;
extern int n_shots,n_hits,enemies_destroyed;
extern int previous_high;
extern TRANSBALL *game;
extern int frames_per_sec;
extern FILE *replayfile;
extern int replaynum;
extern int replay_source;
extern SDLKey THRUST_KEY,ANTITHRUST_KEY,LEFT_KEY,RIGHT_KEY;
extern SDLKey FIRE_KEY,ATRACTOR_KEY;
extern SDLKey PAUSE_KEY;
extern bool pause;
extern unsigned char old_keyboard[SDLK_LAST];
extern SDL_Surface *image,*image2;
extern char edit_text[80];
extern int edit_position;
extern List<char> files;
extern int act_file;
extern int first_file;
extern bool refind_files;
extern int demotimer;
extern bool demoon;
extern int tittle_alpha;
extern List<char> levelpacks;
extern int act_levelpack;
extern char levelpack[256];
extern char *levelnames[MAXLEVELS];
extern char *leveltext[MAXLEVELS];
extern char *levelcode[MAXLEVELS];
extern int initialfuel[MAXLEVELS];


bool state_keyredefinition_cycle(SDL_Surface *screen,int sx,int sy,unsigned char *keyboard)
{
	int i;

	if (SUBSTATE==0) {
		if (image!=0) SDL_FreeSurface(image);
		if (image2!=0) SDL_FreeSurface(image);
		image=IMG_Load("graphics/tittle.pcx");
	} /* if */ 

	SDL_BlitSurface(image,0,screen,0);
	surface_fader(screen,0.5F,0.5F,0.5F,-1,0);

	font_print_centered(sx/2,40,"REDEFINE KEYBOARD:",screen);

	if (SUBSTATE<32) {
		surface_fader(screen,float(SUBSTATE)/32.0F,float(SUBSTATE)/32.0F,float(SUBSTATE)/32.0F,-1,0);
		SUBSTATE++;
	} /* if */ 
	if (SUBSTATE>39) {
		surface_fader(screen,float(64-SUBSTATE)/32.0F,float(64-SUBSTATE)/32.0F,float(64-SUBSTATE)/32.0F,-1,0);
		SUBSTATE++;
		if (SUBSTATE==64) {
			save_configuration();

			SDL_FreeSurface(image);
			image=0;

			STATE=1;
			SUBSTATE=0;
		} /* if */ 
	} /* if */ 

	if (SUBSTATE>=32) {
		bool found;
		char keystr[256];
		char *upstr;

		SUBSTATE-=32;
		if (SUBSTATE>=0) {
			font_print(16,60,"PRESS A KEY FOR THRUST:",screen);
		} /* if */ 
		if (SUBSTATE>=1) {
			strcpy(keystr,SDL_GetKeyName((SDLKey)THRUST_KEY)); 
			upstr=strupr(keystr);
			font_print(200,60,upstr,screen);
			font_print(16,70,"PRESS A KEY FOR ANTITHRUST:",screen);
		} /* if */ 
		if (SUBSTATE>=2) { 
                        strcpy(keystr,SDL_GetKeyName((SDLKey)ANTITHRUST_KEY));
                        upstr=strupr(keystr);
			font_print(200,70,upstr,screen);
			font_print(16,80,"PRESS A KEY FOR LEFT:",screen);
		} /* if */ 
		if (SUBSTATE>=3) {  
			strcpy(keystr,SDL_GetKeyName((SDLKey)LEFT_KEY));
			upstr=strupr(keystr);
			font_print(200,80,upstr,screen);
			font_print(16,90,"PRESS A KEY FOR RIGHT:",screen);
		} /* if */ 
		if (SUBSTATE>=4) { 
			strcpy(keystr,SDL_GetKeyName((SDLKey)RIGHT_KEY));
			upstr=strupr(keystr);
			font_print(200,90,upstr,screen);
			font_print(16,100,"PRESS A KEY FOR FIRE:",screen);
		} /* if */ 
		if (SUBSTATE>=5) { 
			strcpy(keystr,SDL_GetKeyName((SDLKey)FIRE_KEY));
			upstr=strupr(keystr);
			font_print(200,100,upstr,screen);
			font_print(16,110,"PRESS A KEY FOR ATRACTOR:",screen);
		} /* if */ 
		if (SUBSTATE>=6) {
			strcpy(keystr,SDL_GetKeyName((SDLKey)ATRACTOR_KEY));
			upstr=strupr(keystr);
			font_print(200,110,upstr,screen);
			font_print(16,120,"PRESS A KEY FOR PAUSE:",screen);
		} /* if */ 
		if (SUBSTATE>=7) { 
			strcpy(keystr,SDL_GetKeyName((SDLKey)PAUSE_KEY));
			upstr=strupr(keystr);
			font_print(200,120,upstr,screen);
			font_print(32,140,"PRESS ANY KEY",screen);
		} /* if */ 

		found=false;
		for(i=0;!found && i<SDLK_LAST;i++) {
			if (keyboard[i] && !old_keyboard[i]) {
				switch(SUBSTATE) {
				case 0:THRUST_KEY=(SDLKey)i;
					   SUBSTATE++;
					   found=true;
					   break;
				case 1:ANTITHRUST_KEY=(SDLKey)i;
					   SUBSTATE++;
					   found=true;
					   break;
				case 2:LEFT_KEY=(SDLKey)i;
					   SUBSTATE++;
					   found=true;
					   break;
				case 3:RIGHT_KEY=(SDLKey)i;
					   SUBSTATE++;
					   found=true;
					   break;
				case 4:FIRE_KEY=(SDLKey)i;
					   SUBSTATE++;
					   found=true;
					   break;
				case 5:ATRACTOR_KEY=(SDLKey)i;
					   SUBSTATE++;
					   found=true;
					   break;
				case 6:PAUSE_KEY=(SDLKey)i;
					   SUBSTATE++;
					   found=true;
					   break;
				} /* switch */ 
			} /* if */ 
		} /* for */ 

		if (SUBSTATE==7 && 
			((keyboard[SDLK_ESCAPE] && !old_keyboard[SDLK_ESCAPE]) ||
			 (keyboard[FIRE_KEY] && !old_keyboard[FIRE_KEY])
			)) {
			SUBSTATE++;
		} /* if */ 
		SUBSTATE+=32;
	} /* if */ 

	return true;
} /* state_keyredefinition_cycle */ 

