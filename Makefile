all: gfid-list gfid-delete gfid-unlink

gfid-list: gfid-list.c
	gcc -o gfid-list gfid-list.c

gfid-unlink: gfid-unlink.c
	gcc -o gfid-unlink gfid-unlink.c

gfid-delete: gfid-delete.c
	gcc -o gfid-delete gfid-delete.c

gfid-flip: gfid-flip.c
	gcc -o gfid-flip gfid-flip.c
