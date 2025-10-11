/*
 * vertex.h Copyright 2025 Alwin Leerling dna.leerling@gmail.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

 #pragma once

#include <vector>
#include <array>
#include <string>
#include <tuple>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>

using Point3D = std::array<GLfloat,3>;
using Colour =  std::array<GLfloat,3>;

class Vertex
{
public:
	Vertex( Point3D pos, Colour colour ) : position(pos), colour(colour) {}

	struct Attribute {
		GLuint index;
		GLint size;
		GLenum type;
		GLboolean normalized;
		GLsizei stride;
		std::size_t offset;
	};

	std::vector<GLfloat> get_content() const;
	static std::vector<Attribute> get_layout();

	void set_colour( Colour new_colour ) { colour = new_colour; }

private:
	Point3D position;
	Colour colour;
};

