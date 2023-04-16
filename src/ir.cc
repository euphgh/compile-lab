#include "symbol_table.hh"
#include <fmt/core.h>
#include <iterator>
#include <memory>
#include <sstream>
using std::unique_ptr;
using std::make_unique;
using fmt::format;
using std::string;
using std::stringstream;

hitIR* hitIR::append(std::unique_ptr<hitIR> code) {
    for (auto ir : code->ir_list)
        ir_list.push_back(ir);
    return this;
}
std::string hitIR::to_string(){
    stringstream buffer;
    for (auto inst : this->ir_list)
        buffer << inst << "\n";
    return buffer.str();
}
const label_t* label_t::new_label() {
    label_pool.push_back(label_t {total++});
    return &*std::prev(label_pool.cend());
}
unique_ptr<hitIR> label_t::ir_goto() const{
    return make_unique<hitIR>(format("GOTO l{}", id));
}
unique_ptr<hitIR> label_t::ir_mark() const{
    return make_unique<hitIR>(format("LABEL l{} :", id));
}

mem_t::mem_t(unsigned _size, unsigned _id): id(_id), size(_size){}
std::unique_ptr<hitIR> mem_t::dec(var_t* derived_var){
    derived_var->addr = new mem_t {derived_var->type->size, total};
    return make_unique<hitIR>(format("DEC m{:d} {:d}", total++, derived_var->type->size));
}

reg_t* reg_t::new_unique(){
    return new reg_t(total++);
}

reg_t useless_reg(~0);
reg_t* reg_t::useless(){ return &useless_reg; }

std::unique_ptr<hitIR> reg_t::assign(const reg_t* right) const{
    return make_unique<hitIR>(format("r{} := r{}", id, right->id));
}
std::unique_ptr<hitIR> reg_t::assign(int data) const{
    return make_unique<hitIR>(format("r{} := #{}", id, data));
}
std::unique_ptr<hitIR> reg_t::assign(float data) const{
    return make_unique<hitIR>(format("r{} := #{}", id, data));
}
std::unique_ptr<hitIR> reg_t::is_op_of(string op_str, reg_t* src1, reg_t* src2) const {
    return make_unique<hitIR>(format("r{} := r{}{}r{}", id, src1->id, op_str, src2->id));
}
std::unique_ptr<hitIR> reg_t::is_op_of(string op_str, int src1, reg_t* src2) const{
    return make_unique<hitIR>(format("r{} := #{}{}r{}", id, src1, op_str, src2->id));
}
std::unique_ptr<hitIR> reg_t::is_op_of(string op_str, float src1, reg_t* src2) const{
    return make_unique<hitIR>(format("r{} := #{}{}r{}", id, src1, op_str, src2->id));
}
std::unique_ptr<hitIR> reg_t::is_op_of(string op_str, reg_t* src1, int src2) const{
    return make_unique<hitIR>(format("r{} := #{}{}r{}", id, src1->id, op_str, src2));
}
std::unique_ptr<hitIR> reg_t::is_op_of(string op_str, reg_t* src1, float src2) const{
    return make_unique<hitIR>(format("r{} := #{}{}r{}", id, src1->id, op_str, src2));
}
std::unique_ptr<hitIR> reg_t::is_op_of(string op_str, int src1, int src2) const{
    return make_unique<hitIR>(format("r{} := #{}{}r{}", id, src1, op_str, src2));
}
std::unique_ptr<hitIR> reg_t::is_op_of(string op_str, float src1, float src2) const{
    return make_unique<hitIR>(format("r{} := #{}{}r{}", id, src1, op_str, src2));
}
std::unique_ptr<hitIR> reg_t::if_goto(string relop, const reg_t* src2, const label_t* b_true) const {
    return make_unique<hitIR>(format("IF r{} {} r{} GOTO l{}", id, relop, src2->id, b_true->id));
}
std::unique_ptr<hitIR> reg_t::if_goto(string relop, int src2, const label_t* b_true) const{
    return make_unique<hitIR>(format("IF r{} {} r{} GOTO l{}", id, relop, src2, b_true->id));
}
std::unique_ptr<hitIR> reg_t::is_addr_of(const mem_t* src1) const {
    return make_unique<hitIR>(format("r{} := &m{}", id, src1->id));
}
unique_ptr<hitIR> reg_t::load_from(const reg_t* src1) const{
    return make_unique<hitIR>(format("r{} := *r{}", id, src1->id));
}
unique_ptr<hitIR> reg_t::store_to(const reg_t* src1) const{
    return make_unique<hitIR>(format("*r{} := r{}", src1->id, id));
}
unique_ptr<hitIR> reg_t::call(string func_name) const {
    return make_unique<hitIR>(format("r{} := CALL {}", id, func_name));
}
unique_ptr<hitIR> reg_t::ret() const{
    return make_unique<hitIR>(format("RETURN r{}", id));
}
