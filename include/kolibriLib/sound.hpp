#ifndef __SOUND_HPP__
#define __SOUND_HPP__

#include <include_ksys.h>
#include <sound.h>
#include <utility>

namespace KolibriLib
{
	/// @brief Звук
	namespace Sound
	{
		/// @brief Получить состояние спикера(Вкл/выкл)
		/// @return true если спикер разрешён, иначе false
		inline bool SpeakerStatus()
		{
			bool status;

			asm_inline(
				"int $0x40"
				: "=a"(status)
				: "a"(18), "b"(8), "c"(1));

			return !status;
		}

		/// @brief Переключить состояние спикера
		inline void SpeakerSwitch()
		{
			asm_inline(
				"int $0x40" ::"a"(18), "b"(8), "c"(2));
		}

		/// @brief Включить спикер
		/// @details если спикер уже включен, то функция ничего не сделает
		inline void EnableSpeaker()
		{
			if (!SpeakerStatus())
				SpeakerSwitch();
		}

		/// @brief Выключить спикер
		/// @details если спикер уже выключен, то функция ничего не сделает
		inline void DisableSpeaker()
		{
			if (SpeakerStatus())
				SpeakerSwitch();
		}

		/**
		 * @brief 
		 * @warning не работает, не зевершено
		 */
		struct SpeakerSound
		{
			struct Data
			{
				uint8_t length;
				uint8_t note;

				uint8_t GetNote() const;
			};

			Data *data;

			SpeakerSound(std::size_t size);
			~SpeakerSound();
		};

		bool PlayOnSpeaker(SpeakerSound data);

		/**
		 * @brief Ошибка
		 */
		using Error = int;

		/**
		 * @brief Формат звука
		 */
		using SoundFormat = int;

		/**
		 * @brief Тип буфера
		 */
		enum class BufferType	
		{
			/// @brief Статический буфер
			/// @details Простая звуковая сигнализация, озвучка интерфейса и т.п. Позволяет одновременно использовать звуковые данные в разных форматах.
			Out = PCM_OUT,

			/// @brief Потоковый буфер
			/// @details различные аудиоплееры. Программа загружает большие объёмы звуковых данных из сети, накопителей или генерирует их в реальном времени.
			Static = PCM_STATIC,

			/**
			 * @brief Кольцевой буфер
			 * @details игры и другие приложения с повышенными требованиями к синхронности аудио- и видеоряда.
			 * позволяет выводить звук с минимальной задержкой
			 */
			Ring = PCM_RING
		};

		/**
		 * @brief Загрузчик библиотеки
		 */
		class SoundLib
		{
		public:
			SoundLib();
		};

		Error PlaySound(void *buffer, int buffer_size, SoundFormat snd_format);

		/**
		 * @brief Буфер
		 */
		class Buffer
		{
		public:
			/// @brief Конструктор
			/// @param type Error
			/// @param format 
			/// @throw ошибка
			Buffer(BufferType type, SoundFormat format);

			/// @brief Конструктор
			/// @param type 
			/// @param format 
			/// @param ec ошибка
			Buffer(BufferType type, SoundFormat format, Error& ec) noexcept;

			/**
			 * @brief Деструктор
			 */
			~Buffer();

			/// @brief проиграть буфер 
			/// @return 
			Error PlayAsync();

			/// @brief Проиграть буфер(ждать конца)
			/// @return 
			Error PlaySync();

			/**
			 * @brief 
			 * @return 
			 */
			Error Reset();

			/**
			 * @brief 
			 * @param async 
			 * @return 
			 */
			Error Play(bool async);

			/// @brief Устанавливает новый формат звукового буфера
			/// @details Применимо для PCM_OUT и PCM_STATIC буферов.
			/// @param format Новый формат
			/// @return ошибка
			Error SetFormat(SoundFormat format);

			/// @brief Устанавливает уровень громкости для правого и левого каналов.
			/// @details Уровень задаётся как ослабление сигнала в сотых долях децибела в диапазоне 0 - -10000, где 0 - максимальная громкость -10000 (-100dB) - полная тишина
			/// @return Ошибка
			Error SetVolume(int left, int right);

			/// @brief Устанавливает уровень громкости для правого и левого каналов.
			/// @details Уровень задаётся как ослабление сигнала в сотых долях децибела в диапазоне 0 - -10000, где 0 - максимальная громкость -10000 (-100dB) - полная тишина
			/// @return Ошибка
			Error SetVolume(std::pair<int, int> vol);

			/// @brief 
			/// @param left 
			/// @param right 
			/// @return 
			Error GetVolume(int &left, int &right) const;

			/**
			 * @brief 
			 * @param ec 
			 * @return 
			 */
			std::pair<int, int> GetVolume(Error &ec) const;

			/// @brief Устанавливает баланс левого и правого каналов
			/// @details Баланс задаётся в сотых долях децибела в диапазоне -10000 - 10000, где -10000 - тишина в правом канале 10000 - тишина в левом канале 0 - нормальный баланс каналов
			/// @param pan
			/// @return
			Error SetPan(int pan);

			/// @brief Получить баланс левого и правого каналов
			/// @param ec
			/// @return
			int GetPan(Error &ec) const;

			/**
			 * @brief 
			 * @param ec 
			 * @return 
			 */
			int GetSize(Error &ec);

			/**
			 * @brief 
			 * @param ec 
			 * @return 
			 */
			int GetFree(Error &ec);

			/**
			 * @brief 
			 * @param ec 
			 * @return 
			 */
			int GetPos(Error &ec);

			/**
			 * @brief 
			 * @param pos 
			 * @return 
			 */
			Error SetPos(int pos);

			/// @brief 
			/// @param buff буфер
			/// @param buff_size размер буфера
			/// @param offset смешение
			/// @return 
			Error Set(void* buff, int buff_size, int offset = 0);

			/**
			 * @brief 
			 * @return 
			 */
			Error Stop();

		private:
			/**
			 * @brief 
			 */
			SNDBUF _hBuff;
		};

		/**
		 * @brief 
		 */
		extern SoundLib lib;
	}
} // namespace KolibriLib

#endif // __SOUND_HPP__