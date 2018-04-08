#include <QPainter>
#include <QTimer>
#include <QTextStream>
#include <QPixmap>
#include <QPainterPath>
#include <iostream>
#include <boost/algorithm/string.hpp>

#include "Map.hpp"

#define LINE_HEIGHT 30

using namespace std;

Map::Map(pair_queue_ptr& inputQueue, QWidget *parent) :
    QWidget(parent),
    inputQueue_(inputQueue)
{
    cout  << endl << "MAP constructor";
    timerId_ = startTimer(10);
}

void Map::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);

    QPainter painter(this);
    QPainterPath path;

    path.moveTo(lonToPixels(wrX_), latToPixels(wrY_));

    painter.setPen(QPen(QBrush("#0000aa"), 1));
    cout << endl << CoordinateVector_.size();
    //killTimer(timerId);
    for(pair<float,float> coords : CoordinateVector_){
        if((coords.first == 0.0 && coords.second == 0.0))
        {
            path.moveTo(lonToPixels(wrX_), latToPixels(wrY_));
            continue;
        }
        int xDist = std::abs(lonToPixels(coords.first) - path.currentPosition().rx());
        path.cubicTo(
            (lonToPixels(coords.first) + path.currentPosition().rx()) / 2,
            std::min(latToPixels(coords.second), (int)path.currentPosition().ry()) - xDist/5,
            (lonToPixels(coords.first) + path.currentPosition().rx()) / 2,
            std::min(latToPixels(coords.second), (int)path.currentPosition().ry()) - xDist/5,

            lonToPixels(coords.first),
            latToPixels(coords.second)
       );
        painter.drawPath(path);
    }
    cout << endl << "DRAW IT BITCH";
}

void Map::timerEvent(QTimerEvent *e) {

    Q_UNUSED(e);

    if(inputQueue_->pop(coordinates_))
    {
        CoordinateVector_.push_back(coordinates_);
        repaint();
    }
}

int Map::lonToPixels(float lat){

    return 700 + (int)(lat * 686/180);
}

int Map::latToPixels(float lon){
    return 392 - (int)(lon * 356/90);
}
