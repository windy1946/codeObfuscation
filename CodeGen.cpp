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
    LOGD("IntegerCodeGen");
    LLVMContext& context = block->getParent()->getContext();
    Value* value = ConstantInt::get(context, APInt(this->intNumbit, this->num));
    return new ValueInfo(this->num, value);
}



ValueInfo* IntegerArithCodeGen::codegen(BasicBlock* block){
    LOGD("IntegerArithCodeGen");
    //ConstantInt* constint1 = dyn_cast<ConstantInt>(this->val1);
    //ConstantInt* constint2 = dyn_cast<ConstantInt>(this->val2);
    int numval1 = this->val1->getNum();
    int numval2 = this->val2->getNum();
    Value* LHS = this->val1->getValue();
    Value* RHS = this->val2->getValue();
    //if(constint1 && constint2) {
        //numval1 = constint1->getSExtValue();
        //numval2 = constint2->getSExtValue();
    //}else{
        //return nullptr;
    //}
        
    BasicBlock::iterator inst = block->end();
    inst--;
    //Instruction* inst = block->getTerminator();
    BinaryOperator* binop;
        
    switch(this->op){
        case ADD:
            LOGD("ADD");
            binop = BinaryOperator::Create(Instruction::Add, LHS, RHS, "addtmp", &*inst);//builder.CreateAdd(val1, val2, "addtmp");
            this->ret = numval1 + numval2;
            break;
        case SUB:
            LOGD("SUB");
            binop = BinaryOperator::Create(Instruction::Sub, LHS, RHS, "subtmp", &*inst);//builder.CreateAdd(val1, val2, "addtmp");
            this->ret = numval1 - numval2;
            break;
        case MUL:
            LOGD("MUL");
            binop = BinaryOperator::Create(Instruction::Mul, LHS, RHS, "multmp", &*inst);//builder.CreateAdd(val1, val2, "addtmp");
            this->ret = numval1 * numval2;
            break;
        case DIV:
            LOGD("DIV");
            binop = BinaryOperator::Create(Instruction::SDiv, LHS, RHS, "divtmp", &*inst);//builder.CreateAdd(val1, val2, "addtmp");
            this->ret = numval1 / numval2;
            break;
    }
    return new ValueInfo(this->ret, binop);    
}


Value* BoolCmpCodeGen::codegen(BasicBlock* block){
    LOGD("BoolCmpCodeGen");
    //int numval1 = 0;
    //int numval2 = 0;    
    /*
    ConstantInt* constint1 = dyn_cast<ConstantInt>(this->val1);
    ConstantInt* constint2 = dyn_cast<ConstantInt>(this->val2);
    int numval1 = 0;
    int numval2 = 0;
    if(constint1 && constint2) {
        numval1 = constint1->getSExtValue();
        numval2 = constint2->getSExtValue();
    }else{
        LOGE("Operands in boolcmpcodegen is null!");
        return NULL;
    }
    */
    //Value * LHS = ConstantFP::get(Type::getFloatTy(block->getContext()), (float)numval1);
    //Value * RHS = ConstantFP::get(Type::getFloatTy(block->getContext()), (float)numval2);
    Value* LHS = this->val1->getValue();
    Value* RHS = this->val2->getValue();
    
    int numval1 = this->val1->getNum();
    int numval2 = this->val2->getNum();

    TerminatorInst* term = block->getTerminator();
    //CmpInst * condition = new CmpInst(term, FCmpInst::FCMP_TRUE , this->val1, this->val2, "eqcmp");
    IRBuilder<> builder(block);
    builder.SetInsertPoint(term);

    switch(op){
        case EQUAL: 
            LOGD("EQUAL");
            this->ret = (numval1 == numval2); 
            return builder.CreateICmpEQ(LHS, RHS, "equal");
            //new FCmpInst(term, FCmpInst::FCMP_OEQ , LHS, RHS, "eqcmp");
        case UEQUAL: 
            LOGD("UEQUAL");
            this->ret = (numval1 != numval2); 
            return builder.CreateICmpNE(LHS, RHS, "uequal");
            //new FCmpInst(term, FCmpInst::FCMP_ONE , LHS, RHS, "necmp");
        case GREATERT:
            LOGD("GREATERR"); 
            this->ret = (numval1 > numval2); 
            return builder.CreateICmpSGT(LHS, RHS, "greater");
            //new FCmpInst(term, FCmpInst::FCMP_OGE , LHS, RHS, "gecmp");
        case LESST: 
            LOGD("LESST");
            this->ret = (numval1 < numval2); 
            return builder.CreateICmpSLE(LHS, RHS, "lesst");
            //new FCmpInst(term, FCmpInst::FCMP_OLE , LHS, RHS, "lecmp");
        default: return nullptr;
    }
}


ValueInfo* IfThenCodeGen::codegen(BasicBlock* block){
    LOGD("IfThenCodeGen");
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
    LOGD("SwitchCodeGen");
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
    
    ConstantInt* mycond = dyn_cast<ConstantInt>(this->cond->getValue());
    if(mycond == nullptr){
        LOGE("SWITCH COND IS NULL");
        return nullptr;
    }

    int mycondnum = this->cond->getNum();

    //if(mycond) {
        //mycondnum = mycond->getSExtValue();
    //}else{
        //return nullptr;
    //}
    this->truecase = BasicBlock::Create(context, "truecase", block->getParent());

    switchI->addCase(mycond, this->truecase);
    
    BranchInst::Create(successblock, this->truecase);

    for(int i=0;i<this->condnum;i++){
        int num = UtilTool::getrandnum(UtilTool::MAX_NUM);
        ConstantInt* constint = ConstantInt::get(context, APInt(32, num));
        BasicBlock* otherblock = BasicBlock::Create(context, "otherblock", block->getParent());

        switchI->addCase(constint, otherblock);
        BranchInst::Create(successblock, otherblock);
        this->othercases.push_back(otherblock);
    }
    return nullptr;
}


