#include "stdafx.h"
#include "common/base64.h"

#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <sstream>
#include <iostream>

using namespace boost::archive::iterators;

BEGIN_NAMESPACE

std::string QTH_UTILITY_EXPORT base64_encode(const std::string& input)
{
	typedef base64_from_binary<transform_width<std::string::const_iterator, 6, 8>> Base64EncodeIterator;
	static std::stringstream result;
	result.str("");
	result.clear();

	try {
		copy( Base64EncodeIterator( input.begin() ), Base64EncodeIterator( input.end() ), std::ostream_iterator<char>( result ) );
	} catch ( ... ) {
		return std::string();
	}
	size_t equal_count = (3 - input.length() % 3) % 3;
	for ( size_t i = 0; i < equal_count; i++ )
	{
		result.put( '=' );
	}
	
	return result.str();
}

std::string QTH_UTILITY_EXPORT base64_decode(const std::string& input)
{
	typedef transform_width<binary_from_base64<std::string::const_iterator>, 8, 6> Base64DecodeIterator;
	static std::stringstream result;
	result.str("");
	result.clear();

	try {
		copy( Base64DecodeIterator( input.begin() ), Base64DecodeIterator( input.end() ), std::ostream_iterator<char>( result ) );
	} catch ( ... ) {
		return std::string();
	}
	
	return result.str();

}

END_NAMESPACE