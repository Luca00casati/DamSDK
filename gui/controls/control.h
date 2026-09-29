#pragma once
#include "damsdk/gui/base/View.h"

namespace DamSDK {
    namespace Gui {
        namespace Controls { class Control; }
        namespace Platform {
            namespace Windows { class Bitmap; class GDIDrawingContext; }
        }
    }
}

namespace DamSDK {
namespace Gui {
namespace Controls {

    typedef void (*callbackCallback)(Platform::Windows::GDIDrawingContext*, Control*);

    // VTABLE: DELAYLAMA 0x1000bcd4
    class Control : public Base::View {
        public:
            void (*callback)(Platform::Windows::GDIDrawingContext*, Control*);
            int parameterId;
            float prevValue;
            float defaultValue;
            float value;
            float min;
            float max;
            float wheelSensitivity;
            Platform::Windows::Bitmap *bitmap;
        public:
            Control(RECT *pRect, callbackCallback callback, int parameterId, Platform::Windows::Bitmap *bmp);
            virtual ~Control();

            virtual bool isDirty() override;
            virtual void setDirty(bool isDirty) override;
            virtual bool onMouseWheel(Platform::Windows::GDIDrawingContext* drawingContext, POINT* mousePos, float wheelDelta) override;

            // New virtual functions in the original vtable order (VSTGUI 2.x CControl).
            virtual void onIdle();
            virtual void setValue(float newValue);
            virtual float getValue();
            virtual void setMin(float min);
            virtual float getMin();
            virtual void setMax(float max);
            virtual float getMax();
            virtual float getPreviousValue();
            virtual void setDefaultValue(float defaultValue = 0);
            virtual float getDefaultValue();
            virtual void setParameterId(int parameterId);
            virtual Platform::Windows::Bitmap* getBitmap();
            virtual void setWheelSensitivity(float sensitivity);
            virtual float getWheelSensitivity();
            virtual void clampValue();
            virtual void changeBitmap(Platform::Windows::Bitmap* newBitmap);
            bool returnTrue(Platform::Windows::Window* window);
    };
}
}
}