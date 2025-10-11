/*
 * model.h Copyright 2025 Alwin Leerling dna.leerling@gmail.com
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
#include <string>

#include "vertex.h"

class Model
{
public:
	Model();

	bool update( double elapsed_time );

	std::vector<Vertex>& get_vertices() { return vertices; }

	Colour get_background()	{ return { 0.0F, 0.0F, 0.6F }; }
	std::string get_shader_filename() {	return "../res/shaders/simple.glsl"; }

private:
	std::vector<Vertex> vertices;
};
