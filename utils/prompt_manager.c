#include <file_manager.h>
#include <cJSON.h>
#include <stdio.h>
#include <string.h>

void get_random_prompt(const char *PATH, char *prompt, size_t prompt_size, int *difficulty) {
    // Use read_json to read the file at PATH
    // Read the count variable and choose a random prompt
    // Retrieve the prompt and its difficulty and return.

    cJSON *json = read_json(PATH);
    cJSON *prompts = cJSON_GetObjectItemCaseSensitive(json, "prompts");
    if (!cJSON_IsArray(prompts)) {
        fprintf(stderr, "Invalid or missing prompts in JSON.\n");
        cJSON_Delete(json);
        exit(1);
    }

    int count = cJSON_GetArraySize(prompts);
    int index = rand() % count;

    cJSON *selected = cJSON_GetArrayItem(prompts, index);
    cJSON *prompt_json = cJSON_GetObjectItemCaseSensitive(selected, "prompt");
    cJSON *difficulty_json = cJSON_GetObjectItemCaseSensitive(selected, "difficulty");

    if (cJSON_IsString(prompt_json)) strncpy(prompt, prompt_json->valuestring, prompt_size);
    if (cJSON_IsNumber(difficulty_json)) *difficulty = difficulty_json->valueint;
    cJSON_Delete(json);

    return;
}

void get_random_file(char* file, char* name) {
    cJSON *archive_json = read_archive();
    cJSON *files = cJSON_GetObjectItemCaseSensitive(archive_json, "archive");
    if (!cJSON_IsArray(files)) {
        fprintf(stderr, "Invalid or missing files in archive.json.\n");
        cJSON_Delete(archive_json);
        exit(1);
    }

    int count = cJSON_GetArraySize(files);
    int index = rand() % count;

    cJSON *selected = cJSON_GetArrayItem(files, index);
    cJSON *title = cJSON_GetObjectItemCaseSensitive(selected, "name");
    cJSON *revisions = cJSON_GetObjectItemCaseSensitive(selected, "revisions");
    
    int revision_count = cJSON_GetArraySize(revisions);
    int latest_revision = revision_count - 1;

    cJSON *file_string = cJSON_GetArrayItem(revisions, latest_revision);

    if (file_string != NULL && cJSON_IsString(file_string)) {
        file = strncpy(file, file_string->valuestring, 32);
    } else {
        fprintf(stderr, "An error occured getting a random file.\n");
        cJSON_Delete(archive_json);
        exit(1);
    }

    if (title != NULL && cJSON_IsString(title)) {
        name = strncpy(name, title->valuestring, 256);
        cJSON_Delete(archive_json);
    } else {
        fprintf(stderr, "An error occured getting the title of a random file.\n");
        cJSON_Delete(archive_json);
        exit(1);
    }

}