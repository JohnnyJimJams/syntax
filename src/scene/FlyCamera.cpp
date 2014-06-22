#include <scene/FlyCamera.h>
#include <GLFW/glfw3.h>

namespace syn
{

SynRTTIDerived(FlyCamera, Camera);

void FlyCamera::update(float a_deltaTime)
{
	GLFWwindow* window = glfwGetCurrentContext();

	float frameSpeed = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ? a_deltaTime * m_speed * 2 : a_deltaTime * m_speed;

	// translate
	if (glfwGetKey(window, 'W') == GLFW_PRESS)
		m_localTransform[3] -= m_localTransform[2] * frameSpeed;
	if (glfwGetKey(window, 'S') == GLFW_PRESS)
		m_localTransform[3] += m_localTransform[2] * frameSpeed;
	if (glfwGetKey(window, 'D') == GLFW_PRESS)
		m_localTransform[3] += m_localTransform[0] * frameSpeed;
	if (glfwGetKey(window, 'A') == GLFW_PRESS)
		m_localTransform[3] -= m_localTransform[0] * frameSpeed;
	if (glfwGetKey(window, 'Q') == GLFW_PRESS)
		m_localTransform[3] += m_localTransform[1] * frameSpeed;
	if (glfwGetKey(window, 'E') == GLFW_PRESS)
		m_localTransform[3] -= m_localTransform[1] * frameSpeed;

	// check for rotation
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
	{
		if (m_mouseButtonDown == false)
		{
			m_mouseButtonDown = true;
			glfwGetCursorPos(window, &m_lastMouseX, &m_lastMouseY);
		}

		double mouseX = 0, mouseY = 0;
		glfwGetCursorPos(window, &mouseX, &mouseY);

		double deltaX = mouseX - m_lastMouseX;
		double deltaY = mouseY - m_lastMouseY;

		m_lastMouseX = mouseX;
		m_lastMouseY = mouseY;

		glm::mat4 mMat;

		// pitch
		if (deltaY != 0)
		{
			mMat = glm::axisAngleMatrix(m_localTransform[0].xyz(), (float)-deltaY / 150.0f);
			m_localTransform[0] = mMat * m_localTransform[0];
			m_localTransform[1] = mMat * m_localTransform[1];
			m_localTransform[2] = mMat * m_localTransform[2];
		}

		// yaw
		if (deltaX != 0)
		{
			mMat = glm::axisAngleMatrix(m_up, (float)-deltaX / 150.0f);
			m_localTransform[0] = mMat * m_localTransform[0];
			m_localTransform[1] = mMat * m_localTransform[1];
			m_localTransform[2] = mMat * m_localTransform[2];
		}
	}
	else
	{
		m_mouseButtonDown = false;
	}
	
	updateGlobalTransform();
}

} // namespace syn