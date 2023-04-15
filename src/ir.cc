#include "symbol_table.hh"
#include <fmt/core.h>
#include <iterator>
#include <memory>
using std::unique_ptr;

hitIR* hitIR::append(std::unique_ptr<hitIR> code) {
    for (auto ir : code->ir_list)
        ir_list.push_back(ir);
    return this;
}
unsigned label_t::total = 0;
const label_t* label_t::new_label() {
    label_pool.push_back(label_t {total++});
    return &*std::prev(label_pool.cend());
}
unique_ptr<hitIR> label_t::ir_goto() const{
    return std::make_unique<hitIR>(fmt::format("GOTO L{}", id));
}
unique_ptr<hitIR> label_t::ir_mark() const{
    return std::make_unique<hitIR>(fmt::format("LABEL L{} :", id));
}


unsigned mem_t::total =0;
mem_t::mem_t(std::string _name, unsigned _size, unsigned _id): id(_id), name(_name), size(_size){}
std::unique_ptr<hitIR> mem_t::dec(const var_t* derived_var,
        unsigned _size){
    mem_pool.push_back(mem_t {derived_var->name, _size, total++});
    return std::make_unique<hitIR>(fmt::format("DEC {:s} {:d}",derived_var->name, _size));
}
const mem_t* mem_t::map_var(std::string _name){
    for (auto mem = mem_pool.cbegin(); mem!=mem_pool.cend(); ++mem) {
        if (mem->name==_name) 
            return &*mem;
    }
    return nullptr;
}

reg_t* reg_t::new_unique(){
    reg_pool.push_back(reg_t {total++});
    return &*std::prev(reg_pool.end());
}
    static reg_t* define_var(const var_t* basic_var);
    hitIR assign(const reg_t* right) const;
    hitIR assign(int data) const;
    hitIR assign(float data) const;
    enum op_t { op_add, op_sub, op_mul, op_div };
    hitIR is_op_of(op_t op, reg_t* src1, reg_t* src2) const;

    hitIR is_op_of(op_t op, int src1, reg_t* src2) const;
    hitIR is_op_of(op_t op, float src1, reg_t* src2) const;

    hitIR is_op_of(op_t op, reg_t* src1, int src2) const;
    hitIR is_op_of(op_t op, reg_t* src1, float src2) const;

    hitIR is_op_of(op_t op, int src1, int src2) const;
    hitIR is_op_of(op_t op, float src1, float src2) const;

    hitIR if_goto(std::string relop, const reg_t* src2,
                  const label_t* b_true) const;
    hitIR if_goto(std::string relop, int src2, const label_t* b_true) const;

    hitIR is_addr_of(const mem_t* src1) const;
    hitIR load_from(reg_t* src1) const;
    hitIR store_to(reg_t* src1) const;

