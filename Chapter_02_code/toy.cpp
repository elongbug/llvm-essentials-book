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
 Function::arg_iterator AI, AE;
  for (AI = fooFunc->arg_begin(), AE = fooFunc->arg_end(); AI != AE;
       ++AI, ++Idx) 
    AI->setName(Args[Idx]);
    
 BasicBlock *entry = llvm::BasicBlock::Create(Context, "entry", fooFunc);
 builder.SetInsertPoint(entry);
 
 Value *L = fooFunc->arg_begin();
 Value *R = ConstantInt::get(Type::getInt32Ty(Context), 16);
 builder.CreateMul(L, R, "multmp");
 R = ConstantInt::get(Type::getInt32Ty(Context),100);
 L = builder.CreateICmpULT(L, R, "cmptmp");
 //Value *Condtn = builder.CreateZExt(L, Type::getInt32Ty(Context),"booltmp");
 
 Value *Condtn = builder.CreateICmpNE(L, builder.getInt32(0), "ifcond");
 BasicBlock *ThenBB = BasicBlock::Create(Context, "then", fooFunc);
 BasicBlock *ElseBB = BasicBlock::Create(Context,"else");
 BasicBlock *MergeBB = BasicBlock::Create(Context,"ifcont");
 builder.CreateCondBr(Condtn, ThenBB, ElseBB);
 builder.SetInsertPoint(ThenBB);
 Value *ThenVal = builder.CreateAdd(gVar,ConstantInt::get(Type::getInt32Ty(Context),1),"thenaddtmp");
 builder.CreateBr(MergeBB);
 //ThenBB = builder.GetInsertBlock();
 fooFunc->getBasicBlockList().push_back(ElseBB);
 builder.SetInsertPoint(ElseBB);
 Value *ElseVal = builder.CreateAdd(gVar,ConstantInt::get(Type::getInt32Ty(Context),2),"elseaddtmp");
 builder.CreateBr(MergeBB);
 //ElseBB = builder.GetInsertBlock();
 fooFunc->getBasicBlockList().push_back(ThenBB);
 fooFunc->getBasicBlockList().push_back(ElseBB);
 fooFunc->getBasicBlockList().push_back(MergeBB);
 builder.SetInsertPoint(MergeBB);
 PHINode *Phi = builder.CreatePHI(Type::getInt32Ty(getGlobalContext()), 2, "iftmp");
 Phi->addIncoming(ThenVal, ThenBB);
 Phi->addIncoming(ElseVal, ElseBB);


 builder.CreateRet(ConstantInt::get(Type::getInt32Ty(getGlobalContext()),0));
 verifyFunction(*fooFunc);
 Module_Ob->dump();
 return 0;
}

