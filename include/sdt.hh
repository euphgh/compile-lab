#include <string>
extern "C" {
#include "AST.h"
}
#include "symbol_table.hh"
using std::string;
class hitIR;

//////////////////////////////////////////////////////////////////////////////
/// High-Level Definitions
//////////////////////////////////////////////////////////////////////////////
/** Program generate hitIR of all file
 */
hitIR Program_c(node_t& root);

/** ExtDecList merge all ExtDef IR into one
 */
hitIR ExtDecList_c(node_t& node);

/* ExtDef has three option
 * 1. check struct name when def variable
 * 2. insert struct to struct table
 * 3. check struct name when def func
 * */
hitIR ExtDef_c(node_t& node);

/* ExtDecList recieve a inh base type from ExtDef
 * pass type to VarDec and ExtDecList
 * return a var table
 */
var_table ExtDecList_c(node_t& node, const type_t* inh_type);

//////////////////////////////////////////////////////////////////////////////
/// Specifiers
//////////////////////////////////////////////////////////////////////////////
/** insert type to global type table 
 * return a point to it 
 */
const type_t* Specifier_c(node_t& node);

/** StructSpecifier has two option
 * insert struct to global type table when def
 * check global type table when use
 * return a point to it
 */
const type_t* StructSpecifier_c();

/** return unique id if empty
 */
string OptTag_c(node_t& node);

/** return struct id
 */
string Tag_c(node_t& node);

//////////////////////////////////////////////////////////////////////////////
/// Declarators
//////////////////////////////////////////////////////////////////////////////
/** recieve base type from Specifier_c 
 * add array type into global type table 
 * return var_t to ExtDecList, ParamDec or Dec 
 * add var_t to responded var_table 
 * */
const var_t VarDec_c(node_t& node, const type_t* inh_type);

/** add func type to global function table 
 * generate IR of the function
 */
hitIR FunDec_c(node_t& node, const type_t* inh_ret_type) ;

/** return a var table to FunDec for building func type 
 */
var_table VarList_c(node_t& node) ;

/** return a var_t to VarList to build a var table 
 */
var_t ParamDec_c(node_t& node);

//////////////////////////////////////////////////////////////////////////////
/// Statements
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
/// Local Definitions
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
/// Expressions
//////////////////////////////////////////////////////////////////////////////
