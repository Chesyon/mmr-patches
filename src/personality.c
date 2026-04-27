#include <pmdsky.h>
#include <cot.h>
#include "personality.h"

int CustomGetPersonality() {
    int personality = 0;
    for (int i = 0; i < 4; i++) {
        if (PERSONALITY_TEST_PTR->personality_points[i] >= 128) {
            personality += (1 << (3 - i));
        }
    }
    return personality;
}
