#include <pmdsky.h>
#include <cot.h>
#include "personality.h"

__attribute__((used)) int CustomGetPersonality() {
    int personality = 0;
    for (int i = 0; i < 4; i++) {
        if (PERSONALITY_TEST_PTR->personality_points[i] >= 128) {
            personality += (1 << (3 - i));
        }
    }
    return personality;
}

__attribute__((used)) void TryRandomizePartnerFirstKind() {
    if(LoadScriptVariableValue(NULL, VAR_PARTNER_FIRST_KIND) == 0){
        int species = 100 + RandInt(16); // first monster
        SaveScriptVariableValue(NULL, VAR_PARTNER_FIRST_KIND, species);
    }
}

__attribute__((used)) void AssignPartnerFirstKind() {
    SaveScriptVariableValue(NULL, VAR_PARTNER_FIRST_KIND, HEROS[GetPersonality()][0].val);
    FadeOutBgm(0xb4); // originalish instruction
}
