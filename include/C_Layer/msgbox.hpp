#ifndef __MSGBOX_HPP__
#define __MSGBOX_HPP__

#include <C_Layer/INCLUDE/kolibri_msgbox.h>
#include <string>
#include <vector>

/// @brief Загрузчик Message Box
class msgBoxLib
{
public:
	msgBoxLib();
};

extern msgBoxLib _msgBoxLib;

/// @brief Message Box
/// @details Обёртка для библиотеки Message Box
class msgBox
{
public:

	/**
	 * @brief 
	 */
	using Callback = msgbox_callback;

	/// @brief Конструктор
	/// @param title заголовок
	/// @param text текст
	/// @param ButtonsText текст для кнопок
	msgBox(const std::string &title, const std::string& text, const std::vector<std::string> &ButtonsText = {"Ok"});

	/// @brief Деструктор
	~msgBox();

	/// @brief Отправить MessageBox
	/// @param funcs функции вызываемые при нажатии на кнопку
	/// при нажатии на кнопку со вторым текстом (из вектора ButtonsText из конструктора) вызовется функция вторая функция из этого вектора
	void msg(const std::vector<msgBox::Callback> &funcs);

	/**
	 * @brief 
	 */
	void reinit();
private:
	/**
	 * @brief 
	 */
	msgbox* _struct;

	/**
	 * @brief 
	 */
	msgBox::Callback* _funcs = nullptr;
};

#endif // __MSGBOX_HPP__