all: gfid-list

gfid-list: gfid-list.c
	gcc -o gfid-list gfid-list.c