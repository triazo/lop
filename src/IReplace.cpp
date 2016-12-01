#include "llvm/Pass.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/InstVisitor.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
//#include "llvm/Constants.h"

using namespace llvm;

// rpiLop_add

namespace{
    struct IReplaceVisitor: public InstVisitor<IReplaceVisitor> {
        unsigned Count;
        IReplaceVisitor() : Count(0) {}


        void visitBinaryOperator(BinaryOperator &I) {
            Count++;
            if (I.getOpcode() == Instruction::Add) {
                auto fname = (I.getOperand(0)->getType() == Type::getInt64Ty(I.getContext()))?"rpiLop_add64":"rpiLop_add";
                auto M = I.getParent()->getParent()->getParent();
                std::vector<Type*> dualint(2, I.getOperand(0)->getType());
                auto t = FunctionType::get(I.getOperand(0)->getType(),
                                           dualint,
                                           false);
                auto addFunc = cast<Function>(M->getOrInsertFunction(fname, t));
                auto ci = CallInst::Create(t, addFunc, {I.getOperand(0), I.getOperand(1)}, "");
                ReplaceInstWithInst(&I, ci);
            }
            else if (I.getOpcode() == Instruction::Sub) {
                auto fname = (I.getOperand(0)->getType() == Type::getInt64Ty(I.getContext()))?"rpiLop_sub64":"rpiLop_sub";
                auto M = I.getParent()->getParent()->getParent();
                std::vector<Type*> dualint(2, I.getOperand(0)->getType());
                auto t = FunctionType::get(I.getOperand(0)->getType(),
                                           dualint,
                                           false);
                auto subFunc = cast<Function>(M->getOrInsertFunction(fname, t));
                auto ci = CallInst::Create(t, subFunc, {I.getOperand(0), I.getOperand(1)}, "");
                ReplaceInstWithInst(&I, ci);
            }
            else if (I.getOpcode() == Instruction::Mul) {
                auto fname = (I.getOperand(0)->getType() == Type::getInt64Ty(I.getContext()))?"rpiLop_mul64":"rpiLop_mul";
                auto M = I.getParent()->getParent()->getParent();
                std::vector<Type*> dualint(2, I.getOperand(0)->getType());
                auto t = FunctionType::get(I.getOperand(0)->getType(),
                                           dualint,
                                           false);
                auto mulFunc = cast<Function>(M->getOrInsertFunction(fname, t));
                auto ci = CallInst::Create(t, mulFunc, {I.getOperand(0), I.getOperand(1)}, "");
                ReplaceInstWithInst(&I, ci);
            }
            else if (I.getOpcode() == Instruction::UDiv) {
                auto fname = (I.getOperand(0)->getType() == Type::getInt64Ty(I.getContext()))?"rpiLop_div64":"rpiLop_div";
                auto M = I.getParent()->getParent()->getParent();
                std::vector<Type*> dualint(2, I.getOperand(0)->getType());
                auto t = FunctionType::get(I.getOperand(0)->getType(),
                                           dualint,
                                           false);
                auto divFunc = cast<Function>(M->getOrInsertFunction(fname, t));
                auto ci = CallInst::Create(t, divFunc, {I.getOperand(0), I.getOperand(1)}, "");
                ReplaceInstWithInst(&I, ci);
            }
            // else if (I.getOpcode() == Instruction::SDiv) {
            //     auto fname = (I.getOperand(0)->getType() == Type::getInt64Ty(I.getContext()))?"rpiLop_div64":"rpiLop_div";
            //     auto M = I.getParent()->getParent()->getParent();
            //     std::vector<Type*> dualint(2, I.getOperand(0)->getType());
            //     auto t = FunctionType::get(I.getOperand(0)->getType(),
            //                                dualint,
            //                                false);
            //     auto sdivFunc = cast<Function>(M->getOrInsertFunction(fname, t));
            //     auto ci = CallInst::Create(t, sdivFunc, {I.getOperand(0), I.getOperand(1)}, "");
            //     ReplaceInstWithInst(&I, ci);
            // }
            else if (I.getOpcode() == Instruction::FAdd) {
                auto M = I.getParent()->getParent()->getParent();
                std::vector<Type*> dualfloat(2, Type::getFloatTy(I.getContext()));
                auto t = FunctionType::get(Type::getFloatTy(I.getContext()),
                                           dualfloat,
                                           false);
                auto addFunc = cast<Function>(M->getOrInsertFunction("rpiLop_fpadd", t));
                auto ci = CallInst::Create(t, addFunc, {I.getOperand(0), I.getOperand(1)}, "");
                ReplaceInstWithInst(&I, ci);
            }
            else if (I.getOpcode() == Instruction::FSub) {
                auto M = I.getParent()->getParent()->getParent();
                std::vector<Type*> dualfloat(2, Type::getFloatTy(I.getContext()));
                auto t = FunctionType::get(Type::getFloatTy(I.getContext()),
                                           dualfloat,
                                           false);
                auto subFunc = cast<Function>(M->getOrInsertFunction("rpiLop_fpsub", t));
                auto ci = CallInst::Create(t, subFunc, {I.getOperand(0), I.getOperand(1)}, "");
                ReplaceInstWithInst(&I, ci);
            }
            else if (I.getOpcode() == Instruction::FMul) {
                auto M = I.getParent()->getParent()->getParent();
                std::vector<Type*> dualfloat(2, Type::getFloatTy(I.getContext()));
                auto t = FunctionType::get(Type::getFloatTy(I.getContext()),
                                           dualfloat,
                                           false);
                auto mulFunc = cast<Function>(M->getOrInsertFunction("rpiLop_fpmul", t));
                auto ci = CallInst::Create(t, mulFunc, {I.getOperand(0), I.getOperand(1)}, "");
                ReplaceInstWithInst(&I, ci);
            }
            else if (I.getOpcode() == Instruction::FDiv) {
                auto M = I.getParent()->getParent()->getParent();
                std::vector<Type*> dualfloat(2, Type::getFloatTy(I.getContext()));
                auto t = FunctionType::get(Type::getFloatTy(I.getContext()),
                                           dualfloat,
                                           false);
                auto divFunc = cast<Function>(M->getOrInsertFunction("rpiLop_fpdiv", t));
                auto ci = CallInst::Create(t, divFunc, {I.getOperand(0), I.getOperand(1)}, "");
                ReplaceInstWithInst(&I, ci);
            }
        }
    };

    struct IReplace: public FunctionPass {
        static char ID;
        IReplace() : FunctionPass(ID) {}

        bool runOnFunction(Function &F) override {
            IReplaceVisitor iiv;
            errs() << "Visiting function: ";
            errs().write_escaped(F.getName()) << '\n';
            iiv.visit(F);
            return false;
        }
    };

}

char IReplace::ID = 0;
static RegisterPass<IReplace> X("ireplace", "instruction inversion pass", false, false);
