#pragma once
#include "UHH2/core/include/AnalysisModule.h"
#include "UHH2/core/include/Event.h"
#include "UHH2/core/include/Utils.h"
#include "UHH2/common/include/Utils.h"

#include <map>


/**
 * Classes to aid automatically running modules for different years and run periods.
 *
 * Thee encapsulate the logic of switching years/runs away, so the user does
 * not need to implement it themselves.
 */


class YearSwitcher: public uhh2::AnalysisModule {
public:
  YearSwitcher(uhh2::Context & ctx);

  // Automatically run the appropriate module.
  // If there isn't a matching one, just return true
  virtual bool process(uhh2::Event & event) override;

  // Methods to assign module for each year
  // Note that the setup<year>v* are more specific, and if set,
  // will take preference over the module passed to setup<year>
  void setup2016(uhh2::AnalysisModule * module);
  void setup2016v2(uhh2::AnalysisModule * module);
  void setup2016v3(uhh2::AnalysisModule * module);

  void setup2017(uhh2::AnalysisModule * module);
  void setup2017v1(uhh2::AnalysisModule * module);
  void setup2017v2(uhh2::AnalysisModule * module);

  void setup2018(uhh2::AnalysisModule * module);

private:
  Year year_;
  bool doneInit_;

  // have modules for each year, plus for the specific versions of each year
  // shared_ptr, because the user might already own it, and we want to ensure
  // it is kept alive, or deleted as necessary if this is the only owner

  // FIXME: broken as this will create a new copy, not extend lifetime of original?!
  std::shared_ptr<uhh2::AnalysisModule> module2016_, module2016v2_, module2016v3_;
  std::shared_ptr<uhh2::AnalysisModule> module2017_, module2017v1_, module2017v2_;
  std::shared_ptr<uhh2::AnalysisModule> module2018_;
  std::shared_ptr<uhh2::AnalysisModule> theModule_;

};


/**
 * Class to handle different AnalysisModules for different run periods in data
 *
 * User should setup a module for each run period (e.g. "A") with the `setupRun()`
 * method.
 * Calling `process()` will then automatically figure out which one to use based
 * on the event run number.
 */
class RunSwitcher: public uhh2::AnalysisModule {
public:
  RunSwitcher(const std::string & year);

  // Run the module corresponding to this year in the relevant run period
  // If none found, return true
  virtual bool process(uhh2::Event & event) override;

  // Method to assign a module to a particular run period
  void setupRun(const std::string & runPeriod, uhh2::AnalysisModule * module);

private:
  std::string year_;
  std::map<std::string, std::pair<int, int>> runNumberMap_;
  std::map<std::string, std::shared_ptr<uhh2::AnalysisModule>> runModuleMap_;
};
