#include "stdio.h"

#include "SDL.h"

extern SDLKey THRUST_KEY,ANTITHRUST_KEY,LEFT_KEY,RIGHT_KEY;
extern SDLKey FIRE_KEY,ATRACTOR_KEY;
extern SDLKey PAUSE_KEY;

extern bool fullscreen;
extern int PIXEL_SIZE;


bool load_configuration(void)
{
	int a,b,c,d,e,f,g;
	FILE *fp;

	fp=fopen("transball.cfg","r");

	if (fp==0) return false;

	if (7!=fscanf(fp,"%i %i %i %i %i %i %i",&a,&b,&c,&d,&e,&f,&g)) {
		fclose(fp);
		return false;
	} /* if */ 

	if (2!=fscanf(fp,"%i %i",&fullscreen,&PIXEL_SIZE)) {
		fclose(fp);
		return false;
	} /* if */ 

	THRUST_KEY=(SDLKey)a;
	ANTITHRUST_KEY=(SDLKey)b;
	LEFT_KEY=(SDLKey)c;
	RIGHT_KEY=(SDLKey)d;
	FIRE_KEY=(SDLKey)e;
	ATRACTOR_KEY=(SDLKey)f;
	PAUSE_KEY=(SDLKey)g;

	fclose(fp);
	return true;
} /* load_configuration */ 


void save_configuration(void)
{
	FILE *fp;

	fp=fopen("transball.cfg","w");
	
	if (fp==0) return;

	fprintf(fp,"%i %i %i %i %i %i %i\n",(int)THRUST_KEY,(int)ANTITHRUST_KEY,(int)LEFT_KEY,(int)RIGHT_KEY,
									    (int)FIRE_KEY,(int)ATRACTOR_KEY,(int)PAUSE_KEY);
	fprintf(fp,"%i %i\n",(int)fullscreen,PIXEL_SIZE);

	fclose(fp);

} /* save_configuration */ 

