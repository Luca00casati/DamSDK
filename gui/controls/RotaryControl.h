#pragma once
#include "control.h"
#include "damsdk/utils/portable_stdint.h"
#include "damsdk/gui/platform/windows/GDIDrawingContext.h"
#include "damsdk/gui/platform/windows/Bitmap.h"
#include "damsdk/api/DamPlugin.h"

namespace DamSDK {
namespace Gui {
namespace Controls {

    // VTABLE: DELAYLAMA 0x1000bd78
    class RotaryControl : public Control {
        public:
            POINT srcPoint;
            Api::ColorRGBA indicatorHighlightColor;
            Api::ColorRGBA indicatorShadowColor;
            Platform::Windows::Bitmap *bmp;
            int knobRadius;
            float startAngle;
            float totalRange;
            float deadZoneSize;
            float angleRange;
            float angleOffset;
            float center;
            float fineTuneDivider;

        public:
            RotaryControl(RECT* pRect, callbackCallback callback, int parameterId, Platform::Windows::Bitmap* bmp1, Platform::Windows::Bitmap* bmp2, POINT* srcPoint);
            ~RotaryControl();
            virtual void onDraw(Platform::Windows::GDIDrawingContext* drawingContext) override;
            void drawIndicator(Platform::Windows::GDIDrawingContext* drawingContext);
            virtual void onMouseDown(Platform::Windows::GDIDrawingContext* drawingContext, POINT* mousePos) override;
            void setStartAngle(float startAngle);
            void setTotalRange(float totalRange);
            void updateMathConstants();
            void calculateXYFromValue(POINT* outPoint);
            float calculateAngleFromPoint(POINT* point);
            void setIndicatorShadowColor(Api::ColorRGBA color);
            void setIndicatorHighlightColor(Api::ColorRGBA color);
            void setBitmap(Platform::Windows::Bitmap* bmp);
            float getStartAngle();
            float getTotalRange();
            void setKnobRadius(int radius);
            void setFineTuneDivider(float divider);
            float getFineTuneDivider();
    };
}
}
}