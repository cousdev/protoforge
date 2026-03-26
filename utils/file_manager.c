#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <cJSON.h>

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

cJSON* read_json(const char *config_path) {
    FILE *f = fopen(config_path, "r");
    if (f == NULL) {
        perror("Failed to open the config.json\n");
        exit(1);
    }

    // Get the file size
    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    rewind(f);

    // Allocate enough for the file.
    char *buffer = malloc(len + 1);
    if (!buffer) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(1);
    }

    fread(buffer, 1, len, f);
    buffer[len] = '\0';
    fclose(f);

    cJSON *json = cJSON_Parse(buffer);
    free(buffer);

    if (!json) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr) fprintf(stderr, "cJSON error: %s\n", error_ptr);
        exit(1);
    }

    return json;
}

cJSON* read_config(void) {
    char home[1024];
    get_wordforge_home(home, sizeof(home));
    char config_path[1024];
    snprintf(config_path, sizeof(config_path), "%s/config.json", home);
    cJSON *json = read_json(config_path);

    return json;
}

void write_config(const char *text) {
    char home[1024];
    get_wordforge_home(home, sizeof(home));
    char config_path[1024];
    snprintf(config_path, sizeof(config_path), "%s/config.json", home);

    FILE *f = fopen(config_path, "w");
    if (f == NULL) {
        perror("Failed to open the config.json\n");
        exit(1);
    }

    fputs(text, f);
    fclose(f);

    return;
}