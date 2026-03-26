#include <stdio.h>
#include <forge.h>
#include <strings.h>
#include <time.h>
#include <stdlib.h>
#include <refine.h>
#include <data_manager.h>
#include <stats.h>

void show_options(void) {
    printf("Wordforge v1.0 - Created by Marcus Cornes\n");
    printf("Usage: wordforge [subcommand]\n\n");
    printf("forge - Write something.\n");
    printf("refine - Improve one of your archived texts.\n");
    printf("archive - View texts in your archive.\n");
    printf("lore - Opens the reference guide.\n");
    printf("stats - View your statistics.\n");

    return;
}


// Routes subcommands to the appropriate files.
int main(int argc, char *argv[]) {
    if (argc < 2) {
        show_options();
        return 1;
    }

    srand(time(NULL));
    if (strcasecmp(argv[1], "forge") == 0) {
        forge();
    } else if (strcasecmp(argv[1], "refine") == 0)
    {
        refine();
    } else if (strcasecmp(argv[1], "stats") == 0) {
        stats();
    } else {
        printf("ERROR: Unknown command.\n\n");
        show_options();
        return 1;
    }
    

    return 0;
}