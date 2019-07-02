#ifndef OWL2FUNCTIONALSTYLEPARSERTOKENMANAGER_H
#define OWL2FUNCTIONALSTYLEPARSERTOKENMANAGER_H
#include "JavaCC.h"
#include "CharStream.h"
#include "Token.h"
#include "TokenManager.h"
#include "OWL2FunctionalStyleParserConstants.h"
#include "CTokenManager.h"

namespace Konclude {
namespace Parser {
namespace FunctionalJAVACC {
class OWL2FunctionalStyleParser;

/** Token Manager. */
class OWL2FunctionalStyleParserTokenManager : public TokenManager, public CTokenManager {
   public:

  /** Debug output. */
  FILE *debugStream;
  /** Set debug output. */

void  setDebugStream(FILE *ds);

 int jjStopStringLiteralDfa_0(int pos, unsigned long long active0, unsigned long long active1);

int  jjStartNfa_0(int pos, unsigned long long active0, unsigned long long active1);

 int  jjStopAtPos(int pos, int kind);

 int  jjMoveStringLiteralDfa0_0();

 int  jjMoveStringLiteralDfa1_0(unsigned long long active0, unsigned long long active1);

 int  jjMoveStringLiteralDfa2_0(unsigned long long old0, unsigned long long active0, unsigned long long old1, unsigned long long active1);

 int  jjMoveStringLiteralDfa3_0(unsigned long long old0, unsigned long long active0, unsigned long long old1, unsigned long long active1);

 int  jjMoveStringLiteralDfa4_0(unsigned long long old0, unsigned long long active0, unsigned long long old1, unsigned long long active1);

 int  jjMoveStringLiteralDfa5_0(unsigned long long old0, unsigned long long active0, unsigned long long old1, unsigned long long active1);

 int  jjMoveStringLiteralDfa6_0(unsigned long long old0, unsigned long long active0, unsigned long long old1, unsigned long long active1);

 int  jjMoveStringLiteralDfa7_0(unsigned long long old0, unsigned long long active0, unsigned long long old1, unsigned long long active1);

 int  jjMoveStringLiteralDfa8_0(unsigned long long old0, unsigned long long active0, unsigned long long old1, unsigned long long active1);

 int  jjMoveStringLiteralDfa9_0(unsigned long long old0, unsigned long long active0, unsigned long long old1, unsigned long long active1);

 int  jjMoveStringLiteralDfa10_0(unsigned long long old0, unsigned long long active0, unsigned long long old1, unsigned long long active1);

 int  jjMoveStringLiteralDfa11_0(unsigned long long old0, unsigned long long active0, unsigned long long old1, unsigned long long active1);

 int  jjMoveStringLiteralDfa12_0(unsigned long long old0, unsigned long long active0, unsigned long long old1, unsigned long long active1);

 int  jjMoveStringLiteralDfa13_0(unsigned long long old0, unsigned long long active0, unsigned long long old1, unsigned long long active1);

 int  jjMoveStringLiteralDfa14_0(unsigned long long old0, unsigned long long active0, unsigned long long old1, unsigned long long active1);

 int  jjMoveStringLiteralDfa15_0(unsigned long long old0, unsigned long long active0, unsigned long long old1, unsigned long long active1);

 int  jjMoveStringLiteralDfa16_0(unsigned long long old0, unsigned long long active0, unsigned long long old1, unsigned long long active1);

 int  jjMoveStringLiteralDfa17_0(unsigned long long old0, unsigned long long active0, unsigned long long old1, unsigned long long active1);

 int  jjMoveStringLiteralDfa18_0(unsigned long long old0, unsigned long long active0, unsigned long long old1, unsigned long long active1);

 int  jjMoveStringLiteralDfa19_0(unsigned long long old0, unsigned long long active0, unsigned long long old1, unsigned long long active1);

 int  jjMoveStringLiteralDfa20_0(unsigned long long old0, unsigned long long active0, unsigned long long old1, unsigned long long active1);

 int  jjMoveStringLiteralDfa21_0(unsigned long long old0, unsigned long long active0, unsigned long long old1, unsigned long long active1);

 int  jjMoveStringLiteralDfa22_0(unsigned long long old0, unsigned long long active0, unsigned long long old1, unsigned long long active1);

 int  jjMoveStringLiteralDfa23_0(unsigned long long old0, unsigned long long active0, unsigned long long old1, unsigned long long active1);

 int  jjMoveStringLiteralDfa24_0(unsigned long long old0, unsigned long long active0, unsigned long long old1, unsigned long long active1);

 int  jjMoveStringLiteralDfa25_0(unsigned long long old0, unsigned long long active0, unsigned long long old1, unsigned long long active1);

 int  jjMoveStringLiteralDfa26_0(unsigned long long old0, unsigned long long active0, unsigned long long old1, unsigned long long active1);

 int  jjMoveStringLiteralDfa27_0(unsigned long long old0, unsigned long long active0, unsigned long long old1, unsigned long long active1);

 int  jjMoveStringLiteralDfa28_0(unsigned long long old0, unsigned long long active0, unsigned long long old1, unsigned long long active1);

 int  jjMoveStringLiteralDfa29_0(unsigned long long old0, unsigned long long active0, unsigned long long old1, unsigned long long active1);

 int  jjMoveStringLiteralDfa30_0(unsigned long long old0, unsigned long long active0, unsigned long long old1, unsigned long long active1);

int jjStartNfaWithStates_0(int pos, int kind, int state);

int jjMoveNfa_0(int startState, int curPos);

 int  jjMoveStringLiteralDfa0_3();

int jjMoveNfa_3(int startState, int curPos);

 int  jjMoveStringLiteralDfa0_5();

 int  jjMoveStringLiteralDfa0_1();

int jjMoveNfa_1(int startState, int curPos);

 int  jjMoveStringLiteralDfa0_6();

 int  jjMoveStringLiteralDfa0_2();

 int  jjMoveStringLiteralDfa0_4();

int jjMoveNfa_4(int startState, int curPos);

bool jjCanMove_0(int hiByte, int i1, int i2, unsigned long long l1, unsigned long long l2);

bool jjCanMove_1(int hiByte, int i1, int i2, unsigned long long l1, unsigned long long l2);

bool jjCanMove_2(int hiByte, int i1, int i2, unsigned long long l1, unsigned long long l2);

Token * jjFillToken();

public: int curLexState;
public: int jjnewStateCnt;
public: int jjround;
public: int jjmatchedPos;
public: int jjmatchedKind;


Token * getNextToken();

void  SkipLexicalActions(Token *matchedToken);

void  TokenLexicalActions(Token *matchedToken);
#define jjCheckNAdd(state)\
{\
   if (jjrounds[state] != jjround)\
   {\
      jjstateSet[jjnewStateCnt++] = state;\
      jjrounds[state] = jjround;\
   }\
}
#define jjAddStates(start, end)\
{\
   for (int x = start; x <= end; x++) {\
      jjstateSet[jjnewStateCnt++] = jjnextStates[x];\
   } /*while (start++ != end);*/\
}
#define jjCheckNAddTwoStates(state1, state2)\
{\
   jjCheckNAdd(state1);\
   jjCheckNAdd(state2);\
}

#define jjCheckNAddStates(start, end)\
{\
   for (int x = start; x <= end; x++) {\
      jjCheckNAdd(jjnextStates[x]);\
   } /*while (start++ != end);*/\
}

#ifndef JAVACC_CHARSTREAM
#define JAVACC_CHARSTREAM CharStream
#endif
  private: OWL2FunctionalStyleParser*parser;
  private: void ReInitRounds();
  public: OWL2FunctionalStyleParserTokenManager(JAVACC_CHARSTREAM *stream, int lexState = 0, OWL2FunctionalStyleParser *parserArg = NULL);
  public: virtual ~OWL2FunctionalStyleParserTokenManager();
  void ReInit(JAVACC_CHARSTREAM *stream, int lexState = 0, OWL2FunctionalStyleParser *parserArg = NULL);
  void SwitchTo(int lexState);
  const JAVACC_SIMPLE_STRING jjKindsForBitVector(int i, unsigned long long vec);
  const JAVACC_SIMPLE_STRING jjKindsForStateVector(int lexState, int vec[], int start, int end);
    JAVACC_CHARSTREAM  *input_stream;
    int jjrounds[9];
    int jjstateSet[2 * 9];
    JAVACC_STRING_TYPE jjimage;
    JAVACC_STRING_TYPE image;
    int jjimageLen;
    int lengthOfMatch;
    JAVACC_CHAR_TYPE curChar;
};
}
}
}
#endif
