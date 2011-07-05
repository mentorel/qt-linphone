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

#include "qt-linphone.h"
#include "ui_form.h"
#include <iostream>
#include <QTextCodec>
#include <QDateTime>
#include <QHostInfo>
#include <QNetworkInterface>
#include <QStandardItemModel>
#include <QTabWidget>

#include "listviewdelegate.h"

#define Missed_N 0
#define Missed_Y 1

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    QCoreApplication::setApplicationName("qt-linphone");
    QCoreApplication::setOrganizationName("MENTOREL");
    QApplication::font().setPointSize(10);
    ui->setupUi(this);
    setFixedSize(320, 240);
    this -> setWindowFlags(Qt::FramelessWindowHint);
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

    guiConfig = new gui_config();
    setupGUI();

    // create State Machine
    states = new stateMachine(WIN_IDLE);

    ui->lp_mode_combo->addItem(tr("Обычный"));
    ui->lp_mode_combo->addItem(tr("Тихий"));
    ui->lp_mode_combo->addItem(tr("Не беспокоить"));
    QObject::connect(ui->lp_mode_combo,SIGNAL(activated(int)),this,SLOT(setMode(int)));
    setMode(PH_MODE_FULL);

    // get current system time
    ui->dateTime->setText(QDateTime::currentDateTime().toString("hh:mm"));
    ui->dateTime->setStyleSheet("color: white");
    // start a timer to tick for 1 secs.
    iTimer = new QTimer();
    iTimer->setInterval(1000);
    QObject::connect(iTimer, SIGNAL(timeout()), this, SLOT(iTimerOut()));
    iTimer->start();

    // get network ip and mask
    if (!getNetInfo("eth0", &my_ipaddr, &my_netmask)) {
        guiConfig->setLabelShadow(ui->my_ipaddr,ui->my_ipaddr_s, my_ipaddr);
    }
    else {
        guiConfig->setLabelShadow(ui->my_ipaddr,ui->my_ipaddr_s, tr("ошибка"));
    }

    // No Missed calls from a cold start
    hasMissed = 0;
    missedLedState = 0;
    setMissed(hasMissed);

    showWinIdle();

    // create an Address Book
    abook = new AddressBook();

    lastPageIndex = 0;

    // create Linphone class
    lp = new linphone();

    ui->lineEditDial->installEventFilter(this);

    ui->menuMainList->setCurrentRow(0);

    // start cancel timer just in case
    cancel_time.start();
}

Widget::~Widget()
{
    iTimer->stop();
    delete ui;
}

void Widget::iTimerOut(void)
{
    QString dt = QDateTime::currentDateTime().toString("hh:mm");
    ui->dateTime->setText(dt);
}

void Widget::setupGUI()
{
    // set background
    ui->background->setPixmap(*guiConfig->getBackground());

    // set Top Panel
    ui->panelTop->setPixmap(*guiConfig->getTopPanel());

    // set Bottom panel
    ui->panelBot->setPixmap(*guiConfig->getBotPanel());

    // set Top Panel text color
    QPalette pal(QColor("white"));
    pal.setColor(QPalette::WindowText, QColor("white"));
    ui->winCaptionMenu->setPalette(pal);
    ui->winCaptionDial->setPalette(pal);
    ui->winCaptionPhbook->setPalette(pal);
    ui->winCaptionContact->setPalette(pal);
    ui->winCaptionNumbers->setPalette(pal);
    ui->winCaptionLog->setPalette(pal);

    // set Font size
    QFont capFont;
    capFont.setPixelSize(16);
    ui->winCaptionMenu->setFont(capFont);
    ui->winCaptionDial->setFont(capFont);
    ui->winCaptionPhbook->setFont(capFont);
    ui->winCaptionContact->setFont(capFont);
    ui->winCaptionNumbers->setFont(capFont);
    ui->winCaptionLog->setFont(capFont);
    ui->aBookList->setFont(capFont);
    ui->menuMainList->setFont(capFont);
    ui->menuMainList->setIconSize(QSize(24,24));
    ui->aBookContact->setFont(capFont);
    ui->lp_mode_combo->setFont(capFont);
    ui->dateTime->setFont(capFont);

    QFont errorFont;
    errorFont.setPixelSize(16);
    QPalette epal;
    epal.setColor(QPalette::WindowText, QColor("red"));
    epal.setColor(QPalette::Window, QColor("white"));
    ui->dialLineError->setAutoFillBackground(true);
    ui->dialLineError->setFont(errorFont);
    ui->dialLineError->setPalette(epal);
    ui->dialLineError->hide();

    QFont winFont;
    winFont.setPixelSize(15);
    ui->label->setFont(winFont);
    ui->label_2->setFont(winFont);
    ui->label_3->setFont(winFont);
    ui->label_4->setFont(winFont);
    ui->my_ipaddr_s->setFont(winFont);
    ui->sip_register_s->setFont(winFont);
    ui->my_ipaddr->setFont(winFont);
    ui->sip_register->setFont(winFont);

    QFont dialFont;
    dialFont.setPixelSize(30);
    ui->lineEditDial->setFont(dialFont);

    qApp->setStyleSheet("QPushButton { border: 0px; color: white; background-color: rgba(51,51,204,200);}");
}

int Widget::getNetInfo(QString iname, QString *ip, QString *mask)
{
    QList<QNetworkInterface> inets = QNetworkInterface::allInterfaces();
    if (inets.size() > 0) {
        for (int i = 0; i < inets.size(); i++) {
            qDebug() << inets.at(i).name();
            if (inets.at(i).name() == iname) {
                QList<QNetworkAddressEntry> iaddrs = inets.at(i).addressEntries();
                if (iaddrs.size() > 0) {
                    *mask = iaddrs.at(0).netmask().QHostAddress::toString();
                    *ip = iaddrs.at(0).ip().QHostAddress::toString();
                    qDebug() << iaddrs.at(0).netmask().QHostAddress::toString();
                    qDebug() << iaddrs.at(0).ip().QHostAddress::toString();
                    return 0;
                }
            }
        }
    }
    return 1;
}

// Set phone mode: full | silent | do not disturb
void Widget::setMode(int mode)
{
    switch (mode) {
    case 0: // switch to full function mode
            qDebug("I: switch to full function mode");
            if (guiConfig->getModePix(PH_MODE_FULL)!=NULL) {
                ui->phone_mode->setPixmap(*guiConfig->getModePix(PH_MODE_FULL));
            }
            else ui->phone_mode->clear();
            setFullMode();
            break;
    case 1: // silent mode
            qDebug("I: switch to silent mode");
            if (guiConfig->getModePix(PH_MODE_SILENT)!=NULL) {
                ui->phone_mode->setPixmap(*guiConfig->getModePix(PH_MODE_SILENT));
            }
            else ui->phone_mode->clear();
            setSilentMode();
            break;
    case 2: // do not disturb
            qDebug("I: switch to DND mode");
            if (guiConfig->getModePix(PH_MODE_DND)!=NULL) {
                ui->phone_mode->setPixmap(*guiConfig->getModePix(PH_MODE_DND));
            }
            else ui->phone_mode->clear();
            setDNDMode();
    default: break;
    }
}

void Widget::setMissed(int s)
{
    if (!s) {
        ui->missed_calls->hide();
        ui->missed_label->hide();
        ui->missed_label_s->hide();
        ui->missedNum->hide();
        ui->missedNum_s->hide();
        missedLed(0);
    }
    else {
        ui->missed_calls->show();
        ui->missed_label->show();
        ui->missed_label_s->show();
        guiConfig->setLabelShadow(ui->missedNum, ui->missedNum_s, QString::number(s));
        ui->missedNum->show();
        ui->missedNum_s->show();
        missedLed(1);
    }
}

void Widget::missedLed(int state)
{
    qDebug("W: MissedLed not implemented!");
    if (missedLedState != state) {
        if (!state) {
            qDebug("I: MissedLed OFF");
            missedLedState = 0;
        }
        else {
            qDebug("I: MissedLed ON");
            missedLedState = 1;
        }
    }
}

void Widget::setFullMode()
{
    qDebug("W: setFullMode not implemented!");
}

void Widget::setSilentMode()
{
    qDebug("W: setSilentMode not implemented!");
}

void Widget::setDNDMode()
{
    qDebug("W: setDNDMode not implemented!");
}

void Widget::on_menuMainList_activated(QModelIndex index)
{
    qDebug() << "on_menuMainList_activated " << index.row();
    switch (index.row()) {
    case 0: // Address book
        showWinAbook(0);
        break;
    case 1: // Calls log
        showWinLog(CALLS_OUTGOING);
        break;
    case 2: // Parameters
        showWinParams();
        break;
    default: // unknown menu item!
        qDebug() << "E: unknown menu item!";
    }
}

void Widget::showWinParams()
{
    ui->guiPages->setCurrentWidget(ui->menuParam);
    states->setState(ui->guiPages->currentIndex());
    ui->menuParamList->setCurrentRow(0);
}

void Widget::showWinAbook(int s)
{
    if (!s) {
        if (abook->init("abook.xml") > 0) {
            //int i=0;
            Contact c;
            while (abook->hasNext()) {
                c = abook->next();
                QString fullName = c.name1 + " " + c.name2;
                ui->aBookList->addItem(fullName);
            }
        }

        ui->aBookList->setCurrentRow(0);
        ui->aBookList->setFocus(Qt::OtherFocusReason);
        ui->aBookList->setFocusPolicy(Qt::StrongFocus);
    }
    ui->guiPages->setCurrentWidget(ui->phbookList);
    states->setState(ui->guiPages->currentIndex());
}

void Widget::on_aBookList_activated(QModelIndex index)
{
    Contact c;
    int added = 0;
    qDebug() << "aBookList_activated" << index.row();

    c = abook->at(index.row());
    ui->winCaptionContact->setText(c.name1 + " " + c.name2);
    ui->guiPages->setCurrentWidget(ui->phbookPage);
    states->setState(ui->guiPages->currentIndex());

    QStandardItemModel *model;
    ListviewDelegate *listdelegate;
    model = new QStandardItemModel();
    listdelegate = new ListviewDelegate();
    listdelegate->setFontSize(14);

    ui->aBookContact->setItemDelegate(listdelegate); //connect the delegate to view
    ui->aBookContact->setModel(model);//connect the model to view.

    if (c.name1.size()!=0) {
        QStandardItem *name1 = new QStandardItem();
        name1->setData(tr("Имя"),ListviewDelegate::headerTextRole);
        name1->setData(c.name1,ListviewDelegate::subHeaderTextrole);
        model->appendRow(name1);
        added++;
    }
    if (c.name2.size()!=0) {
        QStandardItem *name2 = new QStandardItem();
        name2->setData(tr("Фамилия"),ListviewDelegate::headerTextRole);
        name2->setData(c.name2,ListviewDelegate::subHeaderTextrole);
        model->appendRow(name2);
        added++;
    }
    firstNumber = 0;
    if (c.wPhone.size()!=0) {
        QStandardItem *phone = new QStandardItem();
        QIcon icon("pics/factory_icon.png");
        phone->setData(tr("Тел. (служебный)"),ListviewDelegate::headerTextRole);
        phone->setData(c.wPhone,ListviewDelegate::subHeaderTextrole);
        phone->setData(icon,ListviewDelegate::IconRole);
        model->appendRow(phone);
        firstNumber = added++;
    }
    if (c.cPhone.size()!=0) {
        QStandardItem *phone = new QStandardItem();
        QIcon icon("pics/phone_icon.png");
        phone->setData(tr("Тел. (мобильный)"),ListviewDelegate::headerTextRole);
        phone->setData(c.cPhone,ListviewDelegate::subHeaderTextrole);
        phone->setData(icon,ListviewDelegate::IconRole);
        model->appendRow(phone);
        if (!firstNumber) firstNumber = added;
        added++;
    }
    if (c.hPhone.size()!=0) {
        QStandardItem *phone = new QStandardItem();
        QIcon icon("pics/home_icon.png");
        phone->setData(tr("Тел. (домашний)"),ListviewDelegate::headerTextRole);
        phone->setData(c.hPhone,ListviewDelegate::subHeaderTextrole);
        phone->setData(icon,ListviewDelegate::IconRole);
        model->appendRow(phone);
        if (!firstNumber) firstNumber = added;
    }
    QModelIndex i = ui->aBookContact->model()->index(firstNumber,0);
    ui->aBookContact->selectionModel()->setCurrentIndex(i,
                                            QItemSelectionModel::SelectCurrent);

    ui->aBookContact->setFocus();
    ui->aBookContact->setFocusPolicy(Qt::StrongFocus);
}

void Widget::on_aBookContact_activated(QModelIndex index)
{
    qDebug() << "aBookContact_activated" << index.row();
    if (index.row() >= firstNumber) {
        // dial a number
        lp->dialnumber(ui->aBookContact->selectionModel()->
                       currentIndex().data(ListviewDelegate::subHeaderTextrole).toString());
    }
    else {
        if (firstNumber == ui->aBookContact->model()->rowCount()-1) {
            lp->dialnumber(ui->aBookContact->model()->
                    index(firstNumber, 0).data(ListviewDelegate::subHeaderTextrole).toString());
        }
        // ask for number
        qDebug("E: no ASK FOR NUMBER WINDOW!");
    }
}

void Widget::dialNumber(int bookIndex, QString number)
{
    // save the <number> to LogList->Outgoing
    save2log(CALLS_OUTGOING, bookIndex, number);
    lp->dialnumber(number.toStdString().c_str());
}

void Widget::save2log(int logType, int bookIndex, QString number)
{
    switch (logType) {
    case CALLS_MISSED:
        break;
    case CALLS_INCOMING:
        break;
    case CALLS_OUTGOING:
        break;
    default: break;
    }
}

void Widget::showWinLog(int p)
{
    //ui->guiPages->setCurrentIndex(WIN_LOG);
    ui->guiPages->setCurrentWidget(ui->logPage);
    switch (p) {
    case CALLS_MISSED:
        ui->tabsLog->setCurrentIndex(CALLS_MISSED);
        break;
    case CALLS_INCOMING:
        ui->tabsLog->setCurrentIndex(CALLS_INCOMING);
        break;
    case CALLS_OUTGOING:
    default:
        ui->tabsLog->setCurrentIndex(CALLS_OUTGOING);
    }
    states->setState(ui->guiPages->currentIndex());
}

void Widget::showWinDial(int num)
{
    //ui->guiPages->setCurrentIndex(WIN_DIAL);
    ui->guiPages->setCurrentWidget(ui->dialWindow);
    ui->lineEditDial->setFocus(Qt::OtherFocusReason);
    ui->lineEditDial->clear();
    if (num != -1) ui->lineEditDial->setText(QString(QChar(num)));
    states->setState(ui->guiPages->currentIndex());
    ui->dialLineError->hide();
}

void Widget::showWinMenu()
{
    ui->menuMainList->setFocus(Qt::OtherFocusReason);
    //ui->menuMainList->setCurrentRow(0);
    //ui->guiPages->setCurrentIndex(WIN_MENU);
    ui->guiPages->setCurrentWidget(ui->menuMain);
    states->setState(ui->guiPages->currentIndex());
}

bool Widget::qwsEvent(QWSEvent *e)
{
    return guiSM(e);
}

bool Widget::eventFilter(QObject *object, QEvent *event)
{
    if (object == ui->lineEditDial && event->type() == QEvent::KeyPress) {
        // change "*" to "." for IP address entering
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (keyEvent->key() == Qt::Key_Asterisk) {
            QKeyEvent mkey(QEvent::KeyPress, 0, Qt::NoModifier,".", false, 1);
            qApp->sendEvent(object, &mkey);
            return true;
        }
    }
    return false;
}

void Widget::on_lineEditDial_returnPressed()
{
    QString num = ui->lineEditDial->text();

    if (!num.size()) return;
    if (num.contains(".",Qt::CaseSensitive)) {
        // check for valid IP address
        QHostAddress address(num);
        if (address.protocol() != QAbstractSocket::IPv4Protocol &&
                address.protocol() != QAbstractSocket::IPv6Protocol) {
            ui->dialLineError->setText(tr("Ошибка в IP адресе!"));
            ui->dialLineError->show();
            return;
        }
    }
    dialNumber(-1, num);
}

void Widget::showWinIdle()
{
    if (!hasMissed) {
        // no unaswered calls
        ui->butLog->hide();
    }
    else ui->butLog->show();

    //ui->guiPages->setCurrentIndex(WIN_IDLE);
    ui->guiPages->setCurrentWidget(ui->idleWindow);
    states->setState(WIN_IDLE); // very dangerous
}

bool Widget::guiSM(QWSEvent *e)
{
    QWSKeyEvent* KeyEvent =0;

    if(e->type == QWSEvent::Key) {
        KeyEvent = (QWSKeyEvent*) e;
        if (KeyEvent==0) return false;
        qDebug("key %c %d %d %d\n", KeyEvent->simpleData.unicode,
               KeyEvent->simpleData.keycode,
               KeyEvent->simpleData.is_press,
               KeyEvent->simpleData.is_auto_repeat);
        if (KeyEvent->simpleData.keycode == Qt::Key_F7 // Cancel
                && KeyEvent->simpleData.is_press) {
            qDebug("Cancel is pressed");
            cancel_time.restart();
            return false;
        }
        if (KeyEvent->simpleData.keycode == Qt::Key_F7 // Cancel
                && !KeyEvent->simpleData.is_press) {
            qDebug("Cancel is depressed");
            time_diff = cancel_time.elapsed();
            qDebug() << "time_diff " << time_diff;
            if (states->getState() != WIN_IDLE) {
                if (time_diff >= LONG_CANCEL_MS) {
                    qDebug() << "LONG_CANCEL_MS->goto IDLE " << time_diff;
                    showWinIdle();
                    return false;
                }
                else {
                    ui->guiPages->setCurrentIndex(states->getPrevState());
                    return false;
                }
            }
            else showWinMenu();
        }
        switch (states->getState()) {
        case WIN_IDLE:
            if (!KeyEvent->simpleData.is_press) {
                switch (KeyEvent->simpleData.keycode) {
                case Qt::Key_0:
                case Qt::Key_1:
                case Qt::Key_2:
                case Qt::Key_3:
                case Qt::Key_4:
                case Qt::Key_5:
                case Qt::Key_6:
                case Qt::Key_7:
                case Qt::Key_8:
                case Qt::Key_9:
                        showWinDial(KeyEvent->simpleData.unicode);
                        break;
                case Qt::Key_F1:
                    if (hasMissed) showWinLog(CALLS_MISSED);
                    break;
                case Qt::Key_F2:
                    showWinLog(CALLS_OUTGOING);
                    break;
                case Qt::Key_F3:
                    showWinAbook(0);
                    break;
                case Qt::Key_F7:
                    showWinMenu();
                    break;
                default: break;
                }
            }
            if (KeyEvent->simpleData.keycode == Qt::Key_F8 &&
                    KeyEvent->simpleData.is_press) {
                showWinDial(-1);
            }
            break;
        case WIN_DIALINPUT:
            if (KeyEvent->simpleData.keycode >= Qt::Key_0 &&
                    KeyEvent->simpleData.keycode <= Qt::Key_9) {
                if (!ui->dialLineError->isHidden()) ui->dialLineError->hide();
            }
            break;
        default:
            qDebug("E: State Machine not implemented!");
            break;
        }
    }
    return false;
}

void Widget::on_menuParamList_activated(QModelIndex index)
{
    switch (index.row()) {
    case 0: // network setup
        break;
    case 1: // SIP setup
        break;
    case 2: // Date / Time
        break;
    default: // should not be here
        qWarning("unknown item in menuParamList");
    }
}

#if  0
// a set of callback functions from a liblinphone
void Widget::registerEvent(int e)
{

}

void Widget::ringEvent(int callid)
{

}

void Widget::connectEvent(int callid)
{

}

void Widget::answerEvent(int callid)
{

}

void Widget::disconnectEvent(int callid)
{

}
#endif
