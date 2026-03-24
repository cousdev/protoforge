#include <stdio.h>
#include <file_manager.h>

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
    char **files = get_forge_files(forge_path, &count);
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

    printf("\n--- Mode: %s ---\n", files[selected_type]);

    char forge_file[1024];
    snprintf(forge_file, sizeof(forge_file), "%s/%s", forge_path, files[selected_type]);
    for (int i = 0; i < count; i++) {
        free(files[i]);
    }
    free(files);

    char *line = get_random_line(forge_file);
    printf("Prompt: %s", line);
}