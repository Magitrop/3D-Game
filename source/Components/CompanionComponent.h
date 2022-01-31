#ifndef UNTITLED2_COMPANIONCOMPONENT_H
#define UNTITLED2_COMPANIONCOMPONENT_H

#include "Component.h"

#include <string>

class NotAloneComponent;
class CompanionComponent : public Component {
    COMPONENT(CompanionComponent)

    friend class NotAloneComponent;

    NotAloneComponent* notAlone = nullptr;

public:
    std::string value = "im companion";

    std::string toString();
};

#endif //UNTITLED2_COMPANIONCOMPONENT_H
