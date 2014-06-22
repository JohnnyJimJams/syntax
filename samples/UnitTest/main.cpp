#include <Application.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>
#include <utilities/Log.h>
#include <utilities/Gizmos.h>
#include <scene/FlyCamera.h>
#include <render/ResourceLibrary.h>

class TestApp : public syn::Application
{
public:

	TestApp() : m_fps(0), m_window(nullptr){}
	virtual ~TestApp() {}

	float			m_fps;
	GLFWwindow*		m_window;
	syn::NodePtr	m_scene;
	syn::CameraPtr	m_camera;

	virtual bool onCreate(int a_argc, char* a_argv[])
	{
		// initialise glfw systems
		if (glfwInit() != GL_TRUE)
			return false;

		glfwDefaultWindowHints();

		// turn on 8x AA
	//	glfwWindowHint(GLFW_SAMPLES, 8);

		// create a windowed mode window and its OpenGL context
		m_window = glfwCreateWindow(1280, 720, "Syntax Unit Test", nullptr, nullptr);
		if (m_window == nullptr)
		{
			glfwTerminate();
			return false;
		}

		auto major = glfwGetWindowAttrib(m_window, GLFW_CONTEXT_VERSION_MAJOR);
		auto minor = glfwGetWindowAttrib(m_window, GLFW_CONTEXT_VERSION_MINOR);
		auto revision = glfwGetWindowAttrib(m_window, GLFW_CONTEXT_REVISION);
		printf("GL %i.%i.%i\n", major, minor, revision);

		// make the window's context current
		glfwMakeContextCurrent(m_window);

		// initialise glew systems to wrangle GL extensions
		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
		{
			glfwTerminate();
			return false;
		}

		// simply resize the GL viewport when the window size changes
		glfwSetWindowSizeCallback(m_window, [](GLFWwindow*, int w, int h)
		{ 
			glViewport(0, 0, w, h); 
			TestApp* app = (TestApp*)Application::getApp();
			app->m_camera->setPerspectiveProjection(glm::quarter_pi<float>(), w / (float)h, 0.1f, 1000);
		});

		// start systems
		syn::Gizmos::create();
		syn::ResourceLibrary* resourceLibrary = syn::ResourceLibrary::create();
/*
		// create skybox
		const char* skyboxShaderFiles[] = {
			"shaders/skybox.vert",
			nullptr,nullptr,nullptr,
			"shaders/skybox.frag"
		};
		syn::Shader* skyboxShader = resourceLibrary->createShader("skybox", skyboxShaderFiles);
		const char* skyboxTextures[] = {
			"images/skybox/skyrender_posx.bmp",
			"images/skybox/skyrender_negx.bmp",
			"images/skybox/skyrender_posy.bmp",
			"images/skybox/skyrender_negy.bmp",
			"images/skybox/skyrender_posz.bmp",
			"images/skybox/skyrender_negz.bmp",
		};
		syn::Texture* skyboxTexture = resourceLibrary->loadTextureCube(skyboxTextures);
		syn::Material* skyboxMaterial = resourceLibrary->createMaterial("skybox");
		skyboxMaterial->setShader( skyboxShader );
		skyboxMaterial->setTexture(syn::Material::Diffuse, skyboxTexture);
*/
		// create a camera
		m_camera = new syn::FlyCamera(10);
		m_camera->setLocalTranslation(0,2,10);
		m_camera->setActive();

		// create scene
		m_scene = new syn::Node();
		m_scene->setName("root");
		m_scene->attachChild(m_camera);
/*		
		syn::Mesh* skybox = new syn::Mesh();
		skybox->setName("skybox");
		skybox->setMaterial(skyboxMaterial);
		skybox->attachGeometry(syn::Geometry::createBoxSimpleInverted());
		skybox->setLocalScale(256);
		m_scene->attachChild(skybox);
*/		
		glClearColor(0.25f,0.25f,0.25f,1);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		return true;
	}

	virtual void onDestroy()
	{
		m_scene = nullptr;
		m_camera = nullptr;

		syn::Gizmos::destroy();
		syn::ResourceLibrary::destroy();

		glfwTerminate();
	}

	virtual void onUpdate(float a_deltaTime)
	{
		glfwPollEvents();

		syn::Gizmos::clear();

		// frame rate
		static float s_timer = 0;
		static unsigned int s_frame = 0;
		s_timer += a_deltaTime;
		++s_frame;
		if (s_timer >= 1)
		{
			m_fps = s_frame / s_timer;
			s_frame = 0;
			s_timer -= 1;
		}

		m_scene->update(a_deltaTime);

		if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS ||
			glfwWindowShouldClose(m_window) == GL_TRUE)
			destroy();
	}

	virtual void onRender()
	{		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_scene->render();

		debugRender();
		
		glfwSwapBuffers(m_window);
	}

	void debugRender()
	{
		// add an identity matrix gizmo
		syn::Gizmos::addTransform(glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1));

		// add a 20x20 grid on the Z-plane
		for (int i = 0; i < 21; ++i)
		{
			syn::Gizmos::addLine(glm::vec3(-10 + i, 0, 10), glm::vec3(-10 + i, 0, -10),
				i == 10 ? glm::vec4(1, 1, 1, 1) : glm::vec4(0, 0, 0, 1));

			syn::Gizmos::addLine(glm::vec3(10, 0, -10 + i), glm::vec3(-10, 0, -10 + i),
				i == 10 ? glm::vec4(1, 1, 1, 1) : glm::vec4(0, 0, 0, 1));
		}

		syn::Gizmos::draw(m_camera->getProjectionView());
	}
};

SYNTAX_ENTRY(TestApp);