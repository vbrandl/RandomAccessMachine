all:
	gcc -Wall -W -Wstrict-prototypes -Wreturn-type -O -fshort-enums -o ram ram.c

clean:
	rm ram
