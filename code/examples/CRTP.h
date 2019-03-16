#include <iostream>

// using CRTP for compile time polymorphism
template<class Command>
class CommandBase {
public:
    void undo() {
        static_cast<Command*>(this)->doUndo();
    }

    void redo() {
        static_cast<Command*>(this)->doRedo();
    }
};

struct Person {
    std::string name;
    uint32_t age;
};

class SetNameCommand : public CommandBase<SetNameCommand> {
public:
    SetNameCommand(Person* person, const std::string& name) 
        : person_(person)
        , new_name_(name) {
    }
    void doRedo() {
        old_name_ = person_->name;
        person_->name = new_name_;
    }

    void doUndo() {
        person_->name = old_name_;
    }

private:
    Person* person_;
    std::string new_name_;
    std::string old_name_;
};

class SetAgeCommand : public CommandBase<SetAgeCommand> {
public:
    SetAgeCommand(Person* person, uint32_t age)
        : person_(person)
        , new_age_(age) {}

    void doRedo() {
        old_age_ = person_->age;
        person_->age = new_age_;
    }

    void doUndo() {
        person_->age = old_age_;
    }

private:
    Person* person_;
    uint32_t new_age_;
    uint32_t old_age_;
};
