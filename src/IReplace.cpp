#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/InstVisitor.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
//#include "llvm/Constants.h"

using namespace llvm;

namespace{
    struct IReplaceVisitor: public InstVisitor<IReplaceVisitor> {
        unsigned Count;
        IReplaceVisitor() : Count(0) {}

        void visitBinaryOperator(BinaryOperator &I) {
            Count++;
            BinaryOperator* v1, *v2;

            if (I.getOpcode() == Instruction::Add) {
                v1 = BinaryOperator::Create(Instruction::Mul, I.getOperand(0), ConstantInt::get(I.getType(), -1));
                v2 = BinaryOperator::Create(Instruction::Sub, v1, I.getOperand(1));
                ReplaceInstWithInst(&I, v2);
                v1->insertBefore(v2);
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
