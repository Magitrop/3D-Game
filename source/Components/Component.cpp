#include "Component.h"
#include "../EventSystem.h"

Component::~Component()
{
	OnDestroy();
	DETACH_UPDATE
	DETACH_MOUSE_WHEEL
	DETACH_MOUSE_MOVE
	DETACH_MOUSE_BUTTON
	DETACH_KEYBOARD
	DETACH_WINDOW_RESIZE
}