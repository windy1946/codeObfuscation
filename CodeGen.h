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
#include "log.h"
using namespace llvm;


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

    static Value* MergeBlocks(BasicBlock* block1, Value* value1, BasicBlock* block2, Value* value2, BasicBlock* mergeblock){
        block1->getTerminator()->eraseFromParent();
        block2->getTerminator()->eraseFromParent();
        BranchInst::Create(mergeblock, block1);
        BranchInst::Create(mergeblock, block2);
        
        IRBuilder<> builder(mergeblock);
        LLVMContext& context = mergeblock->getContext();
        builder.SetInsertPoint(mergeblock);
        PHINode* pn = builder.CreatePHI(Type::getInt32Ty(context), 2, "phi");
        
        pn->addIncoming(value1, block1);
        pn->addIncoming(value2, block2);

        return pn;
    }


    static Value* MergeBlocks(std::vector<BasicBlock*> blocks, std::vector<Value*> values, BasicBlock* mergeblock){
        if(blocks.size()<=1){
            return nullptr;
        }
        int num = blocks.size();
        LLVMContext& context = mergeblock->getContext();
        IRBuilder<> builder(mergeblock);
        PHINode* pn = builder.CreatePHI(Type::getInt32Ty(context), num, "phi");
        
        for(int i=0; i<num; i++){
            BasicBlock* block = blocks[i];
            Value* value = values[i];
            pn->addIncoming(value, block);
        }

        return pn;
    }
};




class BasicCodeGen{
public:
    virtual ~BasicCodeGen() = default;

    virtual Value* codegen(BasicBlock* block) = 0;
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
    
    Value* codegen(BasicBlock* block) override;
};





enum ARICHOP{ADD=0, SUB, MUL, DIV, NUM};
class IntegerArithCodeGen : public BasicCodeGen{
    Value* val1;
    Value* val2;
    int retval;
    ARICHOP op;
    int ret;
public:
    IntegerArithCodeGen(Value* val1, Value* val2, ARICHOP op){
        this->val1 = val1;
        this->val2 = val2;
        this->op = op;
    }
    int getRet(){
        return this->ret;
    }
    //insert the operation code at the end of the block.
    Value* codegen(BasicBlock* block) override;
};


enum CMPOP{EQUAL=0, UEQUAL, GREATERT, LESST};

class BoolCmpCodeGen : public BasicCodeGen{
    bool ret;
    Value* val1;
    Value* val2;
    CMPOP op;
public:
    BoolCmpCodeGen(Value* val1, Value* val2, CMPOP op){
        this->val1 = val1;
        this->val2 = val2;
        this->op = op;
    }
    bool getRet(){
        return this->ret;
    }
    Value* codegen(BasicBlock* block) override;
};


enum CHOICEOP{VALOP=0, IFTHENOP, BOOLOP};

class IfThenCodeGen : public BasicCodeGen{
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
    Value* codegen(BasicBlock* block) override;
};


class SwitchCodeGen : public BasicCodeGen{
    Value* cond;
    int condnum;
    std::vector<BasicBlock*> othercases;
    BasicBlock* truecase;

public:
    SwitchCodeGen(Value* cond, int condnum){
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
    Value* codegen(BasicBlock* block) override;
};


#endif