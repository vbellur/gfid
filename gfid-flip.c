/*
 * This program flips one of the bytes in the gfid of a file. This is
 * useful to setup a 'fake' mismatch for testing purposes.
 */

#include <sys/xattr.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <errno.h>

#define GLUSTER_GFID_KEY	"trusted.gfid"
#define GLUSTER_GFID_SIZE	16

#define GFID_FORMAT_STR         "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x"
#define GFID_VALUE_STR(v)       v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7], v[8], v[9], v[10], v[11], v[12], v[13], v[14], v[15]

void usage (char *prog)
{
	fprintf (stderr, "Usage: %s <filename>\n", prog);
}


int main (int argc, char **argv)
{
	int ret        = 0;
	char *iname    = NULL;

	unsigned char gfid[GLUSTER_GFID_SIZE];

	if (argc != 2) {
		usage (argv[0]);
		exit (1);
	}

	iname = argv[1];

	ret = lgetxattr (iname, GLUSTER_GFID_KEY, gfid, GLUSTER_GFID_SIZE);
	if (ret == -1) {
		fprintf (stderr, "Error getting gfid: %s\n", strerror (errno));
		exit (1);
	}

	printf ("Old gfid: " GFID_FORMAT_STR "\n", GFID_VALUE_STR(gfid));

	gfid[0]++;

	printf ("New gfid: " GFID_FORMAT_STR "\n", GFID_VALUE_STR(gfid));

	ret = lsetxattr (iname, GLUSTER_GFID_KEY, gfid, GLUSTER_GFID_SIZE, 0);
	if (ret == -1) {
		fprintf (stderr, "Error setting gfid: %s\n", strerror (errno));
		exit (1);
	}

	return 0;
}
