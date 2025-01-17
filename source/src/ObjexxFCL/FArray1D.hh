#ifndef INCLUDED_ObjexxFCL_FArray1D_hh
#define INCLUDED_ObjexxFCL_FArray1D_hh


// FArray1D: Fortran-Compatible 1D Array
//
// Project: Objexx Fortran Compatibility Library (ObjexxFCL)
//
// Version: 3.0.0
//
// Language: C++
//
// Copyright (c) 2000-2009 Objexx Engineering, Inc. All Rights Reserved.
// Use of this source code or any derivative of it is restricted by license.
// Licensing is available from Objexx Engineering, Inc.:  http://objexx.com  Objexx@objexx.com

// ObjexxFCL Headers
#include <ObjexxFCL/FArray1D.fwd.hh>
#include <ObjexxFCL/FArray1.hh>
#include <ObjexxFCL/ObserverMulti.hh>
#include <ObjexxFCL/DynamicIndexRange.hh>
#include <ObjexxFCL/FArrayInitializer.hh>


namespace ObjexxFCL {


/// @brief FArray1D: Fortran-Compatible 1D Array
template< typename T >
class FArray1D :
	public FArray1< T >,
	public ObserverMulti
{


private: // Types


	typedef  FArray1< T >  Super;
	typedef  typename Super::real_FArray  real_FArray;
	typedef  typename Super::proxy_FArray  proxy_FArray;
	typedef  typename Super::arg_FArray  arg_FArray;
	typedef  internal::InitializerSentinel  InitializerSentinel;


private: // Friend


	template< typename > friend class FArray1D;
	friend class FArray1P< T >;
	friend class FArray1A< T >;


public: // Types


	typedef  typename Super::Base  Base;
	typedef  typename Base::Section  Section;
	typedef  typename Super::IR  SIR;
	typedef  DynamicIndexRange  IR;

	// STL Style
	typedef  typename Base::value_type  value_type;
	typedef  typename Base::reference  reference;
	typedef  typename Base::const_reference  const_reference;
	typedef  typename Base::pointer  pointer;
	typedef  typename Base::const_pointer  const_pointer;
	typedef  typename Base::size_type  size_type;
	typedef  typename Base::difference_type  difference_type;

	// C++ Style
	typedef  typename Base::Value  Value;
	typedef  typename Base::Reference  Reference;
	typedef  typename Base::ConstReference  ConstReference;
	typedef  typename Base::Pointer  Pointer;
	typedef  typename Base::ConstPointer  ConstPointer;
	typedef  typename Base::Size  Size;
	typedef  typename Base::Difference  Difference;

	typedef  FArrayInitializer< T, ObjexxFCL::FArray1D >  Initializer;
	typedef  typename Initializer::function_type  InitializerFunction;

	using Super::array_;
	using Super::array_size_;
	using Super::sarray_;
	using Super::shift_;
	using Super::shift_set;
	using Super::size_;
	using Super::size_of;


public: // Creation


	/// @brief Default Constructor
	inline
	FArray1D()
	{
		insert_as_observer();
	}


	/// @brief Copy Constructor
	inline
	FArray1D( FArray1D const & a ) :
		Super( a ),
		ObserverMulti(),
		I_( a.I_ )
	{
		insert_as_observer();
	}


	/// @brief Copy Constructor Template
	template< typename U >
	inline
	explicit
	FArray1D( FArray1D< U > const & a ) :
		Super( a ),
		I_( a.I_ )
	{
		insert_as_observer();
	}


	/// @brief Super Constructor Template
	template< typename U >
	inline
	explicit
	FArray1D( FArray1< U > const & a ) :
		Super( a ),
		I_( a.I() )
	{
		insert_as_observer();
	}


	/// @brief IndexRange Constructor
	inline
	explicit
	FArray1D( IR const & I_a ) :
		Super( size_of( I_a ) ),
		I_( I_a )
	{
		setup_real();
		insert_as_observer();
	}


	/// @brief IndexRange + Initializer Value Constructor
	inline
	FArray1D( IR const & I_a, T const & t ) :
		Super( size_of( I_a ), InitializerSentinel() ),
		I_( I_a ),
		initializer_( t )
	{
		setup_real();
		initialize();
		insert_as_observer();
	}


	/// @brief IndexRange + Initializer Function Constructor
	inline
	FArray1D( IR const & I_a, InitializerFunction const & function_a ) :
		Super( size_of( I_a ), InitializerSentinel() ),
		I_( I_a ),
		initializer_( function_a )
	{
		setup_real();
		initialize();
		insert_as_observer();
	}


	/// @brief Super + IndexRange Constructor Template
	template< typename U >
	inline
	FArray1D( FArray1< U > const & a, IR const & I_a ) :
		Super( size_of( I_a ) ),
		I_( I_a )
	{
		setup_real();
		if ( dimensions_initialized() ) {
			if ( a.dimensions_initialized() ) { // Copy array data where overlap
				int const b( std::max( I_.l(), a.l() ) ), e( std::min( I_.u(), a.u() ) );
				for ( int i = b; i <= e; ++i ) {
					operator ()( i ) = T( a( i ) );
				}
			}
		}
		insert_as_observer();
	}


	/// @brief Super + IndexRange + Fill Value Constructor Template
	template< typename U >
	inline
	FArray1D( FArray1< U > const & a, IR const & I_a, T const & t ) :
		Super( size_of( I_a ) ),
		I_( I_a )
	{
		setup_real();
		if ( dimensions_initialized() ) {
			(*this) = t; // Initialize array with fill value
			if ( a.dimensions_initialized() ) { // Copy array data where overlap
				int const b( std::max( I_.l(), a.l() ) ), e( std::min( I_.u(), a.u() ) );
				for ( int i = b; i <= e; ++i ) {
					operator ()( i ) = T( a( i ) );
				}
			}
		}
		insert_as_observer();
	}


	/// @brief Destructor
	inline
	~FArray1D() override
	{}


public: // Assignment


	/// @brief Copy Assignment
	inline
	FArray1D &
	operator =( FArray1D const & a )
	{
		if ( this != &a ) {
			if ( ! this->equal_dimension( a ) ) dimension( a );
			Base::operator =( a );
		}
		return *this;
	}


	/// @brief Super Assignment
	inline
	FArray1D &
	operator =( Super const & a )
	{
		if ( this != &a ) {
			if ( ! this->equal_dimension( a ) ) dimension( a );
			Base::operator =( a );
		}
		return *this;
	}


	/// @brief Super Assignment Template
	template< typename U >
	inline
	FArray1D &
	operator =( FArray1< U > const & a )
	{
		if ( ! this->equal_dimension( a ) ) dimension( a );
		Base::operator =( a );
		return *this;
	}


	/// @brief += Array Template
	template< typename U >
	inline
	FArray1D &
	operator +=( FArray1< U > const & a )
	{
		Super::operator +=( a );
		return *this;
	}


	/// @brief -= Array Template
	template< typename U >
	inline
	FArray1D &
	operator -=( FArray1< U > const & a )
	{
		Super::operator -=( a );
		return *this;
	}


	/// @brief = Value
	inline
	FArray1D &
	operator =( T const & t )
	{
		Super::operator =( t );
		return *this;
	}


	/// @brief += Value
	inline
	FArray1D &
	operator +=( T const & t )
	{
		Super::operator +=( t );
		return *this;
	}


	/// @brief -= Value
	inline
	FArray1D &
	operator -=( T const & t )
	{
		Super::operator -=( t );
		return *this;
	}


	/// @brief *= Value
	inline
	FArray1D &
	operator *=( T const & t )
	{
		Super::operator *=( t );
		return *this;
	}


	/// @brief /= Value
	inline
	FArray1D &
	operator /=( T const & t )
	{
		Super::operator /=( t );
		return *this;
	}


public: // Subscript


	/// @brief array( i ) const
	inline
	T const &
	operator ()( int const i ) const
	{
		assert( I_.contains( i ) );
		return sarray_[ i ];
	}


	/// @brief array( i )
	inline
	T &
	operator ()( int const i )
	{
		assert( I_.contains( i ) );
		return sarray_[ i ];
	}


	/// @brief Section Starting at array( i )
	inline
	Section const
	a( int const i ) const
	{
		assert( I_.contains( i ) );
		return Section( array_size_ - ( i - shift_ ), sarray_ + i );
	}


	/// @brief Linear Index
	inline
	size_type
	index( int const i ) const
	{
		assert( I_.initialized() );
		return ( i - shift_ );
	}


	/// @brief array[ i ] const: Linear Subscript
	inline
	T const &
	operator []( size_type const i ) const
	{
		assert( i < size_ );
		return array_[ i ];
	}


	/// @brief array[ i ]: Linear Subscript
	inline
	T &
	operator []( size_type const i )
	{
		assert( i < size_ );
		return array_[ i ];
	}


public: // Predicate


	/// @brief Dimensions Initialized?
	inline
	bool
	dimensions_initialized() const override
	{
		return I_.initialized();
	}


	/// @brief Contains Indexed Element?
	inline
	bool
	contains( int const i ) const override
	{
		return I_.contains( i );
	}


	/// @brief Initializer Active?
	inline
	bool
	initializer_active() const override
	{
		return initializer_.is_active();
	}


public: // Inspector


	/// @brief IndexRange
	inline
	IR const &
	I1() const override
	{
		return I_;
	}


	/// @brief Lower Index
	inline
	int
	l1() const override
	{
		return I_.l();
	}


	/// @brief Upper Index
	inline
	int
	u1() const override
	{
		return I_.u();
	}


	/// @brief Size
	inline
	size_type
	size1() const override
	{
		return I_.size();
	}


	/// @brief IndexRange
	inline
	IR const &
	I() const override
	{
		return I_;
	}


	/// @brief Lower Index
	inline
	int
	l() const override
	{
		return I_.l();
	}


	/// @brief Upper Index
	inline
	int
	u() const override
	{
		return I_.u();
	}


public: // Modifier


	/// @brief Clear
	inline
	FArray1D &
	clear() override
	{
		Super::clear();
		I_.clear_no_notify();
		initializer_.clear();
		notify();
		return *this;
	}


	/// @brief Dimension by IndexRanges
	inline
	FArray1D &
	dimension( IR const & I_a )
	{
		initializer_.clear();
		I_.assign_no_notify( I_a );
		dimension_real();
		notify();
		return *this;
	}


	/// @brief Dimension by IndexRanges + Initializer Value
	inline
	FArray1D &
	dimension( IR const & I_a, T const & t )
	{
		initializer_ = t;
		I_.assign_no_notify( I_a );
		dimension_real();
		initialize();
		notify();
		return *this;
	}


	/// @brief Dimension by IndexRanges + Initializer Function
	inline
	FArray1D &
	dimension( IR const & I_a, InitializerFunction const & function_a )
	{
		initializer_ = function_a;
		I_.assign_no_notify( I_a );
		dimension_real();
		initialize();
		notify();
		return *this;
	}


	/// @brief Dimension by Array Template
	template< typename U >
	inline
	FArray1D &
	dimension( FArray1< U > const & a )
	{
		initializer_.clear();
		I_.assign_no_notify( a.I() );
		dimension_real();
		notify();
		return *this;
	}


	/// @brief Dimension by Array + Initializer Value Template
	template< typename U >
	inline
	FArray1D &
	dimension( FArray1< U > const & a, T const & t )
	{
		initializer_ = t;
		I_.assign_no_notify( a.I() );
		dimension_real();
		initialize();
		notify();
		return *this;
	}


	/// @brief Dimension by Array + Initializer Function Template
	template< typename U >
	inline
	FArray1D &
	dimension( FArray1< U > const & a, InitializerFunction const & function_a )
	{
		initializer_ = function_a;
		I_.assign_no_notify( a.I() );
		dimension_real();
		initialize();
		notify();
		return *this;
	}


	/// @brief Data-Preserving Redimension by IndexRanges
	inline
	FArray1D &
	redimension( IR const & I_a )
	{
		FArray1D( *this, I_a ).swap( *this );
		return *this;
	}


	/// @brief Data-Preserving Redimension by IndexRanges + Fill Value
	inline
	FArray1D &
	redimension( IR const & I_a, T const & t )
	{
		FArray1D( *this, I_a, t ).swap( *this );
		return *this;
	}


	/// @brief Data-Preserving Redimension by Array Template
	template< typename U >
	inline
	FArray1D &
	redimension( FArray1< U > const & a )
	{
		FArray1D( *this, a.I() ).swap( *this );
		return *this;
	}


	/// @brief Data-Preserving Redimension by Array + Fill Value Template
	template< typename U >
	inline
	FArray1D &
	redimension( FArray1< U > const & a, T const & t )
	{
		FArray1D( *this, a.I(), t ).swap( *this );
		return *this;
	}


	/// @brief Set Initializer Value
	inline
	FArray1D &
	initializer( T const & t )
	{
		initializer_ = t;
		return *this;
	}


	/// @brief Set Initializer Function
	inline
	FArray1D &
	initializer( InitializerFunction const & function_a )
	{
		initializer_ = function_a;
		return *this;
	}


	/// @brief Clear Initializer
	inline
	FArray1D &
	initializer_clear()
	{
		initializer_.clear();
		return *this;
	}


	/// @brief Initialize
	inline
	FArray1D &
	initialize()
	{
		if ( ( initializer_.is_active() ) && ( dimensions_initialized() ) ) {
			if ( initializer_.is_value() ) {
				(*this) = initializer_.value();
			} else if ( initializer_.is_function() ) {
				initializer_.function()( *this );
			}
		}
		return *this;
	}


	/// @brief Swap
	inline
	FArray1D &
	swap( FArray1D & v )
	{
		this->swap1DB( v );
		I_.swap_no_notify( v.I_ );
		std::swap( initializer_, v.initializer_ );
		notify(); // So proxy FArrays can reattach
		v.notify(); // So proxy FArrays can reattach
		return *this;
	}


public: // Observer Modifier


	/// @brief Update
	inline
	void
	update() override
	{
		dimension_real();
		initialize();
	}


	/// @brief Update for Destruction of a Subject
	inline
	void
	destructed( Subject const & ) override
	{}


public: // Friend


	/// @brief Swap
	template< typename U >
	friend
	void
	swap( FArray1D<U> & a, FArray1D<U> & b );


public: // Generator


	/// @brief -Array
	friend
	inline
	FArray1D
	operator -( Super const & a )
	{
		FArray1D r( a );
		r *= T( -1 );
		return r;
	}


	/// @brief Array + Array
	friend
	inline
	FArray1D
	operator +( Super const & a, Super const & b )
	{
		FArray1D r( a );
		r += b;
		return r;
	}


	/// @brief Array - Array
	friend
	inline
	FArray1D
	operator -( Super const & a, Super const & b )
	{
		FArray1D r( a );
		r -= b;
		return r;
	}


	/// @brief Array + Value
	friend
	inline
	FArray1D
	operator +( Super const & a, T const & t )
	{
		FArray1D r( a );
		r += t;
		return r;
	}


	/// @brief Value + Array
	friend
	inline
	FArray1D
	operator +( T const & t, Super const & a )
	{
		FArray1D r( a );
		r += t;
		return r;
	}


	/// @brief Array - Value
	friend
	inline
	FArray1D
	operator -( Super const & a, T const & t )
	{
		FArray1D r( a );
		r -= t;
		return r;
	}


	/// @brief Value - Array
	friend
	inline
	FArray1D
	operator -( T const & t, Super const & a )
	{
		FArray1D r( a );
		r *= T( -1 );
		r += t;
		return r;
	}


	/// @brief Array * Value
	friend
	inline
	FArray1D
	operator *( Super const & a, T const & t )
	{
		FArray1D r( a );
		r *= t;
		return r;
	}


	/// @brief Value * Array
	friend
	inline
	FArray1D
	operator *( T const & t, Super const & a )
	{
		FArray1D r( a );
		r *= t;
		return r;
	}


	/// @brief Array / Value
	friend
	inline
	FArray1D
	operator /( Super const & a, T const & t )
	{
		FArray1D r( a );
		r /= t;
		return r;
	}


	/// @brief Cross Product of Two 3-Tuple Vectors
	template< typename U >
	friend
	FArray1D<U>
	cross_product( FArray1< U > const & a, FArray1< U > const & b );


	/// @brief Cross Product of Two 3-Tuple Vectors
	template< typename U >
	friend
	FArray1D
	cross( FArray1< U > const & a, FArray1< U > const & b );


protected: // Functions


	/// @brief Dimension by IndexRanges
	inline
	void
	dimension_assign( SIR const & I_a ) override
	{
		initializer_.clear();
		I_.assign_no_notify( I_a );
		dimension_real();
		notify();
	}


private: // Functions


	/// @brief Setup for IndexRange Constructor
	inline
	void
	setup_real()
	{
		if ( dimensions_initialized() ) {
			shift_set( I_.lz() );
		} else {
			shift_set( 0 );
		}
	}


	/// @brief Dimension by Current IndexRanges
	inline
	void
	dimension_real()
	{
		if ( dimensions_initialized() ) {
			this->resize( size_of( I_ ) );
			shift_set( I_.lz() );
#ifdef OBJEXXFCL_FARRAY_SIZE_REPORT
			size_report();
#endif // OBJEXXFCL_FARRAY_SIZE_REPORT
		} else {
			Base::clear();
		}
	}


	/// @brief Insert as Observer of the IndexRanges
	inline
	void
	insert_as_observer()
	{
		I_.insert_observer( *this );
#ifdef OBJEXXFCL_FARRAY_SIZE_REPORT
		size_report();
#endif // OBJEXXFCL_FARRAY_SIZE_REPORT
	}


	/// @brief Remove as Observer of the IndexRanges
	inline
	void
	remove_as_observer()
	{
		I_.remove_observer( *this );
	}


#ifdef OBJEXXFCL_FARRAY_SIZE_REPORT
	/// @brief Report size if at least value defined for OBJEXXFCL_FARRAY_SIZE_REPORT
	/// @note  Size is based on sizeof( T ) so T-controlled heap memory is not counted
	inline
	void
	size_report() const
	{
		if ( size_ * sizeof( T ) >= OBJEXXFCL_FARRAY_SIZE_REPORT ) {
			std::cout << "  Index ranges: " << I_ << std::endl;
		}
	}
#endif // OBJEXXFCL_FARRAY_SIZE_REPORT


private: // Data


	/// @brief Index range
	IR I_;

	/// @brief Array initializer
	Initializer initializer_;


}; // FArray1D


/// @brief Swap
template< typename T >
void
swap( FArray1D< T > & a, FArray1D< T > & b );


/// @brief -Array
template< typename T >
FArray1D< T >
operator -( FArray1< T > const & a );


/// @brief Array + Array
template< typename T >
FArray1D< T >
operator +( FArray1< T > const & a, FArray1< T > const & b );


/// @brief Array - Array
template< typename T >
FArray1D< T >
operator -( FArray1< T > const & a, FArray1< T > const & b );


/// @brief Array + Value
template< typename T >
FArray1D< T >
operator +( FArray1< T > const & a, T const & t );


/// @brief Value + Array
template< typename T >
FArray1D< T >
operator +( T const & t, FArray1< T > const & a );


/// @brief Array - Value
template< typename T >
FArray1D< T >
operator -( FArray1< T > const & a, T const & t );


/// @brief Value - Array
template< typename T >
FArray1D< T >
operator -( T const & t, FArray1< T > const & a );


/// @brief Array * Value
template< typename T >
FArray1D< T >
operator *( FArray1< T > const & a, T const & t );


/// @brief Value * Array
template< typename T >
FArray1D< T >
operator *( T const & t, FArray1< T > const & a );


/// @brief Array / Value
template< typename T >
FArray1D< T >
operator /( FArray1< T > const & a, T const & t );


/// @brief Cross Product of Two 3-Tuple Vectors
template< typename T >
FArray1D< T >
cross_product( FArray1< T > const & a, FArray1< T > const & b );


/// @brief Cross Product of Two 3-Tuple Vectors
template< typename T >
FArray1D< T >
cross( FArray1< T > const & a, FArray1< T > const & b );


/// @brief Swap
template< typename T >
void
swap( FArray1D<T> & a, FArray1D<T> & b )
{
	a.swap( b );
}


/// @brief Cross Product of Two 3-Tuple Vectors
template< typename T >
FArray1D<T>
cross_product( FArray1< T > const & a, FArray1< T > const & b )
{
	assert( equal_dimensions( a, b ) );
	assert( a.size() == 3 );
	FArray1D<T> c( a.I() );
	int const x( a.l() ), y( x + 1 ), z( y + 1 );
	c( x ) = ( a( y ) * b( z ) ) - ( a( z ) * b( y ) );
	c( y ) = ( a( z ) * b( x ) ) - ( a( x ) * b( z ) );
	c( z ) = ( a( x ) * b( y ) ) - ( a( y ) * b( x ) );
	return c;
}


/// @brief Cross Product of Two 3-Tuple Vectors
template< typename T >
FArray1D<T>
cross( FArray1< T > const & a, FArray1< T > const & b )
{
	assert( equal_dimensions( a, b ) );
	assert( a.size() == 3 );
	FArray1D<T> c( a.I() );
	int const x( a.l() ), y( x + 1 ), z( y + 1 );
	c( x ) = ( a( y ) * b( z ) ) - ( a( z ) * b( y ) );
	c( y ) = ( a( z ) * b( x ) ) - ( a( x ) * b( z ) );
	c( z ) = ( a( x ) * b( y ) ) - ( a( y ) * b( x ) );
	return c;
}


} // namespace ObjexxFCL


#ifndef NO_STD_SWAP_OVERLOADS


// std::swap Overloads for Efficiency
//
// Technically you cannot add template functions overloads to namespace std
// but this works with most compilers and makes it much faster if someone uses
// std::swap instead of swap or ObjexxFCL::swap.  The legal alternative would be
// to add specializations of swap for each anticipated instantiation.


namespace std {


/// @brief std::swap( FArray1D, FArray1D )
template< typename T >
inline
void
swap( ObjexxFCL::FArray1D< T > & a, ObjexxFCL::FArray1D< T > & b )
{
	a.swap( b );
}


} // namespace std


#endif // NO_STD_SWAP_OVERLOADS


#endif // INCLUDED_ObjexxFCL_FArray1D_HH
