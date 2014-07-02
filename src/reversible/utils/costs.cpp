/* RevKit: A Toolkit for Reversible Circuit Design (www.revkit.org)
 * Copyright (C) 2009-2014  The RevKit Developers <revkit@informatik.uni-bremen.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "costs.hpp"

#include <boost/tuple/tuple.hpp>

#include "../target_tags.hpp"
#include "../functions/flatten_circuit.hpp"

namespace cirkit
{

  cost_t gate_costs::operator()( const circuit& circ ) const
  {
    return circ.num_gates();
  }

  cost_t line_costs::operator()( const circuit& circ ) const
  {
    return circ.lines();
  }

  cost_t transistor_costs::operator()( const gate& g, unsigned lines ) const
  {
    return 8ull * g.controls().size();
  }

  struct costs_visitor : public boost::static_visitor<cost_t>
  {
    explicit costs_visitor( const circuit& circ ) : circ( circ ) {}

    cost_t operator()( const costs_by_circuit_func& f ) const
    {
      // flatten before if the circuit has modules
      if ( circ.modules().empty() )
      {
        return f( circ );
      }
      else
      {
        circuit flattened;
        flatten_circuit( circ, flattened );
        return f( flattened );
      }
    }

    cost_t operator()( const costs_by_gate_func& f ) const
    {
      cost_t sum = 0ull;
      for ( const auto& g : circ )
      {
        // respect modules
        if ( is_module( g ) )
        {
          sum += costs( *boost::any_cast<module_tag>( g.type() ).reference.get(), f );
        }
        else
        {
          sum += f( g, circ.lines() );
        }
      }
      return sum;
    }

  private:
    const circuit& circ;
  };

  cost_t costs( const circuit& circ, const cost_function& f )
  {
    return boost::apply_visitor( costs_visitor( circ ), f );
  }

}

// Local Variables:
// c-basic-offset: 2
// End: