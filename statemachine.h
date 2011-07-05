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

#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#define BL_SIZE 10

class stateMachine
{
public:
    stateMachine(int);
    int getState();// const {return m_state;}
    int getPrevState();// const {return m_prev;}
    void setState(int);// {m_prev=m_state;m_state=newState;}

private:
    //int m_state; // current state
    //int m_prev; // previous state
    int m_root;
    int list[BL_SIZE];
    int list_sz;
    int list_tail;
};

#endif // STATEMACHINE_H
