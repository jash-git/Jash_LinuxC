#include <ftw.h>
#include <fnmatch.h>
#include <stdio.h>
// google :linux c++ tree list files in directory
//http://stackoverflow.com/questions/983376/recursive-folder-scanning-in-c
/*
FTW_F
fpath is a regular file.
FTW_D
fpath is a directory.
FTW_DNR
fpath is a directory which can't be read.
FTW_NS
The stat(2) call failed on fpath, which is not a symbolic link.
*/
static const char *filters[] = {
    "*.cpp","*.h"
};

static int callback(const char *fpath, const struct stat *sb, int typeflag) {
    /* if it's a file */
    if (typeflag == FTW_F) {
        int i;
        /* for each filter, */
        for (i = 0; i < sizeof(filters) / sizeof(filters[0]); i++) {
            /* if the filename matches the filter, */
            if (fnmatch(filters[i], fpath, FNM_CASEFOLD) == 0) {
                /* do something */
                printf("found image: %s\n", fpath);
                break;
            }
        }
    }

    /* tell ftw to continue */
    return 0;
}

int main() {
    ftw(".", callback, 16);
}
