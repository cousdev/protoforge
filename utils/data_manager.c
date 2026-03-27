#include <cJSON.h>
#include <file_manager.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

int get_xp(void) {
    cJSON *config = read_config();
    cJSON *XP_json = cJSON_GetObjectItemCaseSensitive(config, "xp");

    int xp;
    if (cJSON_IsNumber(XP_json)) {
        xp = XP_json->valueint;
    }

    cJSON_Delete(config);
    return xp;
}

int add_xp(int xp) {
    // Get the xp, add the xp, write to the file.
    int current_xp = get_xp();
    int final_xp = current_xp + xp;

    cJSON *config = read_config();
    cJSON *xp_item = cJSON_GetObjectItemCaseSensitive(config, "xp");
    cJSON_SetNumberValue(xp_item, final_xp);

    // Convert from cJSON object to JSON string.
    char *json_str = cJSON_Print(config);
    write_config(json_str);
    cJSON_free(json_str);
    cJSON_Delete(config);
    return final_xp;
}

int get_forge_count(void) {
    cJSON *config = read_config();
    cJSON *Forges_json = cJSON_GetObjectItemCaseSensitive(config, "total_forges");

    int forge_count;
    if (cJSON_IsNumber(Forges_json)) {
        forge_count = Forges_json->valueint;
    }

    cJSON_Delete(config);
    return forge_count;
}

void add_forge_count(void) {
    int current_forge_count = get_forge_count();
    int new_forge_count = current_forge_count + 1;

    cJSON *config = read_config();
    cJSON *forges_item = cJSON_GetObjectItemCaseSensitive(config, "total_forges");
    cJSON_SetNumberValue(forges_item, new_forge_count);

    // Convert from cJSON object to JSON string.
    char *json_str = cJSON_Print(config);
    write_config(json_str);
    cJSON_free(json_str);
    cJSON_Delete(config);

    return;
}

int get_refine_count(void) {
    cJSON *config = read_config();
    cJSON *Refines_json = cJSON_GetObjectItemCaseSensitive(config, "total_refines");

    int refine_count;
    if (cJSON_IsNumber(Refines_json)) {
        refine_count = Refines_json->valueint;
    }

    cJSON_Delete(config);
    return refine_count;
}

void add_refine_count(void) {
    int current_refine_count = get_refine_count();
    int new_refine_count = current_refine_count + 1;

    cJSON *config = read_config();
    cJSON *refines_item = cJSON_GetObjectItemCaseSensitive(config, "total_refines");
    cJSON_SetNumberValue(refines_item, new_refine_count);

    // Convert from cJSON object to JSON string.
    char *json_str = cJSON_Print(config);
    write_config(json_str);
    cJSON_free(json_str);
    cJSON_Delete(config);

    return;
}

void get_iso_date(char *buf, size_t size) {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    strftime(buf, size, "%Y-%m-%d", tm);
}

int update_streak(void) {
    char today[11];
    get_iso_date(today, sizeof(today));

    cJSON *config = read_config();
    cJSON *last_practice = cJSON_GetObjectItemCaseSensitive(config, "last_practice");
    cJSON *streak_item = cJSON_GetObjectItemCaseSensitive(config, "streak");

    int streak = cJSON_IsNumber(streak_item) ? streak_item->valueint : 1;
    const char *last = cJSON_IsString(last_practice) ? last_practice->valuestring : NULL;

    if (last && strcmp(last, today) == 0) {
        // Already practiced today, don't change streak
        cJSON_Delete(config);
        return streak;
    }

    // Check if last practice was yesterday
    if (last) {
        time_t t = time(NULL);
        struct tm *tm = localtime(&t);
        tm->tm_mday -= 1;
        mktime(tm);  // normalise the struct (handles month rollovers etc.)
        char yesterday[11];
        strftime(yesterday, sizeof(yesterday), "%Y-%m-%d", tm);

        if (strcmp(last, yesterday) == 0)
            streak++;       // continued streak
        else
            streak = 1;     // streak broken
    } else {
        streak = 1;         // no last_practice in config at all
    }

    // Write back
    cJSON_SetNumberValue(streak_item, streak);
    cJSON *last_practice_str = cJSON_GetObjectItemCaseSensitive(config, "last_practice");
    cJSON_SetValuestring(last_practice_str, today);

    char *json_str = cJSON_Print(config);
    write_config(json_str);
    cJSON_free(json_str);
    cJSON_Delete(config);

    return streak;
}

// Get archive.json
// Check if a file with the specified human name exists.
// If not: Create a new one and set the name to HUMAN_NAME, and the revision FILE_NAME.
// If yes: Find it and append FILE_NAME to the revisions.
void save_to_archive(const char *HUMAN_NAME, const char *FILE_NAME) {
    char home[1024];
    get_wordforge_home(home, sizeof(home));
    char archive_path[1024];
    snprintf(archive_path, sizeof(archive_path), "%s/archive.json", home);
    cJSON *archive_json = read_json(archive_path);
    cJSON *archive = cJSON_GetObjectItemCaseSensitive(archive_json, "archive");

    // Iterate over each item in the array, search for an existing item.
    cJSON *entry = NULL;
    cJSON *item = NULL;

    cJSON_ArrayForEach(item, archive) {
        cJSON *name = cJSON_GetObjectItemCaseSensitive(item, "name");
        if (cJSON_IsString(name) && strcmp(name->valuestring, HUMAN_NAME) == 0) {
            entry = item;
            break;
        }
    }

    if (entry == NULL) {
        // Create a new entry
        entry = cJSON_CreateObject();
        cJSON_AddStringToObject(entry, "name", HUMAN_NAME);

        cJSON *revisions = cJSON_CreateArray();
        cJSON_AddItemToArray(revisions, cJSON_CreateString(FILE_NAME));
        cJSON_AddItemToObject(entry, "revisions", revisions);

        cJSON_AddItemToArray(archive, entry);
    } else {
        // Append to the existing revisions
        cJSON *revisions = cJSON_GetObjectItemCaseSensitive(entry, "revisions");
        cJSON_AddItemToArray(revisions, cJSON_CreateString(FILE_NAME));
    }

    // Write back
    char *json_str = cJSON_Print(archive_json);
    write_archive(json_str);
    cJSON_free(json_str);
    cJSON_Delete(archive_json);
}
