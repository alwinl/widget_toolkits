/*
 * app_view.cc Copyright 2025 Alwin Leerling dna.leerling@gmail.com
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

#include "view.h"

#include "shader_utils.h"

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>

View::View ()
{
	set_use_es( false);
	set_required_version( 3, 2 );

	signal_resize().connect( [this]( int width, int height ) { resize(width, height); }, false );
	signal_render().connect( [this]( const Glib::RefPtr<Gdk::GLContext>& ctx ) { render( ctx); return true; }, false );

	set_auto_render();
}

View::~View()
{
    glDeleteVertexArrays( 1, &vao );
    glDeleteBuffers( 1, &vbo );
    glDeleteProgram( program_id );
}

void View::realise()
{
	if( shader_filename.empty() )
		return;

	make_current();

	program_id = make_program( shader_filename );
	glUseProgram( program_id );

	glGenVertexArrays( 1, &vao );
	glBindVertexArray( vao );

	glGenBuffers( 1, &vbo );
	glBindBuffer( GL_ARRAY_BUFFER, vbo );

	set_attributes();
	set_content();

	initialised = true;
}

void View::set_vertex_data( const std::vector<Vertex>& vertices )
{
	vertex_data = vertices;

	if( !initialised ) {

		if( !get_realized() )
			signal_realize().connect( [this](){ realise(); } );
		else
			realise();

		return;
	}

	set_content();
}

void View::set_attributes()
{
    for( const auto& attr : Vertex::get_layout() )
	{
        glEnableVertexAttribArray( attr.index );
        glVertexAttribPointer( attr.index, attr.size, attr.type, attr.normalized, attr.stride, (void*)attr.offset );
    }
}

void View::set_content( )
{
	std::vector<GLfloat> raw_data;

    for( const auto& vertex : vertex_data )
	{
        std::vector<GLfloat> content = vertex.get_content();
        raw_data.insert( raw_data.end(), content.begin(), content.end() );
    }

	glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glBufferData( GL_ARRAY_BUFFER, raw_data.size() * sizeof(GLfloat), raw_data.data(), GL_STATIC_DRAW );

	queue_render();
}

void View::resize( int width, int height )
{
	glViewport( 0, 0, width, height );
}

void View::render( const Glib::RefPtr<Gdk::GLContext>& ctx )
{
	render_background();

	glBindVertexArray( vao );

	glDrawArrays( GL_TRIANGLES, 0, vertex_data.size() );
}

void View::render_background()
{
	auto [red, green, blue] = background;

	glClearColor( red, green, blue, 1.0F );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}
