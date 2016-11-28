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
