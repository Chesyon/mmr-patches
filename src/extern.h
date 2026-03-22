#pragma once

#include <pmdsky.h>
#include <cot.h>

#define SECTION_TEXT_PLAYBILL __attribute__((section(".text.playbill"))) __attribute((used))
#define SECTION_DATA_PLAYBILL __attribute__((section(".data.playbill"))) __attribute((used))

#define TOTAL_SCENES 16 // 15 participant scenes, 1 initial
#define TEXT_STRING_SCENE_SELECTOR_TITLE 8735
#define TEXT_STRING_MYSTERYMAIL_BLURB 8736
#define TEXT_STRING_PARTICIPANT_NAME_START 8737
#define TEXT_STRING_PLAY_SCENE_QUESTION 8763

// Layouts for portraits
enum portrait_layout_types {
    FACE_POS_STANDARD = 0,
    FACE_POS_ABSCOORD = 1,
    FACE_POS_ABSCOORDFACEL = 2,
    FACE_POS_ABSCOORDFACER = 3,
    FACE_POS_BOTTOM_C_FACER = 4,
    FACE_POS_BOTTOM_L_FACEINW = 5,
    FACE_POS_BOTTOM_R_FACEINW = 6,
    FACE_POS_BOTTOM_L_CENTER = 7,
    FACE_POS_BOTTOM_R_CENTER = 8,
    FACE_POS_BOTTOM_C_FACEL = 9,
    FACE_POS_BOTTOM_L_FACEOUTW = 10,
    FACE_POS_BOTTOM_R_FACEOUTW = 11,
    FACE_POS_BOTTOM_LC_FACEOUTW = 12,
    FACE_POS_BOTTOM_RC_FACEOUTW = 13,
    FACE_POS_TOP_C_FACER = 14,
    FACE_POS_TOP_L_FACEINW = 15,
    FACE_POS_TOP_R_FACEINW = 16,
    FACE_POS_TOP_L_CENTER = 17,
    FACE_POS_TOP_R_CENTER = 18,
    FACE_POS_TOP_C_FACEL = 19,
    FACE_POS_TOP_L_FACEOUTW = 20,
    FACE_POS_TOP_RC_FACER = 21,
    FACE_POS_TOP_LC_FACEOUTW = 22,
    FACE_POS_TOP_RC_FACEOUTW = 23,
};

struct performer_spawn {
    int16_t kind;
    struct direction_id_8 direction;
    uint8_t collision_box_size_x;
    uint8_t collision_box_size_y;
    uint8_t x;
    uint8_t y;
    uint8_t flags_x;
    uint8_t flags_y;
};

struct font_data {
    void* kanji_rd_data;
    void* unkno_rd_data;
    undefined4 unk_0x8;
    undefined4 unk_0xC;
    void* kanji_rd_base;
    void* unkno_rd_base;
};

// Symbols not yet documented on pmdsky-debug
extern void ChangeVolumeBgm(int duration, int volume);
extern void TextboxSolid(void);
extern void TextboxTransparent(void);
extern void GetSceneName(char *dst, char *src);
extern int CreateLivePerformer(int follow_idx, struct performer_spawn *performer_spawn, int hanger, int sector, bool attribute_flag);
extern struct rgba TEXTBOX_COLOR_ATTRIBUTES;
extern struct font_data FONT_DATA;

// Generic stuff
extern void SwapFont(const char* filepath, bool swap_unkno);
extern bool SpResetTextboxColor(void);

// Playbill shenangians
extern SECTION_TEXT_PLAYBILL void CreatePlaybill(void);
extern SECTION_TEXT_PLAYBILL void ClosePlaybill(void);
extern SECTION_TEXT_PLAYBILL bool UpdateSceneSelectorMenu(void);

extern int last_selected_scene;