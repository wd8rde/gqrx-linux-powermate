/* File:   powermate.cpp
 * Author: Laakkonen, Calle - http://www.luolamies.org/software/misc/
 *
 * Created on 17 September 2012, 15:00 Utc
 */

/* Copyright (C)
* Laakkonen, Calle 2009 (with permission)
* QtRadio integration by Oliver Goldenstein, DL6KBG
* gqrx integration by Bob Bouterse, WD8RDE
* 
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*
*/
#ifdef POWERMATE

#include <QDebug>
#include "powermate.h"
#include <linux/input.h>
#include <QApplication>

PmInput::PmInput(QObject *parent, const char *device)
    : QThread(parent) {
    if (device) {
        devicename = device;
    } else {
        devicename = "/dev/powermate";
    }
    m_buttons = Qt::NoButton;
}

void PmInput::run() {
    QFile device(devicename);

    if (!device.open(QIODevice::ReadOnly | QIODevice::Unbuffered)) {
        qDebug() << "Error opening device " << device.fileName()
            << ": " << device.error();
        return;
    }

    struct input_event iev;
    while (1) {
        int r = device.read((char *)&iev, sizeof(struct input_event));
        if (r == 0) break;
        if (r != sizeof(struct input_event)) {
            qDebug() << "read only " << r << "bytes, expected " << sizeof(struct input_event);
        } else {
            processEvent(iev);
        }
    }
}

PmInput::~PmInput() {
}

void PmInput::processEvent(const input_event &event) {
    int delta = 0;
    Qt::KeyboardModifiers modifiers = QApplication::queryKeyboardModifiers();

    switch (event.type) {
    case EV_REL:
        if (event.code != REL_DIAL) {
            qDebug() << "Unexpected rotation event: " << event.code;
        } else {
            delta = event.value;
        }
        break;
    case EV_KEY:
        if (event.code != BTN_0) {
            qDebug() << "Unexpected button " << event.code;
        } else {
            if (event.value){
                m_buttons = Qt::MidButton;
            }
            else {
                m_buttons = Qt::NoButton;
            }
        }
        break;
    default:
        return;
        break;
    }
    KnobEvent myevent(delta, m_buttons, modifiers);
    emit knobEvent(myevent);
}
/* 
set powermate led
void PmInput::setLed(const input_event& event)
{

}
*/
#endif //LINUX
