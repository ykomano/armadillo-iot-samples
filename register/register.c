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
    const char* app_id   = "APP ID";
    const char* app_key  = "APP KEY";
    const char* site_url = KII_SITE_JP;

    kii_app_t app = kii_init_app(app_id, app_key, site_url);

    // register KiiThing
    char* vendor_thing_id    = "VENDOR THING ID";
    char* thing_password     = "password";
    kii_thing_t kii_thing    = NULL;
    kii_char_t* access_token = NULL;

    error_code = kii_register_thing(app, vendor_thing_id, thing_password,
        NULL, NULL, &kii_thing, &access_token);
    if (error_code != KIIE_OK) {
        goto CLEANUP;
    }

    // print information of registered KiiThing
    kii_char_t* thingId = kii_thing_serialize(kii_thing);

    printf("serialized_thing: %s\n", thingId);
    printf("access_token: %s\n", access_token);

    kii_dispose_kii_char(thingId);
    kii_dispose_kii_char(access_token);
    kii_dispose_thing(kii_thing);

CLEANUP:
    kii_dispose_app(app);
    kii_global_cleanup();

END:
    return EXIT_SUCCESS;
}
