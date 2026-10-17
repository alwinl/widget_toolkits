#
# CMakeLists.txt Copyright 2024 Alwin Leerling dna.leerling@gmail.com
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
# MA 02110-1301, USA.
#

find_package(PkgConfig REQUIRED)

pkg_check_modules( GTKMM REQUIRED gtkmm-4.0 )

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)

add_executable(
	gtk_demo

	gtk_demo.cc
)

target_include_directories( gtk_demo PRIVATE ${GTKMM_INCLUDE_DIRS})
target_link_libraries(gtk_demo PRIVATE shader_utils ${GTKMM_LIBRARIES} ${OPENGL_LIBRARIES} )
