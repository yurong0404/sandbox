all:hw2

hw2:main.c sandbox.c
	gcc -o sandbox.so -shared -fPIC sandbox.c -ldl
	gcc -o sandbox -Wall -g main.c lib.c

clean:
	rm sandbox sandbox.so
	
