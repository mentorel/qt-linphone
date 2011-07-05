/*
* Copyright (C) 2011 Maxim Podberezny <lisarden@gmail.com>
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*/

#include "statemachine.h"
#include <QDebug>

stateMachine::stateMachine(int root)
{
    m_root = root;
    list_sz = 1;
    list_tail = 0;
    list[list_tail] = root;
}

void stateMachine::setState(int s)
{
    qDebug() << "setState " << s;
    if (s == m_root) {
        list_sz = 1;
        list_tail = 0;
    }
    else {
        if (list_sz >= BL_SIZE) qFatal("setState: list size critical!");
        list_sz++;
        list_tail++;
    }
    list[list_tail] = s;
}

int stateMachine::getState()
{
    qDebug() << "getState " << list[list_tail];
    return list[list_tail];
}

int stateMachine::getPrevState()
{
    if (list_sz > 1) {
        qDebug() << "prevSt=" << list[list_tail-1] << "curSt=" << list[list_tail];
        list_sz --;
        list_tail --;
        return list[list_tail];
    }
    qFatal("getPrevState: I'm at root!");
    return m_root;
}
