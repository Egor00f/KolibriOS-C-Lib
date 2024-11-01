#ifndef __NETWORK_HPP__
#define __NETWORK_HPP__

#include <cstdlib>

#include <string>
#include <include_ksys.h>

/// \brief Основное пространство имён
/// \author Egor00f
namespace KolibriLib
{

    /// @brief Работа с сетью
    namespace Network
    {
        /**
         * @brief Работа с сетевыми устройствами
         */
        namespace Devices
        {
            /**
             * @brief Номер устройства
             */
            using DeviceNumber = std::uint8_t;

            /**
             * @brief Указатель на устройство
             */
            using DevicePointer = void *;

            /**
             * @brief Состояние подключения
             */
            enum class LinkStatus : std::uint8_t
            {
                /**
                 * @brief нет соединения
                 */
                No = 0,

                /**
                 * @brief неизвестное соединение
                 */
                Unknown = 1,

                /**
                 * @brief 10 Мбит
                 */
                mb10 = 4,

                /**
                 * @brief 100 Мбит
                 */
                mb100 = 8,
                /**
                 * @brief 1 Гбит
                 */
                gb1 = 12,
            };

            /// @brief Получить кол-во активных сетевых устройств
            /// @return кол-во активных сетевых устройств
            inline unsigned GetNumberNetworkDevices()
            {
                unsigned number;

                asm_inline(
                    "int $0x40"
                    : "=a"(number)
                    : "a"(74), "b"(-1));

                return number;
            }

            inline unsigned GetDeviceType(DeviceNumber device)
            {
                unsigned type;

                asm_inline(
                    "int $0x40"
                    : "=a"(type)
                    : "a"(74), "b"(0 | device << 8));

                return type;
            }

            /**
             * @brief Получить имя сетевого устройства
             * @param device устройство
             * @param name указатель на имя устройства. 64 символа
             * @return Записывает в name имя устройства
             *         -1 в случае ошибки
             */
            inline int GetDeviceName(DeviceNumber device, char (&name)[64])
            {
                int error;

                asm_inline(
                    "int $0x40"
                    : "=a"(error)
                    : "a"(74), "b"(1 | device << 8), "c"(name));

                return error;
            }

            /// @brief Сбросить устройство
            /// @param Device Сбрасываемое устройство
            /// @return -1 в случае ошибки
            inline int ResetDevice(DeviceNumber device)
            {
                int error;

                asm_inline(
                    "int $0x40"
                    : "=a"(error)
                    : "a"(74), "b"(2 | device << 8));

                return error;
            }

            /**
             * @brief Остановить устройство
             * @param device номер устройства
             * @return -1 в случае ошибки
             */
            inline int StopDevice(DeviceNumber device)
            {
                int error;

                asm_inline(
                    "int $0x40"
                    : "=a"(error)
                    : "a"(74), "b"(3 | device << 8));

                return error;
            }

            /**
             * @brief Получить указатель на устройство
             * @param device номер устройства
             * @return nullptr в случае ошибки
             */
            inline DevicePointer GetDevicePointer(DeviceNumber device)
            {
                DevicePointer pointer;

                asm_inline(
                    "int $0x40"
                    : "=a"(pointer)
                    : "a"(74), "b"(4 | device << 8));

                if (reinterpret_cast<int>(pointer) == -1)
                    return nullptr;
                else
                    return pointer;
            }

            /**
             * @brief Получить количество принятых пакетов.
             * @param device номер устройства
             * @return -1 в случае ошибки
             */
            inline int GetRxPacketCount(DeviceNumber device)
            {
                int number;

                asm_inline(
                    "int $0x40"
                    : "=a"(number)
                    : "a"(74), "b"(7 | device << 8));

                return number;
            }

            /**
             * @brief Получить количество посланных пакетов.
             * @param device номер устройства
             * @return -1 в случае ошибки
             */
            inline int GetTxPacketCount(DeviceNumber device)
            {
                int number;

                asm_inline(
                    "int $0x40"
                    : "=a"(number)
                    : "a"(74), "b"(6 | device << 8));

                return number;
            }

            /**
             * @brief Получить количество принятых байт.
             * @param device номер устройства
             * @return -1 в случае ошибки
             */
            inline int GetRxByteCount(DeviceNumber device)
            {
                int number;

                asm_inline(
                    "int $0x40"
                    : "=a"(number)
                    : "a"(74), "b"(8 | device << 8));

                return number;
            }

            /**
             * @brief Получить количество посланных байт.
             * @param device номер устройства
             * @return -1 в случае ошибки
             */
            inline int GetTxByteCount(DeviceNumber device)
            {
                int number;

                asm_inline(
                    "int $0x40"
                    : "=a"(number)
                    : "a"(74), "b"(9 | device << 8));

                return number;
            }

            /**
             * @brief Проверить, включен ли полный дуплекс
             * @param status статус
             * @return true если включен, false если выключен
             */
            inline bool FullDuplex(LinkStatus status)
            {
                return static_cast<std::uint8_t>(status) & 0b0010;
            }

            /**
             * @brief получить статус соединения.
             * @param device номер устройства
             * @return
             */
            inline LinkStatus GetLinkStatus(DeviceNumber device)
            {
                LinkStatus status;

                asm_inline(
                    "int $0x40"
                    : "=a"(status)
                    : "a"(74), "b"(10 | device << 8));

                return status;
            }

            /**
             * @brief Получить количество пакетов переданных с ошибкой.
             * @param device Номер устройства
             * @return Количество ошибочных пакетов, переданных с момента запуска устройства, -1 при ошибке
             */
            inline int GetTxPacketErrorCount(DeviceNumber device)
            {
                int number;

                asm_inline(
                    "int $0x40"
                    : "=a"(number)
                    : "a"(74), "b"(11 | device << 8));

                return number;
            }

            /**
             * @brief Получить число пакетов отброшенных при отправке.
             * @param device Номер устройства
             * @return число отброшенных пакетов с момента запуска устройства, -1 при ошибке
             */
            inline int GetTxPacketDropCount(DeviceNumber device)
            {
                int number;

                asm_inline(
                    "int $0x40"
                    : "=a"(number)
                    : "a"(74), "b"(12 | device << 8));

                return number;
            }

            /**
             * @brief Получить число пакетов утерянных при отправке.
             * @param device Номер устройства
             * @return Число утерянных пакетов с момента запуска устройства, -1 при ошибке
             */
            inline int GetTxPacketMissCount(DeviceNumber device)
            {
                int number;

                asm_inline(
                    "int $0x40"
                    : "=a"(number)
                    : "a"(74), "b"(13 | device << 8));

                return number;
            }

            /**
             * @brief Получить число пакетов полученных с ошибкой.
             * @param device Номер устройства
             * @return число пакетов, полученных с ошибкой с момента запуска устройства, -1 при ошибке
             */
            inline int GetRxPacketErrorCount(DeviceNumber device)
            {
                int number;

                asm_inline(
                    "int $0x40"
                    : "=a"(number)
                    : "a"(74), "b"(14 | device << 8));

                return number;
            }

            /**
             * @brief Получить число пакетов полученных с ошибкой
             * @param device Номер устройства
             * @return Число отброшенных пакетов с момента запуска устройства, -1 при ошибке
             */
            inline int GetRxPacketDropCount(DeviceNumber device)
            {
                int number;

                asm_inline(
                    "int $0x40"
                    : "=a"(number)
                    : "a"(74), "b"(15 | device << 8));

                return number;
            }

            /**
             * @brief Получить число пакетов утерянных при получении
             * @param device Номер устройства
             * @return Число утерянных пакетов с момента запуска устройства, -1 при ошибке
             */
            inline int GetRxPacketMissCount(DeviceNumber device)
            {
                int number;

                asm_inline(
                    "int $0x40"
                    : "=a"(number)
                    : "a"(74), "b"(16 | device << 8));

                return number;
            }

        } // namespace Devices

        /// @brief Работа с сокетами
        namespace Socket
        {

            using SocketNumber = int;

            /**
             * @brief Тип домена
             */
            enum class Domain
            {
                
            };

            /// @brief Список типов сокета
            enum class SocketTypes
            {
                
            };

            /// @brief Список протоколов
            enum class Protocols
            {
                
            };

            /// @brief Открыть сокет
            /// @param domain тип сети IPv4 или IPv6
            /// @param SocketType тип сокета
            /// @param protocol Используемый протокол
            /// @return номер сокета, -1 в случае ошибки
            inline SocketNumber OpenSocket(Domain domain, SocketTypes SocketType, Protocols protocol)
            {
                SocketNumber socket;

                asm_inline(
                    "int $0x40"
                    : "=a"(socket)
                    : "a"(75), "b"(0), "c"(domain), "d"(SocketType), "S"(protocol)
                );

                return socket;
            }

            /**
             * @brief Закрыть сокет
             * @param Socket номер сокета
             * @param errorCode Код ошибки
             * @return -1 в случае ошибки
             */
            inline int CloseSocket(SocketNumber socket, int &errorCode)
            {
                int a, b;

                asm_inline(
                    "int $0x40"
                    : "=a"(a), "=b"(b)
                    : "a"(75), "b"(1), "c"(socket)
                );

                errorCode = b;

                return a;
            }

        } // namespace Socket

        /**
         * @brief сетевые опции и статистика. 
         */
        namespace
        {

        }

    } // namespace network

} // namespace KolibriLib

#endif // __NETWORK_HPP__
