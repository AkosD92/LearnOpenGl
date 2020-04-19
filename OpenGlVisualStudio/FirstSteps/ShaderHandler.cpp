#include "ShaderHandler.h"
#include <fstream>
#include <iostream>

std::string loadInShader(const char* argSource)
{
	std::string RetVal;
	std::ifstream ifs(argSource);

	// ensure ifstream objects can throw exceptions:
	ifs.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		std::string ReturnContent((std::istreambuf_iterator<char>(ifs)),(std::istreambuf_iterator<char>()));
		ifs.close();
		RetVal = ReturnContent;
	}
	catch (std::ifstream::failure e )	
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	return RetVal;
}
