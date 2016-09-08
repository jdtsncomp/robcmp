#include "node.h"

///////////////////////////////////////////////////////////////////////////////

//#define IGNORE_INVALID_ENGINE_NUMBER
//#define HAS_DISPLAY
#define DBG_ROTATE_DELAY	1000
#define NO_SPINNING			10

///////////////////////////////////////////////////////////////////////////////

#define BUTTON_PORT "2"
#define MAGNET_PORT "19"

///////////////////////////////////////////////////////////////////////////////

Motor g_TabelaMotores[] = {
	{"XX", "YY"},
	{"2", "3"},
	{"4", "5"},
	{"6", "7"},
	{"8", "9"},
	{"10", "12"}
};

///////////////////////////////////////////////////////////////////////////////

Node* RotateCommand(Node* pEngineId, Node* pTimeAmount){
	NumberNode* pMotor = (NumberNode*)(pEngineId);
	NumberNode* pAmount = (NumberNode*)(pTimeAmount);
	
	int indiceMotor = pMotor->AbsValue();
	if(pMotor->IsNegative() || indiceMotor == 0 || indiceMotor > 5){
		#ifndef IGNORE_INVALID_ENGINE_NUMBER
			LOG_ERROR_EX("Indice do motor(%d) invalido !", pMotor->GetValue());
		#endif
		return new DummyNode();
	}	
	
	if(pAmount->GetValue() == 0){
		LOG_ERROR_EX("Tempo de movimentação (%d) invalido !", pAmount->GetValue());
		return new DummyNode();
	}
	
	const char* pPortName = (g_TabelaMotores[indiceMotor])[pAmount->IsNegative()];

	Stmts* pBloco = new Stmts(new OutPort(pPortName, new Int16(255))); 
	pBloco->append(new Delay(new Int16(pAmount->AbsValue())));
	pBloco->append(new OutPort(pPortName, new Int16(0)));
	#ifdef DBG_ROTATE_DELAY
		pBloco->append(new Delay(new Int16(DBG_ROTATE_DELAY)));
	#endif
	
	#ifdef HAS_DISPLAY
		pBloco->append(new Print(new String(pPortName)));
	#endif
	
	return pBloco;
}

///////////////////////////////////////////////////////////////////////////////

Node* WaitButtonPressCommand(){
	Int16* pFalso = new Int16(0);											
	Stmts* pBloco = new Stmts(new Variable("ButtonPressed", pFalso));
	Stmts* pBlocoWhile = new Stmts(new Variable("ButtonPressed", new InPort(BUTTON_PORT)));
	
	#ifdef NO_SPINNING
		pBlocoWhile->append(new Delay(new Int16(NO_SPINNING)));
	#endif
	
	#ifdef HAS_DISPLAY
		pBloco->append(new Print(new String("BTN")));
	#endif
	
	pBloco->append(new While(new CmpOp(new Load("ButtonPressed"), EQ_OP, pFalso), pBlocoWhile));

	return pBloco;
}

///////////////////////////////////////////////////////////////////////////////

Node* MagnetCommand(Node* pValue){
	if(pValue->AbsValue() == 0){
		return new OutPort(MAGNET_PORT, new Int16(0));
	}
	return new OutPort(MAGNET_PORT, new Int16(255));
}

///////////////////////////////////////////////////////////////////////////////
