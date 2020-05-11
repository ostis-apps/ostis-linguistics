/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#pragma once

#include "sc-memory/cpp/sc_addr.hpp"
#include "sc-memory/cpp/sc_object.hpp"

#include "keynodes.generated.hpp"

namespace exampleModule
{

class Keynodes : public ScObject
{
  SC_CLASS()
  SC_GENERATED_BODY()

public:

  SC_PROPERTY(Keynode("question_find_subdividing"), ForceCreate)
  static ScAddr question_find_subdividing;

  SC_PROPERTY(Keynode("nrel_sc_text_translation"), ForceCreate)
  static ScAddr nrel_sc_text_translation;

  SC_PROPERTY(Keynode("rrel_standard"), ForceCreate)
  static ScAddr rrel_standard;

  SC_PROPERTY(Keynode("nrel_paradigm"), ForceCreate)
  static ScAddr nrel_paradigm;

  SC_PROPERTY(Keynode("undefined_word"), ForceCreate)
  static ScAddr undefined_word;

  SC_PROPERTY(Keynode("nrel_text_decomposition"), ForceCreate)
  static ScAddr nrel_text_decomposition;

  SC_PROPERTY(Keynode("rrel_1"), ForceCreate)
  static ScAddr rrel_1;

  SC_PROPERTY(Keynode("nrel_sequence"), ForceCreate)
  static ScAddr nrel_sequence;
};

} // namespace exampleModule
