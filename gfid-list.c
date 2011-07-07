/*
 * (C) 2011 Gluster
 * Author: Vikas Gorur <vikas@gluster.com>
 *
 *
 * gfid-list: Take a text file with a list of files and print the gfid for each of them.
 *
 * For each file in the list, print one of:
 *    the gfid
 *    "inode_missing"       - The inode doesn't exist (ENOENT).
 *    "xattr_missing_key"   - The gfid attribute doesn't exist (ENODATA).
 *    "xattr_missing_value" - The gfid attribute exists but has no value set.
 *
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

#define GFID_FORMAT_STR         "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x"
#define GFID_VALUE_STR(v)       v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7], v[8], v[9], v[10], v[11], v[12], v[13], v[14], v[15]


void usage (char *str)
{
        fprintf (stderr, "Usage: %s /path/to/master_list\n", str);
        fprintf (stderr, "\nTo ensure that you can use the output with gfid-mismatch,\ncd into the export directory first and then run:\n\n# gfid-list /path/to/master_list\n\n");
}


void gfid_for_list (FILE *master_list)
{
        char iname[4096];
        unsigned char gfid[GLUSTER_GFID_SIZE];
        int ret = -1;

        while ((fgets (iname, 4096, master_list)) != NULL) {
                int  error      = 0;
                char *error_str = NULL;

                char *newline = strrchr (iname, '\n');
                *newline      = '\0';

                ret = lgetxattr (iname, GLUSTER_GFID_KEY, gfid, GLUSTER_GFID_SIZE);
                if (ret == -1) {
                        if (errno == ENOENT) {
                                error     = 1;
                                error_str = "inode_missing";
                        } else if (errno = ENODATA) {
                                error     = 1;
                                error_str = "xattr_missing_key";
                        } else {
                                fprintf (stderr, "Unexpected error on lgetxattr(): %s\n", strerror (errno));
                                exit (1);
                        }
                }

                if (ret == 0) {
                        error     = 1;
                        error_str = "xattr_missing_value";
                }

                if (error) {
                        printf ("%s %s\n", error_str, iname);
                } else {
                        printf (GFID_FORMAT_STR " %s\n", GFID_VALUE_STR(gfid), iname);
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

        gfid_for_list (master_list);

        return 0;
}
