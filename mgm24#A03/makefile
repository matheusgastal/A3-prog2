# Flags do Allegro (pegas dinamicamente via pkg-config)
ALLEGRO_FLAGS = $(shell pkg-config --cflags --libs allegro-5 allegro_font-5 allegro_image-5 allegro_primitives-5 allegro_ttf-5 allegro_audio-5 allegro_acodec-5 allegro_dialog-5 allegro_main-5)

# Outras flags suas (adicione aqui se quiser)
FLAGS = -Wall -g

all: main

main: main.o personagem.o Joystick.o telas.o functions.o tiros.o inimigos.o 
	gcc $(FLAGS) -o main main.o personagem.o Joystick.o telas.o functions.o tiros.o inimigos.o $(ALLEGRO_FLAGS) -lm
	
main.o: main.c personagem.h Joystick.h telas.h functions.h inimigos.h 
	gcc $(FLAGS) -c main.c $(ALLEGRO_FLAGS)

personagem.o: personagem.c personagem.h Joystick.h inimigos.h telas.h
	gcc $(FLAGS) -c personagem.c $(ALLEGRO_FLAGS)

Joystick.o: Joystick.c Joystick.h 
	gcc $(FLAGS) -c Joystick.c $(ALLEGRO_FLAGS)

telas.o: telas.c telas.h functions.h Joystick.h tiros.h inimigos.h telas.h
	gcc $(FLAGS) -c telas.c $(ALLEGRO_FLAGS) 

functions.o: functions.c functions.h Joystick.h inimigos.h telas.h
	gcc $(FLAGS) -c functions.c $(ALLEGRO_FLAGS)

tiros.o: tiros.c tiros.h Joystick.h personagem.h telas.h inimigos.h telas.h
	gcc $(FLAGS) -c tiros.c $(ALLEGRO_FLAGS)

inimigos.o: inimigos.c inimigos.h personagem.h telas.h telas.h

clean:
	rm -f *.o main
