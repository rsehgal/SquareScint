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


  HitSet hs1;
  hs1.insert(new Hit(4));
  hs1.insert(new Hit(5));
  hs1.insert(new Hit(2));
  hs1.insert(new Hit(1));

  for(const auto& hit : hs ){
    hit->Print();
}

  std::cout << "Equal Sets : " << EqualSets(hs,hs1) << std::endl;
  return 0;
}
