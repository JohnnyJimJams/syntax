#include <Application.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>
#include <utilities/Log.h>
#include <utilities/Gizmos.h>
#include <scene/FlyCamera.h>
#include <render/ResourceLibrary.h>
#include <render/Mesh.h>

class TestApp : public syn::Application
{
public:

	TestApp() : m_fps(0), m_window(nullptr){}
	virtual ~TestApp() {}

	float				m_fps;
	GLFWwindow*			m_window;
	syn::SceneNodePtr	m_scene;

	virtual bool onCreate(int a_argc, char* a_argv[])
	{
		// initialise glfw systems
		if (glfwInit() != GL_TRUE)
			return false;

		glfwDefaultWindowHints();

		// turn on 8x AA
		glfwWindowHint(GLFW_SAMPLES, 16);

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
		syn::log("GL %i.%i.%i\n", major, minor, revision);

		// make the window's context current
		glfwMakeContextCurrent(m_window);

		// initialise glew systems to wrangle GL extensions
		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
		{
			glfwTerminate();
			return false;
		}

		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(syn::logGLDebugCallback, nullptr);

		// simply resize the GL viewport when the window size changes
		glfwSetWindowSizeCallback(m_window, [](GLFWwindow*, int w, int h)
		{ 
			glViewport(0, 0, w, h); 
			syn::Camera::getActiveCamera()->setPerspectiveProjection(glm::quarter_pi<float>(), w / (float)h, 0.1f, 1000);
		});

		// start systems
		syn::Gizmos::create();
		syn::ResourceLibrary* resourceLibrary = syn::ResourceLibrary::create();

		// create skybox
		const char* skyboxShaderFiles[] = {
			"../../bin/shaders/skybox.vert",
			nullptr,nullptr,nullptr,
			"../../bin/shaders/skybox.frag"
		};
		syn::Shader* skyboxShader = resourceLibrary->createShader("skybox", skyboxShaderFiles);
		const char* skyboxTextures[] = {
			"../../bin/textures/skybox/ennis_cube_posx.tga",
			"../../bin/textures/skybox/ennis_cube_negx.tga",
			"../../bin/textures/skybox/ennis_cube_posy.tga",
			"../../bin/textures/skybox/ennis_cube_negy.tga",
			"../../bin/textures/skybox/ennis_cube_posz.tga",
			"../../bin/textures/skybox/ennis_cube_negz.tga",
		};
		syn::Texture* skyboxTexture = resourceLibrary->loadTextureCube(skyboxTextures);
		syn::Material* skyboxMaterial = resourceLibrary->createMaterial("skybox");
		skyboxMaterial->setShader( skyboxShader );
		skyboxMaterial->setTexture(syn::Material::Diffuse, skyboxTexture);
		
		const char* glassShaderFiles[] = {
			"../../bin/shaders/glass.vert",
			nullptr, nullptr, nullptr,
			"../../bin/shaders/glass.frag"
		};
		syn::Shader* glassShader = resourceLibrary->createShader("glass", glassShaderFiles);

		syn::Material* glassMaterial = resourceLibrary->createMaterial("glass");
		glassMaterial->setShader(glassShader);
		glassMaterial->setTexture(syn::Material::Environment, skyboxTexture);

		syn::Texture* diffuseTexture = resourceLibrary->loadTexture("../../bin/textures/numbered_grid.tga");
		glassMaterial->setTexture(syn::Material::Diffuse, diffuseTexture);
		
		// create scene
		m_scene = new syn::SceneNode("root");

		syn::Camera* camera = new syn::FlyCamera(10);
		camera->lookAtFrom(glm::vec3(10), glm::vec3(0), glm::vec3(0, 1, 0));
		camera->setActive();
		m_scene->attachChild(camera);

		syn::Mesh* skybox = new syn::Mesh("skybox");
		skybox->setMaterial(skyboxMaterial);
		skybox->attachGeometry(syn::Geometry::createBoxSimpleInverted());
		skybox->setLocalScale(256);
		m_scene->attachChild(skybox);

		syn::SceneNode* temp = new syn::SceneNode();
		m_scene->attachChild(temp);

		syn::Mesh* sphere = new syn::Mesh("sphere");
		sphere->setMaterial(glassMaterial);
		sphere->attachGeometry(syn::Geometry::createSphere(syn::Vertex::TexCoord0 | syn::Vertex::Normal, 32, 32));
		sphere->setLocalScale(5);
		sphere->setLocalTranslation(10, 0, 0);
		temp->attachChild(sphere);

		syn::RotationController* rot = new syn::RotationController(glm::quarter_pi<float>(), glm::vec3(0, 1, 0));
		temp->attachController(rot);

		syn::SceneNode* fbx = resourceLibrary->loadFBXScene("../../bin/models/stanford/bunny.fbx");
		m_scene->attachChild(fbx);

		fbx->attachController(rot);

		// hacked together for now
		resourceLibrary->getMaterial("lambert1")->setShader(glassShader);
		resourceLibrary->getMaterial("lambert1")->setTexture(syn::Material::Environment, skyboxTexture);

		glClearColor(0.25f,0.25f,0.25f,1);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		return true;
	}

	virtual void onDestroy()
	{
		m_scene = nullptr;

		syn::Gizmos::destroy();
		syn::ResourceLibrary::destroy();

		glfwTerminate();
	}

	virtual void onUpdate(float a_deltaTime)
	{
		glfwPollEvents();

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

	//	debugRender();
		
		glfwSwapBuffers(m_window);
	}

	void debugRender()
	{
		syn::Gizmos::clear();

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

		syn::Gizmos::draw(syn::Camera::getActiveCamera()->getProjectionView());
	}
};

SYNTAX_ENTRY(TestApp);