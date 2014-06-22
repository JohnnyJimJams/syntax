#include <render/Shader.h>
#include <GL/glew.h>
#include <glm/ext.hpp>
#include <utilities/HashFunctions.h>
#include <utilities/Log.h>
#include <render/SharedShaderUniforms.h>
#include <render/Material.h>
#include <iostream>

namespace syn
{

char Shader::sm_errorLog[2048] = {};
Shader* Shader::sm_boundShader = nullptr;

std::map<unsigned int, Shader::SharedUniformFactoryBase* > Shader::sm_sharedUniformFactories;

Shader::Shader(const char* a_name)
	: m_name(new char[strlen(a_name)+1]),
	m_program(0)
{
	strcpy(m_name,a_name);

	m_shaders[Vertex] = 0;
	m_shaders[Control] = 0;
	m_shaders[Evaluation] = 0;
	m_shaders[Geometry] = 0;
	m_shaders[Fragment] = 0;

	m_shaderFiles[Vertex] = 0;
	m_shaderFiles[Control] = 0;
	m_shaderFiles[Evaluation] = 0;
	m_shaderFiles[Geometry] = 0;
	m_shaderFiles[Fragment] = 0;
}

Shader::~Shader()
{
	unBind();

	delete[] m_name;
	for ( auto u : m_uniforms)
		delete u.second;

	delete[] m_shaderFiles[Vertex];
	delete[] m_shaderFiles[Control];
	delete[] m_shaderFiles[Evaluation];
	delete[] m_shaderFiles[Geometry];
	delete[] m_shaderFiles[Fragment];

	glDeleteShader(m_shaders[Vertex]);
	glDeleteShader(m_shaders[Control]);
	glDeleteShader(m_shaders[Evaluation]);
	glDeleteShader(m_shaders[Geometry]);
	glDeleteShader(m_shaders[Fragment]);
	glDeleteProgram(m_program);
}

bool Shader::compileShaderFromFile(ShaderType a_type, const char* a_filename, bool a_storeFile /* = true */)
{
	if (a_filename == nullptr)
		return false;

	std::string source = "";
	if (readShaderSource(a_filename, source) == false)
	{
		return false;
	}

	glDeleteShader(m_shaders[a_type]);
	m_shaders[a_type] = 0;

	switch (a_type)
	{
	case Vertex:		m_shaders[a_type] = glCreateShader(GL_VERTEX_SHADER);	break;
	case Control:		m_shaders[a_type] = glCreateShader(GL_TESS_CONTROL_SHADER);	break;
	case Evaluation:	m_shaders[a_type] = glCreateShader(GL_TESS_EVALUATION_SHADER);	break;
	case Geometry:		m_shaders[a_type] = glCreateShader(GL_GEOMETRY_SHADER);	break;
	case Fragment:		m_shaders[a_type] = glCreateShader(GL_FRAGMENT_SHADER);	break;
	};

	const char* sourcePtr[1];
	sourcePtr[0] = source.c_str();

	// compile vertex shader and log errors
	glShaderSource(m_shaders[a_type], 1, sourcePtr, 0);
	glCompileShader(m_shaders[a_type]);

	int iSuccess = GL_FALSE;
	glGetShaderiv(m_shaders[a_type], GL_COMPILE_STATUS, &iSuccess);
	glGetShaderInfoLog(m_shaders[a_type], sizeof(sm_errorLog), 0, sm_errorLog);
	
	return iSuccess != GL_FALSE;
}

bool Shader::compileShaderFromString(ShaderType a_type, const char* a_shader)
{
	glDeleteShader(m_shaders[a_type]);
	m_shaders[a_type] = 0;

	switch (a_type)
	{
	case Vertex:		m_shaders[a_type] = glCreateShader(GL_VERTEX_SHADER);	break;
	case Control:		m_shaders[a_type] = glCreateShader(GL_TESS_CONTROL_SHADER);	break;
	case Evaluation:	m_shaders[a_type] = glCreateShader(GL_TESS_EVALUATION_SHADER);	break;
	case Geometry:		m_shaders[a_type] = glCreateShader(GL_GEOMETRY_SHADER);	break;
	case Fragment:		m_shaders[a_type] = glCreateShader(GL_FRAGMENT_SHADER);	break;
	};

	// compile vertex shader and log errors
	glShaderSource(m_shaders[a_type], 1, (const char**)&a_shader, 0);
	glCompileShader(m_shaders[a_type]);

	int iSuccess = GL_FALSE;
	glGetShaderiv(m_shaders[a_type], GL_COMPILE_STATUS, &iSuccess);
	glGetShaderInfoLog(m_shaders[a_type], sizeof(sm_errorLog), 0, sm_errorLog);

	return iSuccess != GL_FALSE;
}

bool Shader::linkProgram()
{
	glDeleteProgram(m_program);

	m_program = glCreateProgram();
	if (m_shaders[Vertex] != 0)
		glAttachShader(m_program, m_shaders[Vertex]);
	if (m_shaders[Control] != 0)
		glAttachShader(m_program, m_shaders[Control]);
	if (m_shaders[Evaluation] != 0)
		glAttachShader(m_program, m_shaders[Evaluation]);
	if (m_shaders[Geometry] != 0)
		glAttachShader(m_program, m_shaders[Geometry]);
	if (m_shaders[Fragment] != 0)
		glAttachShader(m_program, m_shaders[Fragment]);
	glLinkProgram(m_program);

	int success = GL_FALSE;
	glGetProgramiv(m_program, GL_LINK_STATUS, &success);

	if (success == GL_TRUE)
	{
		extractUniforms();
		return true;
	}
	else
	{
		glGetProgramInfoLog(m_program, sizeof(sm_errorLog), 0, sm_errorLog);
		return false;
	}
}

bool Shader::buildProgram(const char** a_shaderFiles)
{
	compileShaderFromFile(Shader::Vertex, a_shaderFiles[0]);
	compileShaderFromFile(Shader::Control, a_shaderFiles[1]);
	compileShaderFromFile(Shader::Evaluation, a_shaderFiles[2]);
	compileShaderFromFile(Shader::Geometry, a_shaderFiles[3]);
	compileShaderFromFile(Shader::Fragment, a_shaderFiles[4]);
	if (linkProgram() == false)
	{
		logError("Shader Link Error for shader [%s]:\n", m_name);
		logError(getLastError());
		log("\n");
		return false;
	}

	return true;
}

void Shader::extractUniforms()
{
	int strLength = 0;
	glGetProgramiv(m_program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &strLength);
	if (strLength > 0)
	{
		char* buffer = new char[strLength + 1];

		int uniformCount = 0;
		glGetProgramiv(m_program, GL_ACTIVE_UNIFORMS, &uniformCount);

		for ( int i = 0 ; i < uniformCount; ++i )
		{
			int uniformSize = 0;
			unsigned int uniformType = 0;
			glGetActiveUniform(m_program, i, strLength, nullptr, &uniformSize, &uniformType, buffer);

			if (uniformSize > 1)
			{
				// remove the array tags?
				char* pos = strchr(buffer,'[');
				*pos = 0;
				i = glGetUniformLocation(m_program, buffer);
			}

			unsigned int id = SynStringHash( buffer );
			switch (uniformType)
			{
			case GL_FLOAT:
				m_uniforms[ id ] = new Uniform<float>(buffer,i,uniformSize,uniformType,this);
				break;
			case GL_FLOAT_VEC2:
				m_uniforms[ id ] = new Uniform<glm::vec2>(buffer,i,uniformSize,uniformType,this);
				break;
			case GL_FLOAT_VEC3:
				m_uniforms[ id ] = new Uniform<glm::vec3>(buffer,i,uniformSize,uniformType,this);
				break;
			case GL_FLOAT_VEC4:
				m_uniforms[ id ] = new Uniform<glm::vec4>(buffer,i,uniformSize,uniformType,this);
				break;
			case GL_INT:
				m_uniforms[ id ] = new Uniform<int>(buffer,i,uniformSize,uniformType,this);
				break;
			case GL_INT_VEC2:
				m_uniforms[ id ] = new Uniform<glm::ivec2>(buffer,i,uniformSize,uniformType,this);
				break;
			case GL_INT_VEC3:
				m_uniforms[ id ] = new Uniform<glm::ivec3>(buffer,i,uniformSize,uniformType,this);
				break;
			case GL_INT_VEC4:
				m_uniforms[ id ] = new Uniform<glm::ivec4>(buffer,i,uniformSize,uniformType,this);
				break;
			case GL_UNSIGNED_INT:
				m_uniforms[ id ] = new Uniform<unsigned int>(buffer,i,uniformSize,uniformType,this);
				break;
			case GL_UNSIGNED_INT_VEC2:
				m_uniforms[ id ] = new Uniform<glm::uvec2>(buffer,i,uniformSize,uniformType,this);
				break;
			case GL_UNSIGNED_INT_VEC3:
				m_uniforms[ id ] = new Uniform<glm::uvec3>(buffer,i,uniformSize,uniformType,this);
				break;
			case GL_UNSIGNED_INT_VEC4:
				m_uniforms[ id ] = new Uniform<glm::uvec4>(buffer,i,uniformSize,uniformType,this);
				break;
			case GL_FLOAT_MAT2:
				m_uniforms[ id ] = new Uniform<glm::mat2>(buffer,i,uniformSize,uniformType,this);
				break;
			case GL_FLOAT_MAT3:
				m_uniforms[ id ] = new Uniform<glm::mat3>(buffer,i,uniformSize,uniformType,this);
				break;
			case GL_FLOAT_MAT4:
				m_uniforms[ id ] = new Uniform<glm::mat4>(buffer,i,uniformSize,uniformType,this);
				break;
			case GL_SAMPLER_1D:
			case GL_SAMPLER_2D:
			case GL_SAMPLER_3D:
			case GL_SAMPLER_CUBE:
				m_uniforms[ id ] = new Uniform<int>(buffer,i,uniformSize,uniformType,this);
				break;
			default:	logWarning("Unknown uniform type for uniform: %s\n", buffer);	break;
			};

			auto global = sm_sharedUniformFactories.find( id );
			if (global != sm_sharedUniformFactories.end())
			{
				m_sharedUniforms.push_back( global->second->create( m_uniforms[id] ) );
			}
		}

		delete[] buffer;
	}
}

void Shader::reload()
{
	if (sm_boundShader == this)
		sm_boundShader = nullptr;

	for ( unsigned int i = 0 ; i < ShaderType_Count ; ++i )
		compileShaderFromFile((ShaderType)i,m_shaderFiles[i],false);

	if (linkProgram() == false)
	{
		logError("Shader Link Error:\n");
		logError(getLastError());
		log("\n");
	}

	for ( auto u : m_uniforms)
		u.second->registerDirty();
}

void Shader::bind()
{
	if (sm_boundShader != this)
	{
		glUseProgram(m_program);
		sm_boundShader = this;
	}

	// set shared state
	for (auto uniform : m_sharedUniforms)
		uniform->set();

	// clean dirty uniforms
	for (auto uniform : m_dirtyUniforms)
		uniform->clean();

	m_dirtyUniforms.clear();
}

void Shader::unBind()
{
	if (sm_boundShader == this)
	{
		glUseProgram(0);
		sm_boundShader = nullptr;
	}
}

void Shader::UniformBase::bind(int i)
{
	glUniform1i( m_location, i );
}

void Shader::UniformBase::bind(unsigned int i)
{
	glUniform1ui( m_location, i );
}

void Shader::UniformBase::bind(float f)
{
	glUniform1f( m_location, f );
}

void Shader::UniformBase::bind(const glm::vec2& v)
{
	glUniform2fv( m_location, 1, glm::value_ptr(v) );
}

void Shader::UniformBase::bind(const glm::vec3& v)
{
	glUniform3fv( m_location, 1, glm::value_ptr(v) );
}

void Shader::UniformBase::bind(const glm::vec4& v)
{
	glUniform4fv( m_location, 1, glm::value_ptr(v) );
}

void Shader::UniformBase::bind(const glm::ivec2& iv)
{
	glUniform2iv( m_location, 1, glm::value_ptr(iv) );
}

void Shader::UniformBase::bind(const glm::ivec3& iv)
{
	glUniform3iv( m_location, 1, glm::value_ptr(iv) );
}

void Shader::UniformBase::bind(const glm::ivec4& iv)
{
	glUniform4iv( m_location, 1, glm::value_ptr(iv) );
}

void Shader::UniformBase::bind(const glm::uvec2& iv)
{
	glUniform2uiv( m_location, 1, glm::value_ptr(iv) );
}

void Shader::UniformBase::bind(const glm::uvec3& iv)
{
	glUniform3uiv( m_location, 1, glm::value_ptr(iv) );
}

void Shader::UniformBase::bind(const glm::uvec4& iv)
{
	glUniform4uiv( m_location, 1, glm::value_ptr(iv) );
}

void Shader::UniformBase::bind(const glm::mat2& m)
{
	glUniformMatrix2fv( m_location, 1, false, glm::value_ptr(m) );
}

void Shader::UniformBase::bind(const glm::mat3& m)
{
	glUniformMatrix3fv( m_location, 1, false, glm::value_ptr(m) );
}

void Shader::UniformBase::bind(const glm::mat4& m)
{
	glUniformMatrix4fv( m_location, 1, false, glm::value_ptr(m) );
}

void Shader::UniformBase::bind(unsigned int a_count, int* i)
{
	glUniform1iv( m_location, a_count, i );
}

void Shader::UniformBase::bind(unsigned int a_count, unsigned int* i)
{
	glUniform1uiv( m_location, a_count, i );
}

void Shader::UniformBase::bind(unsigned int a_count, float* f)
{
	glUniform1fv( m_location, a_count, f );
}

void Shader::UniformBase::bind(unsigned int a_count, const glm::vec2* v)
{
	glUniform2fv( m_location, a_count, glm::value_ptr(*v) );
}

void Shader::UniformBase::bind(unsigned int a_count, const glm::vec3* v)
{
	glUniform3fv( m_location, a_count, glm::value_ptr(*v) );
}

void Shader::UniformBase::bind(unsigned int a_count, const glm::vec4* v)
{
	glUniform4fv( m_location, a_count, glm::value_ptr(*v) );
}

void Shader::UniformBase::bind(unsigned int a_count, const glm::ivec2* iv)
{
	glUniform2iv( m_location, a_count, glm::value_ptr(*iv) );
}

void Shader::UniformBase::bind(unsigned int a_count, const glm::ivec3* iv)
{
	glUniform3iv( m_location, a_count, glm::value_ptr(*iv) );
}

void Shader::UniformBase::bind(unsigned int a_count, const glm::ivec4* iv)
{
	glUniform4iv( m_location, a_count, glm::value_ptr(*iv) );
}

void Shader::UniformBase::bind(unsigned int a_count, const glm::uvec2* iv)
{
	glUniform2uiv( m_location, a_count, glm::value_ptr(*iv) );
}

void Shader::UniformBase::bind(unsigned int a_count, const glm::uvec3* iv)
{
	glUniform3uiv( m_location, a_count, glm::value_ptr(*iv) );
}

void Shader::UniformBase::bind(unsigned int a_count, const glm::uvec4* iv)
{
	glUniform4uiv( m_location, a_count, glm::value_ptr(*iv) );
}

void Shader::UniformBase::bind(unsigned int a_count, const glm::mat2* m)
{
	glUniformMatrix2fv( m_location, a_count, false, glm::value_ptr(*m) );
}

void Shader::UniformBase::bind(unsigned int a_count, const glm::mat3* m)
{
	glUniformMatrix3fv( m_location, a_count, false, glm::value_ptr(*m) );
}

void Shader::UniformBase::bind(unsigned int a_count, const glm::mat4* m)
{
	glUniformMatrix4fv( m_location, a_count, false, glm::value_ptr(*m) );
}

Shader::UniformBase* Shader::getUniform(const char* a_uniform)
{
	unsigned int hash = SynStringHash( a_uniform );
	auto it = m_uniforms.find( hash );
	return it == m_uniforms.end() ? nullptr : it->second;
}

// not the preferred way to set uniforms
void Shader::setUniform(const char* a_uniform, int i)
{
	unsigned int id = SynStringHash(a_uniform);
	auto iter = m_uniforms.find( id );
	if (iter != m_uniforms.end())
	{
		Uniform<int>* uniform = (Uniform<int>*)iter->second;
		uniform->set(i);
	}
}

void Shader::setUniform(const char* a_uniform, float f)
{
	unsigned int id = SynStringHash(a_uniform);
	auto iter = m_uniforms.find( id );
	if (iter != m_uniforms.end())
	{
		Uniform<float>* uniform = (Uniform<float>*)iter->second;
		uniform->set(f);
	}
}

void Shader::setUniform(const char* a_uniform, const glm::ivec4& iv)
{
	unsigned int id = SynStringHash(a_uniform);
	auto iter = m_uniforms.find( id );
	if (iter != m_uniforms.end())
	{
		Uniform<glm::ivec4>* uniform = (Uniform<glm::ivec4>*)iter->second;
		uniform->set(iv);
	}
}

void Shader::setUniform(const char* a_uniform, const glm::ivec2& iv)
{
	unsigned int id = SynStringHash(a_uniform);
	auto iter = m_uniforms.find( id );
	if (iter != m_uniforms.end())
	{
		Uniform<glm::ivec2>* uniform = (Uniform<glm::ivec2>*)iter->second;
		uniform->set(iv);
	}
}

void Shader::setUniform(const char* a_uniform, const glm::vec2& v)
{
	unsigned int id = SynStringHash(a_uniform);
	auto iter = m_uniforms.find( id );
	if (iter != m_uniforms.end())
	{
		Uniform<glm::vec2>* uniform = (Uniform<glm::vec2>*)iter->second;
		uniform->set(v);
	}
}

void Shader::setUniform(const char* a_uniform, const glm::vec4& v)
{
	unsigned int id = SynStringHash(a_uniform);
	auto iter = m_uniforms.find( id );
	if (iter != m_uniforms.end())
	{
		Uniform<glm::vec4>* uniform = (Uniform<glm::vec4>*)iter->second;
		uniform->set(v);
	}
}

void Shader::setUniform(const char* a_uniform, const glm::mat4& m)
{
	unsigned int id = SynStringHash(a_uniform);
	auto iter = m_uniforms.find( id );
	if (iter != m_uniforms.end())
	{
		Uniform<glm::mat4>* uniform = (Uniform<glm::mat4>*)iter->second;
		uniform->set(m);
	}
}

void Shader::setUniform(const char* a_uniform, unsigned int a_count, int* i)
{
	unsigned int id = SynStringHash(a_uniform);
	auto iter = m_uniforms.find( id );
	if (iter != m_uniforms.end())
	{
		Uniform<int>* uniform = (Uniform<int>*)iter->second;
		uniform->set(a_count,i);
	}
}

void Shader::setUniform(const char* a_uniform, unsigned int a_count, float* f)
{
	unsigned int id = SynStringHash(a_uniform);
	auto iter = m_uniforms.find( id );
	if (iter != m_uniforms.end())
	{
		Uniform<float>* uniform = (Uniform<float>*)iter->second;
		uniform->set(a_count,f);
	}
}

void Shader::setUniform(const char* a_uniform, unsigned int a_count, const glm::ivec4* iv)
{
	unsigned int id = SynStringHash(a_uniform);
	auto iter = m_uniforms.find( id );
	if (iter != m_uniforms.end())
	{
		Uniform<glm::ivec4>* uniform = (Uniform<glm::ivec4>*)iter->second;
		uniform->set(a_count,iv);
	}
}

void Shader::setUniform(const char* a_uniform, unsigned int a_count, const glm::vec2* v)
{
	unsigned int id = SynStringHash(a_uniform);
	auto iter = m_uniforms.find( id );
	if (iter != m_uniforms.end())
	{
		Uniform<glm::vec2>* uniform = (Uniform<glm::vec2>*)iter->second;
		uniform->set(a_count,v);
	}
}

void Shader::setUniform(const char* a_uniform, unsigned int a_count, const glm::vec4* v)
{
	unsigned int id = SynStringHash(a_uniform);
	auto iter = m_uniforms.find( id );
	if (iter != m_uniforms.end())
	{
		Uniform<glm::vec4>* uniform = (Uniform<glm::vec4>*)iter->second;
		uniform->set(a_count,v);
	}
}

void Shader::setUniform(const char* a_uniform, unsigned int a_count, const glm::mat4* m)
{
	unsigned int id = SynStringHash(a_uniform);
	auto iter = m_uniforms.find( id );
	if (iter != m_uniforms.end())
	{
		Uniform<glm::mat4>* uniform = (Uniform<glm::mat4>*)iter->second;
		uniform->set(a_count,m);
	}
}

void Shader::registerSharedUniform( const char* a_name, SharedUniformFactoryBase* a_factory )
{
	unsigned int id = SynStringHash(a_name);
	auto shared = sm_sharedUniformFactories.find( id );
	if (shared == sm_sharedUniformFactories.end())
	{
		sm_sharedUniformFactories[ id ] = a_factory;
	}
	else
		logWarning("Shared uniform [%s] already registered!\n", a_name);
}

void Shader::initialiseSharedUniforms()
{
	// initialise common shared uniforms
	Shader::registerSharedUniform("aoBuffer", new SharedIntUniformFactory(Material::AOBuffer));
	Shader::registerSharedUniform("lightBuffer", new SharedIntUniformFactory(Material::LightBuffer));
	Shader::registerSharedUniform("diffuseMap", new SharedIntUniformFactory(Material::Diffuse));
	Shader::registerSharedUniform("normalMap", new SharedIntUniformFactory(Material::Normal));
	Shader::registerSharedUniform("specularMap", new SharedIntUniformFactory(Material::Specular));
	Shader::registerSharedUniform("albedoMap", new SharedIntUniformFactory(0));
	Shader::registerSharedUniform("nearFar", new Shader::SharedUniformFactory<SharedNearFarUniform>());
	Shader::registerSharedUniform("camera.position", new Shader::SharedUniformFactory<SharedCameraPositionUniform>());
	Shader::registerSharedUniform("camera.view", new Shader::SharedUniformFactory<SharedCameraViewUniform>());
	Shader::registerSharedUniform("camera.projection", new Shader::SharedUniformFactory<SharedCameraProjectionUniform>());
	Shader::registerSharedUniform("camera.projectionView", new Shader::SharedUniformFactory<SharedCameraProjectionViewUniform>());
	Shader::registerSharedUniform("camera.invProjection", new Shader::SharedUniformFactory<SharedCameraInvProjectionUniform>());
	Shader::registerSharedUniform("camera.invView", new Shader::SharedUniformFactory<SharedCameraInvViewUniform>());
	Shader::registerSharedUniform("camera.nearFar", new Shader::SharedUniformFactory<SharedNearFarUniform>());
}

void Shader::destroySharedUniforms()
{
	for (auto uniform : sm_sharedUniformFactories)
		delete uniform.second;
	sm_sharedUniformFactories.clear();
}

bool Shader::readShaderSource( const char* a_filename, std::string& a_source )
{
	// open file for text reading
	FILE* file = fopen(a_filename,"rb");
	if (file == nullptr)
	{
		logError("Error: Unable to open file '%s' for reading!\n",a_filename);
		return false;
	}

	// get number of bytes in file
	fseek(file, 0, SEEK_END);
	unsigned int length = ftell(file);
	fseek(file, 0, SEEK_SET);

	// allocate buffer and read file contents
	char* buffer = new char[length + 1];
	memset(buffer,0,length + 1);
	fread(buffer, sizeof(char), length, file);
	fclose(file);

	a_source = buffer;
	delete[] buffer;

	std::string appPath = a_filename;
	int iLastForward = appPath.find_last_of('/');
	int iLastBackward = appPath.find_last_of('\\');
	if (iLastForward > iLastBackward)
	{
		appPath.resize(iLastForward + 1);
	}
	else if (iLastBackward != 0)
	{
		appPath.resize(iLastBackward + 1);
	}
	else
	{
		appPath = "";
	}

	unsigned int startPos = 0;
	std::string includeDir = "#include ";

	while( ( startPos = a_source.find( includeDir, startPos ) ) != std::string::npos )
	{
		int pos = startPos + includeDir.length() + 1;
		int length = a_source.find( "\"", pos );
		std::string filename = a_source.substr( pos, length - pos );
		std::string content = "";

		file = fopen(( appPath + filename ).c_str(),"rb");

		if ( file != nullptr )
		{
			// get number of bytes in file
			fseek(file, 0, SEEK_END);
			unsigned int length = ftell(file);
			fseek(file, 0, SEEK_SET);

			// allocate buffer and read file contents
			buffer = new char[length + 1];
			memset(buffer,0,length + 1);
			fread(buffer, sizeof(char), length, file);
			fclose(file);

			content = buffer;
			delete[] buffer;
		}
		else
		{
			std::cerr << "Couldn't include shader file: " << appPath + filename << "\n";
		}

		a_source.replace( startPos, ( length + 1 ) - startPos, content );
		startPos += content.length();
	}

	return true;
}

} // namespace syn