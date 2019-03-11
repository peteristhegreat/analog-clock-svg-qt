#include <QtWidgets>
#include "analogclock.h"
#include <QSettings>
#include <QMenu>
#include <QApplication>
#include <QDebug>

AnalogClock::AnalogClock(QWidget *parent)
    : QWidget(parent)
{
    m_timer = new QTimer(this);
    this->setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);

    // TODO: Maintain aspect ratio to be a square
    setStyleSheet("background-color: rgba(0,0,0,0)");

    connect(m_timer, SIGNAL(timeout()), this, SLOT(update()));
    m_timer->start(1000);

    setWindowTitle(tr("Analog Clock Widget"));
    resize(200, 200); // will get overridden by readSettings

    readSettings();
    m_tempHide = false;
}

void AnalogClock::resizeEvent(QResizeEvent *event)
{
    m_clock_face->resize(event->size());
    m_second_hand->resize(event->size());
    m_minute_hand->resize(event->size());
    m_hour_hand->resize(event->size());
    event->accept();
}

void AnalogClock::closeEvent(QCloseEvent *event)
{
    qDebug() << Q_FUNC_INFO;
    QObject::disconnect(m_timer);
    writeSettings();
    qApp->quit();
}

void AnalogClock::hideEvent(QHideEvent *event)
{
    if(!m_tempHide){
        event->ignore();
        this->close();
    }
    else{
        QWidget::hideEvent(event);
        m_tempHide = false;
    }
}

void AnalogClock::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
}

void AnalogClock::contextMenuEvent(QContextMenuEvent *e)
{
    QMenu menu;
    menu.popup(e->pos());
}

void AnalogClock::mouseMoveEvent(QMouseEvent *e)
{
    QPoint diff = e->pos() - m_clickOffset;
    this->move(this->pos() + diff);
}

void AnalogClock::mouseDoubleClickEvent(QMouseEvent *e)
{
    m_tempHide = true;
    bool has_frame = !(this->windowFlags() & Qt::FramelessWindowHint);
    if(has_frame){
        this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
        this->setAttribute(Qt::WA_TranslucentBackground);
        this->show();
    }
    else{
        this->setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint);
        this->setAttribute(Qt::WA_TranslucentBackground, false);
        this->show();
    }
}

void AnalogClock::mousePressEvent(QMouseEvent * e)
{
    if(e->button() == Qt::LeftButton){
        this->setCursor(Qt::SizeAllCursor);
        m_clickOffset = e->pos();
    }
    else
    {
        QWidget::mousePressEvent(e);
    }
}

void AnalogClock::mouseReleaseEvent(QMouseEvent * e)
{
    if(e->button() == Qt::LeftButton){
        this->unsetCursor();
    }
    else
    {
        QWidget::mouseReleaseEvent(e);
    }
}

void AnalogClock::readSettings()
{
    QSettings s;
    QString svg_path = s.value("svg_path", ":/img/").toString();
    s.setValue("svg_path", svg_path);

    QString clock_face_file = s.value("clock_face", "face.svg").toString();
    s.setValue("clock_face", clock_face_file);

    QString second_hand_file = s.value("second_hand", "second_hand.svg").toString();
    s.setValue("second_hand", second_hand_file);

    QString minute_hand_file = s.value("minute_hand", "minute_hand.svg").toString();
    s.setValue("minute_hand", minute_hand_file);

    QString hour_hand_file = s.value("hour_hand", "hour_hand.svg").toString();
    s.setValue("hour_hand", hour_hand_file);

    QString preview_file = s.value("preview", "preview.svg").toString();
    s.setValue("preview", preview_file);

    m_show_second_hand = s.value("show_second_hand", true).toBool();
    s.setValue("show_second_hand", m_show_second_hand);

    m_clock_face = new QSvgWidget(svg_path + clock_face_file, 0);
    m_second_hand = new QSvgWidget(svg_path + second_hand_file, 0);
    m_minute_hand = new QSvgWidget(svg_path + minute_hand_file, 0);
    m_hour_hand = new QSvgWidget(svg_path + hour_hand_file, 0);

    this->restoreGeometry(s.value("window_geometry").toByteArray());
}

void AnalogClock::writeSettings()
{
    QSettings s;
    s.setValue("window_geometry", this->saveGeometry());
}


void AnalogClock::paintEvent(QPaintEvent *)
{
    int side = qMin(width(), height());
    QTime time = QTime::currentTime();
    QPainter painter(this);

    painter.setBrush(Qt::transparent);
    painter.fillRect(this->rect(), QBrush(Qt::transparent));
    painter.setRenderHint(QPainter::Antialiasing);
    m_clock_face->render(&painter, QPoint(), QRegion(), QWidget::DrawChildren);
    painter.save();
    painter.translate(width() / 2, height() / 2);
    painter.rotate(30.0 * ((time.hour() + time.minute() / 60.0)));
    painter.translate(-width() / 2, -height() / 2);
    m_hour_hand->render(&painter, QPoint(), QRegion(), QWidget::DrawChildren);
    painter.restore();
    painter.save();
    painter.translate(width() / 2, height() / 2);
    painter.rotate(6.0 * (time.minute() + time.second() / 60.0));
    painter.translate(-width() / 2, -height() / 2);
    m_minute_hand->render(&painter, QPoint(), QRegion(), QWidget::DrawChildren);
    painter.restore();

    if(m_show_second_hand){
        painter.save();
        painter.translate(width() / 2, height() / 2);
        painter.rotate(6.0 *time.second());
        painter.translate(-width() / 2, -height() / 2);
        m_second_hand->render(&painter, QPoint(), QRegion(), QWidget::DrawChildren);
        painter.restore();
    }
}

