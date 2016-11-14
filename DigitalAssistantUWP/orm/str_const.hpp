#pragma once

/// Compile-time string, usable in constexpr functions.
///
/// @author Scott Schurr, Willi Schinmeyer
class str_const { // constexpr string
public:
	template< std::size_t N >
	constexpr str_const( const char( &a )[ N ] )
		: _data( a )
		, _size( N - 1 ) {
	}
	constexpr const char operator[]( std::size_t n ) const {
		return n < _size ? _data[ n ] :
			throw std::out_of_range( "" );
	}
	constexpr const std::size_t size() const {
		return _size;
	}
	constexpr const bool operator==( const str_const& rhs ) const {
		return size() == rhs.size() // recursion termination: differing sizes don't match
			&& ( size() == 0 // recursion termination: empty strings match
				|| ( _data[ 0 ] == rhs[ 0 ] // first character must match
					&& str_const{ _data + 1, _size - 1 } == str_const{ rhs._data + 1, rhs._size - 1 } ) ); // rest must match
	}
private:
	constexpr str_const( const char* const data, const std::size_t size )
		: _data( data )
		, _size( size ) {
	}

	const char* const _data;
	const std::size_t _size;
};
