wmake : hashmap.o wmake.c wmake.h
	gcc hashmap.o wmake.c -o wmake

hashmap.o : hashmap.h hashmap.c
	gcc hashmap.c -c -o hashmap.o

clean :
	rm -f *.o wmake
