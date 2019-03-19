#include <QtWidgets>
#include "analogclock.h"
#include <QSettings>
#include <QMenu>
#include <QApplication>
#include <QDebug>
#include <QtMath>
#include <QEvent>
#include <QScreen>
#include <QGuiApplication>
#include <QDesktopWidget>

AnalogClock::AnalogClock(QWidget *parent)
    : QWidget(parent)
{
    m_timer = new QTimer(this);
    this->setFrame(false);
    m_isDragging = false;
    this->setContextMenuPolicy(Qt::CustomContextMenu);

    m_opacityEffect = new QGraphicsOpacityEffect(this);
    this->setGraphicsEffect(m_opacityEffect);


    QObject::connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(showContextMenu(const QPoint &)));

    connect(m_timer, SIGNAL(timeout()), this, SLOT(update()));
    m_timer->start(1000);

    setWindowTitle(tr("Analog Clock Widget"));
    resize(200, 200); // will get overridden by readSettings

    readSettings();
    m_tempHide = false;

    QObject::connect(qApp, SIGNAL(screenRemoved(QScreen *)), this, SLOT(handleScreenDisconnect(QScreen *)));
    QObject::connect(this, SIGNAL(showSecondHandChanged(bool)), this, SLOT(update()));
    ensureOnScreen();
}

void AnalogClock::ensureOnScreen(){
    static int screenId = -2;
    // Get current screen id for our widget
    int newScreenId = qMax(qApp->desktop()->screenNumber(this), 0);
    if(qApp->screens().length() > 0){

        QScreen * screen = qApp->screens().at(newScreenId);
        if(newScreenId != screenId){
            screenId = newScreenId;
            QObject::connect(screen, SIGNAL(geometryChanged(QRect)), this, SLOT(onScreenResize(QRect)));
        }

        if(!screen->availableGeometry().contains(this->geometry(), true)){
            this->move(screen->geometry().left() + m_screenPercent.x()*screen->geometry().width(),
                       screen->geometry().top() + m_screenPercent.y()*screen->geometry().height());
        }
    }
}

void AnalogClock::handleScreenDisconnect(QScreen * s)
{
    QObject::disconnect(s, SIGNAL(geometryChanged(QRect)), this, SLOT(onScreenResize(QRect)));
    if(qApp->screens().length() > 0)
    {
        QScreen * screen = qApp->screens().at(qMax(qApp->desktop()->screenNumber(this),0));
        this->move(screen->geometry().left() + m_screenPercent.x()*screen->geometry().width(),
                   screen->geometry().top() + m_screenPercent.y()*screen->geometry().height());
    }
}

void AnalogClock::onScreenResize(QRect)
{
    this->ensureOnScreen();
}

void AnalogClock::resizeEvent(QResizeEvent *event)
{
    int side = qMin(event->size().width(), event->size().height());
    m_clock_face->resize(side, side);
    m_second_hand->resize(side, side);
    m_minute_hand->resize(side, side);
    m_hour_hand->resize(side, side);
    event->accept();
}

void AnalogClock::closeEvent(QCloseEvent *event)
{
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

void AnalogClock::showContextMenu(const QPoint &pos)
{
   QMenu contextMenu(this->windowTitle(), this);

//   QAction toggleFrameAction("Toggle Frame", this);
//   connect(&toggleFrameAction, SIGNAL(triggered()), this, SLOT(toggleFrame());
//   contextMenu.addAction(&toggleFrameAction);
   contextMenu.addAction("Toggle Frame", this, SLOT(toggleFrame()));

   QAction toggleSecondHand("Second Hand", this);
   toggleSecondHand.setCheckable(true);
   toggleSecondHand.setChecked(m_showSecondHand);
   QObject::connect(&toggleSecondHand, SIGNAL(triggered(bool)), this, SLOT(setShowSecondHand(bool)));

   contextMenu.addAction(&toggleSecondHand);

   QMenu opacityMenu("Opacity", &contextMenu);
   for(int i = 100; i > 0; i-=10){
       opacityMenu.addAction(QString::number(i) + "%", this, SLOT(setOpacityFromMenu()));
   }

   contextMenu.addMenu(&opacityMenu);
   contextMenu.addAction("About", this, SLOT(showAboutDialogue()));
   contextMenu.addAction("Close", this, SLOT(close()));

   contextMenu.exec(mapToGlobal(pos));
}

void AnalogClock::showAboutDialogue()
{
    QMessageBox::about(this, "About " + this->windowTitle(),
                       "Designed by Peter and Harry with Qt.<br/><br/>"
                       "<a href=\"https://github.com/peteristhegreat/analog-clock-svg-qt\">"
                       "github.com/peteristhegreat/analog-clock-svg-qt</a>");
}

void AnalogClock::setOpacityFromMenu()
{
    qDebug() << QObject::sender();
    QAction * a = qobject_cast<QAction*>(QObject::sender());
    if(a){
//        qDebug() << a->text();
        QString percentString = a->text().split("%").first();
        m_opacityEffect->setOpacity((qreal)percentString.toInt()/100.);
    }
}

void AnalogClock::contextMenuEvent(QContextMenuEvent *e)
{
    QWidget::contextMenuEvent(e);
}

void AnalogClock::mouseMoveEvent(QMouseEvent *e)
{
    if(m_isDragging){
        QPoint diff = e->pos() - m_clickOffset;
        this->move(this->pos() + diff);
    }
}

void AnalogClock::mouseDoubleClickEvent(QMouseEvent *e)
{
    this->toggleFrame();
}

void AnalogClock::toggleFrame(){
    m_tempHide = true;
    bool has_frame = !(this->windowFlags() & Qt::FramelessWindowHint);
    QRect geo = this->geometry();
    QRect fgeo = this->frameGeometry();
    this->setFrame(!has_frame);
    if(!has_frame){
        this->setGeometry(fgeo);
    } else {
        this->setGeometry(geo);
    }
    this->show();
}

void AnalogClock::setFrame(bool add_frame){
    if(add_frame){
        this->setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint);
    //        this->setAttribute(Qt::WA_TranslucentBackground, false);
    }
    else{
        this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
        this->setAttribute(Qt::WA_TranslucentBackground);
    }
}

void AnalogClock::mousePressEvent(QMouseEvent * e)
{
    if(e->button() == Qt::LeftButton){
        m_isDragging = true;
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
        m_isDragging = false;
    }
    else
    {
        QWidget::mouseReleaseEvent(e);
    }
}

void AnalogClock::moveEvent(QMoveEvent *event)
{
    QScreen * screen = qApp->screens().at(qApp->desktop()->screenNumber(this));
    m_screenPercent = QPointF((qreal)(event->pos().x() - screen->geometry().left())/screen->geometry().width(),
                              (qreal)(event->pos().y() - screen->geometry().top())/screen->geometry().height());
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

    m_showSecondHand = s.value("show_second_hand", true).toBool();
    s.setValue("show_second_hand", m_showSecondHand);

    m_opacityEffect->setOpacity(s.value("opacity", 1.0).toReal());
    s.setValue("opacity", m_opacityEffect->opacity());

    m_screenPercent = s.value("screen_percent", QPointF(0.5, 0.5)).toPointF();

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
    s.setValue("screen_percent", m_screenPercent);
    s.setValue("show_second_hand", m_showSecondHand);
    s.setValue("opacity", m_opacityEffect->opacity());
}

qreal degToRad(qreal deg){
    return M_PI * deg / 180;
}

void AnalogClock::paintEvent(QPaintEvent *)
{
    int side = qMin(width(), height());
    QTime time = QTime::currentTime();
    QPainter painter(this);

    // Draw the clock face
    painter.setBrush(Qt::transparent);
    painter.fillRect(this->rect(), QBrush(Qt::transparent));
    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate((width() - side)/2, (height() - side)/2);
    m_clock_face->render(&painter, QPoint(), QRegion(), QWidget::DrawChildren);
    painter.save();

    // Draw the hour hand
    painter.translate(m_hour_hand->width() / 2, m_hour_hand->height() / 2);
    painter.rotate(30.0 * ((time.hour() + time.minute() / 60.0)));

    painter.translate(-m_hour_hand->width() / 2, -m_hour_hand->height() / 2);
    m_hour_hand->render(&painter, QPoint(), QRegion(), QWidget::DrawChildren);
    painter.restore();
    painter.save();

    // Draw the minute hand
    painter.translate(m_minute_hand->width() / 2, m_minute_hand->height() / 2);
    painter.rotate(6.0 * (time.minute() + time.second() / 60.0));
    painter.translate(-m_minute_hand->width() / 2, -m_minute_hand->height() / 2);
    m_minute_hand->render(&painter, QPoint(), QRegion(), QWidget::DrawChildren);
    painter.restore();

//    qreal aspect_ratio = (qreal)this->height()/this->width()*qAbs(qCos(degToRad(90 - 6*time.second())));
    if(m_showSecondHand){
        // Draw the second hand
        painter.save();
        painter.translate(m_second_hand->width() / 2, m_second_hand->height() / 2);
        painter.rotate(6.0 *time.second());
//        painter.scale(1, 1 + 2*aspect_ratio);
//        painter.scale(qCos(degToRad(6.0*time.second())), qSin(degToRad(6.0*time.second())));
        painter.translate(-m_second_hand->width() / 2, -m_second_hand->height() / 2);
        m_second_hand->render(&painter, QPoint(), QRegion(), QWidget::DrawChildren);
        painter.restore();
    }
}

