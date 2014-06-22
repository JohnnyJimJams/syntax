#pragma once

#include <Syntax.h>

namespace syn
{

class SYNTAX_API Application
{
	enum Flag : unsigned short
	{
		Flag_Created	= (1<<0),
		Flag_Destroyed	= (1<<1),
	};

public:

	Application();
	virtual ~Application();

	static Application*	getApp();

	// create the app, passing in the command line
	bool	create(const char* a_commandLine);
	bool	create(int a_argc, char* a_argv[]);

	// our game loop
	virtual void	run();

	// closes the program
	void	destroy();

	// check if the application was created successfully
	bool	isCreated() const;

protected:

	virtual bool	onCreate(const char* a_cmdLine);
	virtual bool	onCreate(int a_argc, char* a_argv[]);

	virtual void	onUpdate(float a_deltaTime) = 0;
	virtual void	onRender() = 0;
	virtual void	onDestroy() = 0;
	
private:

	Application(const Application& a_rhs);
	Application& operator = (const Application& a_rhs);

protected:

	unsigned short		m_flags;

	static Application*	sm_singleton;
};

inline Application* Application::getApp()
{
	return sm_singleton;
}

inline void Application::destroy()
{
	m_flags |= Flag_Destroyed;
}

inline bool Application::isCreated() const
{
	return (m_flags & Flag_Created) != 0;
}

inline bool Application::onCreate(const char* a_cmdLine)
{	
	return true;	
}

inline bool Application::onCreate(int a_argc, char* a_argv[])
{	
	return true;	
}

} // namespace syn

#define SYNTAX_ENTRY(appClass)										\
	int main(int a_argc, char* a_argv[])							\
	{																\
		syn::Application* app = new appClass();						\
		if (app->create(a_argc,a_argv) == true)						\
			app->run();												\
		else														\
			syn::logError("Failed to setup application!\n");		\
		delete app;													\
		return 0;													\
	}

#define SYNTAX_ENTRY_WIN32(appClass)								\
	int WINAPI WinMain( __in HINSTANCE a_instance, __in_opt HINSTANCE a_prevInstance, __in LPSTR a_cmdLine, __in int a_showCmd )							\
	{																\
		syn::Application* app = new appClass();						\
		if (app->create(a_cmdLine) == true)							\
			app->run();												\
		else														\
			syn::logError("Failed to setup application!\n");		\
		delete app;													\
		return 0;													\
	}