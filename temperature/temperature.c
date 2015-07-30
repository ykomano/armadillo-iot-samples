/* vim: set et fenc=utf-8 ff=unix ts=4 sw=4 sts=4 : */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <kii_cloud.h>
#include <jansson.h>

int main(void) {
    kii_error_code_t error_code;

    error_code = kii_global_init();
    if (error_code != KIIE_OK) {
        goto END;
    }

    // initialize application
    const char* app_id   = "APP_ID";
    const char* app_key  = "APP_KEY";
    const char* site_url = KII_SITE_JP;

    kii_app_t app = kii_init_app(app_id, app_key, site_url);

    // generate uuid for vendor thing id
    FILE* fp = fopen("/proc/sys/kernel/random/uuid", "r");
    if (fp == NULL) {
        goto END;
    }

    char uuid[100];
    fgets(uuid, 100, fp);
    fclose(fp);

    // remove '\n'
    uuid[strlen(uuid) - 1] = '\0';

    // register KiiThing
    const char* vendor_thing_id    = uuid;
    const char* thing_password     = "password";
    kii_thing_t kii_thing    = NULL;
    kii_char_t* access_token = NULL;

    printf("Vendor Thing ID: %s\n", vendor_thing_id);
    printf("Password: %s\n", thing_password);

    error_code = kii_register_thing(app, vendor_thing_id, thing_password,
        NULL, NULL, &kii_thing, &access_token);
    if (error_code != KIIE_OK) {
        goto CLEANUP;
    }

    // print information of registered KiiThing
    kii_char_t* thingId = kii_thing_serialize(kii_thing);

    printf("Thing ID: %s\n", thingId);
    printf("Access Token: %s\n", access_token);

    // initialize target kii bucket
    const char* bucket_name = "test";
    kii_bucket_t kii_bucket = kii_init_thing_bucket(kii_thing, bucket_name);

    // get temperature
    FILE* temp_fp = fopen("/sys/devices/platform/i2c-gpio.3/i2c-3/3-0048/temp1_input", "r");
    if (temp_fp == NULL) {
        goto CLEANUP_KIIBUCKET;
    }

    char temp_str[100];
    fgets(temp_str, 100, temp_fp);
    fclose(temp_fp);

    int temp = atoi(temp_str);

    // create json
    json_error_t json_error;
    json_t* json = NULL;

    json = json_pack("{s:i}", "temp", temp);
    if (json == NULL) {
        goto CLEANUP_KIIBUCKET;
    }

    // upload json as kii object
    kii_char_t* object_id = NULL;
    kii_char_t* etag = NULL;

    error_code = kii_create_new_object(app, access_token, kii_bucket, json,
        &object_id, &etag);
    if (error_code != KIIE_OK) {
        goto CLEANUP_JSON;
    }

    printf("Bucket name: %s\n", bucket_name);
    printf("Temperature: %d\n", temp);
    printf("Object ID: %s\n", object_id);
    printf("ETag: %s\n", etag);

    kii_dispose_kii_char(object_id);
    kii_dispose_kii_char(etag);

CLEANUP_JSON:
    json_decref(json);

CLEANUP_KIIBUCKET:
    kii_dispose_bucket(kii_bucket);

CLEANUP_KIITHING:
    kii_dispose_kii_char(thingId);
    kii_dispose_kii_char(access_token);
    kii_dispose_thing(kii_thing);

CLEANUP:
    kii_dispose_app(app);
    kii_global_cleanup();

END:
    return EXIT_SUCCESS;
}
