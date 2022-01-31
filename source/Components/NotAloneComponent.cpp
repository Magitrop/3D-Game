#include "NotAloneComponent.h"

#include "GameObject.h"
#include "CompanionComponent.h"

void NotAloneComponent::makeFriend() {
    companion = gameObject->addComponent<CompanionComponent>();
    companion->notAlone = this;
}

std::string NotAloneComponent::toString() {
    return "im " + value + ", my companion is " + companion->value;
}
