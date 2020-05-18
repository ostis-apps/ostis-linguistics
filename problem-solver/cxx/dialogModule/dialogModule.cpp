#include "dialogModule.hpp"
#include "keynodes/keynodes.hpp"
#include "agents/TextNormalizationAgent.hpp"

using namespace dialogModule;

SC_IMPLEMENT_MODULE(DialogModule)

sc_result DialogModule::InitializeImpl()
{
  if (!dialogModule::Keynodes::InitGlobal())
    return SC_RESULT_ERROR;

  SC_AGENT_REGISTER(TextNormalizationAgent)

  return SC_RESULT_OK;
}

sc_result DialogModule::ShutdownImpl()
{
  SC_AGENT_UNREGISTER(TextNormalizationAgent)

  return SC_RESULT_OK;
}
