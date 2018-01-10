#include "llvm/ADT/STLExtras.h"
#include "llvm/Analysis/Passes.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Transforms/Scalar.h"
#include <vector>
using namespace llvm;
int main(int argc, char* argv[]) {
 LLVMContext &Context = getGlobalContext();
 Module* Module_Ob = new Module("my compiler", Context);
 static IRBuilder<> builder(Context);
 Module_Ob->getOrInsertGlobal("x", builder.getInt32Ty());
 GlobalVariable* gVar = Module_Ob->getNamedGlobal("x");
 gVar->setLinkage(GlobalValue::CommonLinkage);
 gVar->setAlignment(4);

 std::vector<std::string> Args;
 Args.push_back("a");
 Args.push_back("b");

 std::vector<Type*> Integers(Args.size(),Type::getInt32Ty(Context));
 FunctionType *funcType = llvm::FunctionType::get(builder.getInt32Ty(), Integers, false);
 Function *fooFunc = llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, "foo", Module_Ob); 

 unsigned Idx = 0;
 Function::arg_iterator AI;
  for (AI = fooFunc->arg_begin(); Idx != Args.size();
       ++AI, ++Idx) 
    AI->setName(Args[Idx]);
    
 BasicBlock *entry = llvm::BasicBlock::Create(Context, "entry", fooFunc);
 builder.SetInsertPoint(entry);
 
 Value *L = fooFunc->arg_begin();
 Value *R = ConstantInt::get(Type::getInt32Ty(Context), 16);
 builder.CreateMul(L, R, "multmp");

 Value *StartVal = ConstantInt::get(Type::getInt32Ty(Context),1);
 BasicBlock *PreheaderBB = builder.GetInsertBlock();
 BasicBlock *LoopBB = BasicBlock::Create(Context, "loop", fooFunc);
 builder.CreateBr(LoopBB);
 builder.SetInsertPoint(LoopBB);
 PHINode *Variable = builder.CreatePHI(Type::getInt32Ty(Context), 2, "i");
 Variable->addIncoming(StartVal, PreheaderBB);

 
 Value *StepVal = ConstantInt::get(Type::getInt32Ty(Context), 1); 
 Value *NextVar = builder.CreateAdd(Variable, StepVal,"nextvar");
 Value *EndCond = builder.CreateICmpULT(Variable, fooFunc->arg_begin(), "endcond");
 EndCond = builder.CreateICmpNE(EndCond, ConstantInt::get(Type::getInt32Ty(Context), 0), "loopcond");
 BasicBlock *LoopEndBB = builder.GetInsertBlock();
 BasicBlock *AfterBB = BasicBlock::Create(Context, "afterloop",fooFunc);
 builder.CreateCondBr(EndCond, LoopBB, AfterBB);
 builder.SetInsertPoint(AfterBB);
 Variable->addIncoming(NextVar, LoopEndBB);

 builder.CreateRet(ConstantInt::get(Type::getInt32Ty(getGlobalContext()),0));
 Module_Ob->dump();
 return 0;
}

