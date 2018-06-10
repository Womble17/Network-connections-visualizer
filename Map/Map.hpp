#pragma once

#include <QWidget>
#include <memory>
#include <string>
#include <vector>

#include <boost/lockfree/spsc_queue.hpp>

#include <Protocols/Aliases.hpp>


class Map : public QWidget {

  Q_OBJECT

  public:
    Map(drawings_queue_ptr& inputQueue, QWidget *parent = 0);

  protected:
    void paintEvent(QPaintEvent *event);
    void timerEvent(QTimerEvent *event);

  private:
    float wrX_ = 17.038538;
    float wrY_ = 51.107883;
    int timerId_;
    drawings_queue_ptr inputQueue_;
    std::shared_ptr<DrawingPackage> coordinates_;

    int latToPixels(float lat);
    int lonToPixels(float lon);

};
