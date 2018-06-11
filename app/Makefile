app : app.o
	gcc -o app app.o -lwiringPi -lwiringPiDev
app.o : app.c
	gcc -c app.c

clean : 
	rm *.o app
