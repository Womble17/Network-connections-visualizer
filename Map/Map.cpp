#include <iostream>

#include <QPainter>
#include <QTimer>
#include <QTextStream>
#include <QPixmap>
#include <QPainterPath>

#include <boost/algorithm/string.hpp>

#include <Protocols/DrawingPackage.hpp>

#include "Map.hpp"

#define LINE_HEIGHT 30

using namespace std;

Map::Map(drawings_queue_ptr& inputQueue, QWidget *parent) :
    QWidget(parent),
    inputQueue_(inputQueue)
{
    //cout  << endl << "MAP constructor";
    timerId_ = startTimer(10);
}

void Map::paintEvent(QPaintEvent *e)
{

    Q_UNUSED(e);

    QPainter painter(this);


    //killTimer(timerId);
    //cout << "paint data: " << to_string(coordinates_->drawDataPackage.size()) << endl;
    for(const auto& data : coordinates_->drawDataPackage)
    {
        //cout << "TO DRAW: " << to_string(data.x1) << " " << to_string(data.y1) << " " << to_string(data.x2) << " " << to_string(data.y2) << endl;
        if((data.x1 == 0.0 && data.y1 == 0.0) || (data.x2 == 0.0 && data.y2 == 0.0))
        {
            continue;
        }

        // set brush color
        painter.setPen(QPen(QBrush(QColor(data.r, data.g, data.b)), 1));

        QPainterPath path;
        // set brush to x1 y1
        path.moveTo(lonToPixels(data.x1), latToPixels(data.y1));

        // draw line to x2 y2
        int xDist = std::abs(lonToPixels(data.x1) - lonToPixels(data.x2));
        path.cubicTo(
            // control point 1
            (lonToPixels(data.x2) + path.currentPosition().rx()) / 2,
            std::min(latToPixels(data.y2), (int)path.currentPosition().ry()) - xDist/5,
            // control point 2
            (lonToPixels(data.x2) + path.currentPosition().rx()) / 2,
            std::min(latToPixels(data.y2), (int)path.currentPosition().ry()) - xDist/5,
            // end point
            lonToPixels(data.x2),
            latToPixels(data.y2)
       );
       painter.drawPath(path);
    }
}

void Map::timerEvent(QTimerEvent *e) {

    Q_UNUSED(e);

    if(inputQueue_->pop(coordinates_))
    {
        for(auto& data : coordinates_->drawDataPackage)
        {
            //cout << to_string(data.x1) << " " << to_string(data.x2) << endl;
        }

        repaint();
    }
}

int Map::lonToPixels(float lat){

    return 700 + (int)(lat * 686/180);
}

int Map::latToPixels(float lon){
    return 392 - (int)(lon * 356/90);
}
