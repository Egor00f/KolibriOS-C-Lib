#ifndef __UI_HPP__
#define __UI_HPP__

#include <include_ksys.h>

#include <vector>
#include <memory>

#include <kolibriLib/types.hpp>
#include <kolibriLib/color.hpp>
#include <kolibriLib/system/os.hpp>
#include <kolibriLib/UI/buttons/ButtonsIDController.hpp>
#include <kolibriLib/globals.hpp>

namespace KolibriLib
{
	/**
	 * @brief UI
	 * @details Пространство имён в котором только UI
	 */
	namespace UI
	{

		/// @brief Отступы по умолчанию
		const unsigned DefaultMargin = 4;

		/// @brief Размеры по умолчанию
		const Size DefaultSize = {200, 100};

		/// @brief Интерфейс для всех Объектов Gui
		class GuiObject
		{
		public:
			virtual ~GuiObject() = default;

			/// @brief Получить размер элемента
			/// @return Функция возвращает _size
			virtual UDim GetSize() const = 0;

			/// @brief Изменить размер элемента
			/// @param NewSize новый размер
			virtual void SetSize(const UDim &NewSize) = 0;

			/// @brief изменить координаты
			/// @param NewCoord новые координаты
			virtual void SetCoord(const UDim &NewCoord) = 0;

			/// @brief Получить координаты элемента
			/// @return Функция возвращает _coord
			virtual UDim GetCoord() const = 0;

			/// @brief Изменить размер элемента
			/// @param NewSize новый размер
			virtual void SetSize(const Size &NewSize) = 0;

			/// @brief изменить координаты
			/// @param NewCoord новые координаты
			virtual void SetCoord(const Coord &NewCoord) = 0;

			/// @brief Получить размер объекта
			/// @return Размер объекта в пикселях
			virtual Size GetAbsoluteSize() const = 0;

			/// @brief Получить координаты объекта
			/// @return Координаты в пикселях
			virtual Coord GetAbsoluteCoord() const = 0;

			/**
			 * @brief
			 * @return
			 */
			virtual buttons::ButtonsIDController *GetButtonIDController() const = 0;

			/**
			 * @brief
			 * @param buttonsIDController
			 */
			virtual void SetButtonIDController(const buttons::ButtonsIDController *buttonsIDController) = 0;

			/// @brief Получить отступы
			/// @return Функция возвращает _Margin
			unsigned GetMargin() const;

			/// @brief
			/// @param NewMargin
			void SetMargin(unsigned NewMargin);

		protected:
			/// @brief Отступы
			unsigned _Margin;
		};

		///@brief Элемент интерфейса
		/// @note Используется как шаблон для других классов
		class UIElement : public GuiObject
		{
		public:
			/// @brief Конструктор
			/// @param UDim координат
			/// @param size размер
			/// @param MainColor основной цвет
			/// @param Margin отступы
			/// @param relative отностельность
			UIElement(const UDim &coord = point(0), const UDim &size = point(0), const Colors::Color &MainColor = Globals::SystemColors.work_text, const unsigned &Margin = DefaultMargin);

			/// @brief Конструктор копирования
			/// @param cp То что юудет копирваться
			UIElement(const UIElement &cp);

			/// @brief Изменить родительский элемент
			/// @param Parent Указатель на родительский элемент
			/// @details почему этот метод константный? Да потому что по сути не изменяет состояние класса, элементы не очень то и зависят от родительского элемента
			void SetParent(const UIElement *NewParent) const;

			/**
			 * @brief
			 * @param ptr Указатель на родительский элемент
			 */
			void SetParent(std::weak_ptr<UIElement> ptr) const;

			/// @brief Сделать окно родительским элементом
			/// @param Указатель на окно
			void WindowAsParent(const GuiObject *window) const;

			/// @brief Получить указатель на родительский элемент
			/// @return Указатель на родительский элемент
			const GuiObject *GetParent() const;

			/// @brief Получить размер элемента
			/// @return Функция возвращает _size
			UDim GetSize() const override;

			/// @brief Изменить размер элемента
			/// @param NewSize новый размер
			void SetSize(const UDim &NewSize) override;

			/// @brief изменить координаты
			/// @param NewCoord новые координаты
			void SetCoord(const UDim &NewCoord) override;

			/// @brief Изменить размер элемента
			/// @param NewSize новый размер
			void SetSize(const Size &NewSize) override;

			/// @brief
			/// @param NewCoord
			void SetCoord(const Coord &NewCoord) override;

			/// @brief Получить абсолютный размер элемента
			/// @return размер
			Size GetAbsoluteSize() const override;

			/// @brief Получить абсолютные координаты элемента
			/// @return координаты левого верхнего угла этого элемента относительно левого верхнего угла родитесльского элемента
			Coord GetAbsoluteCoord() const override;

			/// @brief Получить координаты элемента
			/// @return Функция возвращает _coord
			UDim GetCoord() const override;

			/// @brief
			/// @return
			buttons::ButtonsIDController *GetButtonIDController() const override;

			/**
			 * @brief
			 * @param buttonsIDController
			 */
			void SetButtonIDController(const buttons::ButtonsIDController *buttonsIDController) override;

			/// @brief Получить осносной цвет элемента
			/// @return Функция возвращает _MainColor
			Colors::Color GetColor() const;

			/// @brief Изменить цвет
			/// @param NewColor новый цвет
			void SetColor(const Colors::Color &NewColor);

			/// @brief Повернуть элемент
			/// @param NewAngle Новый угол наклона
			void Rotate(unsigned NewAngle);

			/// @brief Получить угол наклона элемента
			/// @return Функция возвращает _angle
			unsigned GetRotate() const;

			/// @brief Проверить лежит ли курсор мыши над элементом
			/// @return true если курсор мыши находится в этом элементе, иначе false
			bool Hover() const;

			/// @brief Обработчик
			/// @return
			virtual int Handler(OS::Event event);

			/**
			 * @brief Действие на ивент
			 * @param ID нажатой кнопки
			 * @return true если объект кнопка, и она нажата
			 * @note Вызывается только если элемент был добавлен в окно (Вызывается в Window::Handler)
			 */
			virtual bool OnButtonEvent(buttons::ButtonID PressedButtonID);

			/// @note Вызывается только если элемент был добавлен в окно (Вызывается в Window::Handler)
			virtual bool OnKeyEvent();

			/// @note Вызывается только если элемент был добавлен в окно (Вызывается в Window::Handler)
			virtual bool OnMouseEvent();

			/// @brief отрисовать элемент
			virtual void Render() const;

			/// @brief Получить список всех элементов, для которых этот является родительсим
			/// @return указатель на вектор указателей
			std::vector<std::weak_ptr<UIElement>> &GetChildren();

			/// @brief Получить список всех элементов, для которых этот является родительсим
			/// @details Более медленная версия(в сравнении с неконстантной), т.к. копирует вектор
			/// @return вектор указателей
			std::vector<std::weak_ptr<UIElement>> GetChildren() const;

			/// @brief
			/// @param Element
			/// @return
			UIElement &operator=(const UIElement &Element);

			/// @brief
			/// @param Element
			/// @return
			bool operator==(const UIElement &Element) const;

			/// @brief
			/// @param Element
			/// @return
			bool operator!=(const UIElement &Element) const;

			/// @brief Заменить
			/// @param e
			void swap(UIElement &e);

		protected:
			/// @brief Координаты
			UDim _coord;

			/// @brief Размер
			UDim _size;

			/// @brief Основной цвет элемента
			Colors::Color _MainColor;

			int _rotation;

			/// @brief Элемент gui относительно которого просиходят расчёты относительного размера
			mutable std::weak_ptr<GuiObject> Parent;

			/// @brief Список элементов для которых этот элемент указан как Parent
			mutable std::vector<std::weak_ptr<UIElement>> _childs;

			/// @brief отображется ли элемент при отрисовке
			bool Visible = true;

			/// @brief указатель на то что Parent указывает на окно
			mutable bool ParentIsWindow = false;

		private:
			/// @brief Добавить
			void AddChildren(const UIElement *child) const;

			/// @brief Удалить
			void DeleteChildren(const UIElement *child) const;
		};


		/**
		 * @brief Фрейм
		 */
		using Frame = UIElement;
	}
}

inline std::ostream &operator<<(std::ostream &os, const KolibriLib::UI::UIElement &element)
{
	return os << "UIElement" << std::endl
			  << "Coord: " << element.GetCoord() << std::endl
			  << "Size: " << element.GetSize() << std::endl
			  << "MainColor" << element.GetColor() << std::endl;
}

#endif // __UI_HPP__
