#include <C_Layer/libini.hpp>
#include <kolibriLib/debug.hpp>

libini::Loader libini::_libiniLoader;

libini::Loader::Loader()
{
	KolibriLib::logger << microlog::LogLevel::Debug << "Init libini: ";

	int err = kolibri_libini_init();

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

libini::iniFile::iniFile(const KolibriLib::filesystem::path &file)
	: _file(file)
{
}

bool libini::iniFile::EnumSections(LIBINI_enum_sections_func func) const
{
	return LIBINI_enum_sections(_file.c_str(), func);
}

bool libini::iniFile::EnumKeys(const std::string &section, LIBINI_enum_keys_func func) const
{
	return LIBINI_enum_keys(_file.c_str(), section.c_str(), func);
}

KolibriLib::Colors::Color libini::iniFile::GetColor(const std::string &section, const std::string &key, Error &ec) const
{
	LIBINI_get_color(_file.c_str(), section.c_str(), key.c_str(), static_cast<uint32_t>(Error::Error));

	return;
}

const libini::iniFile *usingIniFile;

std::vector<libini::iniFile::Section> sections;

void EnumSections(const char *fileName, const char *sectionName)
{
	sections.push_back(libini::iniFile::Section(sectionName, usingIniFile));
};

std::vector<libini::iniFile::Section> libini::iniFile::GetAllSection() const
{
	usingIniFile = this;

	LIBINI_enum_sections(_file.c_str(), &::EnumSections);

	std::vector<libini::iniFile::Section> ret = sections;

	sections.clear();

	return ret;
}

std::vector<libini::iniFile::Key> keys;

void EnumKeys(const char *fileName, const char *sectionName, const char *keyName, const char *value)
{
}

std::vector<libini::iniFile::Key> libini::iniFile::Section::GetAllKeys() const
{

	LIBINI_enum_keys(_file._file.c_str(), _name.c_str(), &::EnumKeys);

	std::vector<libini::iniFile::Key> ret = keys;

	keys.clear();

	return ret;
}