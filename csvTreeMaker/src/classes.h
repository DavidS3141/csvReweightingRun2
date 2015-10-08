//Add includes for your classes here
#include "DataFormats/Common/interface/Wrapper.h"
#include "csvReweightingRun2/csvTreeMaker/interface/csvEventVars.h"

#include <vector>

namespace {
   struct csvReweightingRun2_csvTreeMaker{
  
     //add 'dummy' Wrapper variable for each class type you put into the Event
     EventVars EventVarsDummy0;
     std::vector<EventVars> EventVarsDummy1;  
     edm::Wrapper<EventVars> EventVarsDumm2;
     edm::Wrapper<std::vector<EventVars> > EventVarsDummy4;

   };
}
