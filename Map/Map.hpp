#pragma once

#include <QWidget>
#include <memory>
#include <string>
#include <vector>

#include <boost/lockfree/spsc_queue.hpp>

using pair_queue_type = boost::lockfree::spsc_queue<std::pair<float,float>>;
using pair_queue_ptr = std::shared_ptr<pair_queue_type>;


class Map : public QWidget {

  Q_OBJECT

  public:
    Map(pair_queue_ptr& inputQueue, QWidget *parent = 0);

  protected:
    void paintEvent(QPaintEvent *event);
    void timerEvent(QTimerEvent *event);

  private:
    int x_;
    int y_;
    int timerId_;
    std::pair<float,float> coordinates_;
    std::vector<std::pair<float,float>> CoordinateVector_;
    pair_queue_ptr inputQueue_;

    int latToPixels(float lat);
    int lonToPixels(float lon);

};
