/**
 * @file LamaTiming.hpp
 *
 * @license
 * Copyright (c) 2009-2015
 * Fraunhofer Institute for Algorithms and Scientific Computing SCAI
 * for Fraunhofer-Gesellschaft
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * @endlicense
 *
 * @brief Class that is useful for timing in MPI programs
 * @author Thomas Brandes
 * @date 09.07.2013
 */

#pragma once

#include <scai/common/Walltime.hpp>
#include <iostream>
#include <scai/lama/Communicator.hpp>

/** Class for Timing.
 *
 *  \code
 *      LamaTiming time( comm, "Redistribution" )
 *  \endcode
 */

class LamaTiming 
{

public:

    /** Constructor. */

    LamaTiming( const scai::lama::Communicator& comm, const char* name );

    /** Destructor, prints timing on root processor */

    ~LamaTiming();

private:

    const scai::lama::Communicator& mComm;
    const char* mName;
    double mStart;
};

/* ---------------------------------------------------------------------------- */
  
LamaTiming::LamaTiming( const scai::lama::Communicator& comm, const char* name ) :
    mComm( comm ),
    mName( name )
{
    mStart = scai::common::Walltime::get();
}

LamaTiming::~LamaTiming()
{
    double myTime = scai::common::Walltime::get() - mStart;

    double maxTime = mComm.max( myTime );

    if ( mComm.getRank() == 0 )
    {
        std::cout << mName << ": took " << maxTime << " seconds" << std::endl;
    }
}
