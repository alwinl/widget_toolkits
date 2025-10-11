/*
 * controller.cc Copyright 2025 Alwin Leerling dna.leerling@gmail.com
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

#include "controller.h"

Controller::Controller() : Gtk::ApplicationWindow()
{
	set_title( "Hello from GTK GLArea");
	set_default_size( 640, 480 );

	set_child( view );

	view.set_shader_filename( model.get_shader_filename() );
	view.set_background( model.get_background() );

	auto update_func = [this]( const Glib::RefPtr<Gdk::FrameClock> &frame_clock )
	{
		static gint64 previous_time = 0;
		gint64 current_time = frame_clock->get_frame_time();

		if( previous_time )
			update( (current_time - previous_time) / 1'000.0 );

		previous_time = current_time;
		return true;
	};

    add_tick_callback( update_func );
}

void Controller::update( double elapsed_time )
{
	if( model.update( elapsed_time ) )
		view.set_vertex_data( model.get_vertices() );
}
