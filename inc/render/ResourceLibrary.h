#pragma once

#include <render/Texture.h>
#include <render/Shader.h>
#include <render/Material.h>
#include <render/Geometry.h>
#include <render/FrameBuffer.h>
#include <scene/SceneNode.h>

namespace syn
{

class Mesh;

class SYNTAX_API ResourceLibrary
{
public:

	static ResourceLibrary*	create();
	static ResourceLibrary*	getSingleton();
	static void				destroy();

	// Textures
	// TODO: change to support sRGB as primary format, and retain access to texture pixel data if requested
	Texture*		createTexture(const char* a_name, unsigned int a_width, unsigned int a_height, 
								  unsigned int a_format, unsigned int a_channels, unsigned int a_dataType,
								  unsigned int a_type = Texture::Texture2D, const void* a_data = nullptr, bool a_ownData = false);
	Texture*		loadTexture(const char* a_filename, unsigned int a_type = Texture::Texture2D);
	CubeTexture*	loadTextureCube(const char* a_name, const char** a_filenames);
	Texture*		reloadTexture(const char* a_name);
	Texture*		reloadTexture(Texture* a_texture);
	CubeTexture*	reloadTextureCube(const char* a_name, const char** a_filenames);
	unsigned int	getTextureCount() const;
	Texture*		getTextureByIndex(unsigned int a_index);
	Texture*		getTexture(const char* a_name);
	void			releaseTexture(Texture* a_texture);
	void			releaseAllTextures();

	// Materials
	Material*		createMaterial(const char* a_name);
	unsigned int	getMaterialCount() const;
	Material*		getMaterialByIndex(unsigned int a_index);
	Material*		getMaterial(const char* a_name);
	void			releaseMaterial(Material* a_material);
	void			releaseAllMaterials();

	// Shaders
	// TODO: simplify and allow for compute shaders
	Shader*			createShader(const char* a_name);
	Shader*			createShader(const char* a_name, const char** a_shaderFiles);
	Shader*			getShader(const char* a_name);
	void			releaseShader(Shader* a_shader);
	void			releaseAllShaders();
	void			reloadShaders();
	
	// Scenes
	// TODO: change to use a plugin system to allow loading of different file extensions
	SceneNode*		loadFBXScene(const char* a_filename, glm::vec4* a_ambientLight = nullptr, bool a_retainMeshData = false);
	SceneNode*		getScene(const char* a_name);
	void			releaseScene(SceneNode* a_node);
	void			releaseAllScenes();
	
	// Frame Buffers
	FrameBuffer*	createFrameBuffer(const char* a_name, 
									  unsigned int a_width, unsigned int a_height, 
									  unsigned int a_targetCount = 1, bool a_depth = true, 
									  unsigned int* a_bufferFormat = nullptr, unsigned int* a_channels = nullptr, unsigned int* a_bufferType = nullptr);
	FrameBuffer*	getFrameBuffer(const char* a_name);
	void			releaseFrameBuffer(FrameBuffer* a_frameBuffer);
	void			releaseAllFrameBuffers();

private:
	

	void			fbxExtractObject(SceneNode* a_parent, void* a_object, void* a_fbxScene, const char* a_path, bool a_retainMeshData);
	void			fbxExtractMesh(Mesh* a_mesh, void* a_object, int a_material, const char* a_path, bool a_retainMeshData);
	Material*		fbxExtractMaterial(void* a_mesh, int a_material, const char* a_path);

	void			calculateTangentsBinormals(std::vector<Vertex>& a_vertices, const std::vector<unsigned int>& a_indices);

private:

	ResourceLibrary();
	~ResourceLibrary();

	void	loadDefaults();

	std::map< unsigned int, TexturePtr >		m_textures;
	std::map< unsigned int, ShaderPtr >			m_shaders;
	std::map< unsigned int, MaterialPtr >		m_materials;
	std::map< unsigned int, SceneNodePtr >		m_scenes;
	std::map< unsigned int, FrameBufferPtr >	m_frameBuffers;

	static ResourceLibrary*	sm_singleton;
};

inline ResourceLibrary* ResourceLibrary::getSingleton()
{
	return sm_singleton;
}

inline unsigned int ResourceLibrary::getMaterialCount() const
{
	return m_materials.size();
}

} // namespace syn