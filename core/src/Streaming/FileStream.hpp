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

#ifndef CRIMILD_CORE_STREAMING_STREAM_FILE_
#define CRIMILD_CORE_STREAMING_STREAM_FILE_

#include "Stream.hpp"

namespace crimild {
    
    /**
        \brief Implements a stream that uses files
    */
    class FileStream : public Stream {
    public:
        enum class OpenMode {
            READ,
            WRITE
        };

    public:
    	explicit FileStream( std::string path, OpenMode openMode );
    	virtual ~FileStream( void );

    	virtual bool load( void ) override;

    	virtual bool flush( void ) override;

    public:
        bool open( void );

        bool isOpen( void ) const { return _file != nullptr; }

        bool close( void );

        virtual void writeRawBytes( const void *bytes, size_t size ) override;
        virtual void readRawBytes( void *bytes, size_t size ) override;

    private:
        std::string _path;
        OpenMode _openMode;
        FILE *_file = nullptr;
    };

}

#endif

