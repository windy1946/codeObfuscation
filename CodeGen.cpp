#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "llvm/ADT/APInt.h"
#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include <string>

#include "CodeGen.h"
#include "UtilTool.h"

using namespace llvm;



Value* IntegerCodeGen::codegen(BasicBlock* block){
    errs()<<"integerCodegen: " << num << "\n";
    LLVMContext& context = block->getParent()->getContext();
    errs()<<"after context\n";
    return ConstantInt::get(context, APInt(this->intNumbit, num));
}




Value* IntegerArithCodeGen::codegen(BasicBlock* block){
    errs()<<"operationCodegen!\n";
    ConstantInt* constint1 = dyn_cast<ConstantInt>(this->val1);
    ConstantInt* constint2 = dyn_cast<ConstantInt>(this->val2);
    int numval1 = 0;
    int numval2 = 0;
    if(constint1 && constint2) {
        numval1 = constint1->getSExtValue();
        numval2 = constint2->getSExtValue();
    }else{
        return NULL;
    }
        
    BasicBlock::iterator insert = block->end();
    insert--;
    BinaryOperator* binop;
        
    switch(this->op){
        case ADD:
            errs()<<"add\n";
            binop = BinaryOperator::Create(Instruction::Add, val1, val2, "addtmp", &*insert);//builder.CreateAdd(val1, val2, "addtmp");
            this->ret = numval1 + numval2;
            errs()<<"after add\n";
            break;
        case SUB:
            errs()<<"sub\n";
            binop = BinaryOperator::Create(Instruction::Sub, val1, val2, "subtmp", &*insert);//builder.CreateAdd(val1, val2, "addtmp");
            this->ret = numval1 - numval2;
            errs()<<"after sub\n";
            break;
        case MUL:
            errs()<<"mul\n";
            binop = BinaryOperator::Create(Instruction::Mul, val1, val2, "multmp", &*insert);//builder.CreateAdd(val1, val2, "addtmp");
            this->ret = numval1 * numval2;
            errs()<<"after mul\n";
            break;
        case DIV:
            errs()<<"div\n";
            binop = BinaryOperator::Create(Instruction::SDiv, val1, val2, "divtmp", &*insert);//builder.CreateAdd(val1, val2, "addtmp");
            this->ret = numval1 / numval2;
            errs()<<"after div\n";
            break;
    }
    return (Value*)binop;    
}


Value* BoolCmpCodeGen::codegen(BasicBlock* block){
    errs()<<"cmpCodegen!\n";
    ConstantInt* constint1 = dyn_cast<ConstantInt>(this->val1);
    ConstantInt* constint2 = dyn_cast<ConstantInt>(this->val2);
    int numval1 = 0;
    int numval2 = 0;
    if(constint1 && constint2) {
        numval1 = constint1->getSExtValue();
        numval2 = constint2->getSExtValue();
    }else{
        return NULL;
    }

    IRBuilder<> builder(block);
    switch(op){
        case EQUAL: this->ret = (numval1 == numval2); return builder.CreateICmpEQ(val1, val2, "eqcmp");
        case UEQUAL: this->ret = (numval1 != numval2); return builder.CreateICmpNE(val1,val2,"necmp");
        case GREATERT: this->ret = (numval1 > numval2); return builder.CreateICmpSGE(val1,val2,"gecmp");
        case LESST: this->ret = (numval1 < numval2); return builder.CreateICmpSLE(val1,val2,"lecmp");
        default: return NULL;
    }
}


Value* IfThenCodeGen::codegen(BasicBlock* block){
    errs()<<"ifthenCodegen!\n";
    LLVMContext& context = block->getParent()->getContext();
    
    //考虑block的后继节点。
    
    this->trueblock = BasicBlock::Create(context, "trueblock", block->getParent());
    this->falseblock = BasicBlock::Create(context, "falseblock", block->getParent());
    BranchInst::Create(this->trueblock, this->falseblock, this->cond, block);
    return NULL;
}


Value* SwitchCodeGen::codegen(BasicBlock* block){
    LLVMContext& context = block->getParent()->getContext();
    BasicBlock* defBlock = BasicBlock::Create(context, "defblock", block->getParent());
    SwitchInst* switchI = SwitchInst::Create(this->cond, defBlock, this->condnum, block);
    
    ConstantInt* mycond = dyn_cast<ConstantInt>(this->cond);
    int mycondnum = 0;
    if(mycond) {
        mycondnum = mycond->getSExtValue();
    }else{
        return NULL;
    }

    switchI->addCase(mycond, this->truecases);

    for(int i=0;i<this->condnum;i++){
        int num = UtilTool::getrandnum(UtilTool::MAX_NUM);
        ConstantInt* constint = ConstantInt::get(context, APInt(32, num));
        BasicBlock* otherblock = BasicBlock::Create(context, "otherblock", block->getParent);
        switchI->addCase(constint, otherblock);
        this->othercases.push_back(otherblock);
    }

}


