all: gfid-list gfid-delete

gfid-list: gfid-list.c
	gcc -o gfid-list gfid-list.c

gfid-delete: gfid-delete.c
	gcc -o gfid-delete gfid-delete.c