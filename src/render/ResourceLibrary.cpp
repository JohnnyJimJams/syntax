#include <render/ResourceLibrary.h>
#include <render/Mesh.h>
#include <utilities/HashFunctions.h>
#include <gl_core_4_4.h>
#include <glm/ext.hpp>
#include <fbxsdk.h>
#include <utilities/Log.h>
#include <utilities/Assert.h>
#include <algorithm>
#include <set>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <thread>

namespace syn
{

ResourceLibrary* ResourceLibrary::sm_singleton = nullptr;

ResourceLibrary::ResourceLibrary()
{
	Shader::initialiseSharedUniforms();
	loadDefaults();
}

ResourceLibrary::~ResourceLibrary()
{
	Shader::destroySharedUniforms();
}

void ResourceLibrary::loadDefaults()
{
	// invalid shader
	const char* invalidVS = "#version 330\n layout (location = 0) in vec4 position; out vec4 localPosition; struct Camera { mat4 projectionView; };"
		"uniform Camera camera; uniform mat4 worldTransform; void main(){localPosition = position; gl_Position = camera.projectionView * worldTransform * localPosition;}";
	const char* invalidFS = "#version 330\n in vec4 localPosition; out vec4 fragColour; void main(){"
		"fragColour = vec4(1,0,1,1);	if (mod(localPosition.x, 1.0) < 0.05f || mod(localPosition.y, 1.0) < 0.05f || mod(localPosition.z, 1.0) < 0.05f) fragColour = vec4(0,0,0,1);}";

	Shader::sm_invalidShader = createShader("invalid");
	Shader::sm_invalidShader->compileShaderFromString(Shader::Vertex, invalidVS);
	Shader::sm_invalidShader->compileShaderFromString(Shader::Fragment, invalidFS);
	if (Shader::sm_invalidShader->linkProgram() == false)
	{
		logError("Shader Link Error for shader [invalid]:\n");
		logError(Shader::sm_invalidShader->getLastError());
		log("\n");
	}

	// this doesn't seem needed :(
	Material::sm_invalidMaterial = createMaterial("invalid");

	// default textures
	unsigned char white[] = { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 };
	unsigned char black[] = { 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255 };
	unsigned char pink[] = { 255, 0, 255, 255, 255, 0, 255, 255, 255, 0, 255, 255, 255, 0, 255, 255 };
	unsigned char normal[] = { 128, 128, 255, 255, 128, 128, 255, 255, 128, 128, 255, 255, 128, 128, 255, 255 };
	createTexture("white", 2, 2, GL_RGB, GL_RGBA, GL_UNSIGNED_BYTE, Texture::Texture2D, white);
	createTexture("black", 2, 2, GL_RGB, GL_RGBA, GL_UNSIGNED_BYTE, Texture::Texture2D, black);
	createTexture("pink", 2, 2, GL_RGB, GL_RGBA, GL_UNSIGNED_BYTE, Texture::Texture2D, pink);
	createTexture("normal", 2, 2, GL_RGB, GL_RGBA, GL_UNSIGNED_BYTE, Texture::Texture2D, normal);
}

ResourceLibrary* ResourceLibrary::create()
{
	sm_singleton = new ResourceLibrary();
	return sm_singleton;
}

void ResourceLibrary::destroy()
{
	delete sm_singleton;
	sm_singleton = nullptr;
}

Material* ResourceLibrary::getMaterialByIndex(unsigned int a_index)
{
	Material* material = nullptr;
	auto iter = m_materials.begin();
	unsigned int size = m_materials.size();

	for ( unsigned int i = 0 ; i <= a_index && a_index < size ; ++i, ++iter )
		material = iter->second; 

	return material;
}

Texture* ResourceLibrary::createTexture(const char* a_name, unsigned int a_width, unsigned int a_height, 
										unsigned int a_format, unsigned int a_channels, unsigned int a_dataType,
										unsigned int a_type /* = Texture::Texture2D */, const void* a_data /* = nullptr */, bool a_ownData /* = false */)
{
	Texture* texture = nullptr;

	unsigned int id = SynStringHash(a_name);

	auto iter = m_textures.find( id );
	if (iter == m_textures.end())
	{
		texture = new Texture(id,a_name,(Texture::Type)a_type);

		glGenTextures(1, &texture->m_handle);
		glBindTexture(GL_TEXTURE_2D, texture->m_handle);
		glTexImage2D(GL_TEXTURE_2D, 0, a_format, a_width, a_height, 0, a_channels, a_dataType, a_data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);

		texture->m_format = a_format;
		if (a_ownData)
			texture->m_data = (unsigned char*)a_data;
		texture->m_width = a_width;
		texture->m_height = a_height;
		
		m_textures[id] = texture;
	}

	return texture;
}

Texture* ResourceLibrary::loadTexture(const char* a_filename, unsigned int a_type /* = Texture::Texture2D */)
{
	Texture* texture = nullptr;

	unsigned int id = SynStringHash(a_filename);

	auto iter = m_textures.find( id );
	if (iter == m_textures.end())
	{
		texture = new Texture(id, a_filename, (Texture::Type)a_type);

		texture->m_data = stbi_load(a_filename, &texture->m_width, &texture->m_height, &texture->m_format, STBI_default);

		switch (texture->m_format)
		{
		case STBI_grey: texture->m_format = GL_RED; break;
		case STBI_grey_alpha: texture->m_format = GL_RG; break;
		case STBI_rgb: texture->m_format = GL_RGB; break;
		case STBI_rgb_alpha: texture->m_format = GL_RGBA; break;
		};

		glGenTextures(1, &texture->m_handle);
		glBindTexture(GL_TEXTURE_2D, texture->m_handle);
		glTexImage2D(GL_TEXTURE_2D, 0, texture->m_format, texture->m_width, texture->m_height, 0, texture->m_format, GL_UNSIGNED_BYTE, texture->m_data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);

		m_textures[id] = texture;
	}
	else
	{
		texture = iter->second;
	}

	return texture;
}

Texture* ResourceLibrary::reloadTexture(const char* a_filename)
{
	Texture* texture = nullptr;

	unsigned int id = SynStringHash(a_filename);

	auto iter = m_textures.find( id );
	if (iter != m_textures.end())
	{
		texture = iter->second;

		if (texture->getType() == Texture::TextureCube)
		{
			assertMessage(false, "Can not reload a TextureCube this way!");
		}
		else
		{
			delete[] texture->m_data;
			texture->m_data = stbi_load(a_filename, &texture->m_width, &texture->m_height, &texture->m_format, STBI_default);

			switch (texture->m_format)
			{
			case STBI_grey: texture->m_format = GL_RED; break;
			case STBI_grey_alpha: texture->m_format = GL_RG; break;
			case STBI_rgb: texture->m_format = GL_RGB; break;
			case STBI_rgb_alpha: texture->m_format = GL_RGBA; break;
			};

			glBindTexture(GL_TEXTURE_2D, texture->m_handle);
			glTexImage2D(GL_TEXTURE_2D, 0, texture->m_format, texture->m_width, texture->m_height, 0, texture->m_format, GL_UNSIGNED_BYTE, texture->m_data);
			glGenerateMipmap(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		}
	}

	return texture;
}

Texture* ResourceLibrary::reloadTexture(Texture* a_texture)
{
	if (a_texture->getType() == Texture::TextureCube)
	{
		assertMessage(false, "Can not reload a TextureCube this way!");
	}
	else
	{
		delete[] a_texture->m_data;
		a_texture->m_data = stbi_load(a_texture->getName(), &a_texture->m_width, &a_texture->m_height, &a_texture->m_format, STBI_default);

		switch (a_texture->m_format)
		{
		case STBI_grey: a_texture->m_format = GL_RED; break;
		case STBI_grey_alpha: a_texture->m_format = GL_RG; break;
		case STBI_rgb: a_texture->m_format = GL_RGB; break;
		case STBI_rgb_alpha: a_texture->m_format = GL_RGBA; break;
		};

		glBindTexture(GL_TEXTURE_2D, a_texture->m_handle);
		glTexImage2D(GL_TEXTURE_2D, 0, a_texture->m_format, a_texture->m_width, a_texture->m_height, 0, a_texture->m_format, GL_UNSIGNED_BYTE, a_texture->m_data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}

	return a_texture;
}

CubeTexture* ResourceLibrary::loadTextureCube(const char* a_name, const char** a_filenames)
{
	CubeTexture* texture = nullptr;

	unsigned int id = SynStringHash(a_name);

	auto iter = m_textures.find( id );
	if (iter == m_textures.end())
	{
		int x[6], y[6], comp[6];
		unsigned char* data[6];

		// thread loading the pixel data
		std::vector<std::thread> threads;
		for (int i = 0; i < 6; ++i)
		{
			threads.push_back(std::thread([&x, &y, &comp, &data](int i, const char* filename)
			{
				data[i] = stbi_load(filename, &x[i], &y[i], &comp[i], STBI_default);
			}, i, a_filenames[i]));
		}

		for (auto& t : threads)
			t.join();

		int format = 0;
		switch (comp[0])
		{
		case STBI_grey: format = GL_RED; break;
		case STBI_grey_alpha: format = GL_RG; break;
		case STBI_rgb: format = GL_RGB; break;
		case STBI_rgb_alpha: format = GL_RGBA; break;
		};

		unsigned int handle;
		glGenTextures(1, &handle);
		glBindTexture(GL_TEXTURE_CUBE_MAP, handle);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, format, x[0], y[0], 0, format, GL_UNSIGNED_BYTE, data[0]);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, format, x[1], y[1], 0, format, GL_UNSIGNED_BYTE, data[1]);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, format, x[2], y[2], 0, format, GL_UNSIGNED_BYTE, data[2]);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, format, x[3], y[3], 0, format, GL_UNSIGNED_BYTE, data[3]);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, format, x[4], y[4], 0, format, GL_UNSIGNED_BYTE, data[4]);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, format, x[5], y[5], 0, format, GL_UNSIGNED_BYTE, data[5]);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		texture = new CubeTexture(id, a_name, handle);
		m_textures[id] = texture;

		texture->m_width = x[0];
		texture->m_height = y[0];
		texture->m_format = format;

		texture->m_filenames[0] = a_filenames[0];
		texture->m_filenames[1] = a_filenames[1];
		texture->m_filenames[2] = a_filenames[2];
		texture->m_filenames[3] = a_filenames[3];
		texture->m_filenames[4] = a_filenames[4];
		texture->m_filenames[5] = a_filenames[5];

		texture->m_data = data[0];
		texture->m_extraData[0] = data[1];
		texture->m_extraData[1] = data[2];
		texture->m_extraData[2] = data[3];
		texture->m_extraData[3] = data[4];
		texture->m_extraData[4] = data[5];
	}
	else
	{
		logWarning("Texture with matching name [%s] already loaded! Returning previously loaded texture.\n", a_name);
		texture = (CubeTexture*)(Texture*)iter->second;
	}

	return texture;
}

CubeTexture* ResourceLibrary::reloadTextureCube(const char* a_name, const char** a_filenames)
{
	CubeTexture* texture = nullptr;

	unsigned int id = SynStringHash(a_name);

	auto iter = m_textures.find( id );
	if (iter != m_textures.end())
	{
		assertMessage(iter->second->getType() == Texture::TextureCube, "Can not reaload TextureCube as texture is not a cube map!");

		texture = (CubeTexture*)(Texture*)(iter->second);

		int x[6], y[6], comp[6];
		unsigned char* data[6];

		// thread loading the pixel data
		std::vector<std::thread> threads;
		for (int i = 0; i < 6; ++i)
		{
			threads.push_back(std::thread([&x, &y, &comp, &data](int i, const char* filename)
			{
				data[i] = stbi_load(filename, &x[i], &y[i], &comp[i], STBI_default);
			}, i, a_filenames[i]));
		}

		for (auto& t : threads)
			t.join();

		int format = 0;
		switch (comp[0])
		{
		case STBI_grey: format = GL_RED; break;
		case STBI_grey_alpha: format = GL_RG; break;
		case STBI_rgb: format = GL_RGB; break;
		case STBI_rgb_alpha: format = GL_RGBA; break;
		};

		texture->m_width = x[0];
		texture->m_height = y[0];
		texture->m_format = format;

		glBindTexture(GL_TEXTURE_CUBE_MAP, texture->m_handle);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, format, x[0], y[0], 0, format, GL_UNSIGNED_BYTE, data[0]);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, format, x[1], y[1], 0, format, GL_UNSIGNED_BYTE, data[1]);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, format, x[2], y[2], 0, format, GL_UNSIGNED_BYTE, data[2]);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, format, x[3], y[3], 0, format, GL_UNSIGNED_BYTE, data[3]);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, format, x[4], y[4], 0, format, GL_UNSIGNED_BYTE, data[4]);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, format, x[5], y[5], 0, format, GL_UNSIGNED_BYTE, data[5]);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		texture->m_filenames[0] = a_filenames[0];
		texture->m_filenames[1] = a_filenames[1];
		texture->m_filenames[2] = a_filenames[2];
		texture->m_filenames[3] = a_filenames[3];
		texture->m_filenames[4] = a_filenames[4];
		texture->m_filenames[5] = a_filenames[5];

		delete[] texture->m_data;
		texture->m_data = data[0];
		delete[] texture->m_extraData[0];
		texture->m_extraData[0] = data[1];
		delete[] texture->m_extraData[1];
		texture->m_extraData[1] = data[2];
		delete[] texture->m_extraData[2];
		texture->m_extraData[2] = data[3];
		delete[] texture->m_extraData[3];
		texture->m_extraData[3] = data[4];
		delete[] texture->m_extraData[4];
		texture->m_extraData[4] = data[5];
	}

	return texture;
}

Texture* ResourceLibrary::getTexture(const char* a_filename)
{
	unsigned int id = SynStringHash(a_filename);
	auto iter = m_textures.find( id );
	if (iter != m_textures.end())
		return iter->second;
	return nullptr;
}

void ResourceLibrary::releaseTexture(Texture* a_texture)
{
	auto iter = m_textures.find( a_texture->m_id );
	if (iter != m_textures.end())
	{
		m_textures.erase(iter);
	}
}

void ResourceLibrary::releaseAllTextures()
{
	m_textures.clear();
}

Material* ResourceLibrary::createMaterial(const char* a_name)
{
	Material* material = nullptr;

	unsigned int id = SynStringHash(a_name);
	auto iter = m_materials.find( id );
	if (iter == m_materials.end())
	{
		material = new Material(a_name,id);
		m_materials[id] = material;
	}
	else
	{
		material = iter->second;
	}

	return material;
}

Material* ResourceLibrary::getMaterial(const char* a_name)
{
	unsigned int id = SynStringHash(a_name);
	auto iter = m_materials.find( id );
	if (iter != m_materials.end())
		return iter->second;
	return nullptr;
}

void ResourceLibrary::releaseMaterial(Material* a_material)
{
	auto iter = m_materials.find( a_material->m_id );
	if (iter != m_materials.end())
	{
		m_materials.erase(iter);
	}
}

void ResourceLibrary::releaseAllMaterials()
{
	m_materials.clear();
}

Shader* ResourceLibrary::createShader(const char* a_name)
{
	unsigned int id = SynStringHash(a_name);

	auto iter = m_shaders.find( id );
	if (iter != m_shaders.end())
	{
		logWarning("Shader [%s] already created!\n", a_name);
		return nullptr;
	}

	Shader* shader = new Shader(a_name);
	m_shaders[id] = shader;
	return shader;
}

Shader* ResourceLibrary::createShader(const char* a_name, const char** a_shaderFiles)
{
	Shader* shader = nullptr;

	unsigned int id = SynStringHash(a_name);

	auto iter = m_shaders.find( id );
	if (iter == m_shaders.end())
	{
		shader = new Shader(a_name);
		shader->compileShaderFromFile(Shader::Vertex, a_shaderFiles[0]);
		shader->compileShaderFromFile(Shader::Control, a_shaderFiles[1]);
		shader->compileShaderFromFile(Shader::Evaluation, a_shaderFiles[2]);
		shader->compileShaderFromFile(Shader::Geometry, a_shaderFiles[3]);
		shader->compileShaderFromFile(Shader::Fragment, a_shaderFiles[4]);
		if (shader->linkProgram() == false)
		{
			logError("Shader Link Error for shader [%s]:\n", a_name);
			logError(shader->getLastError());
			log("\n");
		}

		m_shaders[id] = shader;
	}
	else
	{
		shader = iter->second;
	}

	return shader;
}

Shader* ResourceLibrary::getShader(const char* a_name)
{
	unsigned int id = SynStringHash(a_name);
	auto iter = m_shaders.find( id );
	if (iter != m_shaders.end())
		return iter->second;
	return nullptr;
}

void ResourceLibrary::releaseShader(Shader* a_shader)
{
	unsigned int id = SynStringHash(a_shader->getName());
	auto iter = m_shaders.find( id );
	if (iter != m_shaders.end())
	{
		m_shaders.erase(iter);
	}
}

void ResourceLibrary::releaseAllShaders()
{
	m_shaders.clear();
}

void ResourceLibrary::reloadShaders()
{
	for (auto shader : m_shaders)
		shader.second->reload();
}

FrameBuffer* ResourceLibrary::createFrameBuffer(const char* a_name, unsigned int a_width, unsigned int a_height, 
	unsigned int a_targetCount, FrameBufferDesc* a_targetDesc,
	bool a_depth /* = true */, bool a_stencil /* = false */)
{
	unsigned int id = SynStringHash(a_name);
	auto iter = m_frameBuffers.find( id );
	if (iter != m_frameBuffers.end())
	{
		return iter->second;
	}

	FrameBuffer* frameBuffer = new FrameBuffer(a_name,id,a_width,a_height,a_targetCount,a_targetDesc,a_depth,a_stencil);
	m_frameBuffers[id] = frameBuffer;
	return frameBuffer;
}

FrameBuffer* ResourceLibrary::getFrameBuffer(const char* a_name)
{
	unsigned int id = SynStringHash(a_name);
	auto iter = m_frameBuffers.find( id );
	return iter != m_frameBuffers.end() ? iter->second : nullptr;
}

void ResourceLibrary::releaseFrameBuffer(FrameBuffer* a_frameBuffer)
{
	auto iter = m_frameBuffers.find( a_frameBuffer->m_id );
	if (iter != m_frameBuffers.end())
		m_frameBuffers.erase( iter );
}

void ResourceLibrary::releaseAllFrameBuffers()
{
	m_frameBuffers.clear();
}

SceneNode* ResourceLibrary::loadFBXScene(const char* a_filename, glm::vec4* a_ambientLight, bool a_retainMeshData)
{
	SceneNode* root = nullptr;

	unsigned int id = SynStringHash(a_filename);

	if (m_scenes.find(id) != m_scenes.end())
	{
		logError("Scene already loaded!\n");
		return root;
	}
	
	// The first thing to do is to create the FBX SDK manager which is the 
	// object allocator for almost all the classes in the SDK.
	FbxManager* lSdkManager = FbxManager::Create();
	if ( !lSdkManager )
	{
		logError("Unable to create the FBX SDK manager\n");
		return root;
	}

	// create an IOSettings object
	FbxIOSettings * ios = FbxIOSettings::Create(lSdkManager, IOSROOT );
	lSdkManager->SetIOSettings(ios);

	// Load plugins from the executable directory
	FbxString lPath = FbxGetApplicationDirectory();
	lSdkManager->LoadPluginsDirectory(lPath.Buffer());

	// Create the entity that will hold the scene.
	FbxScene* lScene = FbxScene::Create(lSdkManager,"");
	
	// Get the file version number generate by the FBX SDK.
	int lSDKMajor,  lSDKMinor,  lSDKRevision;
	FbxManager::GetFileFormatVersion(lSDKMajor, lSDKMinor, lSDKRevision);

/*	* // create a SdkManager
		* FbxManager *lSdkManager = FbxManager::Create();
	*
		* // create an IOSettings object
		* FbxIOSettings * ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
	*
		* // set some IOSettings options 
		* ios->SetBoolProp(IMP_FBX_MATERIAL, true);
	*ios->SetBoolProp(IMP_FBX_TEXTURE, true);
	*
		* // create an empty scene
		* FbxScene* lScene = FbxScene::Create(lSdkManager, "");
	*
		* // Create an importer.
		* FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "");
	*
		* // Initialize the importer by providing a filename and the IOSettings to use
		* lImporter->Initialize("C:\\myfile.fbx", -1, ios);
	*
		* // Import the scene.
		* lImporter->Import(lScene);
	*
		* // Destroy the importer.
		* lImporter->Destroy();
	*/
	// Create an importer.
	FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "");
	const bool lImportStatus = lImporter->Initialize(a_filename, -1, lSdkManager->GetIOSettings());

	int lFileMajor, lFileMinor, lFileRevision;
	lImporter->GetFileVersion(lFileMajor, lFileMinor, lFileRevision);

	if ( !lImportStatus )
	{
		logError("Call to FbxImporter::Initialize() failed.\n");
		logError("Error returned: %s\n\n", lImporter->GetStatus().GetErrorString());

		if (lImporter->GetStatus().GetCode() == FbxStatus::eInvalidFileVersion)
		{
			logError("FBX version number for this FBX SDK is %d.%d.%d\n", lSDKMajor, lSDKMinor, lSDKRevision);
			logError("FBX version number for file %s is %d.%d.%d\n\n", a_filename, lFileMajor, lFileMinor, lFileRevision);
		}

		lImporter->Destroy();
		return root;
	}

	// Import the scene.
	bool status = lImporter->Import(lScene);
	lImporter->Destroy();
	if (status == false)
	{
		logError("Unable to open FBX file!\n");
		return root;
	}

	// convert the scene to OpenGL axis (right-handed Y up)
	FbxAxisSystem::OpenGL.ConvertScene(lScene);

	FbxNode* lNode = lScene->GetRootNode();

	if (lNode != nullptr)
	{
		std::string path = a_filename;
		int iLastForward = path.find_last_of('/');
		int iLastBackward = path.find_last_of('\\');
		if (iLastForward > iLastBackward)
		{
			path.resize(iLastForward + 1);
		}
		else if (iLastBackward != 0)
		{
			path.resize(iLastBackward + 1);
		}
		else
		{
			path = "";
		}

		root = new SceneNode();
		root->setName(a_filename);
		
		// grab the ambient light data from the scene
		if (a_ambientLight != nullptr)
		{
			a_ambientLight->x = (float)lScene->GetGlobalSettings().GetAmbientColor().mRed;
			a_ambientLight->y = (float)lScene->GetGlobalSettings().GetAmbientColor().mGreen;
			a_ambientLight->z = (float)lScene->GetGlobalSettings().GetAmbientColor().mBlue;
			a_ambientLight->w = (float)lScene->GetGlobalSettings().GetAmbientColor().mAlpha;
		}

		// extract children
		for (int i = 0; i < lNode->GetChildCount(); ++i)
		{
			fbxExtractObject(root, (void*)lNode->GetChild(i), lScene, path.c_str(), a_retainMeshData);
		}

		m_scenes[ id ] = root;
	}

	lSdkManager->Destroy();
	return root;
}

//////////////////////////////////////////////////////////////////////////
void ResourceLibrary::fbxExtractObject(SceneNode* a_parent, void* a_object, void* a_fbxScene, const char* a_path, bool a_retainMeshData)
{
	FbxNode* fbxNode = (FbxNode*)a_object;

	// build local transform
	// use anim evaluator as bones store transforms in a different spot
	FbxAMatrix local = ((FbxScene*)a_fbxScene)->GetAnimationEvaluator()->GetNodeLocalTransform(fbxNode);

	glm::mat4 mLocalTransform( local[0][0], local[0][1], local[0][2], local[0][3],
							   local[1][0], local[1][1], local[1][2], local[1][3],
							   local[2][0], local[2][1], local[2][2], local[2][3],
							   local[3][0], local[3][1], local[3][2], local[3][3] );

	SceneObject* node = nullptr;
	if (fbxNode->GetChildCount() > 0)
	{
		node = new SceneNode();
		if (strlen(fbxNode->GetName()) > 0)
			node->setName(fbxNode->GetName());
	}

	FbxNodeAttribute::EType lAttributeType;
	int i;

	if (fbxNode->GetNodeAttribute() != nullptr)
	{
		lAttributeType = (fbxNode->GetNodeAttribute()->GetAttributeType());

		switch (lAttributeType)
		{
		case FbxNodeAttribute::eMarker:		break;
		case FbxNodeAttribute::eSkeleton:	break;

		case FbxNodeAttribute::eMesh:      
			{
				// output a mesh for every material
				int iMats = fbxNode->GetMaterialCount();
				if (iMats > 1)
				{
					if (node == nullptr)
					{
						node = new SceneNode();
						if (strlen(fbxNode->GetName()) > 0)
							node->setName(fbxNode->GetName());
					}

					for ( i = 0 ; i < iMats ; ++i )
					{
						Mesh* mesh = new Mesh();
						fbxExtractMesh(mesh,fbxNode,i,a_path,a_retainMeshData);
						
						// append material name to mesh name
						std::string name = fbxNode->GetName();
						name += fbxNode->GetMaterial(i)->GetName();
						mesh->setName(name.c_str());						

						((SceneNode*)node)->attachChild(mesh);
					}
				}
				else
				{
					if (node == nullptr)
					{
						node = new Mesh();
						fbxExtractMesh((Mesh*)node,fbxNode,0,a_path,a_retainMeshData);

						if (strlen(fbxNode->GetName()) > 0)
							node->setName(fbxNode->GetName());
					}
					else
					{
						Mesh* mesh = new Mesh();
						fbxExtractMesh(mesh,fbxNode,0,a_path,a_retainMeshData);

						if (strlen(fbxNode->GetName()) > 0)
							mesh->setName(fbxNode->GetName());

						((SceneNode*)node)->attachChild(mesh);
					}
				}
			}
			break;

		case FbxNodeAttribute::eNurbs:	break;
		case FbxNodeAttribute::ePatch:	break;

		case FbxNodeAttribute::eCamera:   
			{
// 				node = new FBXCameraNode();
// 				ExtractCamera((FBXCameraNode*)node,fbxNode);
// 
// 				if (strlen(fbxNode->GetName()) > 0)
// 					strncpy(node->m_name,fbxNode->GetName(),MAX_PATH-1);
// 
// 				m_cameras[node->m_name] = (FBXCameraNode*)node;
			}
			break;

		case FbxNodeAttribute::eLight:   
			{
// 			node = new FBXLightNode();
// 				ExtractLight((FBXLightNode*)node,fbxNode);
// 
// 				if (strlen(fbxNode->GetName()) > 0)
// 					strncpy(node->m_name,fbxNode->GetName(),MAX_PATH-1);
// 
// 				m_lights[node->m_name] = (FBXLightNode*)node;
			}
			break;
		}   
	}
	
	// if null then use it as a plain 3D object
	if (node == nullptr)
	{
		node = new SceneObject();
		if (strlen(fbxNode->GetName()) > 0)
			node->setName(fbxNode->GetName());
	}
	
	// add to parent's children and update parent
	a_parent->attachChild(node);

	// set global based off local and parent's global
	node->setLocalTransform(mLocalTransform);

	// children
	for (i = 0; i < fbxNode->GetChildCount(); i++)
	{
		fbxExtractObject((SceneNode*)node, (void*)fbxNode->GetChild(i), a_fbxScene, a_path, a_retainMeshData);
	}
}

//////////////////////////////////////////////////////////////////////////
void ResourceLibrary::fbxExtractMesh(Mesh* a_mesh, void* a_object, int a_material, const char* a_path, bool a_retainMeshData)
{
	FbxNode* fbxNode = (FbxNode*)a_object;
	FbxMesh* fbxMesh = (FbxMesh*)fbxNode->GetNodeAttribute();

	int i, j, lPolygonCount = fbxMesh->GetPolygonCount();
	FbxVector4* lControlPoints = fbxMesh->GetControlPoints(); 
	FbxGeometryElementMaterial* lMaterialElement = fbxMesh->GetElementMaterial(0);

	Vertex vertex;
	unsigned int vertexIndex[4] = {};

	int materialCount = fbxNode->GetMaterialCount();

	unsigned int vertexAttributes = 0;
	std::vector<Vertex> vertexBuffer;
	std::vector<unsigned int> indexBuffer;

	unsigned int nextIndex = 0;
	std::map<Vertex,unsigned int> vertexIndexMap;

	FbxGeometryElementVertexColor* fbxColours = fbxMesh->GetElementVertexColor(0);
	FbxGeometryElementUV* fbxTexCoord0 = fbxMesh->GetElementUV(0);
	FbxGeometryElementUV* fbxTexCoord1 = fbxMesh->GetElementUV(1);
	FbxGeometryElementNormal* fbxNormal = fbxMesh->GetElementNormal(0);
	FbxGeometryElementTangent* fbxTangent = fbxMesh->GetElementTangent(0);
	FbxGeometryElementBinormal* fbxBiNormal = fbxMesh->GetElementBinormal(0);

	int vertexId = 0;
	for (i = 0; i < lPolygonCount; i++)
	{
		int lPolygonSize = fbxMesh->GetPolygonSize(i);
		
		bool usePolygon = lMaterialElement->GetIndexArray().GetAt(i) == a_material;

		for (j = 0; j < lPolygonSize && j < 4 ; j++)
		{
			int lControlPointIndex = fbxMesh->GetPolygonVertex(i, j);
			
			FbxVector4 vPos = lControlPoints[lControlPointIndex];
			vertex.position.x = (float)vPos[0];
			vertex.position.y = (float)vPos[1];
			vertex.position.z = (float)vPos[2];
			vertex.position.w = 1;

			vertexAttributes |= Vertex::Position;

// 			if (fbxColours != nullptr)
// 			{
// 				switch (fbxColours->GetMappingMode())
// 				{
// 				case FbxGeometryElement::eByControlPoint:
// 					switch (fbxColours->GetReferenceMode())
// 					{
// 					case FbxGeometryElement::eDirect:
// 						{						
// 							FbxColor colour = fbxColours->GetDirectArray().GetAt(lControlPointIndex);
// 							vertexAttributes |= Vertex::Colour;
// 
// 							vertex.colour.x = (float)colour.mRed;
// 							vertex.colour.y = (float)colour.mGreen;
// 							vertex.colour.z = (float)colour.mBlue;
// 							vertex.colour.w = (float)colour.mAlpha;
// 						}
// 						break;
// 					case FbxGeometryElement::eIndexToDirect:
// 						{
// 							int id = fbxColours->GetIndexArray().GetAt(lControlPointIndex);
// 							FbxColor colour = fbxColours->GetDirectArray().GetAt(id);
// 							vertexAttributes |= Vertex::Colour;
// 
// 							vertex.colour.x = (float)colour.mRed;
// 							vertex.colour.y = (float)colour.mGreen;
// 							vertex.colour.z = (float)colour.mBlue;
// 							vertex.colour.w = (float)colour.mAlpha;
// 						}
// 						break;
// 					default:
// 						break; // other reference modes not shown here!
// 					}
// 					break;
// 
// 				case FbxGeometryElement::eByPolygonVertex:
// 					{
// 						switch (fbxColours->GetReferenceMode())
// 						{
// 						case FbxGeometryElement::eDirect:
// 							{							
// 								FbxColor colour = fbxColours->GetDirectArray().GetAt(vertexId);
// 								vertexAttributes |= Vertex::Colour;
// 
// 								vertex.colour.x = (float)colour.mRed;
// 								vertex.colour.y = (float)colour.mGreen;
// 								vertex.colour.z = (float)colour.mBlue;
// 								vertex.colour.w = (float)colour.mAlpha;
// 							}
// 							break;
// 						case FbxGeometryElement::eIndexToDirect:
// 							{
// 								int id = fbxColours->GetIndexArray().GetAt(vertexId);
// 								FbxColor colour = fbxColours->GetDirectArray().GetAt(id);
// 								vertexAttributes |= Vertex::Colour;
// 
// 								vertex.colour.x = (float)colour.mRed;
// 								vertex.colour.y = (float)colour.mGreen;
// 								vertex.colour.z = (float)colour.mBlue;
// 								vertex.colour.w = (float)colour.mAlpha;
// 							}
// 							break;
// 						default:
// 							break; // other reference modes not shown here!
// 						}
// 					}
// 					break;
// 
// 				case FbxGeometryElement::eByPolygon: // doesn't make much sense for UVs
// 				case FbxGeometryElement::eAllSame:   // doesn't make much sense for UVs
// 				case FbxGeometryElement::eNone:       // doesn't make much sense for UVs
// 					break;
// 				}
// 			}
			if (fbxTexCoord0 != nullptr)
			{
				switch (fbxTexCoord0->GetMappingMode())
				{
				case FbxGeometryElement::eByControlPoint:
					switch (fbxTexCoord0->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						{
							FbxVector2 uv = fbxTexCoord0->GetDirectArray().GetAt(lControlPointIndex);
							vertexAttributes |= Vertex::TexCoord0;

							vertex.texcoord.x = (float)uv[0];
							vertex.texcoord.y = (float)uv[1];
						}
						break;
					case FbxGeometryElement::eIndexToDirect:
						{
							int id = fbxTexCoord0->GetIndexArray().GetAt(lControlPointIndex);
							FbxVector2 uv = fbxTexCoord0->GetDirectArray().GetAt(id);
							vertexAttributes |= Vertex::TexCoord0;

							vertex.texcoord.x = (float)uv[0];
							vertex.texcoord.y = (float)uv[1];
						}
						break;
					default:
						break; // other reference modes not shown here!
					}
					break;

				case FbxGeometryElement::eByPolygonVertex:
					{
						int lTextureUVIndex = fbxMesh->GetTextureUVIndex(i, j);
						switch (fbxTexCoord0->GetReferenceMode())
						{
						case FbxGeometryElement::eDirect:
						case FbxGeometryElement::eIndexToDirect:
							{
								FbxVector2 uv = fbxTexCoord0->GetDirectArray().GetAt(lTextureUVIndex);
								vertexAttributes |= Vertex::TexCoord0;

								vertex.texcoord.x = (float)uv[0];
								vertex.texcoord.y = (float)uv[1];
							}								
							break;
						default:
							break; // other reference modes not shown here!
						}
					}
					break;

				case FbxGeometryElement::eByPolygon: // doesn't make much sense for UVs
				case FbxGeometryElement::eAllSame:   // doesn't make much sense for UVs
				case FbxGeometryElement::eNone:       // doesn't make much sense for UVs
					break;
				}
			}
			if (fbxTexCoord1 != nullptr)
			{
				switch (fbxTexCoord1->GetMappingMode())
				{
				case FbxGeometryElement::eByControlPoint:
					switch (fbxTexCoord1->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						{
							FbxVector2 uv = fbxTexCoord1->GetDirectArray().GetAt(lControlPointIndex);
							vertexAttributes |= Vertex::TexCoord1;

							vertex.texcoord.z = (float)uv[0];
							vertex.texcoord.w = (float)uv[1];
						}
						break;
					case FbxGeometryElement::eIndexToDirect:
						{
							int id = fbxTexCoord1->GetIndexArray().GetAt(lControlPointIndex);
							FbxVector2 uv = fbxTexCoord1->GetDirectArray().GetAt(id);
							vertexAttributes |= Vertex::TexCoord1;

							vertex.texcoord.z = (float)uv[0];
							vertex.texcoord.w = (float)uv[1];
						}
						break;
					default:
						break; // other reference modes not shown here!
					}
					break;

				case FbxGeometryElement::eByPolygonVertex:
					{
						int lTextureUVIndex = fbxMesh->GetTextureUVIndex(i, j);
						switch (fbxTexCoord1->GetReferenceMode())
						{
						case FbxGeometryElement::eDirect:
						case FbxGeometryElement::eIndexToDirect:
							{
								FbxVector2 uv = fbxTexCoord1->GetDirectArray().GetAt(lTextureUVIndex);
								vertexAttributes |= Vertex::TexCoord1;

								vertex.texcoord.z = (float)uv[0];
								vertex.texcoord.w = (float)uv[1];
							}								
							break;
						default:
							break; // other reference modes not shown here!
						}
					}
					break;

				case FbxGeometryElement::eByPolygon: // doesn't make much sense for UVs
				case FbxGeometryElement::eAllSame:   // doesn't make much sense for UVs
				case FbxGeometryElement::eNone:       // doesn't make much sense for UVs
					break;
				}
			}
			if (fbxNormal != nullptr)
			{
				if (fbxNormal->GetMappingMode() == FbxGeometryElement::eByControlPoint)
				{
					switch (fbxNormal->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						{
							FbxVector4 normal = fbxNormal->GetDirectArray().GetAt(lControlPointIndex);
							vertexAttributes |= Vertex::Normal;

							vertex.normal.x = (float)normal[0];
							vertex.normal.y = (float)normal[1];
							vertex.normal.z = (float)normal[2];
							vertex.normal.w = 0;
						}
						break;
					case FbxGeometryElement::eIndexToDirect:
						{
							int id = fbxNormal->GetIndexArray().GetAt(lControlPointIndex);
							FbxVector4 normal = fbxNormal->GetDirectArray().GetAt(id);
							vertexAttributes |= Vertex::Normal;

							vertex.normal.x = (float)normal[0];
							vertex.normal.y = (float)normal[1];
							vertex.normal.z = (float)normal[2];
							vertex.normal.w = 0;
						}
						break;
					default:
						break; // other reference modes not shown here!
					}
				}
				else if(fbxNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					switch (fbxNormal->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						{
							FbxVector4 normal = fbxNormal->GetDirectArray().GetAt(vertexId);
							vertexAttributes |= Vertex::Normal;

							vertex.normal.x = (float)normal[0];
							vertex.normal.y = (float)normal[1];
							vertex.normal.z = (float)normal[2];
							vertex.normal.w = 0;
						}
						break;
					case FbxGeometryElement::eIndexToDirect:
						{
							int id = fbxNormal->GetIndexArray().GetAt(vertexId);
							FbxVector4 normal = fbxNormal->GetDirectArray().GetAt(id);
							vertexAttributes |= Vertex::Normal;

							vertex.normal.x = (float)normal[0];
							vertex.normal.y = (float)normal[1];
							vertex.normal.z = (float)normal[2];
							vertex.normal.w = 0;
						}
						break;
					default:
						break; // other reference modes not shown here!
					}
				}
			}
			if (fbxTangent != nullptr)
			{
				if (fbxTangent->GetMappingMode() == FbxGeometryElement::eByControlPoint)
				{
					switch (fbxTangent->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						{
							FbxVector4 normal = fbxTangent->GetDirectArray().GetAt(lControlPointIndex);
							vertexAttributes |= Vertex::Tangent;

							vertex.tangent.x = (float)normal[0];
							vertex.tangent.y = (float)normal[1];
							vertex.tangent.z = (float)normal[2];
							vertex.tangent.w = 0;
						}
						break;
					case FbxGeometryElement::eIndexToDirect:
						{
							int id = fbxTangent->GetIndexArray().GetAt(lControlPointIndex);
							FbxVector4 normal = fbxTangent->GetDirectArray().GetAt(id);
							vertexAttributes |= Vertex::Tangent;

							vertex.tangent.x = (float)normal[0];
							vertex.tangent.y = (float)normal[1];
							vertex.tangent.z = (float)normal[2];
							vertex.tangent.w = 0;
						}
						break;
					default:
						break; // other reference modes not shown here!
					}
				}
				else if(fbxTangent->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					switch (fbxTangent->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						{
							FbxVector4 normal = fbxTangent->GetDirectArray().GetAt(vertexId);
							vertexAttributes |= Vertex::Tangent;

							vertex.tangent.x = (float)normal[0];
							vertex.tangent.y = (float)normal[1];
							vertex.tangent.z = (float)normal[2];
							vertex.tangent.w = 0;
						}
						break;
					case FbxGeometryElement::eIndexToDirect:
						{
							int id = fbxTangent->GetIndexArray().GetAt(vertexId);
							FbxVector4 normal = fbxTangent->GetDirectArray().GetAt(id);
							vertexAttributes |= Vertex::Tangent;

							vertex.tangent.x = (float)normal[0];
							vertex.tangent.y = (float)normal[1];
							vertex.tangent.z = (float)normal[2];
							vertex.tangent.w = 0;
						}
						break;
					default:
						break; // other reference modes not shown here!
					}
				}
			}
			if (fbxBiNormal != nullptr)
			{
				if (fbxBiNormal->GetMappingMode() == FbxGeometryElement::eByControlPoint)
				{
					switch (fbxBiNormal->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						{
							FbxVector4 normal = fbxBiNormal->GetDirectArray().GetAt(lControlPointIndex);
							vertexAttributes |= Vertex::BiNormal;

							vertex.binormal.x = (float)normal[0];
							vertex.binormal.y = (float)normal[1];
							vertex.binormal.z = (float)normal[2];
							vertex.binormal.w = 0;
						}
						break;
					case FbxGeometryElement::eIndexToDirect:
						{
							int id = fbxBiNormal->GetIndexArray().GetAt(lControlPointIndex);
							FbxVector4 normal = fbxBiNormal->GetDirectArray().GetAt(id);
							vertexAttributes |= Vertex::BiNormal;

							vertex.binormal.x = (float)normal[0];
							vertex.binormal.y = (float)normal[1];
							vertex.binormal.z = (float)normal[2];
							vertex.binormal.w = 0;
						}
						break;
					default:
						break; // other reference modes not shown here!
					}
				}
				else if(fbxBiNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					switch (fbxBiNormal->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						{
							FbxVector4 normal = fbxBiNormal->GetDirectArray().GetAt(vertexId);
							vertexAttributes |= Vertex::BiNormal;

							vertex.binormal.x = (float)normal[0];
							vertex.binormal.y = (float)normal[1];
							vertex.binormal.z = (float)normal[2];
							vertex.binormal.w = 0;
						}
						break;
					case FbxGeometryElement::eIndexToDirect:
						{
							int id = fbxBiNormal->GetIndexArray().GetAt(vertexId);
							FbxVector4 normal = fbxBiNormal->GetDirectArray().GetAt(id);
							vertexAttributes |= Vertex::BiNormal;

							vertex.binormal.x = (float)normal[0];
							vertex.binormal.y = (float)normal[1];
							vertex.binormal.z = (float)normal[2];
							vertex.binormal.w = 0;
						}
						break;
					default:
						break; // other reference modes not shown here!
					}
				}
			}

			// add to 
			if (usePolygon)
			{
				auto iter = vertexIndexMap.find( vertex );
				if (iter == vertexIndexMap.end())
				{
					vertexBuffer.push_back(vertex);
					vertexIndexMap[ vertex ] = nextIndex++;
					vertexIndex[j] = nextIndex - 1;
				}
				else
				{
					vertexIndex[j] = iter->second;
				}
			}
			vertexId++;
		}

		// add triangle indices
		if (usePolygon)
		{
			indexBuffer.push_back(vertexIndex[0]);
			indexBuffer.push_back(vertexIndex[1]);
			indexBuffer.push_back(vertexIndex[2]);

			// handle quads
			if (lPolygonSize == 4)
			{
				indexBuffer.push_back(vertexIndex[0]);
				indexBuffer.push_back(vertexIndex[2]);
				indexBuffer.push_back(vertexIndex[3]);
			}
		}
	}

	vertexAttributes |= Vertex::Tangent|Vertex::BiNormal;

	// reclalc because I don't trust maya now!
	if ((vertexAttributes & (Vertex::Tangent|Vertex::BiNormal)) != 0)
		calculateTangentsBinormals(vertexBuffer,indexBuffer);

	Geometry* geometry = new Geometry(Geometry::Triangles, vertexBuffer.size(), vertexBuffer.data(), vertexAttributes, indexBuffer.size(), indexBuffer.data(), a_retainMeshData);
	a_mesh->attachGeometry( geometry );
	a_mesh->setMaterial( fbxExtractMaterial(fbxMesh,a_material,a_path) );
}

//////////////////////////////////////////////////////////////////////////
Material* ResourceLibrary::fbxExtractMaterial(void* a_mesh, int a_material, const char* a_path)
{
	FbxGeometry* pGeometry = (FbxGeometry*)a_mesh;

	// get material count
	int lMaterialCount = 0;
	FbxNode* lNode = nullptr;
	if(pGeometry)
	{
		lNode = pGeometry->GetNode();
		if (lNode)
			lMaterialCount = lNode->GetMaterialCount();    
	}

	if (lMaterialCount > 0)
	{
		FbxSurfaceMaterial *lMaterial = lNode->GetMaterial(a_material);

		unsigned int id = SynStringHash(lMaterial->GetName());

		auto oIter = m_materials.find( id );
		if (oIter != m_materials.end())
		{
			return oIter->second;
		}
		else
		{			
			Material* material = createMaterial(lMaterial->GetName());

			bool phong = false;

			// get the implementation to see if it's a hardware shader.
			const FbxImplementation* lImplementation = GetImplementation(lMaterial, FBXSDK_IMPLEMENTATION_HLSL);
			if (lImplementation == nullptr)
			{
				lImplementation = GetImplementation(lMaterial, FBXSDK_IMPLEMENTATION_CGFX);
			}
			if (lImplementation != nullptr)
			{
				FbxBindingTable const* lRootTable = lImplementation->GetRootTable();
				FbxString lFileName = lRootTable->DescAbsoluteURL.Get();
				FbxString lTechniqueName = lRootTable->DescTAG.Get(); 

				logError("Unsupported hardware shader material!\n");
				logError("File: %s\n",lFileName.Buffer());
				logError("Technique: %s\n\n",lTechniqueName.Buffer());
			}
			else if (lMaterial->GetClassId().Is(FbxSurfacePhong::ClassId))
			{
				// We found Phong!
				FbxSurfacePhong* pPhong = (FbxSurfacePhong*)lMaterial;
				phong = true;

				material->setAmbient( glm::vec4((float)pPhong->Ambient.Get()[0], (float)pPhong->Ambient.Get()[1], (float)pPhong->Ambient.Get()[2], (float)pPhong->AmbientFactor.Get()) );
				material->setDiffuse( glm::vec4((float)pPhong->Diffuse.Get()[0], (float)pPhong->Diffuse.Get()[1], (float)pPhong->Diffuse.Get()[2], (float)pPhong->TransparencyFactor.Get()) );
				material->setSpecular( glm::vec4((float)pPhong->Specular.Get()[0], (float)pPhong->Specular.Get()[1], (float)pPhong->Specular.Get()[2], (float)pPhong->Shininess.Get()) );
				material->setEmissive( glm::vec4((float)pPhong->Emissive.Get()[0], (float)pPhong->Emissive.Get()[1], (float)pPhong->Emissive.Get()[2], (float)pPhong->EmissiveFactor.Get()) );
			}
			else if(lMaterial->GetClassId().Is(FbxSurfaceLambert::ClassId) )
			{
				// We found a Lambert!
				FbxSurfaceLambert* pLambert = (FbxSurfaceLambert*)lMaterial;

				material->setAmbient( glm::vec4((float)pLambert->Ambient.Get()[0], (float)pLambert->Ambient.Get()[1], (float)pLambert->Ambient.Get()[2], (float)pLambert->AmbientFactor.Get()) );
				material->setDiffuse( glm::vec4((float)pLambert->Diffuse.Get()[0], (float)pLambert->Diffuse.Get()[1], (float)pLambert->Diffuse.Get()[2], (float)pLambert->TransparencyFactor.Get()) );
				material->setSpecular( glm::vec4(0,0,0,0) );
				material->setEmissive( glm::vec4((float)pLambert->Emissive.Get()[0], (float)pLambert->Emissive.Get()[1], (float)pLambert->Emissive.Get()[2], (float)pLambert->EmissiveFactor.Get()) );
			}
			else
			{
				logWarning("Unknown type of Material!\n");
			}

			unsigned int auiTextureLookup[] =
			{
				FbxLayerElement::eTextureDiffuse - FbxLayerElement::sTypeTextureStartIndex,
				FbxLayerElement::eTextureEmissive - FbxLayerElement::sTypeTextureStartIndex,
				FbxLayerElement::eTextureAmbient - FbxLayerElement::sTypeTextureStartIndex,
				FbxLayerElement::eTextureSpecular - FbxLayerElement::sTypeTextureStartIndex,
				FbxLayerElement::eTextureShininess - FbxLayerElement::sTypeTextureStartIndex,
				FbxLayerElement::eTextureNormalMap - FbxLayerElement::sTypeTextureStartIndex,
				FbxLayerElement::eTextureTransparency - FbxLayerElement::sTypeTextureStartIndex,
				FbxLayerElement::eTextureReflection - FbxLayerElement::sTypeTextureStartIndex,
				FbxLayerElement::eTextureDisplacement - FbxLayerElement::sTypeTextureStartIndex,
			};
			
			phong = true;
			for ( unsigned int i = 0 ; i <= Material::Displacement ; ++i )
			{
				FbxProperty pProperty = lMaterial->FindProperty(FbxLayerElement::sTextureChannelNames[auiTextureLookup[i]]);						
				if ( pProperty.IsValid() &&
					pProperty.GetSrcObjectCount<FbxTexture>() > 0)
				{
					FbxFileTexture *lFileTexture = FbxCast<FbxFileTexture>(pProperty.GetSrcObject<FbxTexture>(0));
					if (lFileTexture)
					{
						const char* szLastForward = strrchr(lFileTexture->GetFileName(),'/');
						const char* szLastBackward = strrchr(lFileTexture->GetFileName(),'\\');
						const char* szFilename = lFileTexture->GetFileName();

						if (szLastForward != nullptr && szLastForward > szLastBackward)
							szFilename = szLastForward + 1;
						else if (szLastBackward != nullptr)
							szFilename = szLastBackward + 1;

						std::string texturePath = a_path;
						texturePath += szFilename;

						material->setTexture((Material::TextureChannel)i, loadTexture(texturePath.c_str()));

						if (i == Material::Diffuse)
							material->setShader( getShader("compositeTextured") );
					}   
				}
			}

			if (material->getTexture(Material::Specular) == nullptr)
				material->setTexture(Material::Specular, getTexture("white"));

			if (material->getShader() == nullptr)
				material->setShader( getShader("composite") );

			return material;
		}
	}
	return nullptr;
}

//////////////////////////////////////////////////////////////////////////
void ResourceLibrary::calculateTangentsBinormals(std::vector<Vertex>& a_vertices, const std::vector<unsigned int>& a_indices)
{
	unsigned int vertexCount = a_vertices.size();
	glm::vec4* tan1 = new glm::vec4[vertexCount * 2];
	glm::vec4* tan2 = tan1 + vertexCount;
	memset(tan1, 0, vertexCount * sizeof(glm::vec4) * 2);

	unsigned int indexCount = a_indices.size();
	for (unsigned int a = 0; a < indexCount; a += 3)
	{
		long i1 = a_indices[a];
		long i2 = a_indices[a + 1];
		long i3 = a_indices[a + 2];

		const glm::vec4& v1 = a_vertices[i1].position;
		const glm::vec4& v2 = a_vertices[i2].position;
		const glm::vec4& v3 = a_vertices[i3].position;

		const glm::vec2& w1 = a_vertices[i1].texcoord.st;
		const glm::vec2& w2 = a_vertices[i2].texcoord.st;
		const glm::vec2& w3 = a_vertices[i3].texcoord.st;

		float x1 = v2.x - v1.x;
		float x2 = v3.x - v1.x;
		float y1 = v2.y - v1.y;
		float y2 = v3.y - v1.y;
		float z1 = v2.z - v1.z;
		float z2 = v3.z - v1.z;

		float s1 = w2.x - w1.x;
		float s2 = w3.x - w1.x;
		float t1 = w2.y - w1.y;
		float t2 = w3.y - w1.y;

		float r = 1.0F / (s1 * t2 - s2 * t1);
		glm::vec4 sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r,
			(t2 * z1 - t1 * z2) * r,0);
		glm::vec4 tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r,
			(s1 * z2 - s2 * z1) * r,0);

		tan1[i1] += sdir;
		tan1[i2] += sdir;
		tan1[i3] += sdir;

		tan2[i1] += tdir;
		tan2[i2] += tdir;
		tan2[i3] += tdir;
	}

	for (unsigned int a = 0; a < vertexCount; a++)
	{
		const glm::vec3& n = a_vertices[a].normal.xyz;
		const glm::vec3& t = tan1[a].xyz;

		// Gram-Schmidt orthogonalize
		a_vertices[a].tangent = glm::vec4( glm::normalize(t - n * glm::dot(n, t)), 0 );

		// Calculate handedness
		a_vertices[a].tangent.w = (glm::dot(glm::cross(n.xyz(), t.xyz()), tan2[a].xyz()) < 0.0F) ? 1.0F : -1.0F;

		// calculate binormal
		a_vertices[a].binormal = glm::vec4( glm::cross(a_vertices[a].normal.xyz(),a_vertices[a].tangent.xyz()) * a_vertices[a].tangent.w, 0 );

		a_vertices[a].tangent.w = 0;
	}

	delete[] tan1;
}

} // namespace syn