#include "controllers/settings/HotkeyActions.h"


QString Hotkeys::GetActionNameFromEnum(Actions action) {
    switch (action) {
    case Actions::CYCLE_AUDIO_OUTPUTS: return "Switch audio output";
    default:                           return "";
    }
}
