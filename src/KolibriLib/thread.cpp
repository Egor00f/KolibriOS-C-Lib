#include <kolibriLib/system/thread.hpp>
#include <cstdlib>
#include <cstring>
#include <kolibriLib/debug.hpp>

using namespace KolibriLib;
using namespace Thread;

PID KolibriLib::Thread::CreateThread_(void *ThreadEntry, unsigned ThreadStackSize)
{
	void *th_stack = std::malloc(ThreadStackSize);

	if (!th_stack) // Если памяти не удалось получить
	{
		logger << microlog::LogLevel::Error << "Memory allocation error for thread!" << std::endl;
		return -1;
	}

	PID TID = _ksys_create_thread(ThreadEntry, ((uint8_t *)th_stack) + ThreadStackSize);

	if (TID == -1) // Если поток не был создан
	{
		logger << microlog::LogLevel::Error << "Unable to create a new thread!" << std::endl;
	}

	return TID;
}

KolibriLib::Thread::ThreadInfo KolibriLib::Thread::GetThreadInfo(const Slot &thread)
{
	int ec;
	return GetThreadInfo(thread, ec);
}

void KolibriLib::Thread::GetThreadInfo(ThreadInfo &info, Slot thread)
{
	ksys_thread_t val;

	_ksys_thread_info(&val, thread);

	info = ThreadInfo(val);
}

ThreadInfo KolibriLib::Thread::GetThreadInfo(const Slot &thread, int &ec)
{
	ksys_thread_t ret;

	ec = _ksys_thread_info(&ret, thread);

	return ThreadInfo(ret);
}

KolibriLib::Thread::ThreadInfo::ThreadInfo(const ksys_thread_t &t)
	: cpu_usage(t.cpu_usage),
	  pos_in_window_stack(static_cast<window::Pos>(t.pos_in_window_stack)),
	  num_window_stack(t.slot_num_window_stack),
	  memstart(t.memstart),
	  memused(t.memused),
	  pid(t.pid),
	  slot_state(static_cast<SlotState>(t.slot_state)),
	  window_state(static_cast<ThreadInfo::WindowStatus>(t.window_state)),
	  event_mask(t.event_mask),
	  key_input_mode(static_cast<keyboard::InputMode>(t.key_input_mode)),
	  WindowCoord(t.winx_start, t.winy_start),
	  WindowSize(t.winx_size, t.winy_size),
	  ClientCoord(t.clientx, t.clienty),
	  ClientSize(t.clientwidth, t.clientheight),
	  name(t.name, 12)
{
}

KolibriLib::Thread::ThreadInfo::operator ksys_thread_t() const
{
	ksys_thread_t ret;

	ret.cpu_usage = cpu_usage;
	ret.pid = pid;
	ret.window_state = static_cast<std::uint8_t>(window_state);
	ret.winx_size = WindowSize.x;
	ret.winy_size = WindowSize.y;

	std::strcpy(ret.name, name.c_str());

	return ret;
}