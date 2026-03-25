#include <stdio.h>
#include <forge.h>
#include <strings.h>
#include <time.h>
#include <stdlib.h>
#include <refine.h>
#include <data_manager.h>

// Routes subcommands to the appropriate files.
int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Wordforge v1.0 - Created by Marcus Cornes\n");
        printf("Usage: wordforge [subcommand]\n\n");
        printf("forge - Write something.\n");
        printf("refine - Improve one of your archived texts.\n");
        printf("archive - View texts in your archive.\n");
        printf("lore - Opens the reference guide.\n");
        printf("stats - View your statistics.\n");

        return 1;
    }

    srand(time(NULL));
    if (strcasecmp(argv[1], "forge") == 0) {
        forge();
    } else if (strcasecmp(argv[1], "refine") == 0)
    {
        refine();
    }
    

    return 0;
}