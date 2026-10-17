/*
 * gtk_demo.cc Copyright 2024 Alwin Leerling dna.leerling@gmail.com
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

#include <vector>
#include <string>
#include <type_traits>

#include <gtkmm.h>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>

#include "shader_utils.h"

struct Attribute
{
	GLuint index;
	GLint size;
	GLenum type;
	GLboolean normalized;
	GLsizei stride;
	std::size_t offset;
};

using Point3D = std::array<GLfloat,3>;
using Colour  = std::array<GLfloat,3>;

struct Vertex
{
	Vertex( Point3D pos, Colour colour ) : position(pos), colour(colour) {
		static_assert( std::is_standard_layout_v<Vertex> );
		static_assert( std::is_trivially_copyable_v<Vertex> );
	}

	static std::vector<Attribute> get_layout() {
		return {
			{ 0, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), offsetof( Vertex, position ), },
			{ 1, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), offsetof( Vertex, colour   ), }
		};
	}

	void set_colour( Colour new_colour ) { colour = new_colour; }

private:
	Point3D position;
	Colour colour;
};

class Model
{
public:
	Model() : vertices(
		{
			Vertex( { -1.0F, -1.0F, 0.0F }, { 1.0F, 0.0F, 0.0F } ),
			Vertex( {  0.0F,  1.0F, 0.0F }, { 0.0F, 1.0F, 0.0F } ),
			Vertex( {  1.0F, -1.0F, 0.0F }, { 0.0F, 0.0F, 1.0F } )
		}
	) {}

	bool update( double elapsed_time );

	std::string get_shader_filename() const {	return "../res/shaders/simple.glsl"; }
	Colour get_background()	const { return { 0.0F, 0.0F, 0.6F }; }

	const std::vector<Attribute> get_vertex_layout() const { return Vertex::get_layout(); };
	const std::vector<Vertex>& get_vertices() const { return vertices; }
	size_t get_vertex_count() const { return vertices.size(); }

private:
	std::vector<Vertex> vertices;
	double accumulated = 0;
	int round = 0;
};

bool Model::update( double elapsed_time )
{
	accumulated += elapsed_time;
	if( accumulated < 500 )
		return false;

	accumulated = 0;

	round = (round + 1) % 3;

	switch( round ) {
	case 0:
		vertices[0].set_colour( { 1.0F, 0.0F, 0.0F} );
		vertices[1].set_colour( { 0.0F, 1.0F, 0.0F} );
		vertices[2].set_colour( { 0.0F, 0.0F, 1.0F} );
		break;
	case 1:
		vertices[0].set_colour( { 0.0F, 0.0F, 1.0F} );
		vertices[1].set_colour( { 1.0F, 0.0F, 0.0F} );
		vertices[2].set_colour( { 0.0F, 1.0F, 0.0F} );
		break;
	case 2:
		vertices[0].set_colour( { 0.0F, 1.0F, 0.0F} );
		vertices[1].set_colour( { 0.0F, 0.0F, 1.0F} );
		vertices[2].set_colour( { 1.0F, 0.0F, 0.0F} );
		break;
	}

	return true;
}

class View : public Gtk::GLArea
{
public:
	View( Model& _model ) : model( _model ), shader_filename( model.get_shader_filename() ), background( model.get_background() )
	{
		set_use_es( false);
		set_required_version( 3, 3 );
		set_auto_render();
	}

	~View() = default;

	void upload_vertex_data( );

private:
	unsigned int vao = 0;
	unsigned int vbo = 0;
	unsigned int program_id = 0;

	Model& model;
	std::string shader_filename;
	Colour background;

	void on_realize() override;
	void on_unrealize() override;

	bool on_render( const Glib::RefPtr<Gdk::GLContext> & context ) override;
	void on_resize( int width, int height ) override { glViewport( 0, 0, width, height ); }
};

 class Controller : public Gtk::ApplicationWindow
 {
 public:
	 Controller();

 private:
	 Model model;
};

int main( int argc, char * argv[] )
{
	Glib::set_init_to_users_preferred_locale( false );

	auto app = Gtk::Application::create();

	app->make_window_and_run<Controller>( argc, argv );
}

Controller::Controller() : Gtk::ApplicationWindow()
{
	set_title( "Hello from GTK GLArea");
	set_default_size( 640, 480 );

	auto view = Gtk::make_managed<View>( model );

	set_child( *view );

	auto update_func = [this, view]( const Glib::RefPtr<Gdk::FrameClock> &frame_clock )
	{
		gint64 current_time = frame_clock->get_frame_time();
		static gint64 previous_time = current_time;

		double dt = (current_time - previous_time) / 1'000.0;

		if( model.update( dt ) )
			view->upload_vertex_data( );

		previous_time = current_time;
		return true;
	};

    add_tick_callback( update_func );
}

void View::on_realize()
{
	Gtk::GLArea::on_realize();

	make_current();

	program_id = make_program( shader_filename );
	glUseProgram( program_id );

	glGenVertexArrays( 1, &vao );
	glBindVertexArray( vao );

	glGenBuffers( 1, &vbo );
	glBindBuffer( GL_ARRAY_BUFFER, vbo );

    for( const auto& attr : model.get_vertex_layout() ) {

        glEnableVertexAttribArray( attr.index );
        glVertexAttribPointer( attr.index, attr.size, attr.type, attr.normalized, attr.stride, (void*)attr.offset );
    }

	const auto& vertices = model.get_vertices();

	glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glBufferData( GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_DYNAMIC_DRAW );
}

void View::on_unrealize()
{
	make_current();

	if( vao ) glDeleteVertexArrays( 1, &vao );
	if( vbo ) glDeleteBuffers( 1, &vbo );
	if( program_id ) glDeleteProgram( program_id );

    vao = 0;
    vbo = 0;
    program_id = 0;

	Gtk::GLArea::on_unrealize();
}

bool View::on_render( const Glib::RefPtr<Gdk::GLContext> &context )
{
    if( !program_id || !vao )
        return true;

	auto [red, green, blue] = background;

	glClearColor( red, green, blue, 1.0F );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glBindVertexArray( vao );

	glDrawArrays( GL_TRIANGLES, 0, model.get_vertex_count() );

	return true;
}

void View::upload_vertex_data(  )
{
    if( !vbo )
        return;

	make_current();

	const auto& vertices = model.get_vertices();

	glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glBufferData( GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_DYNAMIC_DRAW );

	queue_render();
}
