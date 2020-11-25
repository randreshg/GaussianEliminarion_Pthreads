parallelgaussianelimination: utils/utils.cpp gaussian.cpp
	g++ -pthread utils/utils.cpp gaussian.cpp -o gaussian

#fractaling.o: fractaling.c
#	gcc -c -pthread fractaling.c

#tpool.o: tpool.c
#	gcc -c -pthread tpool.c

#xmem.o: xmem.c
#	gcc -c xmem.c

#clean:
#	rm *.o fractaling fractaling.ppm
