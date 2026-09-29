#pragma once
#include <windows.h>
#include <windef.h>
#include "damsdk/gui/base/View.h"
#include "damsdk/utils/portable_stdint.h"

namespace DamSDK {
    namespace Api {
        class EditorBase;
    }
    namespace Gui {
        namespace Controls {
            class Control;
        }
        namespace Platform {
            namespace Windows {
                class DropTarget;
            }
        }
    }
}

namespace DamSDK {
namespace Gui {
namespace Platform {
namespace Windows {

    // VTABLE: DELAYLAMA 0x1000bc48
    class Window : public Base::View {
        public:
            Api::EditorBase *editor;
            HWND handle;
            Bitmap *backgroundBitmap;
            int numChildren;
            int maxChildren;
            Controls::Control **children;
            Base::View *modalView;
            COLORREF* unknownClass;
            bool redrawPending;
            bool visible;
            char unused3[2];
            HWND hWnd;
            bool isActive;
            char unused4[3];
            void *closeParameter;
        public:
            Window(RECT *pRect, HWND hParent, Api::EditorBase *editor);
            virtual ~Window();
            virtual void update(GDIDrawingContext *drawingContext) override;
            virtual void onDraw(GDIDrawingContext *drawingContext) override;
            bool openPluginWindow(HWND hParent);
            bool needsRedraw();
            virtual bool registerControl(Controls::Control *control);
            virtual bool onMouseWheel(GDIDrawingContext *drawingContext, POINT *relativeMousePoint, float scrollDelta) override;
            void drawControlOrSelf(Controls::Control *target);
            void refresh();
            void setBackgroundBitmap(Bitmap *background);
            void resetVtable(Window* frame);
            void cleanup();
            bool closeWindow();
            bool setDragAndDropState(bool enable);
            virtual void onMouseDown(GDIDrawingContext* drawingContext, POINT* mousePos) override;
            virtual bool routeMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, POINT* mousePos) override;
            virtual bool removeChild(Controls::Control* child, bool shouldRelease);
            virtual bool destroyChildren(bool* callExtraFlag);
            virtual bool containsChild(Controls::Control* target);
            int32_t setModalView(Base::View* view);
            void beginEdit(int parameterId);
            void endEdit(int parameterId);
            Controls::Control* getChildAtMousePos();
            void getLocalMousePos(POINT* mousePos);
            static bool registerWindowClass();
            static void unregisterWindowClass();
    };
}
}
}
}