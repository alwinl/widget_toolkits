/*
 * model.cc Copyright 2025 Alwin Leerling dna.leerling@gmail.com
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

#include "model.h"

Model::Model()
{
	vertices.insert( vertices.begin(),
		{
			Vertex( { -1.0F, -1.0F, 0.0F }, { 1.0F, 0.0F, 0.0F } ),
			Vertex( {  0.0F,  1.0F, 0.0F }, { 0.0F, 1.0F, 0.0F } ),
			Vertex( {  1.0F, -1.0F, 0.0F }, { 0.0F, 0.0F, 1.0F } )
		}
	);
}

bool Model::update( double elapsed_time )
{
	static double accumulated = 1000;

	accumulated += elapsed_time;
	if( accumulated < 100 )
		return false;

	accumulated = 0;

	static int round = 0;
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
