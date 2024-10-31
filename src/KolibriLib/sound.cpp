#include <kolibriLib/sound.hpp>
#include <kolibriLib/debug.hpp>
#include <cstdlib>

KolibriLib::Sound::SoundLib KolibriLib::Sound::lib;

/*
	Спикер
*/

KolibriLib::Sound::SpeakerSound::SpeakerSound(std::size_t size)
{
	data = static_cast<Data *>(std::malloc(size * 1));
}

uint8_t KolibriLib::Sound::SpeakerSound::Data::GetNote() const
{
	return note;
}

bool KolibriLib::Sound::PlayOnSpeaker(SpeakerSound data)
{
	return false;
}

/*
	sound.h
*/

KolibriLib::Sound::SoundLib::SoundLib()
{
	Error err;
	int version = -1;

	KolibriLib::logger << microlog::LogLevel::Debug << "Loading Sound: ";

	err = InitSound(&version);

	if (err != 0 ||
				  (SOUND_VERSION > (version & 0xFFFF)) ||
				  (SOUND_VERSION < (version >> 16)))
	{
		KolibriLib::logger << microlog::LogLevel::Fatal << "Error loading Sound" << std::endl;
		throw err;
	}
	else
	{
		KolibriLib::logger << "OK" << std::endl;
	}
}

KolibriLib::Sound::Error KolibriLib::Sound::PlaySound(void *buffer, int buffer_size, SoundFormat snd_format)
{
	SNDBUF hBuff;

	Error err;

	if ((err = CreateBuffer(static_cast<unsigned>(snd_format) | PCM_OUT, 0, &hBuff)) == 0)
	{
		WaveOut(hBuff, buffer, buffer_size);
	};

	return err;
}

/*
	Buffer
*/

KolibriLib::Sound::Buffer::Buffer(BufferType type, SoundFormat format)
{
	Error err = CreateBuffer(static_cast<unsigned>(format) | static_cast<unsigned>(type), 0, &_hBuff);
	if (err)
		throw err;
}

KolibriLib::Sound::Buffer::Buffer(BufferType type, SoundFormat format, Error &ec) noexcept
{
	ec = CreateBuffer(static_cast<unsigned>(format) | static_cast<unsigned>(type), 0, &_hBuff);
}

KolibriLib::Sound::Buffer::~Buffer()
{
	Error err = DestroyBuffer(_hBuff);
	if (err)
		throw err;
}

KolibriLib::Sound::Error KolibriLib::Sound::Buffer::Play(bool async)
{
	if (async)
		return PlayBuffer(_hBuff, 0);
	else
		return PlayBuffer(_hBuff, PLAY_SYNC);
}

KolibriLib::Sound::Error KolibriLib::Sound::Buffer::PlayAsync()
{
	return PlayBuffer(_hBuff, 0);
}

KolibriLib::Sound::Error KolibriLib::Sound::Buffer::PlaySync()
{
	return PlayBuffer(_hBuff, PLAY_SYNC);
}

KolibriLib::Sound::Error KolibriLib::Sound::Buffer::Reset()
{
	return ResetBuffer(_hBuff, 0);
}

KolibriLib::Sound::Error KolibriLib::Sound::Buffer::SetFormat(SoundFormat format)
{
	return ::SetFormat(_hBuff, format);
}

KolibriLib::Sound::Error KolibriLib::Sound::Buffer::SetVolume(int left, int right)
{
	return ::SetVolume(_hBuff, left, right);
}

KolibriLib::Sound::Error KolibriLib::Sound::Buffer::SetVolume(std::pair<int, int> vol)
{
	return SetVolume(vol.first, vol.second);
}

KolibriLib::Sound::Error KolibriLib::Sound::Buffer::GetVolume(int &left, int &right) const
{
	return ::GetVolume(_hBuff, &left, &right);
}

std::pair<int, int> KolibriLib::Sound::Buffer::GetVolume(Error &ec) const
{
	int l, r;
	ec = GetVolume(l, r);
	return std::pair<int, int>(l, r);
}

KolibriLib::Sound::Error KolibriLib::Sound::Buffer::SetPan(int pan)
{
	return ::SetPan(_hBuff, pan);
}

int KolibriLib::Sound::Buffer::GetPan(KolibriLib::Sound::Error &ec) const
{
	int ret;

	ec = ::GetPan(_hBuff, &ret);

	return ret;
}

int KolibriLib::Sound::Buffer::GetFree(Error &ec)
{
	int ret;

	ec = GetBufferFree(_hBuff, &ret);

	return ret;
}

int KolibriLib::Sound::Buffer::GetSize(Error &ec)
{
	int ret;

	ec = GetBufferSize(_hBuff, &ret);

	return ret;
}

int KolibriLib::Sound::Buffer::GetPos(Error &ec)
{
	int ret;

	ec = GetBufferPos(_hBuff, &ret);

	return ret;
}

KolibriLib::Sound::Error KolibriLib::Sound::Buffer::SetPos(int pos)
{
	return SetBufferPos(_hBuff, pos);
}

KolibriLib::Sound::Error KolibriLib::Sound::Buffer::Set(void *buff, int buff_size, int offset)
{
	return SetBuffer(_hBuff, buff, offset, buff_size);
}

KolibriLib::Sound::Error KolibriLib::Sound::Buffer::Stop()
{
	return StopBuffer(_hBuff);
}