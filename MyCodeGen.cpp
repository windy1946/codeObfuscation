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


///该函数需要后期继续改进。。。。
  std::vector<BasicBlock*> getBlocksLink(Function* f, int num){

    std::vector<Instruction*> instlist;
    
    for(Function::iterator tbb=f->begin(), tendbb=f->end(); tbb!=tendbb; tbb++){
      BasicBlock* ttbb = &*tbb;
      BasicBlock::iterator tinst=ttbb->begin();
      tinst++;
      BasicBlock::iterator tendinst=ttbb->end();
      for( ;tinst!=tendinst; tinst++){
        int randnum = UtilTool::getrandnum(100);
        if(randnum < num){
          instlist.push_back(&*tinst);
        }
      }
    }
    
    for(int i=0; i<instlist.size(); i++){
      Instruction* inst = instlist[i];
      BasicBlock* block = inst->getParent();
      block->splitBasicBlock(inst);
    }
    


    std::vector<BasicBlock*> blockslink;
    Function::iterator ibb=f->begin();
    Function::iterator endibb=f->end();
    //blockslink.push_back(&*bb);
    //bb++;
    if(ibb == endibb){
      return blockslink;
    }
    //BasicBlock* tbb = &*bb;
    
    BasicBlock* bb = &*ibb;

    while(true){
      if( ibb == endibb ){
        return blockslink;
      }
      if(!ischoose(blockslink, bb)){
        blockslink.push_back(bb);
      }

      TerminatorInst* terminst = bb->getTerminator();
      int succnum = terminst->getNumSuccessors();
      if(succnum <= 0){
        ibb++;
        if( ibb == endibb ){
          return blockslink;
        }
        bb = &*ibb; 
        continue;
      }
      BasicBlock* nextbb = terminst->getSuccessor(0);
      bool ischooseflag = false;
      for(int i=0; i<succnum; i++){
        nextbb = terminst->getSuccessor(i);
        if(ischoose(blockslink, nextbb)){
          continue;
        }else{
          //blockslink.push_back(nextbb);
          bb = nextbb;
          ischooseflag = true;
          break;
        }
      }
      if(!ischooseflag){
        ibb++;
        if( ibb == endibb ){
          return blockslink;
        }
        bb = &*ibb;
        continue;
      }
      
    }
/*
//-----------------bug--------------------------------
    for(Function::iterator bb=f->begin(), endbb=f->end(); bb!=endbb; bb++){
      BasicBlock* block = &*bb;
      if(!ischoose(blockslink, block)){
        blockslink.push_back(block);
      }
    }
//----------------------------------------------------
*/
    return blockslink;

  }

  void DeadcodeGen(Function* f){
    LOGDLN("-----------begin-------------");
    LOGDLN(f->getName());

    std::vector<BasicBlock*> blockslink = getBlocksLink(f, 15);

    BasicBlock* entryblock = &*(f->begin());
    Instruction* beginst = &*(entryblock->begin());
    BasicBlock* restblock = entryblock->splitBasicBlock(beginst);

    Instruction* nextbeginst = &*(restblock->begin());
    BasicBlock* nextrestblock = restblock->splitBasicBlock(nextbeginst);

    blockslink[0] = nextrestblock;

//--------------------------------------------------------------
      
    SemanticGen semgen;
    BlockInfo cur_blockinfo(entryblock);
    cur_blockinfo.setChildblockNum(20);
    BlockInfo blockinfo = semgen.expressionGen(cur_blockinfo);
    //return blockinfo;

    //LOGD("===========blockinfo values size : ");
    //LOGDLN(blockinfo.getValuesSize());
//------------------------------------------------------------------
    
    SemanticGen0 semgent0(blockinfo.getValues(), restblock);

    BlockInfo restblockinfo(restblock);
    restblockinfo.setFlag(true);
    restblockinfo.setChildblockNum(5  );

    semgent0.expressionGen(restblockinfo);

    std::vector<BasicBlock*> blocks1 = semgent0.getBlocksLink();
    
    LOGD("-------------------------BLOCK VALUES NUM: ");
    LOGDLN(blockinfo.getValuesSize());  

    LOGD("------------------------blocks1 links size : ");
    LOGDLN(blocks1.size());
    
    LOGD("------------------------blocks2 links size : ");
    LOGDLN(blockslink.size());

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
      LOGD("function name : ");
      LOGDLN(F.getName());
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
