/*
 * Author: Thong Nguyen (tumtumtum@gmail.com)
 *
 *
 * gfid-unlink: Take a text file with a list of gfids and deletes the links from the 
 * hidden .glusterfs directory
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

#define PATH_FORMAT_STR		"./%c%c/%c%c/%c%c%c%c%c%c%c%c-%c%c%c%c-%c%c%c%c-%c%c%c%c-%c%c%c%c%c%c%c%c%c%c%c%c"
#define PATH_VALUE_STR(v)	v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7], v[8], v[9], v[10], v[11], v[12], v[13], v[14], v[15], v[16], v[17], v[18], v[19], v[20], v[21], v[22], v[23], v[24], v[25], v[26], v[27], v[28], v[29], v[30], v[31]

void usage (char *str)
{
        fprintf (stderr, "Usage: %s /path/to/brick_mismatch_gfid_list\n", str);
        fprintf (stderr, "\nMake sure you cd into the .glusterfs directory first and then run:\n\n# %s /path/to/brick_mismatch_gfid_list\n\n", str);
}


void gfid_unlink (FILE *master_list)
{
        char line[4096];
        char path[4096];
        unsigned char gfid[GLUSTER_GFID_SIZE];
        int ret = -1;

        while ((fgets (line, 4096, master_list)) != NULL) {
                int  error      = 0;
                char *error_str = NULL;

                if (!((line[0] >= 'a' && line[0] <= 'f') || (line[0] >= '0' && line[0] <= '9'))) continue;

                line[32] = '\0';
		
                sprintf(path, PATH_FORMAT_STR, line[0], line[1], line[2], line[3], PATH_VALUE_STR(line));

                ret = unlink(path);

                if (ret == -1) {
                        fprintf (stderr, "%s: %s\n", path, strerror(errno));
                } else {
                        fprintf (stderr, "%s: unlinked OK\n", path);
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
                fprintf (stderr, "Could not open brick mismatched gfid list: %s\n", strerror (errno));
                return 1;
        }

        gfid_unlink (master_list);

        return 0;
}
