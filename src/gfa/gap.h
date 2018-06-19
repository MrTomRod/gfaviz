#pragma once
#include "gfa/defines.h"
#include "gfa/line.h"
#include "gfa/error.h"
#include "gfa/filereader.h"
#include "gfa/types.h"
#include "gfa/alignment.h"

class GfaGraph;
class GfaSegment;

class GfaGap: public GfaReferenceableLine {
  public:
    GfaGap();
    void fromLine(GfaFileReader* fr);
    void print() const;
    void resolve(GfaGraph* _g);
    
    GfaSegment* getSegment(int segnum) const;
    GfaSegment* getOppositeSegment(GfaSegment* seg) const;
    bool isInedge(int segnum) const;
    bool isInedge(const GfaSegment *seg) const;
    bool isOutedge(int segnum) const;
    bool isOutedge(const GfaSegment *seg) const;
    
  private:
    GfaRef segments[2];
    unsigned long dist;
    GfaVariance variance;
};
