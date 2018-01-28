#ifndef DEADCODEGEN
#define DEADCODEGEN
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

using namespace llvm;

enum CMPOP{EQUAL=0, UEQUAL, GREATERT, LESST};
enum CHOICEOP{VALOP=0, IFTHENOP, BOOLOP};
enum ARICHOP{ADD=0, SUB, MUL, DIV, NUM};

struct Info{
        Value* val;
        BasicBlock* block;
};

class DeadCodeGen{
public:
    DeadCodeGen(int deadcodenum, int deadcodedeep){
        this->deadcodenum = deadcodenum;
        this->deadcodedeep = deadcodedeep;
    };

    void beginCodeGen(Instruction* inst){
        errs()<<"-------------------start begincode gen\n";
        BasicBlock* block = inst->getParent();
        LLVMContext& context = block->getContext();
        BasicBlock* restBB = block->splitBasicBlock(inst, "restbb");
        if(restBB == nullptr){
            errs() << "nullptr restBB\n";
            return ;
        }
        BasicBlock* newBB = BasicBlock::Create(context, "newbb", block->getParent(), restBB);
        BranchInst::Create(restBB, newBB);
        block->getTerminator()->eraseFromParent();
        BranchInst::Create(newBB, block);
        Value*val1 = this->IntegerCodeGen(newBB, 20);
        Value*val2 = this->IntegerCodeGen(newBB, 30);
        Value* retval = this->OperationCodeGen(newBB, ADD, val1, val2);
        this->OperationCodeGen(newBB, ADD, val1, retval);
        this->ifthenop(newBB, this->deadcodedeep);
        errs()<<"-------------------finish begincode gen\n";
        //this->CodeGen(newBB, nullptr);
    }

    void CodeGen(BasicBlock* block, Value* val){
        this->deadcodenum--;
        if(this->deadcodenum <= 0){
            return ;
        }

        if(val == nullptr){
            int randnum = this->getrandnum(100);
            val = this->IntegerCodeGen(block, randnum);
        }
        errs()<<"---------------after choiceop\n";
        Value* valnew = this->choiceOp(block, this->deadcodedeep);;
        int randchoice = this->getrandnum(4);
        Value* result = this->OperationCodeGen(block, (ARICHOP)randchoice, val, valnew);
        errs()<<"after-----op code gen\n";
        //CodeGen(block, result);
    }

    Value* IntegerCodeGen(BasicBlock* block, int num){ 
        errs()<<"integerCodegen: " << num << "\n";
        LLVMContext& context = block->getParent()->getContext();
        errs()<<"after context\n";
        return ConstantInt::get(context, APInt(this->intNumbit, num));
    }

    Value* OperationCodeGen(BasicBlock* block, ARICHOP op, Value* val1, Value* val2){
        errs()<<"operationCodegen!\n";
        //IRBuilder<> builder(block);
        BasicBlock::iterator insert = block->end();
        insert--;
        BinaryOperator* binop;
        switch(op){
            case ADD:
                errs()<<"add\n";
                binop = BinaryOperator::Create(Instruction::Add, val1, val2, "addtmp", &*insert);//builder.CreateAdd(val1, val2, "addtmp");
                errs()<<"after add\n";
                break;
                //return (Value*) binadd;
            case SUB:
                errs()<<"sub\n";
                //return (Value*)BinaryOperator::Create(Instruction::Sub, val1, val2, "subtmp", (&*insert));//builder.CreateSub(val1, val2, "subtmp");
                binop = BinaryOperator::Create(Instruction::Sub, val1, val2, "addtmp", &*insert);//builder.CreateAdd(val1, val2, "addtmp");
                errs()<<"after add\n";
                break;
                //return (Value*) binsub;
            case MUL:
                errs()<<"mul\n";
                //return (Value*)BinaryOperator::Create(Instruction::Mul, val1, val2, "multmp", (&*insert));//builder.CreateMul(val1, val2, "multmp");
                binop = BinaryOperator::Create(Instruction::Mul, val1, val2, "addtmp", &*insert);//builder.CreateAdd(val1, val2, "addtmp");
                errs()<<"after add\n";
                break;
                //return (Value*) binmul;
            case DIV:
                errs()<<"div\n";
                //return (Value*)BinaryOperator::Create(Instruction::Add, val1, val2, "divtmp", (&*insert));//builder.CreateUDiv(val1, val2, "divtmp");
                binop = BinaryOperator::Create(Instruction::Add, val1, val2, "addtmp", &*insert);//builder.CreateAdd(val1, val2, "addtmp");
                errs()<<"after add\n";
                break;
                //return (Value*) bindiv;

        }
        return (Value*)binop;
    }

    
    Value* CmpCodeGen(BasicBlock* block, int op, Value* val1, Value* val2){
        errs()<<"cmpCodegen!\n";
        IRBuilder<> builder(block);
        switch(op){
            case EQUAL: return builder.CreateICmpEQ(val1, val2, "eqcmp");
            case UEQUAL: return builder.CreateICmpNE(val1,val2,"necmp");
            case GREATERT: return builder.CreateICmpSGE(val1,val2,"gecmp");
            case LESST: return builder.CreateICmpSLE(val1,val2,"lecmp");
            default: return LogErrorV("invalid cmp operator");
        }
    }

    void IfThenCodeGen(BasicBlock* block, Value* cmpval, BasicBlock* &block1, BasicBlock* &block2, BasicBlock* &blockmerge){
        errs()<<"ifthenCodegen!\n";
        LLVMContext& context = block->getParent()->getContext();
        //BasicBlock* successorBB = block->getSingleSuccessor();
        
        BasicBlock::iterator lastinst = block->end();
        lastinst--;
        BasicBlock* restBB = block->splitBasicBlock(lastinst, "restbb");
        blockmerge = BasicBlock::Create(context, "blockmerge", block->getParent(), restBB);
        
        BranchInst::Create(restBB, blockmerge);
        
        block1 = BasicBlock::Create(context, "then", block->getParent(), blockmerge);
        block2 = BasicBlock::Create(context, "else", block->getParent(), blockmerge);
        BranchInst::Create(blockmerge, block1);
        BranchInst::Create(blockmerge, block2);
        block->getTerminator()->eraseFromParent();
        BranchInst::Create(block1, block2, cmpval, block);
    }

    Value* PHICodeGen(BasicBlock* mergeBB, BasicBlock* thenBB, Value* thenval, BasicBlock* elseBB, Value* elseval){
        IRBuilder<> builder(mergeBB);
        LLVMContext& context = mergeBB->getContext();
        builder.SetInsertPoint(mergeBB);
        PHINode* pn = builder.CreatePHI(Type::getInt32Ty(context), 2, "phi");
        
        pn->addIncoming(thenval, thenBB);
        pn->addIncoming(elseval, elseBB);

        return pn;
    }
    

private:
    int getrandnum(int num);
    int t = 0;
    int intNumbit = 32;
    int deadcodenum = 20;
    int deadcodedeep = 3;
    
    std::string valname = "dxval";
    std::string boolname = "cmp";
    std::map<std::string, Value *> NamedValues;

    Value* choiceOp(BasicBlock* block, int maxdeep);
    Value* valop(BasicBlock* block, int maxdeep);
    Value* valop(BasicBlock* block, Value* val, int maxdeep);
    Value* boolop(BasicBlock* block, int maxdeep);
    Value* ifthenop(BasicBlock* block, int maxdeep);

    Value* LogError(const char *Str) {
        fprintf(stderr, "Error: %s\n", Str);
        return nullptr;
    }

    Value *LogErrorV(const char *Str) {
        LogError(Str);
        return nullptr;
    }

    
};




#endif