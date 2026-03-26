#include <pmdsky.h>
#include <cot.h>
#include "extern.h"

uint16_t text_numerator = 1;
uint16_t text_denominator = 1;
uint16_t text_count = 0;

void ResetTextSpeedValues(void) {
	text_numerator = 1;
	text_denominator = 1;
	text_count = 0;
}

/*
	Hijacks the scene name of "DECOI" to instead load a scene name based off the result of Message Menu 80.
	Also resets some shared variables to their default values.
*/
__attribute((used)) void CustomGetActingSceneName(char* truncated_scene_name, char* full_scene_name) {
	GetSceneName(truncated_scene_name, full_scene_name);
	if (strncmp(truncated_scene_name, "DECOI", 8) != 0)
		return;
	if (last_selected_scene == 0)
		strncpy(truncated_scene_name, "initial", 8);
	else
		snprintf(truncated_scene_name, 8, "%02d", last_selected_scene);
	SetPerformanceFlagWithChecks(62, 0);
	ResetTextSpeedValues();
	// Reset the SCENARIO_SUB variables...
	// SkyTemple's debugger actually raises errors for out-of-bounds indexing, so just play nicely.
	uint8_t* buffer = (uint8_t*)&(DIALOGUE_BOX_DEFAULT_WINDOW_PARAMS.x_offset);
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 2; j++) {
			SaveScriptVariableValueAtIndex(NULL, VAR_SCENARIO_SUB1+i, j, *buffer);
			buffer++;
		}
	}
}

/*
	Hijacks a call to CreateDialogueBox to use an entirely new set of window_params, if PERFORMANCE_PROGRESS_LIST[62] is set.
*/
__attribute((used)) int CustomCreateDialogueBox(struct window_params* window_params) {
	struct window_params new_params;
	if (GetPerformanceFlagWithChecks(62)) {
		uint8_t* buffer = (uint8_t*)&(new_params.x_offset);
		for (int i = 0; i < 6; i++)
			buffer[i] = LoadScriptVariableValueAtIndex(NULL, VAR_SCENARIO_SUB1, i); // Intentional out-of-bounds indexing, yay!
		window_params = &new_params;
	}
	return CreateDialogueBox(window_params);
}

/*
	Hijacks a call to CreatePortraitBox to place a portrait loaded by a script command on the Top Screen, if PERFORMANCE_PROGRESS_LIST[62] is set.
	Please note that framing doesn't seem to work for portraits on the Top Screen.
*/
__attribute((used)) int CustomCreatePortraitBox(enum screen screen, uint32_t palette_idx, bool framed) {
	enum screen new_screen = screen;
	if (GetPerformanceFlagWithChecks(62))
		new_screen = LoadScriptVariableValueAtIndex(NULL, VAR_SCENARIO_SUB3, 0);
	return CreatePortraitBox(new_screen, palette_idx, framed);
}

/*
	Changes a currently-loaded font.
		- "kanji_rd.dat" is the main font in NA EoS.
		- "unkno_rd.dat" is the Unown font in NA EoS.
		
	Any arbitrary font binary can be loaded, so long as it's formatted like the above two files.
*/
void SwapFont(const char* filepath, bool swap_unkno) {
	struct iovec iov;
	void** base = NULL;
	void** data = NULL;
	if(swap_unkno) {
		base = &(FONT_DATA.unkno_rd_base);
		data = &(FONT_DATA.unkno_rd_data);
	}
	else {
		base = &(FONT_DATA.kanji_rd_base);
		data = &(FONT_DATA.kanji_rd_data);
	}
	MemFree(*base);
	LoadFileFromRom(&iov, filepath, 1);
	*base = iov.iov_base;
	*data = (void*)((uint32_t)(iov.iov_base) + 0x4);
}

/*
	Parses custom uppercase text tags.
		- "VS:X:Y" ("VITESSE") modifies text speed by X/Y. For example, "[VS:1:2]" halves speed, but "[VS:4]" quadruples it. The second parameter is optional, and if missing, will default to 1.
		- "VR" reverts text speed to normal (equivalent to "[VS:1:1]" and "[VS:1]").
		- "VAR:X:Y:Z" ("VARIABLE") sets script variable X at index Z to the value Y. The third parameter is optional, and if missing, will default to 0.
		- "U:X" ("UNLOCK") unlocks the the Xth scripting lock. 
		
	To ignore a text tag in a textbox that doesn't scroll, check for dialogue_display_state::flags.timer_2.
*/
__attribute__((used)) bool ParseCustomUppercaseTextTags(struct dialogue_display_state* state, const char* tag, const char** tag_params, int tag_param_count) {
	int tag_vals[4] = {0};
	if(tag_param_count > ARRAY_LENGTH(tag_vals))
		tag_param_count = ARRAY_LENGTH(tag_vals);
		
	for(int i = 0; i < tag_param_count; i++)
		tag_vals[i] = AtoiTag(tag_params[i]);
		
	// Checking for an actual tag...
	if(StrcmpTag(tag, "VS")) {
		if(state->flags.timer_2)
			return true;
		if(tag_param_count > 0) {
			text_numerator = tag_vals[0];
			text_denominator = tag_param_count == 1 ? 1 : tag_vals[1];
			text_count = 0;
			state->text_scrolling_done = 0;
		}
		return true;
	}
	else if(StrcmpTag(tag, "VR")) {
		if(state->flags.timer_2)
			return true;
		ResetTextSpeedValues();
		state->text_scrolling_done = 0;
		return true;
	}
	else if(StrcmpTag(tag, "VAR")) {
		if(state->flags.timer_2)
			return true;
		if(tag_param_count >= 2) { // NOT >= 3 on purpose!
			// VAR:X:Y:Z
			// Set script variable X[Z] = Y
			// Z is optional, defaulting to just zero
			SaveScriptVariableValueAtIndex(NULL, tag_vals[0], tag_vals[2], tag_vals[1]);
		}
		return true;
	}
	else if(StrcmpTag(tag, "U")) {
		if(state->flags.timer_2)
			return true;
		if(OverlayIsLoaded(OGROUP_OVERLAY_11))
			UnlockScriptingLock(tag_vals[0]);
		return true;
	}
	return false;
}

__attribute((used)) uint32_t TryChangeTextSpeed(struct dialogue_display_state* state) {
	unsigned long long result = _s32_div_f((state->text_speed * text_numerator + text_count), text_denominator);
	text_count = result >> 32;
	return (uint32_t)result;
}

__attribute((naked)) void HijackTextSpeed(void) {
	asm("mov r0,r4");
	asm("b TryChangeTextSpeed");
}

__attribute((naked)) void HijackTextLoop(void) {
	asm("ldr r0,[r4,#0x80]");
	asm("cmp r0,#0x0");
	asm("bxne r14");
	// Per the original comment: "Here is the fun part"
	asm("mov r0,#0x0");
	asm("b AnalyzeTextReturn");
}
