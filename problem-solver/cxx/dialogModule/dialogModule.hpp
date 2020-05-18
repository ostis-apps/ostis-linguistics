#pragma once

#include <sc_memory.h>
#include <sc-memory/cpp/sc_module.hpp>

#include "dialogModule.generated.hpp"

class DialogModule : public ScModule
{
  SC_CLASS(LoadOrder(50))
  SC_GENERATED_BODY()

  virtual sc_result InitializeImpl() override;

  virtual sc_result ShutdownImpl() override;
};
