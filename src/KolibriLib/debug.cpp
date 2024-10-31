#include <kolibriLib/debug.hpp>

#include <kolibriLib/filesystem/filesystem.hpp>

using namespace KolibriLib;

microlog::logger KolibriLib::logger(filesystem::temp_directory_path() / "debug.log");
