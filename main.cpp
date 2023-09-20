#include <QtWidgets>
#include <cmath>

class AnalogClock : public QWidget {
public:
    AnalogClock(QWidget *parent = nullptr) : QWidget(parent), clockDraggable(false) {
        setMouseTracking(true);
        QTimer *timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(update()));
        timer->start(1000); // Update the clock every second
    }

protected:
    void paintEvent(QPaintEvent *) override {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        // Draw the clock face
        painter.setPen(Qt::black);
        painter.drawEllipse(rect().center(), 100, 100);

        // Draw numbers inside the clock
        painter.setFont(QFont("Arial", 12));
        painter.setPen(Qt::black);
        for (int i = 1; i <= 12; ++i) {
            qreal angle = (i - 3) * 30.0; // Calculate angle for each number
            qreal radian = qDegreesToRadians(angle);
            int x = rect().center().x() + 80 * qCos(radian) - 5; // Adjust position for better alignment
            int y = rect().center().y() + 80 * qSin(radian) + 5; // Adjust position for better alignment
            painter.drawText(x, y, QString::number(i));
        }

        // Get the current time
        QTime time = QTime::currentTime();

        // Draw clock hands
        painter.setPen(Qt::red);
        QPoint secondHandEnd = calculateHandEndPoint(time.second(), 60, 80); // Second hand
        painter.drawLine(rect().center(), secondHandEnd);
        painter.setPen(Qt::black);
        QPoint minuteHandEnd = calculateHandEndPoint(time.minute(), 60, 60); // Minute hand
        painter.drawLine(rect().center(), minuteHandEnd);
        painter.setPen(Qt::blue);
        QPoint hourHandEnd = calculateHandEndPoint((time.hour() % 12) * 5 + time.minute() / 12, 60, 40); // Hour hand
        painter.drawLine(rect().center(), hourHandEnd);
    }

    void mousePressEvent(QMouseEvent *event) override {
        if (event->buttons() & Qt::LeftButton) {
            if (event->pos().manhattanLength() <= 100) {
                // Clicked inside the clock, make it draggable
                clockDraggable = true;
                initialMousePos = event->pos();
                initialClockPos = pos();
            }
        }
    }

    void mouseMoveEvent(QMouseEvent *event) override {
        if (clockDraggable) {
            QPoint delta = event->pos() - initialMousePos;
            move(initialClockPos + delta);
        }
    }

    void mouseReleaseEvent(QMouseEvent *event) override {
        Q_UNUSED(event);
        clockDraggable = false;
    }

private:
    QPoint calculateHandEndPoint(int value, int maxValue, int handLength) {
        qreal angle = 90 - (360.0 * value / maxValue);
        qreal radian = qDegreesToRadians(angle);
        int x = rect().center().x() + handLength * qCos(radian);
        int y = rect().center().y() - handLength * qSin(radian);
        return QPoint(x, y);
    }

    bool clockDraggable;
    QPoint initialMousePos;
    QPoint initialClockPos;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    AnalogClock analogClock;
    analogClock.setWindowTitle("Draggable Analog Clock with Numbers");
    analogClock.resize(250, 250);
    analogClock.show();

    return app.exec();
}


