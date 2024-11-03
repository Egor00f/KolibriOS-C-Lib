#include <kolibriLib/debug.hpp>

#include <kolibriLib/filesystem/filesystem.hpp>
#include <kolibriLib/system/thread.hpp>

using namespace KolibriLib;


microlog::logger KolibriLib::logger(filesystem::temp_directory_path() / (Thread::GetThreadInfo().name + ".log"));
