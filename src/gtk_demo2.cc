/*
 * gtk_demo.cc Copyright 2025 Alwin Leerling dna.leerling@gmail.com
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

#include <gtkmm.h>

#include "shader_utils.h"

bool realise_view( Gtk::GLArea * view, DemoScene& scene )
{
	view->on_realize();

	view->make_current();

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

Gtk::GLArea * make_view( DemoScene& scene)
{
	Gtk::GLArea* view = Gtk::make_managed<Gtk::GLArea>();

	auto render_func = [&scene, view]( const Glib::RefPtr<Gdk::GLContext>& ctx )
	{
		scene.render_scene( view->get_allocated_width(), view->get_allocated_height() );
		return true;
	};

	auto realise_func = [&scene, view](){ view->make_current(); scene.make_scene(); };

	view->set_use_es( false);
	view->set_required_version( 3, 3 );
	view->set_auto_render();
    view->set_size_request(400, 400);
    view->signal_realize().connect(realise_func, false );
	view->signal_render().connect(render_func, false );

	return view;
}

Gtk::ApplicationWindow * make_window( DemoScene& scene, Gtk::GLArea * view )
{
	Gtk::ApplicationWindow * window = Gtk::make_managed<Gtk::ApplicationWindow>();

	auto update_func = [&scene, view]( const Glib::RefPtr<Gdk::FrameClock> &frame_clock )
	{
		if( scene.update() )
			view->queue_render();

		return true;
	};

	window->set_child( *view );
	window->add_tick_callback( update_func );
	window->set_title( "GLCanvas App" );
	window->present();

	return window;
}

int main(int argc, char** argv)
{
	Glib::set_init_to_users_preferred_locale(false);

	auto app = Gtk::Application::create();

	DemoScene scene;

	app->signal_activate().connect( [app, &scene]()
	{
		auto view = make_view( scene );
		auto window = make_window( scene, view );

		app->add_window( *window );
	});

	return app->run(argc, argv);
}
