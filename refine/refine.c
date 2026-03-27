#include <file_manager.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <data_manager.h>
#include <prompt_manager.h>
#include <time.h>



// Read the archive.json
// Choose a random file
// Get the latest file revision
// Open it.
void refine() {
    char home[1024];
    get_wordforge_home(home, sizeof(home));

    char archive_path[1024];
    snprintf(archive_path, sizeof(archive_path), "%s/archive", home);

    char chosen_file[32];
    char title[256];
    get_random_file(chosen_file, title);

    char file_path[1024];
    snprintf(file_path, sizeof(file_path), "%s/%s", archive_path, chosen_file);

    // Read the file from file_path, copy it with a new unix timestamp, open it.
    FILE *source = fopen(file_path, "r");
    if (source == NULL) {
        printf("Cannot open file %s\n", file_path);
        exit(1);
    }

    // Read entire file into memory
    fseek(source, 0, SEEK_END);
    long size = ftell(source);
    fseek(source, 0, SEEK_SET);

    char *buffer = malloc(size + 1);
    if (!buffer) {
        fclose(source);
        printf("Memory allocation failed\n");
        return;
    }

    fread(buffer, 1, size, source);
    buffer[size] = '\0';
    fclose(source);

    // Create new filename using Unix timestamp
    time_t now = time(NULL);
    char new_name[64];
    snprintf(new_name, sizeof(new_name), "%ld", now);

    // Build full path for new file
    char new_path[1024];
    snprintf(new_path, sizeof(new_path), "%s/%s", archive_path, new_name);

    // Write copied text to new file
    write_text(new_path, buffer);

    free(buffer);

    // Open the editor
    char* EDITOR = getenv("EDITOR");
    if (EDITOR == NULL) {
        printf("Warning: You have not set an EDITOR environment variable. Using the default 'nano'.\n");
        EDITOR = "nano";
    }

    char command[512];
    snprintf(command, sizeof(command), "%s %s", EDITOR, new_path);
    system(command);
    save_to_archive(title, new_name);
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