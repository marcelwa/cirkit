#include <alice/alice.hpp>

namespace alice
{
ALICE_ADD_FILE_TYPE_WRITE_ONLY( projectq, "ProjectQ" );
ALICE_ADD_FILE_TYPE_WRITE_ONLY( qasm, "QASM" );
ALICE_ADD_FILE_TYPE( quil, "Quil" );
}
