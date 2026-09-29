#pragma once
#include <Windows.h>
#include <windef.h>
#include "DamPlugin.h"

// Forward declarations
namespace DamSDK {
    namespace Api { class AudioBaseExtended; }
    namespace Gui {
        namespace Platform {
            namespace Windows { class Window; }
        }
    }
}

namespace DamSDK {
namespace Api {

    extern int GLOBAL_KNOB_MODE;

    // VTABLE: DELAYLAMA 0x1000bb80
    class EditorBase {
        public:
            AudioBaseExtended* mainPlugin;
            HWND hParent;
            BOOL needsRedraw;
            Rect rect;
            Gui::Platform::Windows::Window* window;
            DWORD lastIdleTick;
            bool isInIdleUpdate;
        public:
            EditorBase(AudioBaseExtended* plugin);
            // Virtual functions in the original vtable order (VTABLE 0x1000bb80,
            // VST 2.x AEffEditor). Do not reorder.
            virtual ~EditorBase();
            virtual int32_t getRect(Rect** outRect);
            virtual int32_t open(HWND hParent);
            virtual void close();
            virtual void onIdle();
            virtual void update();
            virtual void invalidate();
            virtual int32_t keyDown(KeyCode* keycode);
            virtual int32_t keyUp(KeyCode* keycode);
            virtual void setKnobMode(int32_t mode);
            virtual bool onMouseWheel(float wheelDelta);
            virtual void dispatcher(int parameterIndex, float value);
            virtual void draw();
            virtual void idleHandler();
            void sleep(DWORD milliseconds);
    };
}
}