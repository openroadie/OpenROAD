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


#pragma once


#include <vector>
#include <deque>
#include "rectangle.h"


namespace dpo
{

class Architecture;
class Node;

class SpaceManager
{
public:
    class RectangleSorter1
    {
    public:
        RectangleSorter1( int method = 0 ):
            _method( method )
        {
        }

        inline bool operator () ( Rectangle * a, Rectangle * b ) const
        {
            if( _method == 1 ) 
            {
                // Sort left to right.  Use below vs above to break ties.
                if (a->xmin() == b->xmin() )
                {
                    return a->ymin() < b->ymin();
                }
                return a->xmin() < b->xmin();
            } 
            else
            {
                // Sort by area.
                double aa = (a->xmax() - a->xmin()) * (a->ymax() - a->ymin() );
                double ab = (b->xmax() - b->xmin()) * (b->ymax() - b->ymin() );
                return aa > ab;
            } 
        }
    public:
        int _method; // how to sort.
    };

    class RectangleSorter2
    {
    public:
        RectangleSorter2( double xmin, double xmax, double ymin, double ymax ):
            _xmin( xmin ),
            _xmax( xmax ),
            _ymin( ymin ),
            _ymax( ymax )
        {
        }

        inline bool operator () ( Rectangle * a, Rectangle * b ) const
        {
            // Sort by distance to rectangle provided in ctor.

            double distX, distY;

            distX = 0.;
            if     ( _xmin <= a->xmin() ) distX = a->xmin() - _xmin;
            else if( _xmax >= a->xmax() ) distX = _xmax - a->xmax();
            distY = 0.;
            if     ( _ymin <= a->ymin() ) distY = a->ymin() - _ymin;
            else if( _ymax >= a->ymax() ) distY = _ymax - a->ymax();

            double da = std::sqrt( distX*distX+distY*distY );

            distX = 0.;
            if     ( _xmin <= b->xmin() ) distX = b->xmin() - _xmin;
            else if( _xmax >= b->xmax() ) distX = _xmax - b->xmax();
            distY = 0.;
            if     ( _ymin <= b->ymin() ) distY = b->ymin() - _ymin;
            else if( _ymax >= b->ymax() ) distY = _ymax - b->ymax();

            double db = std::sqrt( distX*distX+distY*distY );

            return da < db;
        }
    public:
        double _xmin;
        double _xmax;
        double _ymin;
        double _ymax;
    };

    class RectangleSorter3
    {
    public:
        RectangleSorter3( double xmin, double xmax, double ymin, double ymax ):
            _xmin( xmin ),
            _xmax( xmax ),
            _ymin( ymin ),
            _ymax( ymax )
        {
        }

        inline bool operator () ( Rectangle * a, Rectangle * b ) const
        {
            // Sort by a weird measure of overlap.  Assumes rectangles are
            // centered and computes overlap measure.

            double minW, minH;

            minW = std::min( (_xmax - _xmin), (a->xmax() - a->xmin()) );
            minH = std::min( (_ymax - _ymin), (a->ymax() - a->ymin()) );

            double oa = (minW * minH)/((_xmax- _xmin) * (_ymax - _ymin));

            minW = std::min( (_xmax - _xmin), (b->xmax() - b->xmin()) );
            minH = std::min( (_ymax - _ymin), (b->ymax() - b->ymin()) );

            double ob = (minW * minH)/((_xmax - _xmin) * (_ymax - _ymin));

            return oa < ob;
        }
    public:
        double _xmin;
        double _xmax;
        double _ymin;
        double _ymax;
    };

public:

public:
    SpaceManager( void );
    SpaceManager( double xmin, double xmax, double ymin, double ymax );
    virtual ~SpaceManager( void );

    Rectangle* AddFullRectangle( double xmin, double xmax, double ymin, double ymax );
    void RemFullRectangle( Rectangle* ptr );

    void GetRectanglesFit( double xmin, double xmax, double ymin, double ymax, 
            std::vector<Rectangle*>& possibleLEs, double radius = std::numeric_limits<double>::max() );

    void Reset( double xmin, double xmax, double ymin, double ymax );
    bool GetLargestFreeSpace( double& xmin, double& xmax, double& ymin, double& ymax );

    void single( double xmin, double xmax, double ymin, double ymax, Architecture* arch );

    std::vector<Rectangle*>&    getFreeRectangles( void ) { return _freeSpaceRectangles; }

    void GetClosestRectanglesLarge( bool onlyNeedOne, 
            double xmin, double xmax, double ymin, double ymax,
            std::vector<Rectangle*>&, 
            double radius = std::numeric_limits<double>::max()
            );
    void GetClosestRectanglesLarge( bool onlyNeedOne, 
            double xmin, double xmax, double ymin, double ymax,
            std::vector<Rectangle*>&, 
            Architecture* arch, Node* ndi, 
            double radius = std::numeric_limits<double>::max()
            );

    void GetClosestRectanglesRange( bool onlyNeedOne, 
            double xmin, double xmax, double ymin, double ymax,
            std::vector<Rectangle*>&, 
            double radius = std::numeric_limits<double>::max()
            );

protected:
    void cleanup0( void );
    void cleanup1( void );

    void determineNeighbors( Rectangle* f, std::vector<Rectangle*>& neighbors );
    void delRectangle( Rectangle*& rect );
    Rectangle* getRectangle( double xmin, double xmax, double ymin, double ymax );

protected:
    std::vector<Rectangle*>     _fullSpaceRectangles; // Used space.
    std::vector<Rectangle*>     _freeSpaceRectangles; // Free space.
    std::deque<Rectangle*>      _availableRectangles; // Allocated rectangles.
};


} // namespace dpo
