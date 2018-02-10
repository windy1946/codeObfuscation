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
#include "llvm/IR/InstrTypes.h"

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

#include "Common.h"
#include "CodeGen.h"
#include "UtilTool.h"
#include "log.h"

using namespace llvm;

ValueInfo* IntegerCodeGen::codegen(BasicBlock* block){
    //LOGDLN("IntegerCodeGen");
    LLVMContext& context = block->getParent()->getContext();
    Value* value = ConstantInt::get(context, APInt(this->intNumbit, this->num));
    return new ValueInfo(this->num, value);
}



ValueInfo* IntegerArithCodeGen::codegen(BasicBlock* block){
    //LOGDLN("IntegerArithCodeGen");
    
    int numval1 = this->val1->getNum();
    int numval2 = this->val2->getNum();
    Value* LHS = this->val1->getValue();
    Value* RHS = this->val2->getValue();
        
    BasicBlock::iterator inst = block->end();
    inst--;

    BinaryOperator* binop;
        
    switch(this->op){
        case ADD:
            //LOGDLN("ADD");
            binop = BinaryOperator::Create(Instruction::Add, LHS, RHS, "addtmp", &*inst);
            this->ret = numval1 + numval2;
            break;
        case SUB:
            //LOGDLN("SUB");
            binop = BinaryOperator::Create(Instruction::Sub, LHS, RHS, "subtmp", &*inst);
            this->ret = numval1 - numval2;
            break;
        case MUL:
            //LOGDLN("MUL");
            binop = BinaryOperator::Create(Instruction::Mul, LHS, RHS, "multmp", &*inst);
            this->ret = numval1 * numval2;
            break;
        case DIV:
            //LOGDLN("DIV");
            binop = BinaryOperator::Create(Instruction::SDiv, LHS, RHS, "divtmp", &*inst);
            this->ret = numval1 / numval2;
            break;
    }
    return new ValueInfo(this->ret, binop);    
}


Value* BoolCmpCodeGen::codegen(BasicBlock* block){
    //LOGDLN("BoolCmpCodeGen");
    
    Value* LHS = this->val1->getValue();
    Value* RHS = this->val2->getValue();
    
    int numval1 = this->val1->getNum();
    int numval2 = this->val2->getNum();


    IRBuilder<> builder(block);
    TerminatorInst* term = block->getTerminator();
    builder.SetInsertPoint(term);

    switch(op){
        case EQUAL: 
            //LOGDLN("EQUAL");
            this->ret = (numval1 == numval2); 
            return builder.CreateICmpEQ(LHS, RHS, "equal");
        case UEQUAL: 
            //LOGDLN("UEQUAL");
            this->ret = (numval1 != numval2); 
            return builder.CreateICmpNE(LHS, RHS, "uequal");
        case GREATERT:
            //LOGDLN("GREATERR"); 
            this->ret = (numval1 > numval2); 
            return builder.CreateICmpSGT(LHS, RHS, "greater");
        case LESST: 
            //LOGDLN("LESST");
            this->ret = (numval1 < numval2); 
            return builder.CreateICmpSLE(LHS, RHS, "lesst");
        default: return nullptr;
    }
}


ValueInfo* IfThenCodeGen::codegen(BasicBlock* block){
    //LOGDLN("IfThenCodeGen");
    LLVMContext& context = block->getParent()->getContext();
    BasicBlock* successblock = block->getSingleSuccessor();
    if(successblock == nullptr){
        LOGE("if then code gen has multiply successors");
        return nullptr;
    }

    block->getTerminator()->eraseFromParent();

    //考虑block的后继节点。
    
    this->trueblock = BasicBlock::Create(context, "trueblock", block->getParent());
    this->falseblock = BasicBlock::Create(context, "falseblock", block->getParent());

    BranchInst::Create(successblock, this->trueblock);
    BranchInst::Create(successblock, this->falseblock);

    BranchInst::Create(this->trueblock, this->falseblock, this->cond, block);

    return nullptr;
}


ValueInfo* SwitchCodeGen::codegen(BasicBlock* block){
    //LOGDLN("SwitchCodeGen");
    BasicBlock* successblock = block->getSingleSuccessor();
    if(successblock == nullptr){
        LOGE("switch code gen has multiply successors");
        return nullptr;
    }
    LLVMContext& context = block->getParent()->getContext();
    BasicBlock* defBlock = BasicBlock::Create(context, "defblock", block->getParent());
    BranchInst::Create(successblock, defBlock);

    block->getTerminator()->eraseFromParent();
    
    SwitchInst* switchI = SwitchInst::Create(this->cond->getValue(), defBlock, this->condnum, block);
    
    //ConstantInt* mycond = dyn_cast<ConstantInt>(this->cond->getValue());
    //if(mycond == nullptr){
        //LOGE("SWITCH COND IS NULL");
        //return nullptr;
    //}

    int mycondnum = this->cond->getNum();
    
    ConstantInt* mycond = ConstantInt::get(context, APInt(32, mycondnum));

    this->truecase = BasicBlock::Create(context, "truecase", block->getParent());

    switchI->addCase(mycond, this->truecase);
    
    BranchInst::Create(successblock, this->truecase);

    int uper = mycondnum;
    int lower = mycondnum;
    int othercondnum = mycondnum;
    for(int i=0;i<this->condnum;i++){
        int num = UtilTool::getrandnum(UtilTool::MAX_NUM)+1;
        if(num > mycondnum){
            uper = uper + num;
            othercondnum = uper;
        }else{
            lower = lower - num;
            othercondnum = lower;
        }
        ConstantInt* constint = ConstantInt::get(context, APInt(32, othercondnum));
        BasicBlock* otherblock = BasicBlock::Create(context, "otherblock", block->getParent());

        switchI->addCase(constint, otherblock);
        BranchInst::Create(successblock, otherblock);
        this->othercases.push_back(otherblock);
    }
    return nullptr;
}


