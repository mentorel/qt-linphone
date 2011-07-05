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

#ifndef CONTACT_H
#define CONTACT_H

#include <QString>
#include <QtXml>

class Contact
{
public:
    Contact(QString iName1="", QString iName2="", QString iwPhone="", QString icPhone = "", QString ihPhone="");
    QString name1;
    QString name2;
    QString wPhone; // work phone
    QString cPhone; // cell phone
    QString hPhone; // home phone
};

#endif // CONTACT_H
