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
#include "log.h"
#include "Common.h"

using namespace llvm;


#ifndef DEADCODEGEN
#define DEADCODEGEN

class BasicCodeGen{
public:
    virtual ~BasicCodeGen() = default;

    virtual ValueInfo* codegen(BasicBlock* block) = 0;
};


class IntegerCodeGen : public BasicCodeGen{
    int num;
    int intNumbit;
public:
    IntegerCodeGen(int num, int intNumbit = 32){
        this->num = num;
        this->intNumbit = intNumbit;
    }
    int getNum(){
        return this->num;
    }
    
    ValueInfo* codegen(BasicBlock* block) override;
};

enum ARICHOP{ADD=0, SUB, MUL, DIV, NUM};
class IntegerArithCodeGen : public BasicCodeGen{
    ValueInfo* val1 = nullptr;
    ValueInfo* val2 = nullptr;

    int retval;
    ARICHOP op;
    int ret;
public:
    IntegerArithCodeGen(ValueInfo* val1, ValueInfo* val2, ARICHOP op){
        this->val1 = val1;
        this->val2 = val2;
        this->op = op;
    }
    int getRet(){
        return this->ret;
    }
    //insert the operation code at the end of the block.
    ValueInfo* codegen(BasicBlock* block) override;
};


enum CMPOP{EQUAL=0, UEQUAL, GREATERT, LESST};

class BoolCmpCodeGen{
    bool ret;
    ValueInfo* val1 = nullptr;
    ValueInfo* val2 = nullptr;
    CMPOP op;
public:
    BoolCmpCodeGen(ValueInfo* val1, ValueInfo* val2, CMPOP op){
        this->val1 = val1;
        this->val2 = val2;
        this->op = op;
    }

    bool getRet(){
        return this->ret;
    }

    Value* codegen(BasicBlock* block);
};


enum CHOICEOP{VALOP=0, IFTHENOP, BOOLOP};

class IfThenCodeGen{
    Value* cond ;
    BasicBlock* trueblock = NULL;
    BasicBlock* falseblock = NULL;
public:
    IfThenCodeGen(Value* cond){
        this->cond = cond;
    }
    BasicBlock* getTrueBlock(){
        return this->trueblock;
    }
    BasicBlock* getFalseBlock(){
        return this->falseblock;
    }
    ValueInfo* codegen(BasicBlock* block);
};


class SwitchCodeGen{
    ValueInfo* cond;
    int condnum;
    std::vector<BasicBlock*> othercases;
    BasicBlock* truecase;

public:
    SwitchCodeGen(ValueInfo* cond, int condnum){
        this->cond = cond;
        this->condnum = condnum;
    }

    int getCondNum(){
        return this->condnum;
    }
    BasicBlock* getTruecaseBlock(){
        return this->truecase;
    }
    std::vector<BasicBlock*> getOthercaseBlocks(){
        return this->othercases;
    }
    ValueInfo* codegen(BasicBlock* block);
};

class CodeGenTool{
public :   
    //block->jumpto
    static void BlockJump(BasicBlock* block, BasicBlock* jumpto){
        BranchInst::Create(jumpto, block);
    }

    static BasicBlock* CreateBlock(BasicBlock* block){
        BasicBlock* successor = block->getSingleSuccessor();
        if(successor == nullptr){
            LOGE("CREATE BLOCK HAS MULTIPLY SUCCESSOR");
            return nullptr;
        }

        LLVMContext& context = block->getParent()->getContext();
        BasicBlock* newblock = BasicBlock::Create(context, "newblock", block->getParent());
        BranchInst::Create(successor, newblock);
        return newblock;
    }

    static ValueInfo* MergeBlocks(BasicBlock* block1, ValueInfo* value1, BasicBlock* block2, ValueInfo* value2, BasicBlock* mergeblock){
        LOGD("Merge block");
        block1->getTerminator()->eraseFromParent();
        block2->getTerminator()->eraseFromParent();
        BranchInst::Create(mergeblock, block1);
        BranchInst::Create(mergeblock, block2);
        
        IRBuilder<> builder(mergeblock);
        LLVMContext& context = mergeblock->getContext();
        Instruction* beginst = &*(mergeblock->begin());
        builder.SetInsertPoint(beginst);
        PHINode* pn = builder.CreatePHI(Type::getInt32Ty(context), 2, "phi");
        
        pn->addIncoming(value1->getValue(), block1);
        pn->addIncoming(value2->getValue(), block2);


        return new ValueInfo(value1->getNum(), pn);
    }


    static ValueInfo* MergeBlocks(std::vector<BasicBlock*> blocks, std::vector<ValueInfo*> values, BasicBlock* mergeblock){
        if(blocks.size()<=1){
            return nullptr;
        }
        int num = blocks.size();
        LLVMContext& context = mergeblock->getContext();
        IRBuilder<> builder(mergeblock);
        PHINode* pn = builder.CreatePHI(Type::getInt32Ty(context), num, "phi");
        
        for(int i=0; i<num; i++){
            BasicBlock* block = blocks[i];
            Value* value = values[i]->getValue();
            pn->addIncoming(value, block);
        }
        int pnnum = values[0]->getNum();
        return new ValueInfo(pnnum, pn);
    }
};
#endif