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

#ifndef ADDRESSBOOK_H
#define ADDRESSBOOK_H

#include "contact.h"
#include <QList>
#include <QHash>

class AddressBook
{
public:
    AddressBook();
    int init(QString file); // return a number of contacts in a book
    void add(Contact c);
    void del(int index);
    void edit(int index, Contact c);
    Contact at(int index) const {return m_contacts.at(index);};
    Contact next(void);
    bool hasNext(void);
    void setCurrentIndex(int index);
    int getCurrentIndex(void) const {return m_curIndex;}
    int size(void) const {return m_contacts.size();}
    void save(QString file);
    void save(void);
    bool hasContact(QString number) const {return m_hash.contains(number);}
    Contact findContact(QString number) const {return m_contacts.at(m_hash.value(number));}
    int numType(Contact c, QString n);

private:
    int m_curIndex;
    QString m_file;
    QList<Contact> m_contacts;
    void save2file(QString file);
    QHash<QString, int> m_hash;
    void append2hash(int);
};

#define NUM_UNKNOWN 0
#define NUM_WORK 1
#define NUM_CELL 2
#define NUM_HOME 3

#endif // ADDRESSBOOK_H
