#ifndef CandFwd_h
#define CandFwd_h

#include <cassert>
#include <vector>
#include <map>

//
// useful global names
//
class Candidate;

enum AngleUnit   { kRad=0,      kDeg };

typedef std::vector< Vertex* > VtxList;
typedef VtxList::const_iterator VtxListIterator;

typedef std::vector< Candidate* > CandList;
typedef CandList::const_iterator CandListIterator;

typedef std::multimap< Candidate*, Candidate* > CandMap;
typedef CandMap::const_iterator CandMapIterator;

typedef std::vector< size_t > CandIdList;
typedef CandIdList::const_iterator CandIdListIterator;

typedef std::multimap< size_t, size_t > CandIdMap;
typedef CandIdMap::const_iterator CandIdMapIterator;

typedef std::pair<size_t,float> indexDRPair;

struct SortIndexDRPairs
{
  bool operator()( const indexDRPair& p1, const indexDRPair& p2 ) const 
  {
    double dr1 = p1.second;
    double dr2 = p2.second;
    if( dr1>0 && dr2>0 )
      return dr1 < dr2;
    else if( dr1>0 ) return false;
    else if( dr2>0 ) return true;
    return fabs(dr1) < fabs(dr2);
  } 
};
  
typedef std::multimap< size_t, indexDRPair > CandIdDrMap; 
typedef CandIdDrMap::const_iterator CandIdDrMapIterator;


#endif
