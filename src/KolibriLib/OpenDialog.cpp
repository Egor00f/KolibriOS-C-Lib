#include <kolibriLib/OpenDialog.hpp>
#include <cstring>

using namespace KolibriLib;

KolibriLib::OpenDialog::OpenDialog(Modes mode, Size winSize, Coord winCoord, const std::vector<FilterElement> &filter, const filesystem::Path &defaultPath)
{
	_opendialog = (open_dialog *)malloc(sizeof(open_dialog));

	_opendialog	->	mode	= mode;
	_opendialog	->	x_size	= winSize.x;
	_opendialog	->	y_size	= winSize.x;
	_opendialog	->	x_start	= winCoord.x;
	_opendialog	->	y_start	= winCoord.y;
	_opendialog	->	status	= 0;
	_opendialog	->	com_area	= 0;
	_opendialog	->	procinfo	= new char[1024];
	_opendialog	->	start_path	= (char*)sz_start_path;
	_opendialog	->	filter_area	= FilterArea::Create(filter);
	_opendialog	->	draw_window	= &fake_on_redraw;
	_opendialog	->	opendir_path	= new char[4096];
	_opendialog	->	openfile_path	= new char[4096];
	_opendialog	->	filename_area	= new char[256];
	_opendialog	->	com_area_name	= (char*)sz_com_area_name;
	_opendialog	->	dir_default_path	= new char[defaultPath.operator std::string().size()];
	
	for (std::size_t i = 0; i < defaultPath.length(); i++)
	{
		_opendialog->dir_default_path[i] = defaultPath.operator std::string()[i];
	}
}

KolibriLib::OpenDialog::~OpenDialog()
{
	delete _opendialog->dir_default_path;
	delete _opendialog->openfile_path;
	delete _opendialog->opendir_path;
	delete _opendialog->procinfo;
	delete _opendialog->filename_area;

	free(_opendialog->filter_area);
	free(_opendialog);
}

KolibriLib::OpenDialog::Status KolibriLib::OpenDialog::Open()
{
	OpenDialog_start((open_dialog*)_opendialog);

	return (Status)_opendialog->status;
}

filesystem::Path KolibriLib::OpenDialog::GetPath() const
{
	if( _opendialog->status == Status::Opened)
	{
		if (_opendialog->mode == Mode::OpenFile || _opendialog->mode == Mode::Save)
		{
			return _opendialog->filename_area;
		}
		else if (_opendialog->mode == Mode::Select)
		{
			return _opendialog->opendir_path;
		}
	}

	return PathNotSet;
}

void KolibriLib::OpenDialog::SetFilter(const std::vector<FilterElement> &f)
{
	_opendialog->filter_area = FilterArea::Create(f);
}

KolibriLib::OpenDialog::FilterArea::FilterArea()
{
	area = (od_filter *)malloc(sizeof(od_filter));

	area->size = 0;
	area->end = 0;
}

KolibriLib::OpenDialog::FilterArea::FilterArea(const std::vector<FilterElement> &elements)
{
	area = Create(elements);
}

KolibriLib::OpenDialog::FilterArea::~FilterArea()
{
	free(area);
}

od_filter* KolibriLib::OpenDialog::FilterArea::Create(const std::vector<FilterElement> &elements)
{
	/* наверно фильтры добавляются как то так

	Код для ASSЕМБЛЕРА(канешн фасм):
	Filter:
		dd	Filter.end - Filter
	.1:
		db	'TXT',0
		db	'ASM',0
		db	'INC',0
		db	'MAC',0
		db	'DBG',0
		db	'SH',0

	.end:
		db	0
	Поесть фильтр это

	Размер структуры в байтах
	3 символа расширение + 0
	конец 0

	*/

	od_filter* a;

	if (elements.size() > 0)
	{
		// Размер филт
		std::size_t s = 0;

		for (std::string i : elements)
		{
			s += i.length();
		}

		a = (od_filter *)malloc(4 + s + 1); // 4 на size, s на сами фильтры, 1 на 0 в конце

		a->size = s;

		std::size_t i = 4;
		for (; i / 4 < a->size;)
		{
			for (uint8_t j = 0; j < elements[i].size(); j++)
			{
				((char *)a)[i + j] = elements[i][j];
			}

			i += elements[i].size();
		}

		((char *)a)[i] = 0; // в конце нолик
	}
	else
	{
		a = (od_filter *) malloc(sizeof(od_filter));
		a->size = 0;
		a->end = 0;
	}

	return a;
}