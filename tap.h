#ifndef TAP_H
#define TAP_H
/**
 * written by tokuhirom.
 * public domain.
 */

#include <stdio.h>

int TEST_COUNT = 0;

static void ok(int x, const char *msg) {
    printf("%s %d - %s\n", (x ? "ok" : "not ok"), ++TEST_COUNT, msg ? msg : "");
}

static void diag(const char *msg) {
    printf("# %s\n", msg ? msg : "");
}

static void done_testing() {
    printf("1..%d\n", TEST_COUNT);
}

#endif /* TAP_H */
