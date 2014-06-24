#pragma once

#include <RefObject.h>
#include <glm/glm.hpp>
#include <map>
#include <vector>

namespace syn
{

DeclareSmartPtr(Shader);

class SYNTAX_API Shader : public RefObject
{
	friend class UniformBase;
	friend class ResourceLibrary;

public:

	Shader(const char* a_name);
	virtual ~Shader();

	const char*		getName() const	{	return m_name;	}

	enum ShaderType : unsigned int
	{
		Vertex,
		Control,
		Evaluation,
		Geometry,
		Fragment,

		ShaderType_Count,
	};

	void	reload();

	bool	compileShaderFromFile(ShaderType a_type, const char* a_filename, bool a_storeFile = true);
	bool	compileShaderFromString(ShaderType a_type, const char* a_shader);

	bool	linkProgram();

	bool	buildProgram(const char** a_shaderFiles);

	void	bind();
	void	unBind();

	bool	isBound() const;

	unsigned int	getProgram() const;

	// Uniforms
	class UniformBase
	{
		friend class Shader;

	public:

		const char*		getName() const		{	return m_name;		}
		Shader*			getShader() const	{	return m_shader;	}
		int				getLocation() const	{	return m_location;	}
		unsigned int	getSize() const		{	return m_size;		}
		unsigned int	getType() const		{	return m_type;		}

	private:

		UniformBase(const char* a_name, unsigned int a_location, unsigned int a_size, unsigned int a_type, Shader* a_shader) 
			: m_dirty(true), m_name(new char[strlen(a_name)+1]), m_location(a_location), m_size(a_size), m_type(a_type), m_shader(a_shader)
		{
			strcpy(m_name, a_name); 
			registerDirty();
		}
		virtual ~UniformBase() { delete[] m_name;	}

		void			bind(float f);
		void			bind(int i);
		void			bind(unsigned int u);
		void			bind(const glm::vec2& v);
		void			bind(const glm::vec3& v);
		void			bind(const glm::vec4& v);
		void			bind(const glm::ivec2& iv);
		void			bind(const glm::ivec3& iv);
		void			bind(const glm::ivec4& iv);
		void			bind(const glm::uvec2& iv);
		void			bind(const glm::uvec3& iv);
		void			bind(const glm::uvec4& iv);
		void			bind(const glm::mat2& m);
		void			bind(const glm::mat3& m);
		void			bind(const glm::mat4& m);

		void			bind(unsigned int a_count, float* f);
		void			bind(unsigned int a_count, int* i);
		void			bind(unsigned int a_count, unsigned int* u);
		void			bind(unsigned int a_count, const glm::vec2* v);
		void			bind(unsigned int a_count, const glm::vec3* v);
		void			bind(unsigned int a_count, const glm::vec4* v);
		void			bind(unsigned int a_count, const glm::ivec2* iv);
		void			bind(unsigned int a_count, const glm::ivec3* iv);
		void			bind(unsigned int a_count, const glm::ivec4* iv);
		void			bind(unsigned int a_count, const glm::uvec2* iv);
		void			bind(unsigned int a_count, const glm::uvec3* iv);
		void			bind(unsigned int a_count, const glm::uvec4* iv);
		void			bind(unsigned int a_count, const glm::mat2* m);
		void			bind(unsigned int a_count, const glm::mat3* m);
		void			bind(unsigned int a_count, const glm::mat4* m);

		void			registerDirty();
		virtual void	clean() = 0;

		bool			m_dirty;
		char*			m_name;
		int				m_location;
		unsigned int	m_type;
		unsigned int	m_size;
		Shader*			m_shader;
	};

	template <typename T>
	class Uniform : public UniformBase
	{
		friend class Shader;

	public:

		void	set(const T& a_value);
		void	set(unsigned int a_count, const T* a_values);

	private:

		virtual void	clean();

		Uniform(const char* a_name, unsigned int a_location, unsigned int a_size, unsigned int a_type, Shader* a_owner) 
			: UniformBase(a_name, a_location, a_size, a_type, a_owner)
		{
			m_value = new T[a_size];
		}
		virtual ~Uniform() 
		{
			delete[] m_value;
		}
		
		T*				m_value;
	};

	UniformBase*	getUniform(const char* a_uniform);

	// shared uniforms
	class SharedUniformBase
	{
	public:

		SharedUniformBase(UniformBase* a_uniform) : m_uniform(a_uniform) {}
		virtual ~SharedUniformBase() {}

		virtual void set() = 0;

	protected:

		UniformBase*	m_uniform;
	};

	class SharedUniformFactoryBase
	{
	public:

		virtual ~SharedUniformFactoryBase() {}
		virtual SharedUniformBase* create(UniformBase* a_uniform) = 0;
	};

	template <typename T>
	class SharedUniformFactory : public SharedUniformFactoryBase
	{
	public:

		virtual ~SharedUniformFactory() {}
		virtual SharedUniformBase* create(UniformBase* a_uniform)
		{
			return new T(a_uniform);
		}
	};

	static void		registerSharedUniform( const char* a_name, SharedUniformFactoryBase* a_factory );

	static void		initialiseSharedUniforms();
	static void		destroySharedUniforms();

	// not the preferred way to set uniforms
	void			setUniform(const char* a_uniform, int i);
	void			setUniform(const char* a_uniform, float f);
	void			setUniform(const char* a_uniform, const glm::ivec2& iv);
	void			setUniform(const char* a_uniform, const glm::ivec4& iv);
	void			setUniform(const char* a_uniform, const glm::vec2& iv);
	void			setUniform(const char* a_uniform, const glm::vec4& v);
	void			setUniform(const char* a_uniform, const glm::mat4& m);

	void			setUniform(const char* a_uniform, unsigned int a_count, int* i);
	void			setUniform(const char* a_uniform, unsigned int a_count, float* f);
	void			setUniform(const char* a_uniform, unsigned int a_count, const glm::ivec4* iv);
	void			setUniform(const char* a_uniform, unsigned int a_count, const glm::vec2* v);
	void			setUniform(const char* a_uniform, unsigned int a_count, const glm::vec4* v);
	void			setUniform(const char* a_uniform, unsigned int a_count, const glm::mat4* m);

	static Shader*		getBoundShader();

	static Shader*		getInvalidShader();

	static const char*	getLastError();

private:

	void			extractUniforms();
	void			registerDirty(UniformBase* a_uniform);

	// adds #include support
	bool			readShaderSource( const char* a_filename, std::string& a_source );

	char*			m_name;

	unsigned int	m_program;
	unsigned int	m_shaders[ShaderType_Count];
	char*			m_shaderFiles[ShaderType_Count];

	std::map< unsigned int, UniformBase* >	m_uniforms;
	std::vector< UniformBase* >				m_dirtyUniforms;

	std::vector< SharedUniformBase* >							m_sharedUniforms;
	static std::map< unsigned int, SharedUniformFactoryBase* >	sm_sharedUniformFactories;

	static char		sm_errorLog[2048];
	static Shader*	sm_boundShader;
	static Shader*	sm_invalidShader;
};

inline bool Shader::isBound() const
{
	return this == sm_boundShader;
}

inline Shader* Shader::getBoundShader()
{
	return sm_boundShader;
}

inline Shader* Shader::getInvalidShader()
{
	return sm_invalidShader;
}

inline const char* Shader::getLastError()
{
	return sm_errorLog;
}

inline void Shader::registerDirty(UniformBase* a_uniform)
{
	m_dirtyUniforms.push_back(a_uniform);
}

inline void Shader::UniformBase::registerDirty()
{
	m_dirty = true;
	m_shader->registerDirty(this);
}

template <typename T>
inline void Shader::Uniform<T>::set(const T& a_value)
{
	if (m_dirty == false &&
		m_value[0] != a_value)
	{
		registerDirty();
	}
	m_value[0] = a_value;
}

template <typename T>
inline void Shader::Uniform<T>::set(unsigned int a_count, const T* a_values)
{
	if (m_dirty == false &&
		memcmp(m_value,a_values,a_count * sizeof(T)) > 0)
	{
		registerDirty();
	}
	memcpy(m_value,a_values,a_count * sizeof(T));
}

template <typename T>
inline void Shader::Uniform<T>::clean()
{
	if (m_size > 1)
		bind(m_size,m_value);
	else
		bind(m_value[0]);
	m_dirty = false;
}

inline unsigned int Shader::getProgram() const
{
	return m_program;
}

} // namespace syn