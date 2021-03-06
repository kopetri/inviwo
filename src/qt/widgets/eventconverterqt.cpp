/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2013-2015 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 *********************************************************************************/

#include <inviwo/qt/widgets/eventconverterqt.h>
#include <inviwo/qt/widgets/keyboardutils.h>

#include <warn/push>
#include <warn/ignore/all>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QInputEvent>
#include <QKeyEvent>
#include <QGesture>
#include <warn/pop>

namespace inviwo {

EventConverterQt::EventConverterQt() {}
EventConverterQt::~EventConverterQt() {}

MouseEvent::MouseButton inviwo::EventConverterQt::getMouseButton(const QMouseEvent* e) {
    switch (e->buttons())
    {
    case Qt::LeftButton:
        return MouseEvent::MOUSE_BUTTON_LEFT;
    case Qt::RightButton:
        return MouseEvent::MOUSE_BUTTON_RIGHT;
    case Qt::MiddleButton:
        return MouseEvent::MOUSE_BUTTON_MIDDLE;
    default:
        return MouseEvent::MOUSE_BUTTON_NONE;
    }
}

MouseEvent::MouseButton EventConverterQt::getMouseButtonCausingEvent(const QMouseEvent* e) {
    // QMouseEvent::getButtons does not
    // include the button that caused the event.
    // The QMouseEvent::getButton function
    // returns the button that caused the event
    #include <warn/push>
    #include <warn/ignore/switch-enum>
    switch (e->button())
    {
    case Qt::LeftButton:
        return MouseEvent::MOUSE_BUTTON_LEFT;
    case Qt::RightButton:
        return MouseEvent::MOUSE_BUTTON_RIGHT;
    case Qt::MiddleButton:
        return MouseEvent::MOUSE_BUTTON_MIDDLE;
    default:
        return MouseEvent::MOUSE_BUTTON_NONE;
    }
    #include <warn/pop>
}

MouseEvent::MouseButton inviwo::EventConverterQt::getMouseWheelButton(const QWheelEvent* e) {
    if (e->buttons() == Qt::LeftButton)
        return MouseEvent::MOUSE_BUTTON_LEFT;
    else if (e->buttons() == Qt::RightButton)
        return MouseEvent::MOUSE_BUTTON_RIGHT;
    else if (e->buttons() == Qt::MiddleButton)
        return MouseEvent::MOUSE_BUTTON_MIDDLE;

    return MouseEvent::MOUSE_BUTTON_NONE;
}

GestureEvent::GestureState inviwo::EventConverterQt::getGestureState(const QGesture* gesture){
    if (gesture->state() == Qt::GestureStarted)
        return GestureEvent::GESTURE_STATE_STARTED;
    else if (gesture->state() == Qt::GestureUpdated)
        return GestureEvent::GESTURE_STATE_UPDATED;
    else if (gesture->state() == Qt::GestureFinished)
        return GestureEvent::GESTURE_STATE_ENDED;
    else if (gesture->state() == Qt::GestureCanceled)
        return GestureEvent::GESTURE_STATE_CANCELED;

    return GestureEvent::GESTURE_STATE_NONE;
}

InteractionEvent::Modifier inviwo::EventConverterQt::getModifier(const QInputEvent* e) {
    int modifier = static_cast<int>(InteractionEvent::MODIFIER_NONE);
    if (e->modifiers() == Qt::ShiftModifier)
        modifier |= static_cast<int>(InteractionEvent::MODIFIER_SHIFT);
    if (e->modifiers() == Qt::ControlModifier)
        modifier |= static_cast<int>(InteractionEvent::MODIFIER_CTRL);
    if (e->modifiers() == Qt::AltModifier)
        modifier |= static_cast<int>(InteractionEvent::MODIFIER_ALT);

    return static_cast<InteractionEvent::Modifier>(modifier);
}

int EventConverterQt::getKeyButton(const QKeyEvent* e) {
    return static_cast<int>(util::mapKeyFromQt(e));
}

} // namespace