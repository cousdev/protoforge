#include <stdio.h>
#include <forge.h>
#include <strings.h>
#include <time.h>
#include <stdlib.h>
#include <refine.h>
#include <data_manager.h>
#include <stats.h>
#include <colour.h>
#include <archive.h>

void show_options(void) {
    printf("Wordforge v1.0 - Created by Marcus Cornes\n");
    printf("Usage: wordforge [subcommand]\n\n");
    printf("forge - Write something.\n");
    printf("refine - Improve one of your archived texts.\n");
    printf("archive - View texts in your archive.\n");
    printf("stats - View your statistics.\n");

    return;
}

void show_archive_options(void) {
    printf("Wordforge v1.0 - Created by Marcus Cornes\n");
    printf("Archive Options:\n\n");
    printf("archive list - List all memoirs in the archive.\n");
    printf("archive revisions [number] - List the revisions of a text.\n");
    printf("archive refine [number] - Refines a text (from the latest revision).\n");
    printf("archive open [number] [version] - Opens a text at a specified revision in read-only mode.\n");

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
    } else if (strcasecmp(argv[1], "archive") == 0) {
        if (argc <= 2) {
            show_archive_options();
            return 1;
        }

        if (strcasecmp(argv[2], "list") == 0) {
            archive_list();
        } else if (strcasecmp(argv[2], "revisions") == 0) {
            if (argc <= 3) {
                fprintf(stderr, "Usage: archive revisions [number]\n");
                return 1;
            }
            int index = atoi(argv[3]) - 1;
            archive_revisions(index);
        } else if (strcasecmp(argv[2], "refine") == 0) {
            if (argc <= 3) {
                fprintf(stderr, "Usage: archive refine [number]\n");
                return 1;
            }
            int index = atoi(argv[3]) - 1;
            archive_refine(index);
        } else if (strcasecmp(argv[2], "open") == 0) {
            if (argc <= 4) {
                fprintf(stderr, "Usage: archive open [number] [version]\n");
                return 1;
            }
            int index = atoi(argv[3]) - 1;
            int version = atoi(argv[4]) - 1;
            archive_open(index, version);
        } else {
            fprintf(stderr, "Unknown archive command: %s\n", argv[2]);
            show_archive_options();
            return 1;
        }
    }
    else {
        printf(RED "ERROR: Unknown command.\n\n" RESET);
        show_options();
        return 1;
    }
    

    return 0;
}