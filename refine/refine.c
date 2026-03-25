#include <file_manager.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <data_manager.h>


// Get all files in the archive folder
// Choose a random file from the folder
// Open it.

void refine() {
    char home[1024];
    get_wordforge_home(home, sizeof(home));

    char archive_path[1024];
    snprintf(archive_path, sizeof(archive_path), "%s/archive", home);

    int count = 0;
    char** files = get_files_from_folder(archive_path, &count);

    int n = 0;
    char *chosen_file = NULL;
    for (int i = 0; i < count; i++) {
        n++;
        if(rand() % n == 0) {
            chosen_file = files[i];
        }
    }

    char file_path[1024];
    snprintf(file_path, sizeof(file_path), "%s/%s", archive_path, chosen_file);

    // Open the editor
    char* EDITOR = getenv("EDITOR");
    if (EDITOR == NULL) {
        printf("Warning: You have not set an EDITOR environment variable. Using the default 'nano'.\n");
        EDITOR = "nano";
    }

    char command[512];
    snprintf(command, sizeof(command), "%s %s", EDITOR, file_path);
    system(command);
    add_refine_count();
    int streak = update_streak();
    int total_xp = add_xp(25);

    printf("Refine complete!\n\n");
    printf("+25 XP (refine)\n\n");
    printf("Total gained: 25 XP\n");
    printf("Current streak: %d\n", streak);
    printf("Total XP: %d\n", total_xp);
    return;
}