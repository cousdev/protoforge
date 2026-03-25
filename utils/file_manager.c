#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <time.h>

void get_wordforge_home(char *buf, size_t size) {
    char *home = getenv("HOME");
    if (!home) {
        fprintf(stderr, "Could not determine your home directory. Is the $HOME environment variable set?\n");
        exit(1);
    }

    snprintf(buf, size, "%s/wordforge", home);

    return;
}

char** get_files_from_folder(const char *PATH, int *count) {
    DIR *d = opendir(PATH);
    if (!d) {
        perror("Failed to open the directory.");
        exit(1);
    }

    // Count all of the files in the directory
    int n = 0;
    struct dirent *dir;
    while ((dir = readdir(d)) != NULL) {
        if (dir->d_name[0] == '.') continue;
        if (dir->d_type != DT_REG) continue;
        n++;
    }


    // Allocate an array of n string pointers
    char **files = malloc(n * sizeof(char *));
    if (!files) {
        fprintf(stderr, "Failed to allocate memory to get files.\n");
        exit(1);
    }

    // Second pass - Fill the array
    rewinddir(d);
    int i = 0;
    while((dir = readdir(d)) != NULL) {
        if (dir->d_name[0] == '.') continue;
        if (dir->d_type != DT_REG) continue;
        files[i] = malloc(strlen(dir->d_name) + 1);
        strcpy(files[i], dir->d_name);
        i++;
    }

    closedir(d);
    *count = n;
    return files;
}

char* get_random_line(const char *PATH) {
    FILE *f = fopen(PATH, "r");
    if (!f) {
        perror("Failed to open file.");
        exit(1);
    }

    char *chosen = NULL;
    char line[1024];
    int n = 0;

    while (fgets(line, sizeof(line), f)) {
        n++;
        if (rand() %n == 0) {
            free(chosen);
            chosen = malloc(strlen(line) + 1);
            strcpy(chosen, line);
        }
    }

    fclose(f);

    // Strip trailing newline
    if (chosen) {
        size_t len = strlen(chosen);
        if (len > 0 && chosen[len - 1] == '\n') {
            chosen[len - 1] = '\0';
        }
    }

    return chosen;
}

void create_new_file(const char *PATH, const char** LINES, int count) {
    FILE *f = fopen(PATH, "w");
    if (!f) {
        perror("Failed to create a new file.");
        exit(1);
    }

    for (int i = 0; i < count; i++) {
        fprintf(f, "%s\n", LINES[i]);
    }

    fclose(f);
}

