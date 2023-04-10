#ifndef __SDT_HH__
#define __SDT_HH__
#include <string>
#include "ast.hh"
#include "symbol_table.hh"
using std::string;
using std::vector;
class hitIR {
    string code;

  public:
    hitIR append(hitIR code);
};
class reg_t;
class label_t;

//////////////////////////////////////////////////////////////////////////////
/// High-Level Definitions
//////////////////////////////////////////////////////////////////////////////
/** Program generate hitIR of all file
 */
hitIR Program_c(const node_t& root);

/** ExtDecList merge all ExtDef IR into one
 */
hitIR ExtDefList_c(const node_t& node);

/* ExtDef has three option
 * 1. check struct name when def variable
 * 2. insert struct to struct table
 * 3. check struct name when def func
 *    set a function point and a local table point
 * Error17: struct id is not def
 * */
hitIR ExtDef_c(const node_t& node);

/* ExtDecList recieve a inh base type from ExtDef
 * pass inherit type to VarDec and ExtDecList
 * insert var returned by VarDec into global var table
 * Error03: var can not redefine
 */
void ExtDecList_c(const node_t& node, const type_t* inh_type);

//////////////////////////////////////////////////////////////////////////////
/// Specifiers
//////////////////////////////////////////////////////////////////////////////

/** insert type to global type table
 * return a point to it
 */
const type_t* Specifier_c(const node_t& node);

/** StructSpecifier has two option
 * insert struct to global type table when def
 * check global type table when use
 * return a point to it
 * Error16: id is not same with struct table and global var
 */
const type_t* StructSpecifier_c();

/** return unique id if empty
 */
string OptTag_c(const node_t& node);

/** return struct id
 */
string Tag_c(const node_t& node);

//////////////////////////////////////////////////////////////////////////////
/// Declarators
//////////////////////////////////////////////////////////////////////////////

/** recieve base type from Specifier_c
 * add array type into global type table
 * return var_t to ExtDecList, ParamDec or Dec
 * add var_t to responded var_table
 * Error03: var name is node duplicate with local table and global var/struct
 * */
const var_t VarDec_c(const node_t& node, const type_t* inh_type);

/** add func type to global function table
 * new a function var stack whose bottom is var table of args
 * Error04: check global function table for not duplicate
 * set current function point to it
 * generate IR of the function
 */
hitIR FunDec_c(const node_t& node, const type_t* inh_ret_type);

/** return a var table to FunDec for building func type
 */
var_table VarList_c(const node_t& node);

/** return a var_t to VarList to build a var table
 */
var_t ParamDec_c(const node_t& node);

//////////////////////////////////////////////////////////////////////////////
/// Statements
//////////////////////////////////////////////////////////////////////////////

/** need do three things
 * 1. push new empty var table function var stack
 * 2. parser DefList all var into new var table
 * 3. return IR of StmtList
 */
hitIR CompSt_c(const node_t& node);

/** return all Stmt merged
 */
hitIR StatList_c(const node_t& node);

/** has 4 option, see reference page 83
 * 1. new CompSt, set local stack
 * 2. generate return code
 * 3. generate if code
 * 4. generate while code
 * Error08: return type not match
 */
hitIR Stmt_c(const node_t& node);

//////////////////////////////////////////////////////////////////////////////
/// Local Definitions
//////////////////////////////////////////////////////////////////////////////

/** parse all def
 */
hitIR DefList_c(const node_t& node);

/** get base type from Specifier
 * DecList inherit base type from Specifier and def new type
 * Error17: struct id is not def
 */
hitIR Def_c(const node_t& node);

/** parse all Dec with base type
 *  merge all Dec Code
 *  only to distinguish two case
 */
hitIR DecList_compst_c(const node_t& node, const type_t* inh_type);
void DecList_struct_c(const node_t& node, const type_t* inh_type);

/** Dec_compst_c generate init code
 * insert var returned by VarDec into current function stack top
 * Error03: var name nis not same with struct id and local var
 */
hitIR Dec_compst_c(const node_t& node, const type_t* inh_type);

/** Dec_struct_c not generate init code
 * insert var returned by VarDec into struct var table
 * Error15: var name nis not same with struct field, and not init
 */
void Dec_struct_c(const node_t& node, const type_t* inh_type);

//////////////////////////////////////////////////////////////////////////////
/// Expressions
//////////////////////////////////////////////////////////////////////////////

/** use current function point and global var table to check id
 * assign ret_type point to type table
 * ret generate code
 * Error01: var name must has def
 * Error02: function name must has def
 * Error05: function name must has def
 * Error06: left of assignop is not include ...
 * Error07: assignop connect 2 var with same type
 * Error09: function call para match
 * Error10: only array var use [ number ]
 * Error11: only function call use ( para )
 * Error12: array var index type is int
 * Error13: only struct var has Dot
 * Error14: struct var field name is in struct var table
 */
hitIR Exp_c(const node_t& node, const reg_t* place, type_t* ret_type);

/** generate code to eval expr and record repond reg
 */
hitIR Args_c(const node_t& node, vector<reg_t*>& arg_list);

/** generate goto code
 */
hitIR Cond_c(const node_t& node, label_t* b_true, label_t* b_false);
#endif 
