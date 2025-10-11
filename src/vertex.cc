/*
 * vertex.cc Copyright 2025 Alwin Leerling dna.leerling@gmail.com
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


 #include "vertex.h"

 std::vector<GLfloat> Vertex::get_content() const
 {
	std::vector<GLfloat> vertex_data( position.begin(), position.end() );

	vertex_data.insert( vertex_data.end(), colour.begin(), colour.end() );

	return vertex_data;
 }

 std::vector<Vertex::Attribute> Vertex::get_layout()
 {
	return {
		{
			.index = 0,
			.size = 3,
			.type = GL_FLOAT,
			.normalized = GL_FALSE,
			.stride = sizeof( Vertex ),
			.offset = offsetof( Vertex, position ),
		},
		{
			.index = 1,
			.size = 3,
			.type = GL_FLOAT,
			.normalized = GL_FALSE,
			.stride = sizeof( Vertex ),
			.offset = offsetof( Vertex, colour ),
		}
	};
 }

// std::string Vertex::get_shader_filename()
// {
// 	return "../res/shaders/simple.glsl";
// }

// std::tuple<float, float, float> Vertex::get_background()
// {
// 	return std::make_tuple( 0.0F, 0.0F, 0.6F );
// }
