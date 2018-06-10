#ifndef ALIASES_HPP
#define ALIASES_HPP

#include <memory>
#include <string>

#include <boost/lockfree/spsc_queue.hpp>

#include <Protocols/DrawingPackage.hpp>

using string_queue_type = boost::lockfree::spsc_queue<std::string>;
using string_queue_ptr = std::shared_ptr<string_queue_type>;

using drawings_queue_type = boost::lockfree::spsc_queue<std::shared_ptr<DrawingPackage>>;
using drawings_queue_ptr = std::shared_ptr<drawings_queue_type>;

#endif // ALIASES_HPP
