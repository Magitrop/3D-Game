#include "CameraComponent.h"

void CameraComponent::OnCreate()
{
	ATTACH_MOUSE_MOVE
	ATTACH_WINDOW_RESIZE

	RecalculateProjectionMatrix();
	RecalculateViewMatrix();
}

void CameraComponent::OnMouseMove(GLFWwindow* window, double x, double y, Vector2 motion)
{
	if (EventSystem::GetRightMouseButton())
	{
		// translating mouse motion into rotation
		newRotation.x -= EventSystem::GetMouseMotion().y * cameraRotationSpeed;
		newRotation.x = std::clamp(newRotation.x, cameraRestraints.x, cameraRestraints.y);
		newRotation.y -= EventSystem::GetMouseMotion().x * cameraRotationSpeed;

		// rotating cmaera position
		auto cam = Vectors::MultiplyPoint(
			glm::rotate(
				Matrix4x4(1),
				newRotation.y - gameObject->transform->GetLocalRotation().y,
				Vector3(0, 1, 0)),
			gameObject->transform->GetPosition());
		cam = Vectors::MultiplyPoint(
			glm::rotate(
				Matrix4x4(1),
				gameObject->transform->GetLocalRotation().x - newRotation.x,
				gameObject->transform->GetRight()),
			cam);

		// applying rotation
		gameObject->transform->SetLocalRotation(newRotation.x, newRotation.y, 0);
		gameObject->transform->SetPosition(cam);
		gameObject->transform->SetForward(-cam);

		RecalculateViewMatrix();

		glfwSetInputMode(Initializer.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	else
		glfwSetInputMode(Initializer.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void CameraComponent::OnWindowResize(GLFWwindow* window, int width, int height)
{

}