#include <stdio.h>
#include <file_manager.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <data_manager.h>
#include <prompt_manager.h>

void list_forge_files(char **files, int count) {
    for (int i = 0; i < count; i++) {
        printf("%i. %s\n", i + 1, files[i]);
    }
}

void forge(void) {
    printf("Available modes:\n");

    char home[1024];
    get_wordforge_home(home, sizeof(home));

    char forge_path[1024];
    snprintf(forge_path, sizeof(forge_path), "%s/forge", home);

    int count = 0;
    char **files = get_files_from_folder(forge_path, &count);
    list_forge_files(files, count);

    int selected_type;
    printf("Select mode: ");
    scanf("%d", &selected_type);
    selected_type--;

    // Check the option is valid.
    if (selected_type < 0 || selected_type >= count) {
        fprintf(stderr, "Invalid selection, please pick a number between 1 and %d.\n", count);
        exit(1);
    }

    char type[256];
    strncpy(type, files[selected_type], sizeof(type));

    char forge_file[1024];
    snprintf(forge_file, sizeof(forge_file), "%s/%s", forge_path, files[selected_type]);
    for (int i = 0; i < count; i++) {
        free(files[i]);
    }
    free(files);

    char line[512];
    int difficulty;

    get_random_prompt(forge_file, line, sizeof(line), &difficulty);

    char prompt[512];
    char type_line[512];
    char difficulty_line[512];
    snprintf(prompt, sizeof(prompt), "PROMPT: %s", line);
    snprintf(type_line, sizeof(type_line), "TYPE: %s", type);
    snprintf(difficulty_line, sizeof(difficulty_line), "DIFFICULTY (1 - 3): %d", difficulty);


    const char *lines[] = {
        "==============",
        prompt,
        type_line,
        difficulty_line,
        "==============",
        "",
        ""
    };

    // Get filepath for file (create a new one with the unix timestamp)
    // Check if the editor variable is set.
    // If it isn't then default to nano and open it.
    char archive_path[1024];
    time_t unix_datetime;
    unix_datetime = time(NULL);
    snprintf(archive_path, sizeof(archive_path), "%s/archive/%ld", home, unix_datetime);

    // Create a new file and write the lines
    create_new_file(archive_path, lines, 6);
    
    // Open the editor
    char* EDITOR = getenv("EDITOR");
    if (EDITOR == NULL) {
        printf("Warning: You have not set an EDITOR environment variable. Using the default 'nano'.\n");
        EDITOR = "nano";
    }

    char command[512];
    snprintf(command, sizeof(command), "%s %s", EDITOR, archive_path);
    system(command);
    add_forge_count();
    int streak = update_streak();
    

    printf("Forge complete!\n\n");
    printf("+20 XP (forge)\n");

    if (difficulty == 1) {
        printf("+0 XP (Easy difficulty)");
    } else if (difficulty == 2)
    {
        printf("+10 XP (Medium difficulty)");
        add_xp(10);
    } else if (difficulty == 3)
    {
        printf("+20 XP (Hard difficulty)");
        add_xp(20);
    }
    

    int total_xp = add_xp(20);

    printf("\n");
    printf("Total gained: 20 XP\n");
    printf("Current streak: %d\n", streak);
    printf("Total XP: %d\n", total_xp);

    return;
}