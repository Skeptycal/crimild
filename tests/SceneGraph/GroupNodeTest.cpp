/**
 * Crimild Engine is an open source scene graph based engine which purpose
 * is to fulfill the high-performance requirements of typical multi-platform
 * two and tridimensional multimedia projects, like games, simulations and
 * virtual reality.
 *
 * Copyright (C) 2006-2013 Hernan Saez - hhsaez@gmail.com
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include <Crimild.hpp>

#include "gtest/gtest.h"

using namespace Crimild;

TEST( GroupNodeTest, construction)
{
	GroupNodePtr group( new GroupNode( "a group" ) );

	EXPECT_EQ( group->getName(), "a group" );
	EXPECT_FALSE( group->hasNodes() );
}

TEST( GroupNodeTest, destruction )
{
	NodePtr child1( new Node( "child 1" ) );
	NodePtr child2( new Node( "child 2" ) );

	{
		GroupNodePtr parent( new GroupNode( "parent" ) );

		parent->attachNode( child1 );
		EXPECT_TRUE( child1->hasParent() );
		EXPECT_EQ( child1->getParent(), parent.get() );

		parent->attachNode( child2 );
		EXPECT_TRUE( child2->hasParent() );
		EXPECT_EQ( child2->getParent(), parent.get() );
	}

	EXPECT_FALSE( child1->hasParent() );
	EXPECT_EQ( child1->getParent(), nullptr );
	EXPECT_FALSE( child2->hasParent() );
	EXPECT_EQ( child2->getParent(), nullptr );
}

TEST( GroupNodeTest, attachNode )
{
	GroupNodePtr parent( new GroupNode( "parent" ) );

	EXPECT_FALSE( parent->hasNodes() );

	NodePtr child( new Node( "child" ) );
	EXPECT_FALSE( child->hasParent() );
	EXPECT_EQ( child->getParent(), nullptr );

	parent->attachNode( child );

	EXPECT_TRUE( child->hasParent() );
	ASSERT_EQ( child->getParent(), parent.get() );
	EXPECT_TRUE( parent->hasNodes() );

	bool found = false;
	int count = 0;
	parent->foreachNode( [&]( NodePtr node ) mutable {
		++count;
		if ( node == child ) {
			found = true;
		}
	});

	EXPECT_EQ( count, 1 );
	EXPECT_TRUE( found );
}

TEST( GroupNodeTest, attachMultipleNodes )
{
	GroupNodePtr parent( new GroupNode( "parent" ) );
	
	NodePtr child1( new Node( "child 1" ) );
	NodePtr child2( new Node( "child 2" ) );
	NodePtr child3( new Node( "child 3" ) );

	parent->attachNode( child1 );
	parent->attachNode( child2 );
	parent->attachNode( child3 );

	EXPECT_TRUE( parent->hasNodes() );

	int count = 0;
	parent->foreachNode( [&]( NodePtr node ) mutable {
		++count;
	});
	EXPECT_EQ( count, 3 );
}

TEST( GroupNodeTest, reattachNodeToSameParent )
{
	GroupNodePtr parent( new GroupNode( "parent" ) );
	NodePtr child( new Node( "child" ) );

	parent->attachNode( child );

	EXPECT_NO_THROW( parent->attachNode( child ) );

	bool found = false;
	int count = 0;
	parent->foreachNode( [&]( NodePtr node ) mutable {
		++count;
		if ( node == child ) {
			found = true;
		}
	});

	EXPECT_EQ( count, 1 );
	EXPECT_TRUE( found );
}

TEST( GroupNodeTest, reattachNodeToDifferentParent )
{
	GroupNodePtr parent1( new GroupNode( "parent" ) );
	GroupNodePtr parent2( new GroupNode( "another parent" ) );
	NodePtr child( new Node( "child" ) );

	parent1->attachNode( child );

	ASSERT_THROW( parent2->attachNode( child ), HasParentException );
}

TEST( GroupNodeTest, detachNode )
{
	GroupNodePtr parent( new GroupNode( "parent" ) );
	NodePtr child( new Node( "child" ) );

	parent->attachNode( child );
	parent->detachNode( child );

	EXPECT_FALSE( parent->hasNodes() );
	EXPECT_FALSE( child->hasParent() );
	EXPECT_EQ( child->getParent(), nullptr );
}

TEST( GroupNodeTest, detachMultipleNodes )
{
	GroupNodePtr parent( new GroupNode( "parent" ) );
	
	NodePtr child1( new Node( "child 1" ) );
	NodePtr child2( new Node( "child 2" ) );
	NodePtr child3( new Node( "child 3" ) );

	parent->attachNode( child1 );
	parent->attachNode( child2 );
	parent->attachNode( child3 );

	parent->detachNode( child1 );
	parent->detachNode( child2 );

	EXPECT_TRUE( parent->hasNodes() );

	int count = 0;
	parent->foreachNode( [&]( NodePtr node ) {
		EXPECT_EQ( node->getName(), child3->getName() );
		++count;
	});
	EXPECT_EQ( count, 1 );
}

TEST( GroupNodeTest, detachNodeFromDifferentParent )
{
	GroupNodePtr parent1( new GroupNode( "parent" ) );
	GroupNodePtr parent2( new GroupNode( "another parent" ) );
	NodePtr child( new Node( "child" ) );

	parent1->attachNode( child );
	parent2->detachNode( child );

	EXPECT_TRUE( child->hasParent() );
	ASSERT_EQ( child->getParent(), parent1.get() );
	EXPECT_TRUE( parent1->hasNodes() );
	EXPECT_FALSE( parent2->hasNodes() );
}

TEST( GroupNodeTest, detachAllNodes )
{
	GroupNodePtr parent( new GroupNode( "parent" ) );
	
	NodePtr child1( new Node( "child 1" ) );
	NodePtr child2( new Node( "child 2" ) );
	NodePtr child3( new Node( "child 3" ) );

	parent->attachNode( child1 );
	parent->attachNode( child2 );
	parent->attachNode( child3 );

	parent->detachAllNodes();

	EXPECT_FALSE( parent->hasNodes() );

	EXPECT_FALSE( child1->hasParent() );
	EXPECT_EQ( child1->getParent(), nullptr );
	EXPECT_FALSE( child2->hasParent() );
	EXPECT_EQ( child2->getParent(), nullptr );
	EXPECT_FALSE( child3->hasParent() );
	EXPECT_EQ( child3->getParent(), nullptr );

	int count = 0;
	parent->foreachNode( [&]( NodePtr node ) {
		++count;
	});

	EXPECT_EQ( count, 0 );
}

TEST( GroupNodeTest, buildHierarchy )
{
	//		node0
	//		/   \
	//	node1	node2
	//			/	\
	//		node3	node4

	GroupNodePtr node0( new GroupNode( "node0" ) );
	GroupNodePtr node2( new GroupNode( "node2" ) );
	NodePtr node1( new Node( "node1" ) );
	NodePtr node3( new Node( "node3" ) );
	NodePtr node4( new Node( "node4" ) );

	node0->attachNode( node1 );
	node0->attachNode( node2 );
	node2->attachNode( node3 );
	node2->attachNode( node4 );

	EXPECT_TRUE( node0->hasNodes() );
	EXPECT_EQ( node1->getParent(), node0.get() );
	EXPECT_EQ( node2->getParent(), node0.get() );

	EXPECT_TRUE( node2->hasNodes() );
	EXPECT_EQ( node3->getParent(), node2.get() );
	EXPECT_EQ( node4->getParent(), node2.get() );
}

