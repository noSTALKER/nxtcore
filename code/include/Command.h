#pragma once

#include <string>

namespace nxt::core {
/**
 * @brief Base class for all commands
 *
 */
class Command {
public:
    /**
     * @brief Construct a new Command instance
     *
     * @param name Name of the command instance
     */
    Command(const std::string& name)
        : name_(name) {}

    /**
     * @brief Get the Name of the command instance
     *
     * @return Name of the command
     */
    const std::string& getName() const noexcept {
        return name_;
    }

    /**
     * @brief
     *
     * @return true
     * @return false
     */
    bool execute() noexcept {
        return doExecute();
    }

    /**
     * @brief
     *
     * @return true
     * @return false
     */
    bool undo() noexcept {
        return doUndo();
    }

    /**
     * @brief
     *
     * @return true
     * @return false
     */
    bool canUndo() const noexcept {
        return supportsUndo();
    }

    bool canMerge(Command* command) const noexcept {
        return supportsMerge(command);
    }

    bool merge(Command* command) noexcept {
        return mergeWith(command);
    }

    /**
     * @brief Destroy the Command object
     *
     */
    virtual ~Command() = default;

protected:
    virtual bool doExecute() noexcept = 0;
    virtual bool doUndo() noexcept = 0;
    virtual bool supportsUndo() const noexcept = 0;

    /**
     * @brief
     *
     * @param command
     * @return true
     * @return false
     */
    virtual bool supportsMerge(Command* command) const noexcept {
        return false;
    }

    /**
     * @brief
     *
     * @param command
     * @return true
     * @return false
     */
    virtual bool mergeWith(Command* command) noexcept {
        return false;
    }

    std::string name_;
};
}  // namespace next::core
