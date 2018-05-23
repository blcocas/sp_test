project : project.o
	gcc -o project project.o -lwiringPi -lwiringPiDev

project.o : project.c
	gcc -c project.c

clean : 
	rm  *.o project
