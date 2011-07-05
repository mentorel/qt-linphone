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

#ifndef QTLINPHONE_H
#define QTLINPHONE_H

#include <QWidget>
#include <QStackedWidget>
#include "gui_config.h"
#include <QTimer>
#include <QModelIndex>
#include <QShortcut>
#include <QWSEvent>

#include "addressbook.h"
#include "linphone.h"
#include "statemachine.h"

namespace Ui {
    class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    gui_config* guiConfig;
    void setMissed(int);

private:
    Ui::Widget *ui;
    QTimer *iTimer;
    QString my_ipaddr;
    QString my_netmask;
    int amrMode;
    int hasMissed;
    int firstNumber;
    int lastPageIndex;
    linphone *lp;
    int missedLedState;
    AddressBook *abook;
    stateMachine *states;

    void setFullMode(void);
    void setSilentMode(void);
    void setDNDMode(void);
    void showWinAbook(int);
    void showWinLog(int);
    void showWinDial(int num);
    void showWinMenu();
    void showWinIdle();
    void showWinParams();
    void missedLed(int state);
    int getNetInfo(QString iname, QString *ip, QString *mask);
    void setupGUI();
    void dialNumber(int bookIndex, QString number);
    void save2log(int logType, int bookIndex, QString number);
    bool qwsEvent ( QWSEvent * event );
    bool eventFilter(QObject *, QEvent *);
    bool guiSM(QWSEvent *e);
    QTime cancel_time;
    int time_diff;

#if 0
public slots:
    void registerEvent(int);
    void ringEvent(int callid);
    void connectEvent(int callid);
    void answerEvent(int callid);
    void disconnectEvent(int callid);
#endif

protected slots:
    void iTimerOut(void);
    void setMode(int mode);
private slots:
    void on_menuMainList_activated(QModelIndex index);
    void on_aBookList_activated(QModelIndex index);
    void on_aBookContact_activated(QModelIndex index);
    void on_lineEditDial_returnPressed();
    void on_menuParamList_activated(QModelIndex index);
};

#define WIN_IDLE 0
#define WIN_DIALINPUT 1
#define WIN_MENU 2
#define WIN_ABOOK 3
#define WIN_CONTACT 4
#define WIN_ASKNUM 5
#define WIN_LOG 6

#define CALLS_MISSED 0
#define CALLS_INCOMING 1
#define CALLS_OUTGOING 2

#define LONG_CANCEL_MS 1000 /* 1 secs*/

#endif // QTLINPHONE_H
