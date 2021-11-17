///////////////////////////////////////////////////////////////////////////////
// BSD 3-Clause License
//
// Copyright (c) 2021, Andrew Kennings
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// * Neither the name of the copyright holder nor the names of its
//   contributors may be used to endorse or promote products derived from
//   this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.




#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <stack>
#include <utility>
#include <cmath>
#include <boost/tokenizer.hpp>
#include <boost/format.hpp>
#include "utility.h"
#include "plotgnu.h"
#include "utility.h"


// Detailed management of segments.
#include "detailed_segment.h"
#include "detailed_manager.h"
// Detailed placement algorithms.
#include "detailed.h"
#include "detailed_mis.h"
#include "detailed_interleaving.h"
#include "detailed_reorder.h"
#include "detailed_vertical.h"
#include "detailed_global.h"
#include "detailed_random.h"
// Other.
#include "detailed_orient.h"

// Other things not ready.
//#include "detailed_global_vertical.h"
// Detailed placement objectives.
//#include "detailed_hpwl.h"
//#include "detailed_drc.h"
//#include "detailed_abu.h"
//#include "detailed_pin.h"

// Algorithms.



namespace pdo
{


////////////////////////////////////////////////////////////////////////////////
// Defines.
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// Detailed::~Detailed:
////////////////////////////////////////////////////////////////////////////////
Detailed::~Detailed( void )
{
}

////////////////////////////////////////////////////////////////////////////////
// Detailed::improve:
////////////////////////////////////////////////////////////////////////////////
bool Detailed::improve( DetailedMgr& mgr )
//bool Detailed::improve( Architecture* arch, Network* network, RoutingParams* rt )
{
    m_mgr = &mgr;

    m_arch = mgr.getArchitecture();
    m_network = mgr.getNetwork();
    m_rt = mgr.getRoutingParams(); // Can be NULL.

    std::cout << "Detailed placement script: "
        << "'"
        << m_params.m_script.c_str()
        << "'"
        << std::endl;

    int err1, err2, err3, err4, err5;
    (void)err1;
    (void)err2;
    (void)err3;
    (void)err4;
    (void)err5;

    err1 = mgr.checkRegionAssignment(); // If segs build right and assignment okay, should be fine.
    err2 = mgr.checkRowAlignment(); // If segs build right and assignment okay, should be fine.
    err3 = mgr.checkSiteAlignment(); // Might not be okay if initial placement not legal or code made a mistake.
    err4 = mgr.checkOverlapInSegments(); // Might not be okay if initial placement not legal or code made a mistake.
    err5 = mgr.checkEdgeSpacingInSegments(); // Might not be okay.



    // Parse the script string and run each command.
    boost::char_separator<char> separators( " \r\t\n", ";" );
    boost::tokenizer<boost::char_separator<char> > tokens( m_params.m_script, separators );
    std::vector<std::string> args;
    for( boost::tokenizer<boost::char_separator<char> >::iterator it = tokens.begin();
            it != tokens.end();
            it++ )
    {
        std::string temp = *it;
        if( temp.back() == ';' )
        {
            while( !temp.empty() && temp.back() == ';' )
            {
                temp.resize( temp.size()-1 );
            }
            if( !temp.empty() )
            {
                args.push_back( temp );
            }
            // Command ended by a semi-colon.
            doDetailedCommand( args );
            args.clear();
        }
        else
        {
            args.push_back( temp );
        }
    }
    // Last command; possible if no ending semi-colon.
    doDetailedCommand( args );

    err1 = mgr.checkRegionAssignment(); // If segs build right and assignment okay, should be fine.
    err2 = mgr.checkRowAlignment(); // If segs build right and assignment okay, should be fine.
    err3 = mgr.checkSiteAlignment(); // Might not be okay if initial placement not legal or code made a mistake.
    err4 = mgr.checkOverlapInSegments( 10 ); // Might not be okay if initial placement not legal or code made a mistake.
    err5 = mgr.checkEdgeSpacingInSegments(); // Might not be okay.

    return true;
}

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
void Detailed::doDetailedCommand( std::vector<std::string>& args )
{
    if( args.size() == 0 )
    {
        return;
    }

    int err1, err2, err3, err4, err5;
    (void)err1;
    (void)err2;
    (void)err3;
    (void)err4;
    (void)err5;

    std::cout << "Detailed placement command:";
    for( size_t i = 0; i < args.size(); i++ )
    {
        std::cout << " " << args[i].c_str();
    }
    std::cout << std::endl;

    std::cout << "Issues before current detailed placement command:" << std::endl;
    err1 = m_mgr->checkRegionAssignment(); // If segs build right and assignment okay, should be fine.
    err2 = m_mgr->checkRowAlignment(); // If segs build right and assignment okay, should be fine.
    err3 = m_mgr->checkSiteAlignment(); // Might not be okay if initial placement not legal or code made a mistake.
    err4 = m_mgr->checkOverlapInSegments(); // Might not be okay if initial placement not legal or code made a mistake.
    err5 = m_mgr->checkEdgeSpacingInSegments(); // Might not be okay.

    // The first argument is always the command.  XXX: Not implemented, but 
    // include some samples...

    // Comment out some algos I haven't confirmed as working.
    if( strcmp( args[0].c_str(), "mis" ) == 0 )
    {
        std::cout << "Running independent set matching." << std::endl;
        DetailedMis mis( m_arch, m_network, m_rt );
        mis.run( m_mgr, args );
    }
    else if( strcmp( args[0].c_str(), "gs" ) == 0 )
    {
        std::cout << "Running global swaps." << std::endl;
        DetailedGlobalSwap gs( m_arch, m_network, m_rt );
        gs.run( m_mgr, args );
    }
    else if( strcmp( args[0].c_str(), "vs" ) == 0 )
    {
        std::cout << "Running vertical swaps." << std::endl;
        DetailedVerticalSwap vs( m_arch, m_network, m_rt );
        vs.run( m_mgr, args );
    }
    else if( strcmp( args[0].c_str(), "interleave" ) == 0 )
    {
        std::cout << "Running interleaving." << std::endl;
        DetailedInterleave interleave( m_arch, m_network, m_rt );
        interleave.run( m_mgr, args );
    }
    else if( strcmp( args[0].c_str(), "ro" ) == 0 )
    {
        std::cout << "Running reordering." << std::endl;
        DetailedReorderer ro( m_arch, m_network, m_rt );
        ro.run( m_mgr, args );
    }
    else if( strcmp( args[0].c_str(), "default" ) == 0 )
    {
        std::cout << "Running random improvement." << std::endl;
        DetailedRandom random( m_arch, m_network, m_rt );
        random.run( m_mgr, args );
    }
    else
    {
        std::cout << "Unknown command." << std::endl;
    }

    std::cout << "Issues after current detailed placement command:" << std::endl;
    err1 = m_mgr->checkRegionAssignment(); // If segs build right and assignment okay, should be fine.
    err2 = m_mgr->checkRowAlignment(); // If segs build right and assignment okay, should be fine.
    err3 = m_mgr->checkSiteAlignment(); // Might not be okay if initial placement not legal or code made a mistake.
    err4 = m_mgr->checkOverlapInSegments(); // Might not be okay if initial placement not legal or code made a mistake.
    err5 = m_mgr->checkEdgeSpacingInSegments(); // Might not be okay.
}


} // namespace pdo
