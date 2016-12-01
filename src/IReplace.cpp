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
                auto M = I.getParent()->getParent()->getParent();
                std::vector<Type*> dualint(2, Type::getInt32Ty(I.getContext()));
                auto t = FunctionType::get(Type::getInt32Ty(I.getContext()),
                                           dualint,
                                           false);
                auto addFunc = cast<Function>(M->getOrInsertFunction("rpiLop_add", t));
                auto ci = CallInst::Create(t, addFunc, {I.getOperand(0), I.getOperand(1)}, "");
                ReplaceInstWithInst(&I, ci);
            }
            else if (I.getOpcode() == Instruction::Sub) {
                auto M = I.getParent()->getParent()->getParent();
                std::vector<Type*> dualint(2, Type::getInt32Ty(I.getContext()));
                auto t = FunctionType::get(Type::getInt32Ty(I.getContext()),
                                           dualint,
                                           false);
                auto subFunc = cast<Function>(M->getOrInsertFunction("rpiLop_subtract", t));
                auto ci = CallInst::Create(t, subFunc, {I.getOperand(0), I.getOperand(1)}, "");
                ReplaceInstWithInst(&I, ci);
            }
            else if (I.getOpcode() == Instruction::Mul) {
                auto M = I.getParent()->getParent()->getParent();
                std::vector<Type*> dualint(2, Type::getInt32Ty(I.getContext()));
                auto t = FunctionType::get(Type::getInt32Ty(I.getContext()),
                                           dualint,
                                           false);
                auto mulFunc = cast<Function>(M->getOrInsertFunction("rpiLop_multiply", t));
                auto ci = CallInst::Create(t, mulFunc, {I.getOperand(0), I.getOperand(1)}, "");
                ReplaceInstWithInst(&I, ci);
            }
            else if (I.getOpcode() == Instruction::UDiv) {
                auto M = I.getParent()->getParent()->getParent();
                std::vector<Type*> dualint(2, Type::getInt32Ty(I.getContext()));
                auto t = FunctionType::get(Type::getInt32Ty(I.getContext()),
                                           dualint,
                                           false);
                auto divFunc = cast<Function>(M->getOrInsertFunction("rpiLop_divide", t));
                auto ci = CallInst::Create(t, divFunc, {I.getOperand(0), I.getOperand(1)}, "");
                ReplaceInstWithInst(&I, ci);
            }
            else if (I.getOpcode() == Instruction::SDiv) {
                auto M = I.getParent()->getParent()->getParent();
                std::vector<Type*> dualint(2, Type::getInt32Ty(I.getContext()));
                auto t = FunctionType::get(Type::getInt32Ty(I.getContext()),
                                           dualint,
                                           false);
                auto sdivFunc = cast<Function>(M->getOrInsertFunction("rpiLop_divide", t));
                auto ci = CallInst::Create(t, sdivFunc, {I.getOperand(0), I.getOperand(1)}, "");
                ReplaceInstWithInst(&I, ci);
            }
            else if (I.getOpcode() == Instruction::FAdd) {
                auto M = I.getParent()->getParent()->getParent();
                std::vector<Type*> dualfloat(2, Type::getFloat32Ty(I.getContext()));
                auto t = FunctionType::get(Type::getFloat32Ty(I.getContext()),
                                           dualfloat,
                                           false);
                auto addFunc = cast<Function>(M->getOrInsertFunction("rpiLop_fadd", t));
                auto ci = CallInst::Create(t, addFunc, {I.getOperand(0), I.getOperand(1)}, "");
                ReplaceInstWithInst(&I, ci);
            }
            else if (I.getOpcode() == Instruction::FSub) {
                auto M = I.getParent()->getParent()->getParent();
                std::vector<Type*> dualfloat(2, Type::getFloat32Ty(I.getContext()));
                auto t = FunctionType::get(Type::getFloat32Ty(I.getContext()),
                                           dualfloat,
                                           false);
                auto subFunc = cast<Function>(M->getOrInsertFunction("rpiLop_fsub", t));
                auto ci = CallInst::Create(t, subFunc, {I.getOperand(0), I.getOperand(1)}, "");
                ReplaceInstWithInst(&I, ci);
            }
            else if (I.getOpcode() == Instruction::FMul) {
                auto M = I.getParent()->getParent()->getParent();
                std::vector<Type*> dualfloat(2, Type::getFloat32Ty(I.getContext()));
                auto t = FunctionType::get(Type::getFloat32Ty(I.getContext()),
                                           dualfloat,
                                           false);
                auto mulFunc = cast<Function>(M->getOrInsertFunction("rpiLop_fmul", t));
                auto ci = CallInst::Create(t, mulFunc, {I.getOperand(0), I.getOperand(1)}, "");
                ReplaceInstWithInst(&I, ci);
            }
            else if (I.getOpcode() == Instruction::FDiv) {
                auto M = I.getParent()->getParent()->getParent();
                std::vector<Type*> dualfloat(2, Type::getFloat32Ty(I.getContext()));
                auto t = FunctionType::get(Type::getFloat32Ty(I.getContext()),
                                           dualfloat,
                                           false);
                auto divFunc = cast<Function>(M->getOrInsertFunction("rpiLop_fdiv", t));
                auto ci = CallInst::Create(t, mulFunc, {I.getOperand(0), I.getOperand(1)}, "");
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
