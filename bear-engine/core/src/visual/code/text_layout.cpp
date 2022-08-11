/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the template methods of bear::visual::text_layout.
 * \author Julien Jorge
 */
#include "visual/text_layout.hpp"

#include <claw/assert.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param f The font used to display the text.
 * \param str The text to arrange.
 * \param s The size of the box around the text.
 * \param h The horizontal alignment of the lines.
 *
 * The instance keep the references on each parameter. So they must live longer
 * than \a this.
 */
bear::visual::text_layout::text_layout
( const font& f, const std::string& str, const size_box_type& s,
  text_align::horizontal_align h )
  : m_size(s), m_text(str), m_font(f), m_horizontal_align(h)
{

} // text_layout::text_layout()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute the left origin of a line of text.
 * \param first The index in m_text of the first character of the text on the
 *        line.
 */
bear::visual::coordinate_type
bear::visual::text_layout::compute_line_left( std::size_t first ) const
{
  if ( m_horizontal_align == text_align::align_left )
    return 0;

  const size_type line_width( compute_line_width(first) );

  // The remaining space on the line.
  coordinate_type result( m_size.x - line_width );

  if ( m_horizontal_align == text_align::align_center )
    result /= 2;

  return result;
} // text_layout::compute_line_left()

/*----------------------------------------------------------------------------*/
/**
 * \brief Computes the width of the text that can be displayed on the line.
 *
 * The method sums the width of the characters starting from the provided index,
 * minus the spaces that would be at the end or the beginning of the line.
 * 
 * \param first The index in m_text of the first character of the text on the
 *        line.
 */
bear::visual::size_type
bear::visual::text_layout::compute_line_width( std::size_t first ) const
{
  const std::string::size_type text_length( m_text.length() );

  // The line begins on the first non space character.
  first = m_text.find_first_not_of( ' ', first );

  if ( first == std::string::npos )
    first = text_length;

  std::string::size_type last(first);

  size_type result(0);
  size_type candidate_length(0);
  size_type width(0);

  // The line ends on the last space character such that the next word would not
  // fit on the line.
  std::string::size_type last_space_sequence( std::string::npos );

  while ( (last != text_length) && (m_text[last] != '\n') )
    {
      if ( m_text[last] == ' ' )
        {
          if ( last_space_sequence == std::string::npos )
            {
              last_space_sequence = last;
              result = candidate_length;
            }
        }
      else
        last_space_sequence = std::string::npos;

      width = m_font.get_metrics( m_text[last] ).get_advance().x;
      if ( (candidate_length + width) > m_size.x )
        break;

      candidate_length += width;
      ++last;
    }

  // if we stopped in the middle of a word or at the end of the line, then we
  // must adjust the result from the candidate length.
  if ( last_space_sequence == std::string::npos )
    {
      // if we stopped at the end of line then the entire line fits
      if ( (last == text_length) || (m_text[last] == '\n') )
        result = candidate_length;
      // else if we did not find a space to break at, use whatever fits
      else if ( result == 0 )
        result = candidate_length;
    }

  CLAW_POSTCOND( result >= 0 );
  CLAW_POSTCOND( result <= m_size.x );

  return result;
} // text_layout::compute_line_width()

/*----------------------------------------------------------------------------*/
/**
 * \brief Computes the height of the part above the baseline of a line of text.
 * \param first The index in m_text of the first character of the text on the
 *        line.
 */
bear::visual::size_type
bear::visual::text_layout::compute_line_height_above_baseline
( std::size_t first ) const
{
  const std::string::size_type text_length( m_text.length() );

  // The line begins on the first non space character.
  first = m_text.find_first_not_of( ' ', first );

  if ( first == std::string::npos )
    first = text_length;

  size_type result(0);
  size_type line_width(0);
  bool stop(false);

  while ( !stop )
    if ( (first == m_text.length()) || (m_text[first] == '\n') )
      stop = true;
    else
      {
        const glyph_metrics m( m_font.get_metrics( m_text[first] ) );

        line_width += m.get_advance().x;

        if ( line_width > m_size.x )
          stop = true;
        else
          {
            result =
              std::max
              ( result,
                m_font.get_sprite( m_text[first] ).height()
                + m.get_bearing().y );
            ++first;
          }
      }

  return result;
} // text_layout::compute_line_height_above_baseline()
