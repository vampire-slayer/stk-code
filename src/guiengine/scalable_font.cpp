//
//  SuperTuxKart - a fun racing game with go-kart
//  Copyright (C) 2016 SuperTuxKart-Team
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 3
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#include "guiengine/scalable_font.hpp"

#include "font/font_settings.hpp"
#include "font/font_with_face.hpp"
#include "utils/translation.hpp"

namespace irr
{
namespace gui
{
// ----------------------------------------------------------------------------
ScalableFont::ScalableFont(FontWithFace* face)
{
    m_face = face;
    m_font_settings = new FontSettings(false/*black_border*/,
        translations->isRTLLanguage());
}   // ScalableFont

// ----------------------------------------------------------------------------
ScalableFont::~ScalableFont()
{
    delete m_font_settings;
}   // ~ScalableFont

// ----------------------------------------------------------------------------
void ScalableFont::updateRTL()
{
    m_font_settings->setRTL(translations->isRTLLanguage());
}   // updateRTL

// ----------------------------------------------------------------------------
void ScalableFont::setShadow(const irr::video::SColor &col)
{
    m_font_settings->setShadow(true);
    m_font_settings->setShadowColor(col);
}   // setShadow

// ----------------------------------------------------------------------------
void ScalableFont::disableShadow()
{
    m_font_settings->setShadow(false);
}   // setShadow

// ----------------------------------------------------------------------------
void ScalableFont::setScale(float scale)
{
    m_font_settings->setScale(scale);
}   // setScale

// ----------------------------------------------------------------------------
float ScalableFont::getScale() const
{
    return m_font_settings->getScale();
}   // getScale

// ----------------------------------------------------------------------------
core::dimension2d<u32> ScalableFont::getDimension(const wchar_t* text) const
{
    return m_face->getDimension(text, m_font_settings);
}   // getDimension

// ----------------------------------------------------------------------------
void ScalableFont::draw(const core::stringw& text,
                        const core::rect<s32>& position, video::SColor color,
                        bool hcenter, bool vcenter,
                        const core::rect<s32>* clip)
{
    m_face->render(text, position, color, hcenter, vcenter, clip,
        m_font_settings);
}   // draw

// ----------------------------------------------------------------------------
void ScalableFont::draw(const core::stringw& text,
                        const core::rect<s32>& position,
                        const video::SColor& color, bool hcenter, bool vcenter,
                        const core::rect<s32>* clip, bool ignoreRTL)
{
    bool previousRTL = m_font_settings->isRTL();
    if (ignoreRTL)
        m_font_settings->setRTL(false);

    m_face->render(text, position, color, hcenter, vcenter, clip,
        m_font_settings);

    if (ignoreRTL)
        m_font_settings->setRTL(previousRTL);

}   // draw

// ----------------------------------------------------------------------------
s32 ScalableFont::getCharacterFromPos(const wchar_t* text, s32 pixel_x) const
{
    return m_face->getCharacterFromPos(text, pixel_x, m_font_settings);
}   // getCharacterFromPos

// ----------------------------------------------------------------------------
IGUISpriteBank* ScalableFont::getSpriteBank() const
{
    return m_face->getSpriteBank();
}   // getSpriteBank

// ------------------------------------------------------------------------
u32 ScalableFont::getSpriteNoFromChar(const wchar_t *c) const
{
    const FontWithFace::FontArea& area =
        m_face->getAreaFromCharacter(*c, NULL/*fallback_font*/);
    return area.spriteno;
}   // getSpriteNoFromChar

// ------------------------------------------------------------------------
u32 ScalableFont::addLazyLoadCharacters(const wchar_t *c)
{
    IGUISpriteBank* sp = m_face->getSpriteBank();
    u32 tex_count = sp->getTextureCount();
    if (!m_face->supportLazyLoadChar()) return tex_count;

    m_face->insertCharacters(c);
    m_face->updateCharactersList();

    // Make sure text of billboard wasn't located in the last texture
    for (const wchar_t p = *c; *c; c++)
    {
        const core::array<SGUISprite>& s = sp->getSprites();
        const FontWithFace::FontArea& area =
            m_face->getAreaFromCharacter(p, NULL/*fallback_font*/);
        u32 cur_tex_no = s[area.spriteno].Frames[0].textureNumber;
        if (cur_tex_no == (tex_count -1))
        {
            m_face->createNewGlyphPage();
            return tex_count;
        }
    }

    return tex_count -1;

}   // addLazyLoadCharacters

} // end namespace gui
} // end namespace irr
