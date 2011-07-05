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

#include "addressbook.h"
#include <QFile>
#include <QtXml>
#include <iostream>

AddressBook::AddressBook()
{
}

int AddressBook::init(QString bookFile)
{
    QFile file(bookFile);
    QDomDocument doc("aBookML");

    if( !file.open( QIODevice::ReadOnly ) ) {
        qDebug("E: error open file");
        return -1; // error open file
    }
    if( !doc.setContent( &file ) ) {
      file.close();
      qDebug("E: error XML parse");
      return -2; // error XML parse
    }
    file.close();

    m_file = bookFile;
    // begin to parse XML phone book
    QDomElement root = doc.documentElement();
    std::cout << root.tagName().toStdString().c_str() << std::endl;
    if( root.tagName() != "adbook" ) {
        qDebug("E: no root element");
        return -3; // no root element
    }
    // everything is fine now
    m_hash.clear();
    m_contacts.clear();
    m_curIndex = -1; // so as next works correctly
    QDomNode node = root.firstChild();
    while (!node.isNull()) {
        int ignore = 0;
        if (node.toElement().tagName() == "person") {
            QDomNode childNode = node.firstChild();
            Contact c;
            while (!childNode.isNull()) {
                if (childNode.toElement().tagName() == "name1") {
                    c.name1 = childNode.toElement().text();
                    //std::cout << c.name1.toStdString().c_str() << std::endl;
                }
                else if (childNode.toElement().tagName() == "name2") {
                    c.name2 = childNode.toElement().text();
                }
                else if (childNode.toElement().tagName() == "wphone") {
                    c.wPhone = childNode.toElement().text();
                }
                else if (childNode.toElement().tagName() == "cphone") {
                    c.cPhone = childNode.toElement().text();
                }
                else if (childNode.toElement().tagName() == "hphone") {
                    c.hPhone = childNode.toElement().text();
                }
                else // unknown field - ignore all!
                    ignore = 1;
                childNode = childNode.nextSibling();
            }
            if (!ignore) {
                m_contacts.append( c );
                append2hash(m_contacts.size()-1);
            }
        }
        node = node.nextSibling();
    }

    return m_contacts.size();
}

bool AddressBook::hasNext()
{
    if (m_curIndex+2 <= m_contacts.size())
        return true;
    else
        return false;
}

Contact AddressBook::next()
{
    // return next Contact
    Contact c = m_contacts.at(m_curIndex+1);
    m_curIndex++;
    return c;
}

void AddressBook::setCurrentIndex(int index)
{
    if (index < 0) return; // just in case
    if (index < m_contacts.size()) m_curIndex=index;
}

void AddressBook::add(Contact c)
{
    m_contacts.append(c);
}

void AddressBook::edit(int index, Contact c)
{
    m_contacts.replace(index, c);
}

void AddressBook::del(int index)
{
    m_contacts.removeAt(index);
}

void AddressBook::save(QString file)
{
    save2file(file);
}

void AddressBook::save(void)
{
    save2file(m_file);
}

void AddressBook::save2file(QString f)
{
    QDomDocument doc;
    QDomProcessingInstruction instr = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\" ");
    doc.appendChild(instr);

    QDomElement root = doc.createElement( "adbook" );
    doc.appendChild( root );

    for( QList<Contact>::iterator it = m_contacts.begin(); it != m_contacts.end(); ++it ) {
        QDomElement person = doc.createElement( "person" );
        QDomElement name1 = doc.createElement("name1");
        QDomElement name2 = doc.createElement("name2");
        QDomElement wphone = doc.createElement("wphone");
        QDomElement cphone = doc.createElement("cphone");
        QDomElement hphone = doc.createElement("hphone");

        QDomText text1 = doc.createTextNode( (*it).name1 );
        QDomText text2 = doc.createTextNode( (*it).name2 );
        QDomText text3 = doc.createTextNode( (*it).wPhone );
        QDomText text4 = doc.createTextNode( (*it).cPhone );
        QDomText text5 = doc.createTextNode( (*it).hPhone );

        root.appendChild( person );

        person.appendChild( name1 );
        person.appendChild( name2 );
        person.appendChild( wphone );
        person.appendChild( cphone );
        person.appendChild( hphone );

        name1.appendChild( text1 );
        name2.appendChild( text2 );
        wphone.appendChild( text3 );
        cphone.appendChild( text4 );
        hphone.appendChild( text5 );
    }

    QFile file( f );
    if( !file.open( QIODevice::WriteOnly | QIODevice::Text ) )
    {
      qDebug( "Failed to open file for writing." );
      return;
    }

    QTextStream stream( &file );
    doc.save(stream, 4);

    file.close();
}

void AddressBook::append2hash(int c)
{
    if (m_contacts.at(c).wPhone.size()) m_hash.insert(m_contacts.at(c).wPhone, c);
    if (m_contacts.at(c).cPhone.size()) m_hash.insert(m_contacts.at(c).cPhone, c);
    if (m_contacts.at(c).hPhone.size()) m_hash.insert(m_contacts.at(c).hPhone, c);
}

int AddressBook::numType(Contact c, QString n)
{
    if (c.wPhone == n) return NUM_WORK;
    if (c.cPhone == n) return NUM_CELL;
    if (c.hPhone == n) return NUM_HOME;
    return NUM_UNKNOWN;
}
