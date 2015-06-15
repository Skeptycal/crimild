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

#ifndef CRIMILD_SIMULATION_SYSTEM_
#define CRIMILD_SIMULATION_SYSTEM_

#include "Foundation/SharedObject.hpp"
#include "Foundation/NamedObject.hpp"
#include "Foundation/Memory.hpp"

#include "Messaging/MessageQueue.hpp"

#include "Simulation/Task.hpp"

#include <string>

namespace crimild {

	class System;

	using SystemPtr = SharedPointer< System >;

	namespace messages {

		struct SystemWillStart {
			SystemPtr system;
		};

		struct SystemWillStop {
			SystemPtr system;
		};

	}

	class System : 
		public SharedObject,
		public NamedObject,
		public Messenger {

		CRIMILD_DISALLOW_COPY_AND_ASSIGN( System )

	public:
		class Updater : public Task {
		public:
			Updater( System *system )
				: _system( system )
			{
				setRepeatMode( Task::RepeatMode::REPEAT );
			}

			virtual ~Updater( void )
			{

			}

			virtual void run( void ) override 
			{
				_system->update();
			}

		private:
			System *_system;
		};

		using UpdaterPtr = SharedPointer< Updater >;

	public:
		explicit System( std::string name );

		virtual ~System( void );

		virtual bool start( void );

		virtual void stop( void );

		virtual void update( void );

		void enableUpdater( void );

		UpdaterPtr getUpdater( void ) { return _updater; }

	private:
		UpdaterPtr _updater;
	};

}

#endif

