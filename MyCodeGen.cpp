//===- Hello.cpp - Example code from "Writing an LLVM Pass" ---------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements two versions of the LLVM "Hello World" pass described
// in docs/WritingAnLLVMPass.html
//
//===----------------------------------------------------------------------===//

#include "llvm/ADT/Statistic.h"
#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/Bitcode/BitcodeWriterPass.h" 

#include "CodeGen.h"
#include "UtilTool.h"
#include "log.h"
#include "Common.h"
#include "SemanticGen.h"

#include "SemanticGen0.h"
#include <vector>

using namespace llvm;

#define DEBUG_TYPE "mycodegen"


STATISTIC(HelloCounter, "Counts number of functions greeted");


namespace {

  //return true if it has been chosen;
  bool ischoose(std::vector<BasicBlock*> blockslink, BasicBlock* block){
    for(int i=0; i<blockslink.size(); i++){
      if(blockslink[i] == block){
        return true;
      }
    }
    return false;
  }

  std::vector<BasicBlock*> getBlocksLink(Function* f){
    std::vector<BasicBlock*> blockslink;
    Function::iterator bb=f->begin();
    Function::iterator endbb=f->end();
    blockslink.push_back(&*bb);
    bb++;
    for(; bb!=endbb; bb++){
      BasicBlock* tbb = &*bb;
      TerminatorInst* terminst = tbb->getTerminator();
      int succnum = terminst->getNumSuccessors();
      if(succnum <= 0){
        return blockslink;
      }
      BasicBlock* nextbb = terminst->getSuccessor(0);
      if(ischoose(blockslink, nextbb)){
        return blockslink;
      }else{
        blockslink.push_back(nextbb);
      }
    }

    return blockslink;

  }
  void DeadcodeGen(Function* f){
    std::vector<BasicBlock*> blockslink = getBlocksLink(f);

    BasicBlock* entryblock = &*(f->begin());
    Instruction* beginst = &*(entryblock->begin());
    BasicBlock* restblock = entryblock->splitBasicBlock(beginst);

    Instruction* nextbeginst = &*(restblock->begin());
    BasicBlock* nextrestblock = restblock->splitBasicBlock(nextbeginst);

    blockslink[0] = nextrestblock;

//--------------------------------------------------------------
      
    SemanticGen semgen;
      
    BlockInfo blockinfo = semgen.expressionGen(BlockInfo(entryblock));
    //return blockinfo;
    LOGD("===========blockinfo values size : ");
    LOGDLN(blockinfo.getValuesSize());
//------------------------------------------------------------------
    
    SemanticGen0 semgent0(blockinfo.getValues(), restblock);

    semgent0.expressionGen(BlockInfo(restblock));

    std::vector<BasicBlock*> blocks1 = semgent0.getBlocksLink();

    CodeGenTool::MixBlockLinks(blocks1, blockslink);

    CodeGenTool::myFixStack(f);
  }

  void insertDeadCode(Instruction* inst){
      BasicBlock* block = inst->getParent();
      BasicBlock* restblock = block->splitBasicBlock(inst, "restbb");
      if(restblock == nullptr){
        return ;
      }
      BasicBlock* newblock = BasicBlock::Create(block->getContext(), "newblock", block->getParent());
      BranchInst::Create(restblock, newblock);

      block->getTerminator()->eraseFromParent();
      BranchInst::Create(newblock, block);
      
      SemanticGen semgen;
      
      BlockInfo blockinfo = semgen.expressionGen(BlockInfo(newblock));
      //return blockinfo;
      LOGD("===========blockinfo values size : ");
      LOGDLN(blockinfo.getValuesSize());

      
    }
  // Hello - The first implementation, without getAnalysisUsage.
  struct MyCodeGen : public FunctionPass {
    static char ID; // Pass identification, replacement for typeid
    MyCodeGen() : FunctionPass(ID) {}
    
    bool runOnFunction(Function &F) override {

      DeadcodeGen(&F);
      /*
      int percent = 20; //max num : 100
      ++HelloCounter;
      int randnum = 0; 
      
      std::vector<Instruction*> inserts;      
      Function::iterator endbb = F.end();
      for(Function::iterator bb=F.begin(); bb!=endbb; bb++){
        BasicBlock* eachbb = &*bb;
        if(bb->size() <= 2){
          continue;
        }
        BasicBlock::iterator endinst = eachbb->end();
        endinst--;
        BasicBlock::iterator inst = eachbb->begin();
        inst++;
        for(; inst != endinst; inst++){
          randnum = UtilTool::getrandnum(100);
          if(randnum < percent){
            inserts.push_back(&*inst);
          }
        }
      }
      LOGD("INSERT NUMBER :");
      LOGDLN(inserts.size());
      for(int i=0; i<inserts.size(); i++){
        LOGDLN("----------------------------------------");
        Instruction* inst = inserts[i];
        insertDeadCode(inst);
      }
      */
      LOGDLN("FINISH");
      return true;
    }
  };

  
}

char MyCodeGen::ID = 0;

static RegisterPass<MyCodeGen> X("mycodegen", "Hello World Pass");


namespace {
  // Hello2 - The second implementation with getAnalysisUsage implemented.
  struct MyCodeGen2 : public FunctionPass {
    static char ID; // Pass identification, replacement for typeid
    MyCodeGen2() : FunctionPass(ID) {}

    bool runOnFunction(Function &F) override {
      ++HelloCounter;
      errs() << "Hello: ";
      errs().write_escaped(F.getName()) << '\n';
      return false;
    }

    // We don't modify the program, so we preserve all analyses.
    void getAnalysisUsage(AnalysisUsage &AU) const override {
      AU.setPreservesAll();
    }
  };
}

char MyCodeGen2::ID = 0;
static RegisterPass<MyCodeGen2>
Y("mycodegen2", "Hello World Pass (with getAnalysisUsage implemented)");
