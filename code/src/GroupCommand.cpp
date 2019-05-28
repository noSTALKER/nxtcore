#include "../include/GroupCommand.h"

namespace nxt::core {

GroupCommand::GroupCommand(const std::string& name)
    : Command(name)
    , has_executed_once_(true) {}

bool
GroupCommand::addCommand(Command* command) {
    if (!has_executed_once_ && command->canUndo()) {
        // TODO : Can check if the command is already added but not needed for now
        child_commands_.emplaceBack(command);

        return true;
    }
    return false;
}

bool
GroupCommand::doExecute() noexcept {
    bool result = true;
    int index = 0;
    while (index < static_cast<int>(child_commands_.size())) {
        result = result && child_commands_[index]->execute();
        // break if any command fails to execute
        if (!result) {
            // go to last successful executed command so that we can start undoing the changes
            --index;
            break;
        }
        ++index;
    }

    // if there was an error while executing the commands, undo commands till the error was found
    if (!result) {
        while (index >= 0) {
            child_commands_[index]->undo();
            --index;
        }
    }

    return result;
}

bool
GroupCommand::doUndo() noexcept {
    bool result = true;
    int index = static_cast<int>(child_commands_.size()) - 1;
    while (index >= 0) {
        result = result && child_commands_[index]->undo();
        // break if any command fails to undo
        if (!result) {
            // go to last successful undoed command so that we can start redo the changes
            ++index;
            break;
        }
        ++index;
    }

    // if there was an error while executing the commands, undo commands till the error was found
    if (!result) {
        while (index < static_cast<int>(child_commands_.size()) - 1) {
            child_commands_[index]->execute();
            --index;
        }
    }

    return result;
}

bool
GroupCommand::supportsMerge(Command* command) const noexcept {
    GroupCommand* group_command = dynamic_cast<GroupCommand*>(command);
    return group_command != nullptr;
}

bool
GroupCommand::mergeWith(Command* command) noexcept {
    // group command can only merge with another group command
    GroupCommand* group_command = dynamic_cast<GroupCommand*>(command);
    if (group_command != nullptr) {
        // while merging, just add add the commands to end of the current command list
        child_commands_.reserve(child_commands_.size() + group_command->child_commands_.size());
        for (auto& command : group_command->child_commands_) {
            child_commands_.emplaceBack(std::move(command));
        }
        group_command->child_commands_.clear();
    }
    return false;
}

bool
GroupCommand::supportsUndo() const noexcept {
    return true;
}
}  // namespace nxt::core
