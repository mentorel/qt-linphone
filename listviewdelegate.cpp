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
 * ListviewDelegate.cpp
 *
 *  Created on: Jul 17, 2010
 *      Author: http://qt-articles.blogspot.com
 */

#include "listviewdelegate.h"

ListviewDelegate::ListviewDelegate()
{
    font = QApplication::font();
    SubFont = QApplication::font();
    font.setBold(true);
    SubFont.setWeight(SubFont.weight()-2);
}

ListviewDelegate::~ListviewDelegate()
{
    // TODO Auto-generated destructor stub
}

//alocate each item size in listview.
QSize ListviewDelegate::sizeHint(const QStyleOptionViewItem &  option ,
                              const QModelIndex & index) const
{
    QIcon icon = qvariant_cast<QIcon>(index.data(IconRole));
    QSize iconsize = icon.actualSize(option.decorationSize);
    QFont font = QApplication::font();
    QFontMetrics fm(font);

    return(QSize(iconsize.width(),/*iconsize.height()+*/fm.height()*2+4));

}

void ListviewDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                           const QModelIndex &index) const
{
    QStyledItemDelegate::paint(painter,option,index);

    painter->save();

    QFontMetrics fm(font);

    QIcon icon = qvariant_cast<QIcon>(index.data(IconRole));
    QString headerText = qvariant_cast<QString>(index.data(headerTextRole));
    QString subText = qvariant_cast<QString>(index.data(subHeaderTextrole));

    QSize iconsize = icon.actualSize(option.decorationSize);

    QRect headerRect = option.rect;
    QRect subheaderRect = option.rect;
    QRect iconRect = subheaderRect;

    iconRect.setRight(iconsize.width()+30);
    iconRect.setTop(iconRect.top());
    headerRect.setLeft(30);
    subheaderRect.setLeft(30);
    headerRect.setTop(headerRect.top()+1);
    headerRect.setBottom(headerRect.top()+fm.height());

    subheaderRect.setTop(headerRect.bottom()-5);

    painter->drawPixmap(QPoint(iconRect.left()+iconsize.width()/2,iconRect.top()+iconsize.height()/2),icon.pixmap(iconsize.width(),iconsize.height()));

    painter->setFont(font);
    painter->drawText(headerRect,headerText);


    painter->setFont(SubFont);
    painter->drawText(subheaderRect.left(),subheaderRect.top()+17,subText);

    painter->restore();

}

void ListviewDelegate::setFontSize(int size)
{
    font.setPixelSize(size);
    SubFont.setPixelSize(size);
}
