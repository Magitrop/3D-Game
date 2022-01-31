#ifndef UNTITLED2_NOTALONECOMPONENT_H
#define UNTITLED2_NOTALONECOMPONENT_H

#include "Component.h"

#include <string>

class CompanionComponent;

class NotAloneComponent : public Component {
    COMPONENT(NotAloneComponent)
    CompanionComponent* companion = nullptr;

public:
    std::string value = "im not alone";

    void makeFriend();

    std::string toString();
};

#endif //UNTITLED2_NOTALONECOMPONENT_H
