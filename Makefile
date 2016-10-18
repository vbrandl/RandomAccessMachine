all:
	gcc -Wall -W -Wstrict-prototypes -Wreturn-type -O -fshort-enums -o ram ram.c

debug:
	gcc -Wall -W -Wstrict-prototypes -Wreturn-type -O -fshort-enums -g -o ram ram.c

clean:
	rm ram
