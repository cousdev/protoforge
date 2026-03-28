#include <file_manager.h>
#include <cJSON.h>
#include <stdio.h>
#include <time.h>
#include <refine.h>
#include <string.h>
#include <sys/stat.h>

// Read archive.json
// Iterate through each item
// Print it to the screen.
void archive_list(void) {
    printf("Wordforge Archive\n");
    printf("-----------------\n\n");


    cJSON* archive_json = read_archive();
    cJSON* archive = cJSON_GetObjectItemCaseSensitive(archive_json, "archive");
    
    if(!cJSON_IsArray(archive)) {
        fprintf(stderr, "Failed to read archive.json");
        cJSON_Delete(archive_json);
        exit(1);
    }

    cJSON *item = NULL;
    int i = 1;
    cJSON_ArrayForEach(item, archive) {
        cJSON *name = cJSON_GetObjectItemCaseSensitive(item, "name");
        cJSON *revisions = cJSON_GetObjectItemCaseSensitive(item, "revisions");

        int revision_count = cJSON_GetArraySize(revisions);
        cJSON *latest = cJSON_GetArrayItem(revisions, revision_count - 1);

        // Convert Unix timestamp to human readable
        char readable[64] = "Unknown";
        if (cJSON_IsString(latest)) {
            time_t t = (time_t) atol(latest->valuestring);
            struct tm *tm = localtime(&t);
            strftime(readable, sizeof(readable), "%d %b %Y, %I:%M %p", tm);
        }

        if (cJSON_IsString(name)) printf("#%d  %s - %d revisions, last edited: %s\n", i, name->valuestring, revision_count, readable);
        i++;
    }

    cJSON_Delete(archive_json);
    return;

}

// Read archive.json
// Check the index supplied was valid
// Get the file from revisions[index]
// Print all the revisions of that file.
void archive_revisions(int index) {
    cJSON* archive_json = read_archive();
    cJSON* archive = cJSON_GetObjectItemCaseSensitive(archive_json, "archive");
    
    if(!cJSON_IsArray(archive)) {
        fprintf(stderr, "Failed to read archive.json");
        cJSON_Delete(archive_json);
        exit(1);
    }

    int archive_length = cJSON_GetArraySize(archive);
    if (index < 0 || index >= archive_length) {
        fprintf(stderr, "Invalid index passed.\n");
        cJSON_Delete(archive_json);
        exit(1);
    }

    cJSON *file = cJSON_GetArrayItem(archive, index);
    cJSON *name = cJSON_GetObjectItemCaseSensitive(file, "name");
    cJSON *revisions = cJSON_GetObjectItemCaseSensitive(file, "revisions");

    if (cJSON_IsString(name))
        printf("Revisions for \"%s\":\n\n", name->valuestring);

    int i = 1;
    cJSON *revision = NULL;
    cJSON_ArrayForEach(revision, revisions) {
        char readable[64] = "Unknown";
        if (cJSON_IsString(revision)) {
            time_t t = (time_t) atol(revision->valuestring);
            struct tm *tm = localtime(&t);
            strftime(readable, sizeof(readable), "%d %b %Y, %I:%M %p", tm);
        }
        printf("#%d  %s\n", i++, readable);
    }

    cJSON_Delete(archive_json);
}
 
// Just refactor and reuse the refine code for this to create a specific revision instead of using a random one.
void archive_refine(int index) {
    cJSON *archive_json = read_archive();
    cJSON *archive = cJSON_GetObjectItemCaseSensitive(archive_json, "archive");

    if (!cJSON_IsArray(archive)) {
        fprintf(stderr, "Failed to read archive.json\n");
        cJSON_Delete(archive_json);
        exit(1);
    }

    int archive_length = cJSON_GetArraySize(archive);
    if (index < 0 || index >= archive_length) {
        fprintf(stderr, "Invalid index passed.\n");
        cJSON_Delete(archive_json);
        exit(1);
    }

    cJSON *file = cJSON_GetArrayItem(archive, index);
    cJSON *title_json = cJSON_GetObjectItemCaseSensitive(file, "name");
    cJSON *revisions = cJSON_GetObjectItemCaseSensitive(file, "revisions");

    // Get the latest revision
    int revision_count = cJSON_GetArraySize(revisions);
    cJSON *latest = cJSON_GetArrayItem(revisions, revision_count - 1);

    char title[512];
    strncpy(title, title_json->valuestring, sizeof(title));

    // Build paths
    char home[1024];
    get_wordforge_home(home, sizeof(home));
    char archive_path[1024];
    snprintf(archive_path, sizeof(archive_path), "%s/archive", home);
    char file_path[1024];
    snprintf(file_path, sizeof(file_path), "%s/%s", archive_path, latest->valuestring);

    cJSON_Delete(archive_json);

    create_new_revision(file_path, title, archive_path);
}

// Read archive.json
// Check index is valid
// Get the correct revision
// Make it read-only and open it
// When done, make it read-write.
void archive_open(int index, int version) {
    cJSON *archive_json = read_archive();
    cJSON *archive = cJSON_GetObjectItemCaseSensitive(archive_json, "archive");

    if (!cJSON_IsArray(archive)) {
        fprintf(stderr, "Failed to read archive.json\n");
        cJSON_Delete(archive_json);
        exit(1);
    }

    int archive_length = cJSON_GetArraySize(archive);
    if (index < 0 || index >= archive_length) {
        fprintf(stderr, "Invalid index passed.\n");
        cJSON_Delete(archive_json);
        exit(1);
    }

    cJSON *file = cJSON_GetArrayItem(archive, index);
    cJSON *revisions = cJSON_GetObjectItemCaseSensitive(file, "revisions");
    int revision_count = cJSON_GetArraySize(revisions);

    if (version < 0 || version >= revision_count) {
        fprintf(stderr, "Invalid revision index.\n");
        cJSON_Delete(archive_json);
        exit(1);
    }

    cJSON *revision = cJSON_GetArrayItem(revisions, version);

    char home[1024];
    get_wordforge_home(home, sizeof(home));
    char file_path[1024];
    snprintf(file_path, sizeof(file_path), "%s/archive/%s", home, revision->valuestring);

    cJSON_Delete(archive_json);

    // Make read-only
    chmod(file_path, S_IRUSR | S_IRGRP | S_IROTH);

    char *EDITOR = getenv("EDITOR");
    if (EDITOR == NULL) {
        printf("Warning: No EDITOR set, defaulting to nano.\n");
        EDITOR = "nano";
    }

    char command[512];
    snprintf(command, sizeof(command), "%s %s", EDITOR, file_path);
    system(command);

    // Restore permissions
    chmod(file_path, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
}