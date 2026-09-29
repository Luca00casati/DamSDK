#include "HorizontalSlider.h"
#include "damsdk/gui/platform/windows/OffscreenGDIDrawingContext.h"
#include "damsdk/gui/platform/windows/Window.h"

namespace DamSDK {
namespace Gui {
namespace Controls {

    // FUNCTION: DELAYLAMA 0x1000a010
    HorizontalSlider::HorizontalSlider(RECT *pRect, callbackCallback callback, int parameterId, int minValue, int maxValue, Platform::Windows::Bitmap *handleBmp, Platform::Windows::Bitmap *backgroundBmp, POINT* offset, int flags) : Control(pRect, callback, parameterId, backgroundBmp)
    {
        this->backgroundOffset.x = offset->x;
        this->backgroundOffset.y = offset->y;
        this->handlePos.x = 0;
        this->handlePos.y = 0;
        this->flags = flags;
        this->handleImage = handleBmp;
        this->trackMinX = minValue;
        this->trackMaxX = maxValue;
        this->snapToMouse = true;
        this->isHandleTransparent = true;
        if (handleBmp == nullptr) {
            this->handleWidth = 1;
            this->handleHeight = 1;
        }
        else {
            handleBmp->remember();
            handleImage = this->handleImage;
            this->handleWidth = handleImage->width;
            this->handleHeight = handleImage->height;
        }
        this->trackWidth = this->rect.right - this->rect.left;
        this->trackHeight = this->rect.bottom - this->rect.top;
        this->handlePos.x = 0;
        this->handlePos.y = 0;
        this->handleMinPos = minValue - this->rect.left;
        this->handleMaxPos = (this->handleWidth - this->rect.left) + maxValue;
        this->fineTuneDivider = 10.0;
    }
    
    // FUNCTION: DELAYLAMA 0x1000a140
    HorizontalSlider::~HorizontalSlider() {
        if (this->handleImage != nullptr) {
            this->handleImage->unregisterBitmap();
        }
    }

    // FUNCTION: DELAYLAMA 0x10009b50
    void HorizontalSlider::setIsHandleTransparent(bool transparent) {
        this->isHandleTransparent = transparent;
    }

    // FUNCTION: DELAYLAMA 0x10009b60
    void HorizontalSlider::setSnapToMouse(bool snapToMouse) {
        this->snapToMouse = snapToMouse;
    }

    // FUNCTION: DELAYLAMA 0x10009b70
    bool HorizontalSlider::getSnapToMouse() {
        return this->snapToMouse;
    }

    // FUNCTION: DELAYLAMA 0x10009b80
    void HorizontalSlider::setHandlePos(POINT* handlePos) {
        this->handlePos.x = handlePos->x;
        this->handlePos.y = handlePos->y;
    }

    // FUNCTION: DELAYLAMA 0x10009ba0
    Platform::Windows::Bitmap* HorizontalSlider::getHandleImage() {
        return this->handleImage;
    }

    // FUNCTION: DELAYLAMA 0x10009bb0
    void HorizontalSlider::setFinetuneDivider(float currentValue) {
        this->fineTuneDivider = currentValue;
    }

    // FUNCTION: DELAYLAMA 0x10009bc0
    float HorizontalSlider::getFinetuneDivider() {
        return this->fineTuneDivider;
    }

    // FUNCTION: DELAYLAMA 0x10009fe0
    void HorizontalSlider::changeHandle(Platform::Windows::Bitmap* newHandle) {
        if (this->handleImage != nullptr) {
            this->handleImage->unregisterBitmap();
        }
        this->handleImage = newHandle;
        if (newHandle != nullptr) {
            newHandle->remember();
        }
    }

     // FUNCTION: DELAYLAMA 0x1000a1a0
    void HorizontalSlider::onDraw(Platform::Windows::GDIDrawingContext* drawingContext) {
        // Same steps as VSTGUI's CSlider::draw, always through an offscreen context.
        float value;
        if (this->flags & 8)
            value = this->value;
        else
            value = 1.0f - this->value;

        Platform::Windows::OffscreenGDIDrawingContext* offscreen =
            new Platform::Windows::OffscreenGDIDrawingContext(this->parent, this->trackWidth, this->trackHeight, Platform::Windows::DAT_BACK_COLOR);

        // Background
        RECT rect = {0, 0, this->trackWidth, this->trackHeight};
        if (this->bitmap) {
            if (this->useAlphaBlending)
                this->bitmap->drawMasked(offscreen, &rect, &this->backgroundOffset);
            else
                this->bitmap->blit(offscreen, &rect, &this->backgroundOffset);
        }

        // Handle position
        RECT handleRect;
        handleRect.top = this->handlePos.y;
        handleRect.bottom = handleRect.top + this->handleHeight;
        handleRect.left = this->handlePos.x + (int)(value * (this->trackMaxX - this->trackMinX));
        if (handleRect.left < this->handleMinPos)
            handleRect.left = this->handleMinPos;
        handleRect.right = handleRect.left + this->handleWidth;
        if (handleRect.right > this->handleMaxPos)
            handleRect.right = this->handleMaxPos;

        if (this->handleImage) {
            if (this->isHandleTransparent) {
                POINT zero = {0, 0};
                this->handleImage->drawMasked(offscreen, &handleRect, &zero);
            }
            else {
                POINT zero = {0, 0};
                this->handleImage->blit(offscreen, &handleRect, &zero);
            }
        }

        offscreen->copyToScreen(drawingContext, this->rect.left, this->rect.top, this->rect.right, this->rect.bottom, 0, 0);
        delete offscreen;

        this->trackLeftX = this->rect.left + handleRect.left;
        this->setDirty(false);
    }

    // FUNCTION: DELAYLAMA 0x1000a360
    void HorizontalSlider::onMouseDown(Platform::Windows::GDIDrawingContext* drawingContext, POINT* mousePos) {
        if (this->isEnabled != false) {
            uint8_t modifiers = View::GetPressedModifiersAndMouseButtons();
            // Ctrl + Left Click -> Reset to default value
            if (modifiers == 0x11) {  // 0x10 (Ctrl) | 0x01 (Left Button)
                float defaultValue = this->getDefaultValue();
                this->value = defaultValue;
                if (this->isDirty()) {
                    this->callback(drawingContext, this);
                }
                return;
            }
            else {

                int horizontalAnchor;

                //Check for Left Click (0x01)
                if ((modifiers & 1) != 0) {
                    int trackMinX = this->trackMinX;
                    if (this->snapToMouse == false) {
                        int trackLeft = this->trackLeftX;
                        int trackTop = this->rect.top;
                        int mouseX = mousePos->x;
                        if (mouseX < trackLeft) {
                        return;
                        }
                        if (this->handleWidth + trackLeft < mouseX) {
                            return;
                        }
                        if (mousePos->y < trackTop) {
                            return;
                        }
                        if (this->handleHeight + trackTop < mousePos->y) {
                            return;
                        }
                        horizontalAnchor = trackMinX + (mouseX - trackLeft);
                    }
                    else {
                        horizontalAnchor = trackMinX + -1 + this->handleWidth / 2;
                    }

                    trackMaxX = this->trackMaxX;
                    float curValue = this->value;
                    this->parent->beginEdit(this->parameterId);

                    modifiers = View::GetPressedModifiersAndMouseButtons();
                    uint32_t previousModifiers = modifiers;

                    float previousValue = this->value;
                    while ((modifiers & 1) != 0) {
                        if (modifiers != previousModifiers) {
                            if ((modifiers & 8) != 0) {
                                previousValue = this->value;
                            }
                            previousModifiers = modifiers;
                        }
                        else {
                            curValue = this->value;
                        }
                        flags = this->flags;
                        float calculatedValue = (float)(mousePos->x - horizontalAnchor) / (float)(trackMaxX - trackMinX);
                        this->value = calculatedValue;
                        
                        if ((flags & 0x10) != 0) {
                            this->value = 1.0f - calculatedValue;
                        }

                        if ((modifiers  & 8) != 0) {
                            this->value = (this->value - (float)curValue) / this->fineTuneDivider + (float)curValue;
                        }

                        this->clampValue();
                        bool isDirty = this->isDirty();
                        if (isDirty != false) {
                            this->callback(drawingContext,this);
                        }
                        drawingContext->getRelativeMousePos(mousePos);
                        this->onIdle();
                        modifiers = View::GetPressedModifiersAndMouseButtons();
                    }
                    this->parent->endEdit(this->parameterId);
                }
            }
        }
    }
}
}
}