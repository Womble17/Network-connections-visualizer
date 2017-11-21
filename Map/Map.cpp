#include <QPainter>
#include <QTimer>
#include <QTextStream>
#include <QPixmap>
#include <boost/algorithm/string.hpp>

#include "Map.hpp"

Map::Map(pair_queue_ptr& inputQueue, QWidget *parent) :
    QWidget(parent),
    inputQueue_(inputQueue)
{

  x_ = 1;
  timerId_ = startTimer(100);
}

void Map::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);

    QPainter painter(this);

    painter.setPen(QPen(QBrush("#570000"), 4));

    //killTimer(timerId);
    for(std::pair<float,float> coords : CoordinateVector_){
        //painter.drawLine(766,187, latToPixels(coords.second), lonToPixels(coords.first));
        //painter.drawPoint(latToPixels(coords.second), lonToPixels(coords.first));
        painter.drawEllipse(latToPixels(coords.second), lonToPixels(coords.first), 4, 4);

    }
}

void Map::timerEvent(QTimerEvent *e) {

    Q_UNUSED(e);

    if(inputQueue_->pop(coordinates_))
    {
        CoordinateVector_.push_back(coordinates_);
        x_ += 1;
        repaint();
    }
}

int Map::latToPixels(float lat){

    return 700 + (int)(lat * 686/180);
}

int Map::lonToPixels(float lon){
    return 392 - (int)(lon * 356/90);
}
