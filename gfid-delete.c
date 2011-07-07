/*
 * (C) 2011 Gluster
 * Author: Vikas Gorur <vikas@gluster.com>
 *
 *
 * gfid-delete: Take a text file with a list of files and
 * delete the gfid for each of them.
 *
 * This program also logs the success or failure of deleting on each
 * file to stderr.
 */


#include <sys/xattr.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <errno.h>
#include <ftw.h>


#define GLUSTER_GFID_KEY        "trusted.gfid"
#define GLUSTER_GFID_SIZE       16


void usage (char *str)
{
        fprintf (stderr, "Usage: %s /path/to/master_list\n", str);
        fprintf (stderr, "\nMake sure you cd into the export directory first and then run:\n\n# gfid-delete /path/to/master_list\n\n");
}


void gfid_delete (FILE *master_list)
{
        char iname[4096];
        unsigned char gfid[GLUSTER_GFID_SIZE];
        int ret = -1;

        while ((fgets (iname, 4096, master_list)) != NULL) {
                int  error      = 0;
                char *error_str = NULL;

                char *newline = strrchr (iname, '\n');
                *newline      = '\0';

                ret = lremovexattr (iname, GLUSTER_GFID_KEY);
                if (ret == -1) {
                        fprintf (stderr, "%s: %s\n", iname, strerror (errno));
                } else {
                        fprintf (stderr, "%s: removed OK\n", iname);
                }
        }
}


int main (int argc, char **argv)
{
        FILE *master_list = NULL;

        if (argc != 2) {
                usage (argv[0]);
                return 1;
        }

        master_list = fopen (argv[1], "r");
        if (master_list == NULL) {
                fprintf (stderr, "Could not open master list: %s\n", strerror (errno));
                return 1;
        }

        gfid_delete (master_list);

        return 0;
}
