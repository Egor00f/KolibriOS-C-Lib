#include <kolibriLib/UI/buttons/ImageButton.hpp>

using namespace KolibriLib;
using namespace UI;
using namespace buttons;


void KolibriLib::UI::buttons::ImageButton::Render() const
{
    Define(GetAbsoluteCoord(), GetAbsoluteCoord());
    Draw(GetAbsoluteCoord());
}