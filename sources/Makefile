OBJS := \
	auxiliar.o           configuration.o     encoder.o \
	enemies.o            fonts.o       	 game.o \
	main.o               maps.o              replays.o \
	state_changepack.o   state_chooseship.o  state_endsequence.o \
	state_game.o         state_gameover.o    state_instructions.o \
	state_interphase.o   state_keyredefinition.o \
	state_levelfinished.o state_logo.o       state_mainmenu.o \
	state_replay.o       state_replaymanager.o \
	state_typetext.o     tiles.o             transball.o \
	sge/sge_blib.o 		 sge/sge_collision.o sge/sge_misc.o \
	sge/sge_primitives.o sge/sge_rotation.o  sge/sge_shape.o \
	sge/sge_surface.o

all: stransball2

%.o: %.cpp
	c++ -c -g3 -O3 $< -o $@ `sdl-config --cflags` -I/usr/local/include/SDL -I/opt/local/include/SDL -Isge

# dynamically linked binary:
stransball2: $(OBJS)
	c++ $^ -o $@ `sdl-config --libs` -lSDL_image -lSDL_mixer -lSDL_sound -I/usr/local/include/SDL
	mv ./stransball2 ..

clean:
	rm -f stransball2
	rm -f *.o
