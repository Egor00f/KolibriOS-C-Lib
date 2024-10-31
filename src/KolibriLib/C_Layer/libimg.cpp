#include <C_Layer/libimg.hpp>
#include <kolibriLib/debug.hpp>

libimgLib libimgLibv;

libimgLib::libimgLib()
{
	KolibriLib::logger << microlog::LogLevel::Debug << "Init libimg: ";

	int err = kolibri_libimg_init();
	if (err == -1)
	{
		KolibriLib::logger << microlog::LogLevel::Fatal << "Error" << std::endl;
		throw err;
	}
	else
	{
		KolibriLib::logger << "OK";
	}

	KolibriLib::logger << std::endl;
}