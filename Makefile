UnixShell: UnixShell.o
	gcc -o ./bin/UnixShell.exe UnixShell.o

UnixShell.o: UnixShell.c UnixShell.h
	gcc -c -Wall UnixShell.c

clean:
	rm *.o
	rm ./bin/*.exe
