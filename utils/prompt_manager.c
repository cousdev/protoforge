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