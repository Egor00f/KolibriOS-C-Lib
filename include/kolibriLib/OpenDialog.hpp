#ifndef __KOLIBRILIB_OPENDIALOG_HPP__
#define __KOLIBRILIB_OPENDIALOG_HPP__

#include <kolibriLib/filesystem/filesystem.hpp>
#include <kolibriLib/types.hpp>
#include <kolibriLib/window/windowBase.hpp>
#include <C_Layer/opendialog.hpp>

namespace KolibriLib
{
	/// @brief Диалог выбора файлов
	/// @details это отдельная программа (если что)
	/// Памяти требует эта штука много (около 9500 байт)
	/// @warning На данный момент не завершено
	class OpenDialog
	{	
	public:

		/// @brief Режим открытия OpenDialog
		enum class Mode
		{
			/// @brief Открыть файл
			/// @image OpenD1.png
			OpenFile = open_dialog_mode::OPEN,

			/// @brief Сохранить файл
			/// @image OpenD2.png
			Save = open_dialog_mode::SAVE,

			/// @brief Выбрать папку
			/// @image OpenD3.png
			Select = open_dialog_mode::SELECT
		};

		/// @brief Статус
		enum class Status
		{
			/// @brief Пользователь нажал Cancel
			Cancel = 0,

			/// @brief Пользователь нажал Open
			Opened = 1,

			/// @brief OpenDialog не смог отрыться
			Error = 2
		};

		/// @note должно обязательно заканчиавться на '\0'
		using FilterElement = std::string;

		/// @brief Фильтр расширений фалов
		struct FilterArea
		{
			od_filter* area;

			/// @brief 
			FilterArea();

			/// @brief 
			/// @param elements 
			FilterArea(const std::vector<FilterElement> &elements);

			/// @brief Деструктор
			~FilterArea();

			static od_filter* Create(const std::vector<FilterElement> &elements);

			FilterElement & operator[](std::size_t n);
			FilterElement operator[](std::size_t n) const;
		};

		/// @brief Указатель на то что путь не указан, нужен только для GetPath
		const filesystem::Path PathNotSet = "/";

		OpenDialog(OpenDialog::Mode mode, Size winSize = window::DefaultWindowSize, Coord winCoord = window::DefaultWindowCoord, const std::vector<FilterElement> &filter = {}, const filesystem::Path &defaultPath = sz_dir_default_path);

		/// @brief Открыть OpenDialog
		/// @return Путь который выбрал пользователь
		Status Open();


		/// @brief Получить путь, выбранный пользователем
		/// @return 
		filesystem::path GetPath() const;

		/// @brief Добавить еще фильтров
		void SetFilter(const std::vector<FilterElement> &f);

		/// @brief деструктор
		~OpenDialog();

	private:
		open_dialog* _opendialog;
	};

	
} // namespace KolibriLib

#endif // __KOLIBRILIB_OPENDIALOG_HPP__
