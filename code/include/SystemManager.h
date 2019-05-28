#pragma once

#include <memory>

#include "Vector.h"
#include "System.h"

namespace nxt::core {

class SystemManager {
public:
    static SystemManager* get() {
        static SystemManager manager;
        return &manager;
    }

    template<class T>
    T* getSystem() const noexcept {
        static_assert(std::is_base_of<core::System, T>::value, "Can only get derived class of ess::core::System class");
        assert(ess::core::SystemLookup<T>::system_id != -1 &&
               "System is not registered. Use registerSystem() first before using getSystem()");
        return static_cast<T*>(systems_[ess::core::SystemLookup<T>::system_id].get());
    }

    template<class T>
    void registerSystem() {
        static_assert(std::is_base_of<core::System, T>::value,
                      "Can only register derived class of ess::core::System class");
        T* system = new T;
        if (system->OnRegister()) {
            systems_.EmplaceBack(system);
            system_name_map_.Emplace(system->GetName(), system);

            ess::core::SystemLookup<T>::system_id = systems_.Size() - 1;
        }
    }

    template<class T>
    void isSystemRegistered() const noexcept {
        static_assert(std::is_base_of<core::System, T>::value,
                      "Can only query derived class of ess::core::System class");
        return ess::core::SystemLookup<T>::system_id != -1;
    }

    SystemManager(const SystemManager&) = delete;
    SystemManager& operator=(const SystemManager&) = delete;

    System* getSystem(const std::string& name) const noexcept {
        auto iter = system_name_map_.Find(name);
        if (iter != system_name_map_.End()) {
            return iter->second;
        }
        return nullptr;
    }

private:
    SystemManager() = default;

    Vector<std::unique_ptr<System>> systems_;
    std::unordered_map<std::string, System*> system_name_map_;
};
}  // namespace core
}  // namespace ess
