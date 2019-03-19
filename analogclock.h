#ifndef ANALOGCLOCK_H
#define ANALOGCLOCK_H

#include <QWidget>
#include <QSvgWidget>
#include <QResizeEvent>
#include <QCloseEvent>
#include <QContextMenuEvent>
#include <QMouseEvent>
#include <QTimer>
#include <QHideEvent>
#include <QShowEvent>
#include <QPointF>
#include <QMoveEvent>

class AnalogClock : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(bool showSecondHand READ showSecondHand WRITE setShowSecondHand NOTIFY showSecondHandChanged)

public:
    AnalogClock(QWidget *parent = 0);

    bool showSecondHand() const
    {
        return m_showSecondHand;
    }

protected:
    void paintEvent(QPaintEvent * event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent * event);
    void closeEvent(QCloseEvent * event);
    void hideEvent(QHideEvent * event);
    void showEvent(QShowEvent *event);
    void contextMenuEvent(QContextMenuEvent * e);
    void mouseMoveEvent(QMouseEvent * e);
    void mouseDoubleClickEvent(QMouseEvent * e);
    void mousePressEvent(QMouseEvent * e);
    void mouseReleaseEvent(QMouseEvent * e);
    void moveEvent(QMoveEvent *event);
//    int heightForWidth ( int w ) const;
public slots:
    void readSettings();
    void writeSettings();
    void ensureOnScreen();
    void handleScreenDisconnect(QScreen *);
    void onScreenResize(QRect);
    void toggleFrame();
    void setFrame(bool add_frame);
    void showContextMenu(const QPoint &pos);
    void showAboutDialogue();
    void setShowSecondHand(bool showSecondHand)
    {
        if (m_showSecondHand == showSecondHand)
            return;

        m_showSecondHand = showSecondHand;
        emit showSecondHandChanged(showSecondHand);
    }

signals:
    void showSecondHandChanged(bool showSecondHand);

private:
    QTimer * m_timer;
    QSvgWidget * m_second_hand;
    QSvgWidget * m_minute_hand;
    QSvgWidget * m_hour_hand;
    QSvgWidget * m_clock_face;
    QPoint m_clickOffset;
    bool m_tempHide;
    QPointF m_screenPercent;
    bool m_showSecondHand;
};


#endif
