/**********************************************************
 * n64_file.h
 **********************************************************
 * N64 File:
 *     This class represents a compressible file inside 
 * an N64 ROM. If the _comp_method() and _decomp_method() 
 * is not overwritten by the derived class, both compress()
 * & decompress() will return the same span.
 * 
 *     Compression/decompression is performed lazily. To
 * re-evaluate a compression/decompression a new n64_file
 * needs to be created.
 * 
 * Author: Mittenz 5/15/2020
 **********************************************************/
#pragma once

#include <cstdlib>
#include "simpTypes.h"
#include "n64_span.h"

class n64_file
{
public:
	n64_file() = default;
	n64_file(n64_file&& src); //move constructor
	n64_file(const n64_file& src); //copy constructor
	n64_file(const n64_span& file, bool comp, bool own = false); //references existing buffer
	n64_file(u8* rom_ptr, u32 offset, u32 size, bool comp, bool own = false);

	//frees owned spans
	~n64_file();

	const n64_span& const decompress(void) const;
	const n64_span& const compress(void) const;

protected:
	mutable n64_span* _comp_buffer = nullptr;
	mutable bool _comp_own = false;
	
	mutable bool _decomp_own = false;
	mutable n64_span* _decomp_buffer = nullptr;

	virtual void _comp_method() const;
	virtual void _decomp_method() const;

};