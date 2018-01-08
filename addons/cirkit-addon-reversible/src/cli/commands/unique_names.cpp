/* CirKit: A circuit toolkit
 * Copyright (C) 2009-2015  University of Bremen
 * Copyright (C) 2015-2017  EPFL
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include "unique_names.hpp"

#include <alice/rules.hpp>

#include <reversible/circuit.hpp>
#include <cli/reversible_stores.hpp>

namespace cirkit
{

unique_names_command::unique_names_command( const environment::ptr& env )
  : cirkit_command( env, "Ensure unique I/O names" )
{
  add_option( "--input_pattern", input_pattern, "pattern for input names" );
  add_option( "--output_pattern", output_pattern, "pattern for output names" );
  add_option( "--constant_pattern", constant_pattern, "pattern for constant names" );
  add_option( "--garbage_pattern", garbage_pattern, "pattern for garbage names" );
}

command::rules unique_names_command::validity_rules() const
{
  return {has_store_element<circuit>( env )};
}

void unique_names_command::execute()
{
  auto& circ = env->store<circuit>().current();

  std::vector<std::string> inputs, outputs;

  auto ictr = 0u, octr = 0u, cctr = 0u, gctr = 0u;
  for ( auto i = 0u; i < circ.lines(); ++i )
  {
    if ( (bool)circ.constants()[i] )
    {
      inputs.push_back( boost::str( boost::format( constant_pattern ) % ++cctr ) );
    }
    else
    {
      inputs.push_back( boost::str( boost::format( input_pattern ) % ++ictr ) );
    }

    if ( circ.garbage()[i] )
    {
      outputs.push_back( boost::str( boost::format( garbage_pattern ) % ++gctr ) );
    }
    else
    {
      outputs.push_back( boost::str( boost::format( output_pattern ) % ++octr ) );
    }
  }

  circ.set_inputs( inputs );
  circ.set_outputs( outputs );
}

}

// Local Variables:
// c-basic-offset: 2
// eval: (c-set-offset 'substatement-open 0)
// eval: (c-set-offset 'innamespace 0)
// End:
