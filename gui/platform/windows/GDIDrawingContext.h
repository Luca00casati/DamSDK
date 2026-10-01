#pragma once
#include <Windows.h>
#include <windef.h>
#include <damsdk/utils/portable_stdint.h>
#include <damsdk/api/DamPlugin.h>

namespace DamSDK {
    namespace Gui {
        namespace Platform {
            namespace Windows {
                class Window;
            }
        }
    }
}

namespace DamSDK {
namespace Gui {
namespace Platform {
namespace Windows {

    extern HINSTANCE g_hInstance;

    extern Api::Color DAT_FOREGROUND_COLOR;
    extern Api::Color DAT_GRAY_COLOR;
    extern Api::Color DAT_BACK_COLOR;
    extern Api::Color DAT_TRANSPARENT_COLOR;

    // VTABLE: DELAYLAMA 0x1000bbe4
    class GDIDrawingContext {
        public:
            POINT screenPos;
            POINT drawOffset;
            HDC hDC;
            HWND hWnd;
            void*	parentFrame;
            char unused1[8];
            Api::Color textColor;
            char unused2[8];
            LONG penWidth;
            Api::Color penColor;
            Api::Color backgroundColor;
            int32_t lineStyle;
            char unused3[4];
            RECT rect;
            HBRUSH obj1;
            HPEN obj2;
            HBRUSH obj3;
            HGDIOBJ originalPen;
            HGDIOBJ OriginalBrush;
            HGDIOBJ OriginalFont;
            UINT penStyle;

        public:
            GDIDrawingContext(Window *parentFramePtr,HDC hDC,HWND hWnd);
            virtual ~GDIDrawingContext();
            static int32_t setModuleHandle(HINSTANCE hInstance);
            void setPenColor(Api::Color color);
            void setPenDashMode(int32_t lineStyle);
            void setBackgroundColorAndBrush(Api::Color color);
            void setTextColor(Api::Color color);
            void moveToEx(POINT* target);
            void lineTo(POINT* tageet);
            void drawRectangleOutline(RECT* param_1);
            void fillRectangleInset(RECT* param_1);
            void getRelativeMousePos(POINT* outRelMousePos);
            void copyToScreen(GDIDrawingContext* dest, int dstLeft, int dstTop, int dstRight, int dstBottom, int srcX, int srcY);
            uint32_t getMouseButtons();
    };
}
}
}
}