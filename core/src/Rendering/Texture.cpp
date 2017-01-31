/*
 * Copyright (c) 2013, Hernan Saez
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the <organization> nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "Texture.hpp"

#include "Foundation/Log.hpp"

CRIMILD_REGISTER_STREAM_OBJECT_BUILDER( crimild::Texture )

using namespace crimild;

Texture::Texture( std::string name )
    : NamedObject( name )
{
    
}

Texture::Texture( SharedPointer< Image > const &image, std::string name )
	: NamedObject( name ),
	  _image( image )
{

}

Texture::~Texture( void )
{
    unload();
}

bool Texture::registerInStream( Stream &s )
{
	if ( !StreamObject::registerInStream( s ) ) {
		return false;
	}

	if ( getImage() != nullptr ) {
		getImage()->registerInStream( s );
	}

	return true;
}

void Texture::save( Stream &s )
{
	StreamObject::save( s );

	s.write( _image );
}

void Texture::load( Stream &s )
{
	StreamObject::load( s );

	s.read( _image );
}

