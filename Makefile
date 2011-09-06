all: gfid-list gfid-delete

gfid-list: gfid-list.c
	gcc -o gfid-list gfid-list.c

gfid-delete: gfid-delete.c
	gcc -o gfid-delete gfid-delete.c

gfid-flip: gfid-flip.c
	gcc -o gfid-flip gfid-flip.c