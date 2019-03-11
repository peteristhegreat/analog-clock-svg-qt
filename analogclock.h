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

class AnalogClock : public QWidget
{
    Q_OBJECT

public:
    AnalogClock(QWidget *parent = 0);

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
//    int heightForWidth ( int w ) const;
public slots:
    void readSettings();
    void writeSettings();
private:
    QTimer * m_timer;
    QSvgWidget * m_second_hand;
    QSvgWidget * m_minute_hand;
    QSvgWidget * m_hour_hand;
    QSvgWidget * m_clock_face;
    QPoint m_clickOffset;
    bool m_show_second_hand;
};


#endif
