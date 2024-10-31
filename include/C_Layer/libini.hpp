#ifndef __LIBINI_HPP__
#define __LIBINI_HPP__

#include <C_Layer/INCLUDE/kolibri_libini.h>
#include <kolibriLib/filesystem/filesystem.hpp>
#include <kolibriLib/color.hpp>

#include <string>
#include <vector>

/**
 * @brief Оболочка для Libini
 */
namespace libini
{
	/**
	 * @brief Загрузчик библиотеки Libini
	 */
	class Loader final
	{
	public:
		/**
		 * @brief Конструктор
		 */
		Loader();
	};

	/**
	 * @brief Экземпляр загрузчика
	 */
	extern libini::Loader _libiniLoader;

	/// @brief 
	class iniFile
	{
	public:
		iniFile(const KolibriLib::filesystem::path &file);

		enum class Error
		{
			Ok,
			Error
		};

		class Section;

		class Key
		{
		public:

		private:
			std::string _name;
			Section& _section;
		};

		class Section
		{
		public:
			Section(const std::string& sectionName, const iniFile* file);

			std::vector<Key> GetAllKeys() const;
		private:
			std::string _name;
			iniFile& _file;
		};
	
		

		bool EnumSections(LIBINI_enum_sections_func func) const;

		/// @brief Перебрать ключи в секции
		/// @param section 
		/// @param func 
		/// @return 
		bool EnumKeys(const std::string &section, LIBINI_enum_keys_func func) const;

		KolibriLib::Colors::Color GetColor(const std::string& section, const std::string &key, Error& ec) const;

		/// @brief 
		/// @return 
		std::vector<Section> GetAllSection() const;

	private:
		KolibriLib::filesystem::path _file;
	};
}


#endif // __LIBINI_HPP__