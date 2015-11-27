/*
 * container_wrap.h
 * 
 * Copyright 2015 Ed Hynan <edhynan@gmail.com>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

//
// wrap a container as data member in struct providing
// subset of container interface, but providing a virtual
// destructor, suitable as a base for classes, virtual or not
//

#ifndef _CONTAINER_WRAP_H_
#define _CONTAINER_WRAP_H_

#include <iterator>

// must instantiate with std container with the methods used herein
template<typename T>
class ContainerWrap {
public:
	typedef typename T::value_type       value_type;
	typedef typename T::size_type        size_type;
	typedef typename T::reference        reference;
	typedef typename T::const_reference  const_reference;
	typedef typename T::iterator         iterator;
	typedef typename T::const_iterator   const_iterator;
	typedef typename T::reverse_iterator reverse_iterator;
	typedef typename T::const_reverse_iterator const_reverse_iterator;

	ContainerWrap() {}
	ContainerWrap(size_type size) : ctr(size) {}

	ContainerWrap(const ContainerWrap<T>& o) : ctr(o.ctr) {}

	virtual ~ContainerWrap() {}

	ContainerWrap<T>& operator = (const ContainerWrap<T>& o)
	{
		ctr = o.ctr;
		return *this;
	}

	iterator begin() { return ctr.begin(); }
	iterator end() { return ctr.end(); }
	const_iterator begin() const { return ctr.begin(); }
	const_iterator end() const { return ctr.end(); }
	reverse_iterator rbegin() { return ctr.rbegin(); }
	reverse_iterator rend() { return ctr.rend(); }
	const_reverse_iterator rbegin() const { return ctr.rbegin(); }
	const_reverse_iterator rend() const { return ctr.rend(); }

	size_type size() const { return ctr.size(); }
	size_type max_size() const { return ctr.max_size(); }
	void resize(size_type n, value_type val = value_type())
	{ ctr.resize(n, val); }
	bool empty() const { return ctr.empty(); }

	reference front() { return ctr.front(); }
	const_reference front() const { return ctr.front(); }
	reference back() { return ctr.back(); }
	const_reference back() const { return ctr.back(); }

	template <class InputIterator>
	void assign(InputIterator first, InputIterator afterlast)
	{ ctr.assign(first, afterlast); }
	void assign(size_type n, const value_type& val)
	{ ctr.assign(n, val); }
	void push_back(const value_type& val) { ctr.push_back(val); }
	void pop_back() { ctr.pop_back(); }
	iterator insert(iterator position, const value_type& val)
	{ return ctr.insert(position, val); }
	void insert(iterator position, size_type n, const value_type& v)
	{ ctr.insert(position, n, v); }
	template <class InputIterator>
	void insert(iterator position,
		InputIterator first, InputIterator afterlast)
	{ ctr.insert(position, first, afterlast); }
	iterator erase(iterator pos)
	{ return ctr.erase(pos); }
	iterator erase(iterator first, iterator last)
	{ return ctr.erase(first, last); }
	void swap(ContainerWrap<T>& o) { ctr.swap(o.ctr); }
	void swap(T& o) { ctr.swap(o); }
	void clear() { ctr.clear(); }

	operator T&() { return ctr; }
	operator const T&() const { return ctr; }

	// hacks to make this work:
	std::back_insert_iterator<T> get_bk_ins_itr()
	{ return std::back_inserter(ctr); }

protected:
	T ctr;
};

// must instantiate with std container with the methods used herein
template<typename T>
class ListWrap : public ContainerWrap<T> {
public:
	typedef ContainerWrap<T> base_type;

	typedef typename base_type::value_type       value_type;
	typedef typename base_type::size_type        size_type;
	typedef typename base_type::reference        reference;
	typedef typename base_type::const_reference  const_reference;
	typedef typename base_type::iterator         iterator;
	typedef typename base_type::const_iterator   const_iterator;
	typedef typename base_type::reverse_iterator reverse_iterator;
	typedef typename base_type::const_reverse_iterator
		const_reverse_iterator;

	ListWrap() {}
	ListWrap(size_type size) : ContainerWrap<T>(size) {}

	ListWrap(const ListWrap<T>& o) : ContainerWrap<T>(o) {}

	virtual ~ListWrap() {}

	ListWrap<T>& operator = (const ListWrap<T>& o)
	{
		return base_type::operator = (o);
	}

	void push_front(const value_type& val)
	{ base_type::ctr.push_front(val); }
	void pop_front() { base_type::ctr.pop_front(); }

	void splice(iterator position, T& x)
	{ base_type::ctr.splice(position, x); }
	void splice(iterator position, T& x, iterator i)
	{ base_type::ctr.splice(position, x, i); }
	void splice(iterator position, T& x, iterator first, iterator last)
	{ base_type::ctr.splice(position, x, first, last); }
	void remove(const value_type& val) { base_type::ctr.remove(val); }
	template <class Predicate>
	void remove_if(Predicate pred) { base_type::ctr.remove_if(pred); }
	void unique() { base_type::ctr.unique(); }
	template <class BinaryPredicate>
	void unique(BinaryPredicate pred) { base_type::ctr.unique(pred); }
	void merge(T& x) { base_type::ctr.merge(x); }
	template <class Compare>
	void merge(T& x, Compare comp) { base_type::ctr.merge(x, comp); }
	void sort() { base_type::ctr.sort(); }
	template <class Compare>
	void sort(Compare comp) { base_type::ctr.sort(comp); }
	void reverse() { base_type::ctr.reverse(); }

#if defined(LISTWRAP_OPINDEX)
#	include <stdexcept>
	// mostly for testing with code that used vector<T>::operator[]
	reference operator[](size_type i) {
		iterator b = base_type::begin(),
		         e = base_type::end();
		for ( size_type n = 0;  b != e; ++n, ++b )
		{
			if ( n == i ) {
				return *b;
			}
		}
		// cannot return failure
		throw std::range_error("ListWrap indice out of range");
	}
	const_reference operator[](size_type i) const {
		const_iterator b = base_type::begin(),
		               e = base_type::end();
		for ( size_type n = 0; b != e; ++n, ++b )
		{
			if ( n == i ) {
				return *b;
			}
		}
		// cannot return failure
		throw std::range_error("const ListWrap indice out of range");
	}
#endif // LISTWRAP_OPINDEX
};

// must instantiate with std container with the methods used herein
template<typename T>
class VectorWrap : public ContainerWrap<T> {
public:
	typedef ContainerWrap<T> base_type;

	typedef typename base_type::value_type       value_type;
	typedef typename base_type::size_type        size_type;
	typedef typename base_type::reference        reference;
	typedef typename base_type::const_reference  const_reference;
	typedef typename base_type::iterator         iterator;
	typedef typename base_type::const_iterator   const_iterator;
	typedef typename base_type::reverse_iterator reverse_iterator;
	typedef typename base_type::const_reverse_iterator
		const_reverse_iterator;

	VectorWrap() {}
	VectorWrap(size_type size) : ContainerWrap<T>(size) {}

	VectorWrap(const VectorWrap<T>& o) : ContainerWrap<T>(o) {}

	virtual ~VectorWrap() {}

	VectorWrap<T>& operator = (const VectorWrap<T>& o)
	{
		return base_type::operator = (o);
	}

	size_type capacity() const { return base_type::ctr.capacity(); }
	void reserve(size_type sz) {  base_type::ctr.reserve(sz); }

	reference operator[](size_type i) { return base_type::ctr[i]; }
	const_reference operator[](size_type i) const
	{ return base_type::ctr[i]; }
	reference at(size_type i) { return base_type::ctr.at(i); }
	const_reference at(size_type i) const
	{ return base_type::ctr.at(i); }
};

// need non-member overrides/specialisations, e.g. for procs
// in <iterator> -- these are incomplete and may be built
// incrementally

template<typename _CtrValT>
inline std::back_insert_iterator<_CtrValT>
std::back_inserter(ContainerWrap<_CtrValT>& _ctr)
{ return _ctr.get_bk_ins_itr(); }

#endif // _CONTAINER_WRAP_H_
