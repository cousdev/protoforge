#include <stdlib.h>
#include <cJSON.h>

void get_wordforge_home(char *, size_t);
char** get_files_from_folder(const char *PATH, int *count);
char* get_random_line(const char *PATH);
void create_new_file(const char *PATH, const char** LINES, int count);
cJSON* read_config(void);
void write_config(const char *text);
cJSON* read_json(const char *config_path);
void write_text(const char *PATH, const char *TEXT);
void write_archive(const char *text);
cJSON* read_archive(void);