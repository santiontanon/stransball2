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


bool state_mainmenu_cycle(SDL_Surface *screen,int sx,int sy,unsigned char *keyboard)
{
	if (SUBSTATE==0) {
		if (image!=0) SDL_FreeSurface(image);
		image=IMG_Load("graphics/tittle.pcx");
		SDL_SetColorKey(image,SDL_SRCCOLORKEY,SDL_MapRGB(image->format,0,0,0));
		demotimer=0;
		demoon=false;
	} /* if */ 

	if (demoon) {
		int retval;
		int i;
		unsigned char tmp[SDLK_LAST];

		for(i=0;i<SDLK_LAST;i++) tmp[i]=0;
		tmp[THRUST_KEY]=fgetc(replayfile);
		tmp[ANTITHRUST_KEY]=fgetc(replayfile);
		tmp[LEFT_KEY]=fgetc(replayfile);
		tmp[RIGHT_KEY]=fgetc(replayfile);
		tmp[FIRE_KEY]=fgetc(replayfile);
		tmp[ATRACTOR_KEY]=fgetc(replayfile);
		game->cycle(tmp);
		retval=fgetc(replayfile);

		game->render(screen,sx,sy);

		if (retval!=0) {
			delete game;
			game=0;

			fclose(replayfile);
			replayfile=0;
			demoon=false;
		} /* if */ 
	} else {
		SDL_FillRect(screen,0,0);
	} /* if */ 

	if (demoon) {
		SDL_SetAlpha(image,SDL_SRCALPHA,tittle_alpha--);
		if (tittle_alpha<=0) tittle_alpha=0;
	} else {
		SDL_SetAlpha(image,SDL_SRCALPHA,tittle_alpha);
		tittle_alpha+=2;
		if (tittle_alpha>=255) tittle_alpha=255;
	} /* if */ 
	SDL_BlitSurface(image,0,screen,0);

	{
		SDL_Rect r;

		r.x=SCREEN_X/2-80;
		r.w=160;
		r.y=sy-80;
		r.h=64;
		surface_fader(screen,0.5F,0.5F,0.5F,-1,&r);

		r.x=0;
		r.w=SCREEN_X;
		r.y=sy-16;
		r.h=16;
		surface_fader(screen,0.5F,0.5F,0.5F,-1,&r);
	}

	font_print(SCREEN_X/2-72,sy-72,"FIRE - START GAME",screen);
	font_print(SCREEN_X/2-72,sy-64," C   - ENTER CODE",screen);
	font_print(SCREEN_X/2-72,sy-56," L   - CHANGE LEVEL-PACK",screen);
	font_print(SCREEN_X/2-72,sy-48," K   - REDEFINE KEYBOARD",screen);
	font_print(SCREEN_X/2-72,sy-40," I   - INSTRUCTIONS",screen);
	font_print(SCREEN_X/2-72,sy-32," R   - REPLAYS",screen);
	font_print(SCREEN_X/2-72,sy-24,"ESC  - QUIT GAME",screen);

	{
		char tmp[256];

		sprintf(tmp,"LEVELPACK: %s",levelpack);
		font_print(SCREEN_X/2-strlen(tmp)*3,sy-12,tmp,screen);
	}

	if (SUBSTATE<32) {
		surface_fader(screen,float(SUBSTATE)/32.0F,float(SUBSTATE)/32.0F,float(SUBSTATE)/32.0F,-1,0);
		SUBSTATE++;
	} /* if */ 
	if (SUBSTATE==32) {
		if (!demoon) demotimer++;
		if (demotimer>=256) {
			int i;
			char tmp[80];
			char levelname[256];
			int fuel;
			int v1,v2;

			
			demoon=true;
			demotimer=0;
			tittle_alpha=255;

			sprintf(tmp,"demos/demo%i.rpl",((rand()%40)/10)+1);
			replayfile=fopen(tmp,"rb");
			v1=fgetc(replayfile);
			v2=fgetc(replayfile);	// To maintain compatibility with a previous version

			level=0;
			for(i=0;i<256;i++) levelname[i]=fgetc(replayfile);
			fuel=fgetc(replayfile);

			ship_type=fgetc(replayfile);
			if (game!=0) delete game;
			game=new TRANSBALL("graphics/","sound/","maps/",fuel,levelname,ship_type);
		} /* if */ 

		if (keyboard[FIRE_KEY] && !old_keyboard[FIRE_KEY]) {
			SUBSTATE++;
			SUBSTATE2=0;
			timer=0;
		} /* if */ 
		if (keyboard[SDLK_ESCAPE] && !old_keyboard[SDLK_ESCAPE]) {
			SUBSTATE++;
			SUBSTATE2=1;
		} /* if */ 
		if (keyboard[SDLK_i] && !old_keyboard[SDLK_i]) {
			SUBSTATE++;
			SUBSTATE2=2;
		} /* if */ 
		if (keyboard[SDLK_c] && !old_keyboard[SDLK_c]) {
			SUBSTATE++;
			SUBSTATE2=3;
		} /* if */ 
		if (keyboard[SDLK_r] && !old_keyboard[SDLK_r]) {
			SUBSTATE++;
			SUBSTATE2=4;
		} /* if */ 
		if (keyboard[SDLK_k] && !old_keyboard[SDLK_k]) {
			SUBSTATE++;
			SUBSTATE2=5;
		} /* if */ 
		if (keyboard[SDLK_l] && !old_keyboard[SDLK_l]) {
			SUBSTATE++;
			SUBSTATE2=6;
		} /* if */ 
	} /* if */ 
	if (SUBSTATE>32) {
		surface_fader(screen,float(64-SUBSTATE)/32.0F,float(64-SUBSTATE)/32.0F,float(64-SUBSTATE)/32.0F,-1,0);
		SUBSTATE++;
		if (SUBSTATE==64) {
			SDL_FreeSurface(image);
			image=0;

			if (game!=0) delete game;
			game=0;
			if (replayfile!=0) fclose(replayfile);
			replayfile=0;
			demoon=false;
			demotimer=0;

			if (SUBSTATE2==0) {
				int i;

				for(i=0;i<1000;i++) {
					char tmp[80];
					sprintf(tmp,"replays/replay%.3i.rpl",i);
					remove(tmp);
				} /* for */ 

				STATE=4;
				SUBSTATE=0;
				level=0;
				replaynum=0;
			} /* if */ 
			if (SUBSTATE2==1) {
				return false;
			} /* if */ 
			if (SUBSTATE2==2) {
				STATE=2;
				SUBSTATE=0;
			} /* if */ 
			if (SUBSTATE2==3) {
				STATE=3;
				SUBSTATE=0;
				SUBSTATE2=0;
				edit_text[0]=0;
				edit_position=0;
			} /* if */ 
			if (SUBSTATE2==4) {
				STATE=7;
				SUBSTATE=0;
			} /* if */ 
			if (SUBSTATE2==5) {
				STATE=11;
				SUBSTATE=0;
			} /* if */ 
			if (SUBSTATE2==6) {
				STATE=12;
				SUBSTATE=0;
			} /* if */ 
		} /* if */ 
	} /* if */ 


	return true;
} /* state_mainmenu_cycle */ 
