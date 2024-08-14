#pragma once

#include <QString>


namespace Hotkeys {
    enum class Actions {
        CYCLE_AUDIO_OUTPUTS = 0,

        ACTIONS_COUNT
    };

    QString GetActionNameFromEnum(Actions action);
}
