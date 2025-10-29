/*
**	Filename : TestSet.cpp
**	2025-08-04
**	username : rsehgal
*/
#include <iostream>
#include "Hit.h"
#include "Helpers.h"

int main(int argc, char *argv[])
{
  HitSet hs;
  hs.insert(new Hit(1));
  hs.insert(new Hit(5));
  hs.insert(new Hit(4));
  hs.insert(new Hit(2));
  hs.insert(new Hit(2));
  hs.insert(new Hit(1));
  hs.insert(new Hit(6));


  HitSet hs1;
  hs1.insert(new Hit(4));
  hs1.insert(new Hit(5));
  hs1.insert(new Hit(2));
  hs1.insert(new Hit(1));

  for(const auto& hit : hs ){
    hit->Print();
}

  std::cout << "Equal Sets : " << EqualSets(hs,hs1) << std::endl;
  std::cout << "Sub Sets : " << IsSubset(hs,hs1) << std::endl;
  
  //Adding one more element to violate the subset condition
  hs1.insert(new Hit(8));
  std::cout << "Sub Sets : " << IsSubset(hs,hs1) << std::endl;
 
  hs.insert(new Hit(8));
  hs1.insert(new Hit(6));
  std::cout << "Sub Sets : " << IsSubset(hs,hs1) << std::endl;
  return 0;
}
