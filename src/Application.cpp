#include <Application.h>
#include <utilities/Assert.h>
#include <GLFW/glfw3.h>

namespace syn
{

Application* Application::sm_singleton = nullptr;

Application::Application()
	: m_flags(0)
{
	assert(sm_singleton == nullptr);
	sm_singleton = this;
}

Application::~Application()
{
	sm_singleton = nullptr;
}

bool Application::create(const char* a_commandLine)
{
	if (onCreate(a_commandLine) == true)
	{
		m_flags |= Flag_Created;
		return true;
	}
	return false;
}

bool Application::create(int a_argc, char* a_argv[])
{
	if (onCreate(a_argc, a_argv) == true)
	{
		m_flags |= Flag_Created;
		return true;
	}
	return false;
}

void Application::run()
{
	assert(m_flags & Flag_Created);

	double prevTime = glfwGetTime();
	double currTime = 0;
	do
	{
		currTime = glfwGetTime();

		onUpdate((float)(currTime - prevTime));

		if ((m_flags & Flag_Destroyed) == 0)
			onRender();

		prevTime = currTime;
	} while ((m_flags & Flag_Destroyed) == 0);

	onDestroy();
}

} // namespace syn