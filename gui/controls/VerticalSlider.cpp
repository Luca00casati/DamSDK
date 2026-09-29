#pragma once
#include "VerticalSlider.h"
#include "damsdk/gui/platform/windows/OffscreenGDIDrawingContext.h"
#include "damsdk/gui/platform/windows/Bitmap.h"
#include "damsdk/gui/platform/windows/Window.h"

namespace DamSDK {
namespace Gui {
namespace Controls {

    // FUNCTION: DELAYLAMA 0x10009a40
    VerticalSlider::VerticalSlider(RECT *pRect, callbackCallback callback, int parameterId, int minValue, int maxValue, Platform::Windows::Bitmap *handleBmp, Platform::Windows::Bitmap *backgroundBmp, POINT* offset, int flags) : Control(pRect, callback, parameterId, backgroundBmp)
    {
        this->backgroundOffset.x = offset->x;
        this->backgroundOffset.y = offset->y;
        this->handlePos.x = 0;
        this->handlePos.y = 0;
        this->flags = flags;
        this->handleImage = handleBmp;
        this->trackMinY = minValue;
        this->trackMaxY = maxValue;
        this->snapToMouse = true;
        this->isHandleTransparent = true;
        if (handleBmp == nullptr) {
            this->handleWidth = 1;
            this->handleHeight = 1;
        }
        else {
            handleBmp->remember();
            this->handleWidth = this->handleImage->width;
            this->handleHeight = this->handleImage->height;
        }
        this->trackWidth = this->rect.right - this->rect.left;
        this->trackTopY = maxValue;
        this->trackHeight = this->rect.bottom - this->rect.top;
        this->handlePos.x = 0;
        this->handlePos.y = 0;
        this->fineTuneDivider = 10.0f;
        this->handleMinPos = minValue - this->rect.top;
        this->handleMaxPos = (this->handleHeight - this->rect.top) + maxValue;
    }

    // FUNCTION: DELAYLAMA 0x10009bf0
    VerticalSlider::~VerticalSlider() {
        if (this->handleImage != nullptr) {
          this->handleImage->unregisterBitmap();
        }
    }

     // FUNCTION: DELAYLAMA 0x10009c50
     void VerticalSlider::onDraw(Platform::Windows::GDIDrawingContext* drawingContext) {
        // Same steps as VSTGUI's CSlider::draw, always through an offscreen context.
        float value;
        if (this->flags & 0x20)
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
        handleRect.left = this->handlePos.x;
        handleRect.right = handleRect.left + this->handleWidth;
        handleRect.top = this->handlePos.y + (int)(value * (this->trackMaxY - this->trackMinY));
        if (handleRect.top < this->handleMinPos)
            handleRect.top = this->handleMinPos;
        handleRect.bottom = handleRect.top + this->handleHeight;
        if (handleRect.bottom > this->handleMaxPos)
            handleRect.bottom = this->handleMaxPos;

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

        this->trackTopY = this->rect.top + handleRect.top;
        this->setDirty(false);
    }

    // FUNCTION: DELAYLAMA 0x10009e10
    void VerticalSlider::onMouseDown(Platform::Windows::GDIDrawingContext* drawingContext, POINT* relativeMousePos) {
        if (this->isEnabled != false) {

            uint8_t modifiers = View::GetPressedModifiersAndMouseButtons();

            // Ctrl + Left Click -> Reset
            if (modifiers == 0x11) { 
                this->value = this->getDefaultValue();
                if (this->isDirty()) {
                    this->callback(drawingContext, this);
                }
                return;
            }

            // Left Click dragging
            if ((modifiers & 0x01) != 0) {
                int verticalAnchor;
                
                int trackMinY = this->trackMinY;
                int trackMaxY = this->trackMaxY;

                if (!this->snapToMouse) {
                    trackMaxY = this->rect.left;
                    trackTopY = this->trackTopY;
                    if (relativeMousePos->x < trackMaxY) {
                        return;
                    }
                    if (this->handleWidth + trackMaxY < relativeMousePos->x) {
                        return;
                    }
                    verticalAnchor = relativeMousePos->y;
                    if (verticalAnchor < trackTopY) {
                        return;
                    }
                    if (this->handleHeight + trackTopY < verticalAnchor) {
                        return;
                    }
                    verticalAnchor -= trackTopY;
                } else {
                    verticalAnchor = (this->handleHeight / 2) - 1;
                }

                verticalAnchor += trackMinY;
                
                float previousValue = this->value;
                this->parent->beginEdit(this->parameterId);

                while ((modifiers & 0x01) != 0) {
                    float calculatedValue = (float)(relativeMousePos->y - verticalAnchor) / (float)(trackMaxY - trackMinY);
                    
                    // Reverse if flag 0x40 is set (for vertical slider pitch control)
                    if ((this->flags & 0x40) != 0) {
                        calculatedValue = 1.0f - calculatedValue;
                    }

                    // Fine-tuning logic (usually Shift key = 0x08)
                    if ((modifiers & 0x08) != 0) {
                        this->value = ((calculatedValue - previousValue) / this->fineTuneDivider) + previousValue;
                    } else {
                        this->value = calculatedValue;
                        previousValue = calculatedValue;
                    }

                    this->clampValue();

                    if (this->isDirty()) {
                        this->callback(drawingContext, this);
                    }

                    drawingContext->getRelativeMousePos(relativeMousePos);
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