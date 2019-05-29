#include "../include/Event.h"

namespace nxt::core {
std::atomic<uint32_t> BaseEvent::next_id_{0u};
}
