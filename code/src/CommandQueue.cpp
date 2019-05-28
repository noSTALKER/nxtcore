#include "../include/CommandQueue.h"
#include "../include/Command.h"

namespace nxt::core {

CommandQueue::CommandQueue(const std::string& name)
    : name_(name)
    , current_index_(-1) {}

const std::string&
CommandQueue::getName() const noexcept {
    return name_;
}

bool
CommandQueue::canUndo() const noexcept {
    return current_index_ >= 0 && commands_.size() > 0 && commands_[current_index_]->canUndo();
}

bool
CommandQueue::canRedo() const noexcept {
    return current_index_ < (static_cast<int>(commands_.size()) - 1);
}

void
CommandQueue::undo() {
    if (canUndo()) {
        if (commands_[current_index_]->undo())
            --current_index_;
    }
}

void
CommandQueue::redo() {
    if (canRedo()) {
        if (!commands_[++current_index_]->execute())
            --current_index_;
    }
}

void
CommandQueue::addCommand(Command* command) {
    if (command->execute() && command->canUndo()) {
        // clear the commands above the current index
        clearTopCommands();

        // merge with the top of the command if possible
        if (current_index_ >= 0 && commands_[current_index_]->canMerge(command)) {
            commands_[current_index_]->merge(command);
            return;
        }

        commands_.emplaceBack(command);
        ++current_index_;
    }
}

void
CommandQueue::clearTopCommands() {
    if (current_index_ < commands_.size() - 1) {
        commands_.resize(current_index_ + 1);
    }
}
}  // namespace nxt::core
