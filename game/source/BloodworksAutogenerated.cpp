#include "Bloodworks.h"
#include "autogenerated.h"

#include "Coral.h"
#include "cGlobals.h"

void Bloodworks::startAutoGenerating()
{
#ifdef HAS_AUTO_GENERATED
	return;
#endif
	autoGeneratedFile.open("source/autogenerated.h");

	if (autoGeneratedFile.good())
	{
		autoGenerating = true;

		autoGeneratedFile << "#include <unordered_map>" "\n";
		autoGeneratedFile << "#define HAS_AUTO_GENERATED" "\n";
		autoGeneratedFile << "std::unordered_map<std::string, const char*> fileMap = {" "\n";
	}
	else if (Coral::isDebuggerPresent())
	{
		DebugBreak();
	}
}


void Bloodworks::stopAutoGenerating()
{
	if (autoGenerating)
	{
		autoGeneratedFile << "};" "\n";
		autoGeneratedFile.close();
		autoGenerating = false;
	}
}

void fixText(std::string& data)
{
	data = replaceAll(data, "\"", "\\\"");

	data = replaceAll(data, "\r", "");
	data = replaceAll(data, "\\n", "<<escapednewline>>\"\"");
	data = replaceAll(data, "\n", "<<newline>>\"\"");
}

void Bloodworks::loadJsonFile(nlohmann::json& j, const std::string& filePath)
{
	std::string data;
#ifdef HAS_AUTO_GENERATED
	if (loadAutoGenerated)
	{
		data = fileMap.at(filePath);
		data = replaceAll(data, "<<newline>>", "\n");
		data = replaceAll(data, "<<escapednewline>>", "\\n");
	}
	else
#endif
	{
		textFileRead(filePath, data);
	}

	j = nlohmann::json::parse(data.c_str());

	fixText(data);
	autoGeneratedFile << "{\"" << filePath << "\", \"" << data << "\"}, " "\n";
}

void Bloodworks::loadLuaFile(const std::string& scriptFilePath)
{
#ifdef HAS_AUTO_GENERATED
	out << scriptFilePath << "\n";
	if (loadAutoGenerated)
	{
		std::string data = fileMap.at(scriptFilePath);
		data = replaceAll(data, "<<escapednewline>>", "\\n");
		data = replaceAll(data, "<<newline>>", "\n");
		lua.script(data);
	}
	else
#endif
	{
		lua.script_file(scriptFilePath);
	}


	if (autoGenerating)
	{
		std::string data;
		textFileRead(scriptFilePath, data);

		fixText(data);
		autoGeneratedFile << "{\"" << scriptFilePath << "\", \"" << data << "\"}, " "\n";
	}
}
