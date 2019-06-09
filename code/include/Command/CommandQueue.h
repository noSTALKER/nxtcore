#pragma once

#include <memory>
#include <string>

#include "../Container/Vector.h"

namespace nxt::core {

class Command;

/**
 * @brief
 *
 */
class CommandQueue {
public:
    /**
     * @brief Construct a new Command Queue object
     *
     * @param name
     */
    CommandQueue(const std::string& name);

    CommandQueue(const CommandQueue&) = delete;
    CommandQueue& operator=(const CommandQueue&) = delete;

    /**
     * @brief Get the Name object
     *
     * @return const std::string&
     */
    const std::string& getName() const noexcept;

    /**
     * @brief
     *
     * @return true
     * @return false
     */
    bool canUndo() const noexcept;

    /**
     * @brief
     *
     * @return true
     * @return false
     */
    bool canRedo() const noexcept;

    /**
     * @brief
     *
     */
    void undo();

    /**
     * @brief
     *
     */
    void redo();

    /**
     * @brief
     *
     * @param command
     */
    void addCommand(Command* command);

private:
    /**
     * @brief
     *
     */
    void clearTopCommands();

    std::string name_;
    int current_index_;
    Vector<std::unique_ptr<Command>> commands_;
};
}  // namespace nxt::core
