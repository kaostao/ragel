/*
 *  Copyright 2005-2006 Adrian Thurston <thurston@cs.queensu.ca>
 */

/*  This file is part of Ragel.
 *
 *  Ragel is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 * 
 *  Ragel is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 * 
 *  You should have received a copy of the GNU General Public License
 *  along with Ragel; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
 */

#ifndef _GENDATA_H
#define _GENDATA_H

#include <iostream>
#include "redfsm.h"
#include "fsmcodegen.h"
#include "common.h"

struct NameInst;
typedef DList<Action> ActionList;

typedef unsigned long ulong;

typedef AvlMap<char *, CodeGenData*, CmpStr> CodeGenMap;
typedef AvlMapEl<char *, CodeGenData*> CodeGenMapEl;

#define WO_NOEND    0x01
#define WO_NOERROR  0x02
#define WO_NOPREFIX 0x04
#define WO_NOFF     0x08

struct CodeGenData
{
	CodeGenData( char *fileName, char *fsmName, bool wantComplete )
	:
		fileName(fileName),
		fsmName(fsmName), 
		redFsm(0), 
		allActions(0),
		allActionTables(0),
		allConditions(0),
		allCondSpaces(0),
		allStates(0),
		nameIndex(0),
		startState(0),
		getKeyExpr(0),
		accessExpr(0),
		curStateExpr(0),
		codeGen(0),
		wantComplete(wantComplete),
		writeOps(0),
		writeData(false),
		writeInit(false),
		writeExec(false),
		writeEOF(false),
		hasLongestMatch(false),
		hasEnd(true),
		dataPrefix(true),
		writeFirstFinal(true),
		writeErr(true),
		hasBeenPrepared(false)
	{ }

	/* 
	 * Collecting the machine.
	 */

	char *fileName;
	char *fsmName;
	RedFsmAp *redFsm;
	Action *allActions;
	RedAction *allActionTables;
	Condition *allConditions;
	CondSpace *allCondSpaces;
	RedStateAp *allStates;
	NameInst **nameIndex;
	int startState;
	ActionList actionList;
	ConditionList conditionList;
	CondSpaceList condSpaceList;
	InlineList *getKeyExpr;
	InlineList *accessExpr;
	InlineList *curStateExpr;
	FsmCodeGen *codeGen;
	KeyOps thisKeyOps;
	bool wantComplete;
	int writeOps;
	bool writeData;
	bool writeInit;
	bool writeExec;
	bool writeEOF;
	EntryIdVect entryPointIds;
	EntryNameVect entryPointNames;
	bool hasLongestMatch;

	/* Write options. */
	bool hasEnd;
	bool dataPrefix;
	bool writeFirstFinal;
	bool writeErr;

	void createMachine();
	void initActionList( unsigned long length );
	void newAction( int anum, char *name, int line, int col, InlineList *inlineList );
	void initActionTableList( unsigned long length );
	void initStateList( unsigned long length );
	void setStartState( unsigned long startState );
	void addEntryPoint( char *name, unsigned long entryState );
	void setFinal( int snum );
	void initTransList( int snum, unsigned long length );
	void newTrans( int snum, int tnum, Key lowKey, Key highKey, 
			long targ, long act );
	void finishTransList( int snum );
	void setStateActions( int snum, long toStateAction, 
			long fromStateAction, long eofAction );
	void finishMachine();
	void setForcedErrorState()
		{ redFsm->forcedErrorState = true; }
	
	void initCondSpaceList( ulong length );
	void condSpaceItem( int cnum, long condActionId );
	void newCondSpace( int cnum, int condSpaceId, Key baseKey );

	void initStateCondList( int snum, ulong length );
	void addStateCond( int snum, Key lowKey, Key highKey, long condNum );

	CondSpace *findCondSpace( Key lowKey, Key highKey );
	Condition *findCondition( Key key );

	bool setAlphType( char *data );

	void makeCodeGen();
	void generateGraphviz();
	void resolveTargetStates( InlineList *inlineList );
	Key findMaxKey();

	void generate();
	void generateCode();
	void prepareMachine();
	bool hasBeenPrepared;
};

extern CodeGenData *cgd;

void lineDirective( ostream &out, char *fileName, int line );
void genLineDirective( ostream &out );

#endif /* _GENDATA_H */