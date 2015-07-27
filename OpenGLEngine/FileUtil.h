#ifndef FILEUTIL_H
#define FILEUTIL_H

#define LOG_FILE "log.txt"

#include <iostream>
#include <fstream>
#include <string>

class FileUtil
{
public:
	static std::string ReadFile(const char* filePath);
	static void Log(const char* msg);
	static int GetFileSize(std::ifstream& file);
};

#endif