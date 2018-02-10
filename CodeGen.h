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
#include "llvm/Transforms/Utils/Local.h"
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
    //mix two blocks links
    //blocks1: generated code;
    //blcoks2: origin code
    //(1)blocks1.length = blocks2.length
    //blocks1.a->blocks2.a->blocks1.b->blocks2.b->...->blocks1.n->blocks2.n
    //(2)blocks1.length > blocks2.length
    //blocks1.a->blocks2.a->blocks1.b->blocks2.b->...->blocks1.k->blocks2.n-1->blocks1.k+1->...->blocks1.n->blocks2.n 
    //(3)blocks1.length < blocks2.length
    //blocks1.a->blocks2.a->blocks1.b->blocks2.b->...->blocks1.n->blocks2.k->blocks2.k+1->...->blocks2.n
    //blocks1.a may has multiply successor.
//该函数需要后期再改进。。。。
    static void MixBlockLinks(std::vector<BasicBlock*> blocks1, std::vector<BasicBlock*> blocks2){
        BasicBlock* blocks1_a;
        BasicBlock* blocks1_b;
        BasicBlock* blocks2_a;
        BasicBlock* blocks2_b;
        
        int length1 = blocks1.size();
        int length2 = blocks2.size();
        int minlength = 0;
        if(length1 < length2){
            minlength = length1;
        }else{
            minlength = length2;
        }
        //把closks2的最后一个block插入到blocks1的最后面。

        for(int i=0; i<minlength-1; i++){
            blocks1_a = blocks1[i];
            blocks1_b = blocks1[i+1];
            TerminatorInst* term1_a = blocks1_a->getTerminator();
            blocks2_a = blocks2[i];
            blocks2_b = blocks2[i+1];
            TerminatorInst* term2_a = blocks2_a->getTerminator();

            int successornum = term1_a->getNumSuccessors();
            for(int j=0; j<successornum; j++){
                BasicBlock* successorbb = term1_a->getSuccessor(j);
                if(successorbb == blocks1_b){
                    LOGDLN("set successor");
                    term1_a->setSuccessor(j, blocks2_a);
                        int tsuccessornum = term2_a->getNumSuccessors();

                        for(int k=0; k<tsuccessornum; k++){
                            BasicBlock* tsuccessorbb = term2_a->getSuccessor(k);
                            if(tsuccessorbb == blocks2_b){
                                term2_a->setSuccessor(k, blocks1_b);
                            }
                        }
                          
                }
            }
        }

        blocks1_b = blocks1[blocks1.size()-1];
        blocks2_b = blocks2[minlength-1];
        TerminatorInst* lastinst = blocks1_b->getTerminator();
        lastinst->eraseFromParent();
        BranchInst::Create(blocks2_b, blocks1_b);

    }

    
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
        Instruction* beginst = &*(mergeblock->begin());
        builder.SetInsertPoint(beginst);
        PHINode* pn = builder.CreatePHI(Type::getInt32Ty(context), num, "phi");
        
        for(int i=0; i<num; i++){
            BasicBlock* block = blocks[i];
            Value* value = values[i]->getValue();
            block->getTerminator()->eraseFromParent();
            BranchInst::Create(mergeblock, block);
            pn->addIncoming(value, block);
        }
        int pnnum = values[0]->getNum();
        return new ValueInfo(pnnum, pn);
    }

    static bool valueEscapes(Instruction *Inst) {
        BasicBlock *BB = Inst->getParent();
        for (Value::use_iterator UI = Inst->use_begin(), E = Inst->use_end(); UI != E;
            ++UI) {    
            Instruction *I = cast<Instruction>(*UI);
            if (I->getParent() != BB || isa<PHINode>(I)) {
                return true;
            }
        }
        return false;
    }

    static bool isInvokeNotAllocated(Instruction* Inst){
        if(isa<InvokeInst>(Inst)){
            for (const Use &U : Inst->uses()) {
                const Instruction *I = cast<Instruction>(U.getUser());
                if(I->getOpcode() == Instruction::Store && I->getParent()->getSinglePredecessor()==Inst->getParent()){ 
                    return false;
                }
            }
            return true;
        }else{
            return true;
        }
    }

    static void myFixStack(Function* f){
        std::vector<PHINode *> tmpPhi;
        std::vector<Instruction *> tmpReg;
        BasicBlock *bbEntry = &*f->begin();
        
        int tmpregsize = 0;
        int tmpphisize = 0;
        
        do {
            //times--;
            tmpPhi.clear();
            tmpReg.clear();

            for (Function::iterator i = f->begin(); i != f->end(); ++i) {

                for (BasicBlock::iterator j = i->begin(); j != i->end(); ++j) {
                    if (isa<PHINode>(j)) {
                        PHINode *phi = cast<PHINode>(j);
                        tmpPhi.push_back(phi);
                        continue;
                    }
        
            //-------------------------modify------------------------------------------------------
                    if (!(isa<AllocaInst>(j) && j->getParent() == bbEntry) &&
                        (valueEscapes(&*j) || j->isUsedOutsideOfBlock(&*i)) && isInvokeNotAllocated(&*j)) {
                        tmpReg.push_back(&*j);
                        continue;
                    }
        //--------------------------------------------------------------------------------------
                }
            }

            for (unsigned int i = 0; i != tmpReg.size(); ++i) {
                DemoteRegToStack(*tmpReg.at(i), f->begin()->getTerminator());
            }

            for (unsigned int i = 0; i != tmpPhi.size(); ++i) {
                DemotePHIToStack(tmpPhi.at(i), f->begin()->getTerminator());
            }

            tmpregsize = tmpReg.size();
            tmpphisize = tmpPhi.size();

        } while ( (tmpregsize!=0 || tmpphisize!=0) );
    }


};
#endif