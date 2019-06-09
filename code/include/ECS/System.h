#pragma once

#include <string>

namespace nxt::core {
/**
 * @brief
 *
 * @tparam T
 */
template<class T>
struct SystemLookup {};

/**
 * @brief Abstract base class for all Systems. A System is higher level abstraction over multiple components which would
 *        make it decisions based on the data changes or events
 *
 */
class System {
public:
    System() = default;
    /**
     * @brief Get the name of the System
     *
     * @return Name of the System
     */
    virtual const std::string& getName() const noexcept = 0;

    /**
     * @brief Function called when the System is registered in SystemManager by the application
     *
     */
    bool onRegister() noexcept {
        return doRegister();
    }

    /**
     * @brief Function called by System Manager when new Project is being loaded and All Component data is populated
     *
     */
    bool onProjectLoaded() noexcept {
        return doProjectLoad();
    }

    /**
     * @brief Function called by System Manager when a Project is being unloaded
     *
     */
    bool onProjectUnloaded() noexcept {
        return doProjectUnload();
    }
    virtual ~System() = default;

protected:
    /**
     * @brief
     *
     */
    virtual bool doRegister() noexcept = 0;

    /**
     * @brief
     *
     */
    virtual bool doProjectLoad() noexcept = 0;

    /**
     * @brief
     *
     */
    virtual bool doProjectUnload() noexcept = 0;
};
}  // namespace nxt::core

/**
 * @brief Use this macro to register a new system which needs exporting
 *
 */
#define NXTCORE_REGISTER_SYSTEM(Export, System) \
    namespace nxt::core {                       \
    template<>                                  \
    struct Export SystemLookup<System> {        \
        static size_t system_id;                \
    };                                          \
    }

/**
 * @brief Use this macro to register a new system which doesn't needs exporting
 *
 */
#define NXTCORE_REGISTER_SYSTEM_NO_EXPORT(System) \
    namespace nxt::core {                         \
    template<>                                    \
    struct SystemLookup<System> {                 \
        static size_t system_id;                  \
    };                                            \
    }

/**
 * @brief Use this macro to define a already registered system
 *
 */
#define NXTCORE_DEFINE_SYSTEM(System)            \
    namespace nxt::core {                        \
    size_t SystemLookup<System>::system_id = -1; \
    }
