#include <kolibriLib/types.hpp>
#include <kolibriLib/debug.hpp>
#include <assert.h>


int main()
{
	assert(KolibriLib::point(4) == KolibriLib::point(4, 4));

	assert((KolibriLib::point(4, 4) + KolibriLib::point(4, 4)) == KolibriLib::point(8, 8));

	{
		ksys_pos_t p;
		p.x = 4;
		p.y = 2;

		
		KolibriLib::point a(4, 2);

		assert(a == KolibriLib::point(p));



		assert(a == p);
	}



	KolibriLib::logger << microlog::LogLevel::Info << "test point_test is OK" << std::endl;

	return 0;
}
