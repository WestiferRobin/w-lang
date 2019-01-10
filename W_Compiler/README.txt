// PHASE 1 - Reproduce CSC 151 without Bison

// PHASE 2 - Implement strings, arrays & structs (maybe pointers and enums)

// PHASE 3 - Implement object oriented and libraries to compiler

-------------------------------------------------------------------------------
PHASE 1 - Grammar (IGNORE SPACES)
eps == epsilon

// TODO: FIX THE GRAMMER ITS NOT DONE YET

Goal -> woid wain ((woid | eps)) { Body }

Body -> Body BlockIf
      | Body BlockLoop
      | Body BlockAssign
      | eps

BlockIf -> BlockIf If
         | If
BlockLoop -> BlockLoop Loop
           | Loop
BlockAssign -> BlockAssign Assign
             | Assign

Statement -> StatementPrime && StatementPrime
StatementPrime -> Expr == Expr
                | Expr != Expr

Loop -> While
      | DoWhile
      | ForLoop
While -> awhile(Statement) { Body }
DoWhile -> do { Body } While;
ForLoop -> wor (Assign; Statement; Exp) { Body }


If -> wif (Statement) { Body } Else
Else -> Else welse { Body }
      | welif (Statement) { Body }
      | eps

Assign -> DataType name = Expr;
Expr -> Expr + Term
      | Expr - Term
      | Term
Term -> Term * Factor
      | Term / Factor
      | Factor
Factor -> (Expr)
        | num
        | name
