#include <iostream>
#include <fstream>
#include <sstream>
#include <alloca.h>

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

static ShaderProgramSource ParseShader(std::string filepath)
{
	std::ifstream stream(filepath);

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;

	auto _p = [](const std::string& l, const std::string& v) {return l.find(v) != std::string::npos; };

	while (getline(stream, line)) {
		if (_p(line, "#shader"))
		{
			if (_p(line, "vertex")) type = ShaderType::VERTEX;
			else if (_p(line, "fragment")) type = ShaderType::FRAGMENT;
		}
		else {
			ss[(int)type] << line << "\n";
		}
	}

	return { ss[0].str(),ss[1].str() };
}


int main(){
	ShaderProgramSource a = ParseShader("Basic.shader");

	std::cout << "vertex\n";
	std::cout << a.VertexSource << "\n";

	std::cout << "fragment\n";
	std::cout << a.FragmentSource << "\n";

}