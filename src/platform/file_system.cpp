// This file should contain all common stuff
#include "sani/platform/platform_config.hpp"
#include "sani/platform/file_system.hpp"
#include <sys/stat.h>


namespace sani {
	namespace io {

		FileSystem::FileSystem() {

		}

		FileSystem::~FileSystem() {

		}

		bool FileSystem::isFileOpen(const String& path) const {
			return handles.find(path) != handles.end();
		}

		bool FileSystem::openFile(const String& path, const Filemode mode) {
			if (isFileOpen(path)) return true;

			char access[5] = { 0 };

			if (mode & Filemode::Read && mode & Filemode::Write) {
				access[0] = 'a'; access[1] = '+';  access[2] = 'b';
			}
			else if (mode & Filemode::Read) {
				access[0] = 'r'; access[1] = 'b';
			}
			else if (mode & Filemode::Write) {
				access[0] = 'a'; access[1] = 'b';
			}
			FILE* handle = nullptr;
			handle = fopen(path.c_str(), access);

			if (!handle) return false;

			handles[path] = handle;

			// File open succeeded
			return true;
		}

		void FileSystem::closeFile(const String& path) {
			if (!isFileOpen(path)) return;

			FILE* handle = handles[path];
			fclose(handle);
			handles.erase(path);
		}

		String FileSystem::getFileDataString(const String& path) const  {
			size_t size = 0;
			unsigned char* buffer = getFileData(path, size, true);
			if (size == 0) {
				return "";
			}
			return String((const char*)buffer);
		}

		unsigned char* FileSystem::getFileData(const String& path, size_t& fileSize, bool nullTerminate /*= false*/) const {
			assert(isFileOpen(path));

			FILE* handle = handles.at(path);
			size_t fsize = getFileSize(path);
			unsigned char* buffer = nullptr;
			if (nullTerminate) {
				buffer = (unsigned char*)malloc(fsize + 1);
				buffer[fsize] = '\0';
			}
			else {
				buffer = (unsigned char*)malloc(fsize);
			}

			size_t readBytes = fread(buffer, sizeof(unsigned char), fsize, handle);
			// Failed
			if (readBytes != fsize) {
				if (buffer) {
					free(buffer);
					buffer = nullptr;
				}
				fileSize = 0;
				return nullptr;
			}

			// Success
			fileSize = readBytes;
			return buffer;
		}

		// This will be overwritten in Android/Windows systems
		bool FileSystem::isAbsolutePath(const String& path) const {
			return !path.empty() && path.at(0) == '/';
		}

		bool FileSystem::fileExists(const String& path) const {
			struct stat buffer;
			return (stat(path.c_str(), &buffer) == 0);
		}

		size_t FileSystem::getFileSize(const String& path) const {
			// TODO if the file is opened already?
			struct stat statbuf;
			if (stat(path.c_str(), &statbuf) == -1) {
				// error
				return 0;
			}
			return statbuf.st_size;
		}

		void FileSystem::getBytes(std::vector<unsigned char>& out, const String& path, size_t offset, size_t count) const {
			assert(isFileOpen(path));
			FILE* handle = handles.at(path);
			fseek(handle, offset, SEEK_SET);
			out.resize(count);
			fread(out.data(), sizeof(unsigned char), count, handle);

		}

#if (SANI_TARGET_PLATFORM == SANI_PLATFORM_WIN32) || (SANI_TARGET_PLATFORM != SANI_PLATFORM_WP8)
		void FileSystem::listFiles(std::vector<String>& files, const String& path) const {
			assert(0, "listFiles should be overwritten");
		}
#else
#include <dirent.h>
		// Unix systems
		void FileSystem::listFiles(std::vector<String>& files, const String& path) const {
			DIR *d;
			struct dirent* dir;
			d = opendir(path.c_str());

			if (d) {
				while ((dir = readdir(d)) != NULL) {
					files.push_back(dir->d_name);
				}
				closedir(d);
			}
		}
#endif
	}
}





