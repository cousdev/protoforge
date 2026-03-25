#include <stdlib.h>
void get_wordforge_home(char *, size_t);
char** get_forge_files(const char *PATH, int *count);
char* get_random_line(const char *PATH);
void create_new_file(const char *PATH, const char** LINES, int count);