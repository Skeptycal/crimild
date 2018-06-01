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

#include "CameraSortParticleUpdater.hpp"

#include "SceneGraph/Camera.hpp"

using namespace crimild;

CameraSortParticleUpdater::CameraSortParticleUpdater( void )
{

}

CameraSortParticleUpdater::~CameraSortParticleUpdater( void )
{

}

void CameraSortParticleUpdater::configure( Node *node, ParticleData *particles )
{
	_positions = particles->createAttribArray< Vector3f >( ParticleAttrib::POSITION );
}

void CameraSortParticleUpdater::update( Node *node, double dt, ParticleData *particles )
{
	const auto camera = Camera::getMainCamera();	
	auto cameraPos = camera->getWorld().getTranslate();
	auto cameraDirection = camera->getWorld().computeDirection();
	if ( !particles->shouldComputeInWorldSpace() ) {
		// compute local camera pos only if we're not using
		// the world space
		node->getWorld().applyInverseToPoint( cameraPos, cameraPos );
		node->getWorld().applyInverseToVector( cameraDirection, cameraDirection );
	}

	const auto pCount = particles->getAliveCount();

	const auto ps = _positions->getData< Vector3f >();

	const auto cameraPlane = Plane3f( cameraDirection, cameraPos );

	// precompute distances to camera plane for each particle
	containers::Array< double > ds( pCount );
	for ( crimild::Size i = 0; i < pCount; i++ ) {
		ds[ i ] = Distance::compute( cameraPlane, ps[ i ] );
	}

	// sort partices
	// TODO: I know, bubble sort is slow...
	for ( int i = 1; i < pCount; i++ ) {
		for ( int j = 0; j < pCount - i; j++ ) {
			if ( ds[ j ] < ds[ j + 1 ] ) {
				particles->swap( j, j + 1 );
				Numericd::swap( ds[ j ], ds[ j + 1 ] );
			}
		}
	}
}

void CameraSortParticleUpdater::encode( coding::Encoder &encoder ) 
{
	ParticleSystemComponent::ParticleUpdater::encode( encoder );
}

void CameraSortParticleUpdater::decode( coding::Decoder &decoder )
{
	ParticleSystemComponent::ParticleUpdater::decode( decoder );
}

