/*********************                                                        */
/*! \file statistics_registry.cpp
 ** \verbatim
 ** Original author: taking
 ** Major contributors: mdeters
 ** Minor contributors (to current version): none
 ** This file is part of the CVC4 prototype.
 ** Copyright (c) 2009-2012  The Analysis of Computer Systems Group (ACSys)
 ** Courant Institute of Mathematical Sciences
 ** New York University
 ** See the file COPYING in the top-level source directory for licensing
 ** information.\endverbatim
 **
 ** \brief [[ Add one-line brief description here ]]
 **
 ** [[ Add lengthier description here ]]
 ** \todo document this file
 **/

#include "util/statistics_registry.h"
#include "expr/node_manager.h"
#include "expr/expr_manager_scope.h"
#include "expr/expr_manager.h"
#include "lib/clock_gettime.h"
#include "smt/smt_engine_scope.h"
#include "smt/smt_engine.h"

#ifdef CVC4_STATISTICS_ON
#  define __CVC4_USE_STATISTICS true
#else
#  define __CVC4_USE_STATISTICS false
#endif

namespace CVC4 {

namespace stats {

// This is a friend of SmtEngine, just to reach in and get it.
// this isn't a class function because then there's a cyclic
// dependence.
inline StatisticsRegistry* getStatisticsRegistry(SmtEngine* smt) {
  return smt->d_statisticsRegistry;
}

}/* CVC4::stats namespace */

#ifndef __BUILDING_STATISTICS_FOR_EXPORT

StatisticsRegistry* StatisticsRegistry::current() {
  return stats::getStatisticsRegistry(smt::currentSmtEngine());
}

void StatisticsRegistry::registerStat(Stat* s) throw(AssertionException) {
#ifdef CVC4_STATISTICS_ON
  StatSet& stats = current()->d_stats;
  AlwaysAssert(stats.find(s) == stats.end(),
               "Statistic `%s' was already registered with this registry.", s->getName().c_str());
  stats.insert(s);
#endif /* CVC4_STATISTICS_ON */
}/* StatisticsRegistry::registerStat() */

void StatisticsRegistry::unregisterStat(Stat* s) throw(AssertionException) {
#ifdef CVC4_STATISTICS_ON
  StatSet& stats = current()->d_stats;
  AlwaysAssert(stats.find(s) != stats.end(),
               "Statistic `%s' was not registered with this registry.", s->getName().c_str());
  stats.erase(s);
#endif /* CVC4_STATISTICS_ON */
}/* StatisticsRegistry::unregisterStat() */

#endif /* ! __BUILDING_STATISTICS_FOR_EXPORT */

void StatisticsRegistry::registerStat_(Stat* s) throw(AssertionException) {
#ifdef CVC4_STATISTICS_ON
  AlwaysAssert(d_stats.find(s) == d_stats.end());
  d_stats.insert(s);
#endif /* CVC4_STATISTICS_ON */
}/* StatisticsRegistry::registerStat_() */

void StatisticsRegistry::unregisterStat_(Stat* s) throw(AssertionException) {
#ifdef CVC4_STATISTICS_ON
  AlwaysAssert(d_stats.find(s) != d_stats.end());
  d_stats.erase(s);
#endif /* CVC4_STATISTICS_ON */
}/* StatisticsRegistry::unregisterStat_() */

void StatisticsRegistry::flushStat(std::ostream &out) const {
#ifdef CVC4_STATISTICS_ON
  flushInformation(out);
#endif /* CVC4_STATISTICS_ON */
}

void StatisticsRegistry::flushInformation(std::ostream &out) const {
#ifdef CVC4_STATISTICS_ON
  this->StatisticsBase::flushInformation(out);
#endif /* CVC4_STATISTICS_ON */
}

void TimerStat::start() {
  if(__CVC4_USE_STATISTICS) {
    AlwaysAssert(!d_running);
    clock_gettime(CLOCK_MONOTONIC, &d_start);
    d_running = true;
  }
}/* TimerStat::start() */

void TimerStat::stop() {
  if(__CVC4_USE_STATISTICS) {
    AlwaysAssert(d_running);
    ::timespec end;
    clock_gettime(CLOCK_MONOTONIC, &end);
    d_data += end - d_start;
    d_running = false;
  }
}/* TimerStat::stop() */

RegisterStatistic::RegisterStatistic(ExprManager& em, Stat* stat) :
  d_reg(NodeManager::fromExprManager(&em)->getStatisticsRegistry()),
  d_stat(stat) {
  d_reg->registerStat_(d_stat);
}

RegisterStatistic::RegisterStatistic(SmtEngine& smt, Stat* stat) :
  d_reg(stats::getStatisticsRegistry(&smt)),
  d_stat(stat) {
  d_reg->registerStat_(d_stat);
}

}/* CVC4 namespace */

#undef __CVC4_USE_STATISTICS