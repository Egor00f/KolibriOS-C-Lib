#include <kolibriLib/system/os.hpp>
#include <cstdlib>
#include <cstring>

using namespace KolibriLib;
using namespace OS;

Thread::PID KolibriLib::OS::Exec(const filesystem::Path &AppName, const std::string &args, filesystem::FilesystemErrors &ec, bool debug) noexcept
{
	int ret = _ksys_exec(AppName.c_str(), const_cast<char *>(args.c_str()), debug);

	if (ret > 0)
	{
		ec = filesystem::FilesystemErrors::Successfully;
		return ret;
	}
	else if(ret < 0)
	{
		ec = static_cast<filesystem::FilesystemErrors>(-ret);
		return -1;
	}
}

Thread::PID KolibriLib::OS::Exec(const filesystem::Path &AppName, const std::string &args, bool debug)
{
	filesystem::FilesystemErrors ec;

	Thread::PID ret = Exec(AppName, args, ec, debug);

	if(ret == -1)
		throw ec;

	return ret;
}

CoreVersion KolibriLib::OS::GetCoreVersion()
{
	// Реальная структура
	struct RealCoreVersion
	{
		union
		{
			// Версия
			std::uint32_t version;
			struct
			{
				std::uint8_t a;
				std::uint8_t b;
				std::uint8_t c;
				std::uint8_t d;
			};
		};

		// Зарезервированно
		std::uint8_t reserved;

		// Ревизия
		std::uint16_t Rev;
	};

	RealCoreVersion *data = static_cast<RealCoreVersion*>(std::malloc(16));

	asm_inline(
		"int $0x40" ::"a"(18), "b"(13), "c"(data));

	CoreVersion p;

	p.version = data->version;
	p.Rev = data->Rev;

	std::free(data);

	return p;
}

/*
	=== CoreVersion ===
	Operators
*/

bool KolibriLib::OS::CoreVersion::operator==(const CoreVersion &ver) const
{
	return version == ver.version && Rev == ver.Rev;
}

bool KolibriLib::OS::CoreVersion::operator!=(const CoreVersion &ver) const
{
	return version != ver.version && Rev != ver.Rev;
}

