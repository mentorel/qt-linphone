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

/*
 * ListviewDelegate.h
 *
 *  Created on: Jul 17, 2010
 *      Author: http://qt-articles.blogspot.com
 */

#ifndef ListviewDelegate_H_
#define ListviewDelegate_H_
#include <QtGui>

class ListviewDelegate : public QStyledItemDelegate
{
public:
    ListviewDelegate();
    virtual ~ListviewDelegate();

    enum datarole {headerTextRole = Qt::UserRole + 100,subHeaderTextrole = Qt::UserRole+101,IconRole = Qt::UserRole+102};

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const;

    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index ) const;

    void setFontSize(int);

private:
    QFont font;
    QFont SubFont;
};

#endif /* ListviewDelegate_H_ */
