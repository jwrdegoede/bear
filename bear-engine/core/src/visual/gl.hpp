/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Includes the OpenGL headers with the right configuration.
 * \author Julien Jorge
 */
#ifndef __VISUAL_GL_HPP__
#define __VISUAL_GL_HPP__

#define GL_GLEXT_PROTOTYPES 1

#if defined( _WIN32 )

#include "glew/glew.h"

#elif defined( __ANDROID__ )

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

typedef GLfloat GLdouble;

#else

#include <GL/gl.h>
#include <GL/glext.h>

#endif

#endif // __VISUAL_GL_HPP__
