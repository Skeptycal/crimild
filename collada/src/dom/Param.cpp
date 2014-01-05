#include "Param.hpp"

using namespace crimild;
using namespace crimild::collada;

Param::Param( void )
{
}

Param::~Param( void )
{
}

bool Param::parseXML( xmlNode *input )
{
	XMLUtils::getAttribute( input, COLLADA_NAME, _name );
	XMLUtils::getAttribute( input, COLLADA_TYPE, _type );

	return true;
}
