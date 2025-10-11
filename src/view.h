/*
 * app_view.h Copyright 2025 Alwin Leerling dna.leerling@gmail.com
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

#include <gtkmm.h>

#include "vertex.h"

class View : public Gtk::GLArea
{
public:
	View();
	~View();

	void set_shader_filename( std::string name ) { shader_filename = name; };
	void set_background( Colour colour ) { background = colour; };

	void set_vertex_data( const std::vector<Vertex>& vertices );

private:
	unsigned int vao;
	unsigned int vbo;
	unsigned int program_id;
	bool initialised = false;
	std::string shader_filename;
	Colour background;

	std::vector<Vertex> vertex_data;

	void realise( );
	void resize( int width, int height );
	void render( const Glib::RefPtr<Gdk::GLContext>& ctx );

	void render_background();

	void set_attributes();
	void set_content( );
};

