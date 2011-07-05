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

#include <QColor>
#include <QPixmap>
#include <QDir>
#include "gui_config.h"
#include <iostream>

gui_config::gui_config(QObject *parent) :
    QObject(parent)
{
    load_config();
    mw_background = new QPixmap(320, 240);
    if (is_mw_bg_color) {
        mw_background->fill(mw_bg_color);
    }
    else {
        mw_background->load(mw_bg_picture);
    }
}

int gui_config::load_config(void)
{
    QDir directory = QDir(QDir::currentPath()+QString("/pics/background"));
    QStringList filters;
    filters << "*.png";
    directory.setNameFilters(filters);
    // Get a list of background pictures
    bg_directory = directory.entryList(QDir::AllEntries);
    if (bg_directory.isEmpty()) {
        // no PNG files are found
        is_mw_bg_color = 0; // set Main Window background to picture
    }
    else {
        mw_bg_picture = QDir::currentPath()+QString("/pics/background/")+bg_directory.at(0);
    }

    mw_bg_color = QColor("black");
    mw_tp_color = QColor("steelblue");
    mw_tp_color.setAlpha(200);
    c_TopPanel = new QPixmap(320, 25);
    c_TopPanel->fill(mw_tp_color);
    mw_bp_color = QColor("steelblue");
    mw_bp_color.setAlpha(200);
    c_BotPanel = new QPixmap(320, 40);
    c_BotPanel->fill(mw_bp_color);

    if (ph_modes[0].load(QDir::currentPath()+QString("/pics/spk_silent.png"))==false) {
        ph_modes[0] = NULL;
    }
    if (ph_modes[1].load(QDir::currentPath()+QString("/pics/dnd.png"))==false) {
        ph_modes[1] = NULL;
    }

    return 0;
}

QPixmap* gui_config::getModePix(int m)
{
    switch (m) {
    case PH_MODE_FULL:
        return NULL;
        break;
    case PH_MODE_SILENT:
        return &ph_modes[0];
        break;
    case PH_MODE_DND:
        return &ph_modes[1];
        break;
    default: return NULL;
    }
}

void gui_config::setLabelShadow(QLabel *main, QLabel *shadow, QString str)
{
    main->setText(str);
    shadow->setText(str);
}
