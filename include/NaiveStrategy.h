#ifndef __NAIVE_STRATEGY_H__
#define __NAIVE_STRATEGY_H__

#include "IStrategy.h"

class NaiveStrategy : public IStrategy
{
  public:
    void requestNextBestMoveOn(Board *);
    std::string toString();
};

#endif // __NAIVE_STRATEGY_H__
