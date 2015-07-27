#include "FileUtil.h"

std::string FileUtil::ReadFile(const char* filePath)
{
	std::ifstream file(filePath, std::ios_base::binary);
	if (!file.is_open())
	{
		std::cerr << "Error: Could not open file (" << filePath << ")\n";
		return "";
	}

	// Read all contents from the file
	int fileSize = GetFileSize(file);
	char* buf = new char[fileSize];
	file.read(buf, fileSize);
	std::string contents(buf, fileSize);
	delete[] buf;

	return contents;
}

void FileUtil::Log(const char* msg)
{
	std::ofstream file(LOG_FILE, std::ios_base::app);
	file << msg << std::endl;
}

int FileUtil::GetFileSize(std::ifstream& file)
{
	file.seekg(0, std::ios_base::end);
	int size = (int)file.tellg();
	file.seekg(0, std::ios_base::beg);

	return size;
}