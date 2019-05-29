#pragma once

#include <memory>

#include "Vector.h"
#include "Command.h"

namespace nxt::core {
/**
 * @brief
 *
 */
class GroupCommand : public Command {
public:
    /**
     * @brief Construct a new Group Command with given name
     *
     * @param name Name of the Group Command
     */
    GroupCommand(const std::string& name);

    /**
     * @brief Add a command to the group command
     *
     * @param command Child Command to be added
     * @return True if the child command was added successfully, or else false. Adding a command
     *         will fail if the group command has executed once or a duplicate exists
     */
    bool addCommand(Command* command);

    GroupCommand(const GroupCommand& rhs) = delete;
    GroupCommand& operator=(const GroupCommand& rhs) = delete;

private:
    bool doExecute() noexcept override;
    bool doUndo() noexcept override;
    bool supportsMerge(Command* command) const noexcept override;
    bool mergeWith(Command* command) noexcept override;
    bool supportsUndo() const noexcept override;

    Vector<std::unique_ptr<Command>> child_commands_;
    bool has_executed_once_;
};
}  // namespace core
