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

#ifndef GUI_CONFIG_H
#define GUI_CONFIG_H

#include <QObject>
#include <QPixmap>
#include <QLabel>

class gui_config : public QObject
{
    Q_OBJECT
public:
    explicit gui_config(QObject *parent = 0);
    QPixmap* getBackground(void) const {return mw_background;}
    QPixmap* getTopPanel(void) const {return c_TopPanel;}
    QPixmap* getBotPanel(void) const {return c_BotPanel;}
    QPixmap* getModePix(int m);
    void setLabelShadow(QLabel *main, QLabel *shadow, QString str);

private:
    QPixmap* mw_background; // holds a picture or solid bkgr color
    QPixmap* c_TopPanel;
    QPixmap* c_BotPanel;
    QPixmap ph_modes[2];
    int load_config(void); // loads the config file and parses it

    QStringList bg_directory;
    int is_mw_bg_color;
    QColor mw_bg_color; // main window background color
    QColor mw_tp_color; // main window top panel color
    QColor mw_bp_color; // main window top panel color
    QString mw_bg_picture; // main window background picture

signals:

public slots:

};

#define PH_MODE_FULL 0
#define PH_MODE_SILENT 1
#define PH_MODE_DND 2

#endif // GUI_CONFIG_H
