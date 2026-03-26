#include <stdio.h>
#include <file_manager.h>
#include <cJSON.h>

// Read the config.json file
// Print the stats in a nice list
void stats(void) {
    cJSON *config = read_config();
    cJSON *xp_item = cJSON_GetObjectItemCaseSensitive(config, "xp");
    cJSON *forges_item = cJSON_GetObjectItemCaseSensitive(config, "total_forges");
    cJSON *refines_item = cJSON_GetObjectItemCaseSensitive(config, "total_refines");
    cJSON *streak_item = cJSON_GetObjectItemCaseSensitive(config, "streak");

    int xp;
    if (cJSON_IsNumber(xp_item)) {
        xp = xp_item->valueint;
    } else {
        fprintf(stderr, "Failed to read XP. Is your config.json broken?\n");
        exit(1);
    }

    int forges;
    if (cJSON_IsNumber(forges_item)) {
        forges = forges_item->valueint;
    } else {
        fprintf(stderr, "Failed to read forge count. Is your config.json broken?\n");
        exit(1);
    }

    int refines;
    if (cJSON_IsNumber(refines_item)) {
        refines = refines_item->valueint;
    } else {
        fprintf(stderr, "Failed to read refine count. Is your config.json broken?\n");
        exit(1);
    }

    int streak;
    if (cJSON_IsNumber(streak_item)) {
        streak = streak_item->valueint;
    } else {
        fprintf(stderr, "Failed to read streak. Is your config.json broken?\n");
        exit(1);
    }

    printf("Wordforge - Statistics\n\n");
    printf("Total XP: %d\n", xp);
    printf("Total forges: %d\n", forges);
    printf("Total refines: %d\n", refines);
    printf("Current streak: %d\n", streak);

    return;
}