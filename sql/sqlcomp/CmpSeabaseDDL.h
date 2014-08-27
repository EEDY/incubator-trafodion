// @@@ START COPYRIGHT @@@
//
// (C) Copyright 2014 Hewlett-Packard Development Company, L.P.
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
//
// @@@ END COPYRIGHT @@@

/*
 *****************************************************************************
 *
 * File:         CmpSeabaseDDL.h
 * Description:  This file describes the DDL classes for Trafodion
 *               
 * Contents:
 *   Metadata table descriptors
 *   class CmpSeabaseDDL
 *
 *****************************************************************************
*/

#ifndef _CMP_SEABASE_DDL_H_
#define _CMP_SEABASE_DDL_H_

#include "ComTdb.h"
#include "ExpHbaseDefs.h"
#include "ExpLOBenums.h"
#include "NADefaults.h"
#include "NAColumn.h"
#include "desc.h"
#include "CmpMessage.h"

class ExpHbaseInterface;
class ExeCliInterface;
class Queue;

class StmtDDLCreateTable;
class StmtDDLDropTable;

class StmtDDLCreateHbaseTable;
class StmtDDLDropHbaseTable;

class StmtDDLCreateIndex;
class StmtDDLDropIndex;
class StmtDDLPopulateIndex;

class StmtDDLAlterTableRename;

class StmtDDLAlterTableAddColumn;
class StmtDDLAlterTableDropColumn;

class StmtDDLCreateView;
class StmtDDLDropView;

class StmtDDLCreateLibrary;
class StmtDDLDropLibrary;

class StmtDDLCreateRoutine;
class StmtDDLDropRoutine;

class StmtDDLCreateSequence;
class StmtDDLDropSequence;

class StmtDDLDropSchema;

// Classes for user management
class StmtDDLRegisterUser;
class StmtDDLAlterUser;
class CmpSeabaseDDLauth;

// classes for constraints
class StmtDDLAddConstraint;
class StmtDDLAddConstraintPK;
class StmtDDLAddConstraintUnique;
class StmtDDLAddConstraintRIArray;
class StmtDDLAddConstraintUniqueArray;
class StmtDDLAddConstraintCheckArray;
class StmtDDLDropConstraint;
class StmtDDLAddConstraintCheck;

class ElemDDLColDefArray;
class ElemDDLColRefArray;
class ElemDDLParamDefArray;

class DDLExpr;
class DDLNode;

class NADefaults;

class NAType;

struct desc_struct;
class OutputInfo;

class ByteArrayList;

class HbaseCreateOption;

class Parser;

class NAColumnArray;

struct routine_desc_struct;
struct MDDescsInfo;

#include "CmpSeabaseDDLmd.h"

class CmpSeabaseDDL
{
 public:
  CmpSeabaseDDL(NAHeap *heap, NABoolean syscatInit = TRUE);

  static NABoolean isSeabase(const NAString &catName);

  static NABoolean isHbase(const NAString &catName);

  static NABoolean isSeabaseMD(const NAString &catName,
			       const NAString &schName,
			       const NAString &objName);
 
  static NABoolean isUserUpdatableSeabaseMD(const NAString &catName,
			       const NAString &schName,
			       const NAString &objName);
 
  short existsInHbase(const NAString &objName,
		      ExpHbaseInterface * ehi = NULL);

  void processSystemCatalog(NADefaults * defs);

  short readAndInitDefaultsFromSeabaseDefaultsTable
    (NADefaults::Provenance overwriteIfNotYet, Int32 errOrWarn,
     NADefaults * defs);

  short validateVersions(NADefaults *defs, 
			 ExpHbaseInterface * inEHI = NULL,
			 Int64 * mdMajorVersion = NULL,
			 Int64 * mdMinorVersion = NULL,
			 Lng32 * hbaseErr = NULL,
			 NAString * hbaseErrStr = NULL);

  short executeSeabaseDDL(DDLExpr * ddlExpr, ExprNode * ddlNode,
			  NAString &currCatName, NAString &currSchName);

  desc_struct * getSeabaseTableDesc(const NAString &catName, 
				    const NAString &schName, 
				    const NAString &objName,
				    const char * objType,
				    NABoolean includeInvalidDefs = FALSE);

  short getObjectOwner(ExeCliInterface *cliInterface,
                        const char * catName,
                        const char * schName,
                        const char * objName,
                        Int32 * objectOwner);

  static short genHbaseCreateOptions(
				     const char * hbaseCreateOptionsStr,
				     NAList<HbaseCreateOption*>* &hbaseCreateOptions,
				     NAMemory * heap);

   desc_struct *getSeabaseRoutineDesc(const NAString &catName,
                                      const NAString &schName,
                                      const NAString &objName);

  short createMDdescs(MDDescsInfo *&);

  static NAString getSystemCatalogStatic();

  static NABoolean isEncodingNeededForSerialization(NAColumn * nac);

  static NABoolean enabledForSerialization(NAColumn * nac);

  static NABoolean isSerialized(ULng32 flags)
  {
    return (flags & SEABASE_SERIALIZED) != 0;
  }

  short buildColInfoArray(
			  ElemDDLColDefArray * colArray,
			  ComTdbVirtTableColumnInfo * colInfoArray,
			  NABoolean implicitPK,
			  CollIndex numSysCols,
			  NAMemory * heap = NULL);

 protected:

  enum { 
    METADATA_MAJOR_VERSION = 2,
    METADATA_OLD_MAJOR_VERSION = 2,
    METADATA_MINOR_VERSION = 3,
    METADATA_OLD_MINOR_VERSION = 2,
    DATAFORMAT_MAJOR_VERSION = 1,
    DATAFORMAT_MINOR_VERSION = 1,
    SOFTWARE_MAJOR_VERSION = 1,
    SOFTWARE_MINOR_VERSION = 1
  };

  enum {
    SEABASE_SERIALIZED = 0x0001
  };

  void setFlags(ULng32 &flags, ULng32 flagbits)
  {
    flags |= flagbits;
  }

  void resetFlags(ULng32 &flags, ULng32 flagbits)
  {
    flags &= ~flagbits;
  }

  const char * getSystemCatalog();

  ComBoolean isSeabaseMD(const ComObjectName &name);

  ComBoolean isSeabase(const ComObjectName &name) ;

  ComBoolean isHbase(const ComObjectName &name) ;

  short isMetadataInitialized(ExpHbaseInterface * ehi = NULL);
  short isOldMetadataInitialized(ExpHbaseInterface * ehi);

  ExpHbaseInterface* allocEHI(const char * server, const char * port,
			      const char * interface, const char * zkPort,
                              NABoolean raiseError);
  
  ExpHbaseInterface* allocEHI(NADefaults * defs = NULL);
  
  void deallocEHI(ExpHbaseInterface* &ehi);
  
  // if prevContext is defined, get the controlDB from the previous context so the
  // defaults are copied from the previous cmp context to the new cmp context. This is only
  // required for embedded compilers where a SWITCH is taking place
  short sendAllControlsAndFlags(CmpContext* prevContext=NULL);

  void restoreAllControlsAndFlags();
  
  void processReturn(Lng32 retcode = 0);

  // construct and return the column name value as stored with hbase rows.
  // colNum is 0-based (first col is 0)
  void getColName(const ComTdbVirtTableColumnInfo columnInfo[],
		  Lng32 colNum, NAString &colName);
  
  void getColName(const char * colFam, const char * colQual,
		  NAString &colName);

  desc_struct *getSeabaseRoutineDescInternal(const NAString &catName,
                                             const NAString &schName,
                                             const NAString &objName);

  short createHbaseTable(ExpHbaseInterface *ehi, 
			 HbaseStr *table,
			 const char * cf1, const char * cf2, const char * cf3,
			 NAList<HbaseCreateOption*> * hbaseCreateOptions = NULL,
                         const int numSplits = 0,
                         const int keyLength = 0,
                         char **encodedKeysBuffer = NULL,
			 NABoolean doRetry = TRUE);
  
  short dropHbaseTable(ExpHbaseInterface *ehi, 
		       HbaseStr *table, NABoolean asyncDrop = FALSE);

  short copyHbaseTable(ExpHbaseInterface *ehi, 
		       HbaseStr *currTable, HbaseStr* oldTable);

  NABoolean xnInProgress(ExeCliInterface *cliInterface);
  short beginXn(ExeCliInterface *cliInterface);
  short commitXn(ExeCliInterface *cliInterface);
  short rollbackXn(ExeCliInterface *cliInterface);

  short dropSeabaseObject(ExpHbaseInterface *ehi,
			  const NAString &objName,
			  NAString &currCatName, NAString &currSchName,
			  const char * objType,
			  NABoolean dropFromMD = TRUE,
			  NABoolean dropFromHbase = TRUE);
  
  short dropSeabaseStats(ExeCliInterface *cliInterface,
                         const char * catName,
                         const char * schName,
                         Int64 tableUID);

  short checkDefaultValue(
			  const NAString & colExtName,
			  const NAType    * inColType,
			  ElemDDLColDef   * colNode);
  
  short getTypeInfo(const NAType * naType,
		    Lng32 serializedOption,
		    Lng32 &datatype,
		    Lng32 &length,
		    Lng32 &precision,
		    Lng32 &scale,
		    Lng32 &dtStart,
		    Lng32 &dtEnd,
		    Lng32 &upshifted,
		    Lng32 &nullable,
		    NAString &charset,
		    CharInfo::Collation &collationSequence,
		    ULng32 &colFlags);

  short getColInfo(ElemDDLColDef * colNode, 
		   NAString &colName,
		   Lng32 &datatype,
		   Lng32 &length,
		   Lng32 &precision,
		   Lng32 &scale,
		   Lng32 &dtStart,
		   Lng32 &dtEnd,
		   Lng32 &upshifted,
		   Lng32 &nullable,
		   NAString &charset,
		   ComColumnDefaultClass &defaultClass,
		   NAString &defVal,
		   NAString &heading,
		   LobsStorage &lobStorage,
		   ULng32 &colFlags);
  
  short createRowId(NAString &key,
		    NAString &part1, Lng32 part1MaxLen,
		    NAString &part2, Lng32 part2MaxLen,
		    NAString &part3, Lng32 part3MaxLen,
		    NAString &part4, Lng32 part4MaxLen);
  
  short existsInSeabaseMDTable(
			       ExeCliInterface *cliInterface,
			       const char * catName,
			       const char * schName,
			       const char * objName,
			       const char * objType,
			       NABoolean checkForValidDef = TRUE,
			       NABoolean checkForValidHbaseName = TRUE);
  
  Int64 getConstraintOnIndex(
			     ExeCliInterface *cliInterface,
			     Int64 btUID,
			     Int64 indexUID,
			     const char * constrType,
			     NAString &catName,
			     NAString &schName,
			     NAString &objName);
   Int64 getObjectUID(
                     ExeCliInterface *cliInterface,
                     const char * catName,
                     const char * schName,
                     const char * objName,
                     const char * inObjType,
                     char * outObjType = NULL);

   Int64 getObjectUIDandOwner(
                     ExeCliInterface *cliInterface,
                     const char * catName,
                     const char * schName,
                     const char * objName,
                     const char * inObjType,
                     char * outObjType = NULL,
		     Int32 * objectOwner = NULL,
		     NABoolean reportErrorNow = TRUE );
  
  short getBaseTable(ExeCliInterface *cliInterface,
		     const NAString &indexCatName,
		     const NAString &indexSchName,
		     const NAString &indexObjName,
		     NAString &btCatName,
		     NAString &btSchName,
		     NAString &btObjName,
		     Int64 &btUID);
  
  short getUsingObject(ExeCliInterface *cliInterface,
		       Int64 objUID,
		       NAString &usingObjName);

  short getUsingRoutine(ExeCliInterface *cliInterface,
                        Int64 objUID,
                        NAString &usingObjName);
  
  short getUsingViews(ExeCliInterface *cliInterface,
		      const NAString &usedObjName,
		      NABoolean isTable,
		      Queue * &usingViewsQueue);
  
  short updateSeabaseMDTable(
			     ExeCliInterface *cliInterface,
			     const char * catName,
			     const char * schName,
			     const char * objName,
			     const char * objType,
			     const char * validDef,
			     ComTdbVirtTableTableInfo * tableInfo,
			     Lng32 numCols,
			     const ComTdbVirtTableColumnInfo * colInfo,
			     Lng32 numKeys,
			     const ComTdbVirtTableKeyInfo * keyInfo,
			     Lng32 numIndexes,
			     const ComTdbVirtTableIndexInfo * indexInfo,
                             const Int64 inUID = -1);

  short deleteFromSeabaseMDTable(
				 ExeCliInterface *cliInterface,
				 const char * catName,
				 const char * schName,
				 const char * objName,
				 const char * objType);

  short updateSeabaseMDSPJ(
                            ExeCliInterface *cliInterface,
                            const char * catName,
                            const char * schName,
                            const char * libname,
                            const char * libPath,
                            const ComTdbVirtTableRoutineInfo * routineInfo,
                            Lng32 numCols,
                            const ComTdbVirtTableColumnInfo * colInfo);

  short deleteConstraintInfoFromSeabaseMDTables(
						ExeCliInterface *cliInterface,
						Int64 tableUID,
						Int64 otherTableUID, // valid for ref constrs
						Int64 constrUID,
						Int64 otherConstrUID, // valid for ref constrs
						const char * constrCatName,
						const char * constrSchName,
						const char * constrObjName,
						const char * constrType);

  short updateObjectName(
			 ExeCliInterface *cliInterface,
			 Int64 objUID,
			 const char * catName,
			 const char * schName,
			 const char * objName);

  short updateObjectValidDef(
			     ExeCliInterface *cliInterface,
			     const char * catName,
			     const char * schName,
			     const char * objName,
			     const char * objType,
			     const char * validDef);
  
  short updateObjectAuditAttr(
			    ExeCliInterface *cliInterface,
			    const char * catName,
			    const char * schName,
			    const char * objName,
			    NABoolean audited,
                            const NAString& objType);

  short updateTextTable(ExeCliInterface * cliInterface, Int64 objUID, NAString &text);

  short createEncodedKeysBuffer(char** &encodedKeysBuffer,
				desc_struct * colDescs, desc_struct * keyDescs,
				Lng32 numSplits, Lng32 numKeys, Lng32 keyLength);

  short validateRoutine( 
                        ExeCliInterface *cliInterface,
                        const char * className,
                        const char * methodName,
                        const char * externalPath,
                        char * signature,
                        Int32 numSqlParam,
                        Int32 maxResultSets,
                        const char * optionalSig) ;

  short populateKeyInfo(ComTdbVirtTableKeyInfo &keyInfo,
			OutputInfo * oi, NABoolean isIndex = FALSE);
  
  short dropMDTable(ExpHbaseInterface *ehi, const char * tab);
  
  short populateSeabaseIndexFromTable(
				      ExeCliInterface * cliInterface,
				      NABoolean uniqueIndex,
				      const NAString &indexName, const NAString &tableName,
				      NAList<NAString> &selColList,
				      NABoolean useLoad );
  
  short buildViewText(StmtDDLCreateView * createViewParseNode,
		      NAString &viewText);
  
  short buildViewColInfo(StmtDDLCreateView * createViewParseNode,
			 ElemDDLColDefArray * colDefArray);
  
  short buildColInfoArray(ElemDDLParamDefArray *paramArray,
                          ComTdbVirtTableColumnInfo * colInfoArray);
  
  short buildKeyInfoArray(
			  ElemDDLColDefArray *colArray,
			  ElemDDLColRefArray *keyArray,
			  ComTdbVirtTableColumnInfo * colInfoArray,
			  ComTdbVirtTableKeyInfo * keyInfoArray,
			  NABoolean allowNullableUniqueConstr,
			  NAMemory * heap = NULL);

  const char * computeCheckOption(StmtDDLCreateView * createViewParseNode);
  
  short updateViewUsage(StmtDDLCreateView * createViewParseNode,
			Int64 viewUID,
			ExeCliInterface * cliInterface);
  
  short genPKeyName(StmtDDLAddConstraintPK *addPKNode,
		    const char * catName,
		    const char * schName,
		    const char * objName,
		    NAString &pkeyName);

  short constraintErrorChecks(
			      StmtDDLAddConstraint *addConstrNode,
			      NATable * naTable,
			      ComConstraintType ct,
			      NAList<NAString> &keyColList);

  short updateConstraintMD(
			   NAList<NAString> &keyColList,
			   NAList<NAString> &keyColOrderList,
			   NAString &uniqueStr,
			   Int64 tableUID,
			   Int64 uniqueUID,
			   NATable * naTable,
			   ComConstraintType ct,
			   ExeCliInterface *cliInterface);

  short updateRIConstraintMD(
			     Int64 ringConstrUID,
			     Int64 refdConstrUID,
			     ExeCliInterface *cliInterface);

  short updatePKeyInfo(
		       StmtDDLAddConstraintPK *addPKNode,
		       const char * catName,
		       const char * schName,
		       const char * objName,
		       Lng32 numKeys,
		       Int64 * outPkeyUID,
		       Int64 *outTableUID,
		       const ComTdbVirtTableKeyInfo * keyInfoArray,
		       ExeCliInterface *cliInterface);

  short updateRIInfo(
		       StmtDDLAddConstraintRIArray &riArray,
		       const char * catName,
		       const char * schName,
		       const char * objName,
		       ExeCliInterface *cliInterface);

  short genUniqueName(StmtDDLAddConstraint *addUniqueNode,
		    NAString &pkeyName);

  short updateIndexInfo(
			NAList<NAString> &ringKeyColList,
			NAList<NAString> &ringKeyColOrderList,
			NAList<NAString> &refdKeyColList,
			NAString &uniqueStr,
			Int64 constrUID,
			const char * catName,
			const char * schName,
			const char * objName,
			NATable * naTable,
			NABoolean isUnique, // TRUE: uniq constr. FALSE: ref constr.
			NABoolean noPopulate, // TRUE, dont populate index
			ExeCliInterface *cliInterface);

  short createMetadataViews(ExeCliInterface * cliInterface);
  short dropMetadataViews(ExeCliInterface * cliInterface);
  short createSeqTable(ExeCliInterface * cliInterface);

  void createSeabaseTable(
			  StmtDDLCreateTable                  * createTableNode,
			  NAString &currCatName, NAString &currSchName);
 
  void createSeabaseTableCompound(
			  StmtDDLCreateTable                  * createTableNode,
			  NAString &currCatName, NAString &currSchName);
 
  void createSeabaseTableLike(
			      StmtDDLCreateTable                  * createTableNode,
			      NAString &currCatName, NAString &currSchName);
  
  void dropSeabaseTable(
			StmtDDLDropTable                  * dropTableNode,
			NAString &currCatName, NAString &currSchName);
  
  void createSeabaseIndex(
			  StmtDDLCreateIndex                  * createIndexNode,
			  NAString &currCatName, NAString &currSchName);
  
  void populateSeabaseIndex(
			    StmtDDLPopulateIndex                  * populateIndexNode,
			    NAString &currCatName, NAString &currSchName);
  
  void dropSeabaseIndex(
			StmtDDLDropIndex                  * dropIndexNode,
			NAString &currCatName, NAString &currSchName);

  void renameSeabaseTable(
			  StmtDDLAlterTableRename                  * renameTableNode,
			  NAString &currCatName, NAString &currSchName);

  void addConstraints(
		      ComObjectName &tableName,
		      ComAnsiNamePart &currCatAnsiName,
		      ComAnsiNamePart &currSchAnsiName,
		      StmtDDLAddConstraintPK * pkConstr,
		      StmtDDLAddConstraintUniqueArray &uniqueConstrArr,
		      StmtDDLAddConstraintRIArray &riConstrArr,
		      StmtDDLAddConstraintCheckArray &checkConstrArr);
  
  void alterSeabaseTableAddColumn(
				  StmtDDLAlterTableAddColumn * alterAddColNode,
				  NAString &currCatName, NAString &currSchName);
  
  void alterSeabaseTableDropColumn(
				   StmtDDLAlterTableDropColumn * alterDropColNode,
				   NAString &currCatName, NAString &currSchName);
  
  void alterSeabaseTableAddPKeyConstraint(
					  StmtDDLAddConstraint * alterAddConstraint,
					  NAString &currCatName, NAString &currSchName);
  
  void alterSeabaseTableAddUniqueConstraint(
					    StmtDDLAddConstraint * alterAddConstraint,
					    NAString &currCatName, NAString &currSchName);
  
  short isCircularDependent(
			    CorrName &ringTable,
			    CorrName &refdTable,
			    CorrName &origRingTable,
			    BindWA *bindWA);

  void alterSeabaseTableAddRIConstraint(
					StmtDDLAddConstraint * alterAddConstraint,
					NAString &currCatName, NAString &currSchName);

  short getCheckConstraintText(StmtDDLAddConstraintCheck *addCheckNode,
			       NAString &checkConstrText);

  short getTextFromMD(
		      ExeCliInterface * cliInterface,
		      Int64 constrUID,
		      NAString &constrText);
    
  void alterSeabaseTableAddCheckConstraint(
					StmtDDLAddConstraint * alterAddConstraint,
					NAString &currCatName, NAString &currSchName);

  void alterSeabaseTableDropConstraint(
				  StmtDDLDropConstraint * alterDropConstraint,
				  NAString &currCatName, NAString &currSchName);

  void alterSeabaseTableDisableOrEnableIndex(
					     ExprNode * ddlNode,
					     NAString &currCatName, NAString &currSchName);

  void alterSeabaseTableDisableOrEnableAllIndexes(
                                               ExprNode * ddlNode,
                                               NAString &currCatName,
                                               NAString &currSchName,
                                               NAString &tableName);
  short alterSeabaseTableDisableOrEnableIndex(
                                              const char * catName,
                                              const char * schName,
                                              const char * idxName,
                                              const char * tabName,
                                               NABoolean isDisable);

  void createSeabaseView(
			 StmtDDLCreateView                  * createViewNode,
			 NAString &currCatName, NAString &currSchName);
  
  void dropSeabaseView(
		       StmtDDLDropView                  * dropViewNode,
		       NAString &currCatName, NAString &currSchName);

  void createSeabaseLibrary(StmtDDLCreateLibrary  * createLibraryNode,
                            NAString &currCatName, NAString &currSchName);
  
  void registerSeabaseUser   (StmtDDLRegisterUser  * registerUserNode);
  void unregisterSeabaseUser (StmtDDLRegisterUser  * registerUserNode);
  void alterSeabaseUser      (StmtDDLAlterUser     * registerUserNode);

  void dropSeabaseLibrary(StmtDDLDropLibrary  * dropLibraryNode,
                          NAString &currCatName, NAString &currSchName);

  void createSeabaseRoutine(StmtDDLCreateRoutine  * createRoutineNode,
                            NAString &currCatName, NAString &currSchName);
  
  void dropSeabaseRoutine(StmtDDLDropRoutine  * dropRoutineNode,
                          NAString &currCatName, NAString &currSchName);

  void createSeabaseSequence(StmtDDLCreateSequence  * createSequenceNode,
			     NAString &currCatName, NAString &currSchName);

  void alterSeabaseSequence(StmtDDLCreateSequence  * alterSequenceNode,
			     NAString &currCatName, NAString &currSchName);
  
  void dropSeabaseSequence(StmtDDLDropSequence  * dropSequenceNode,
			   NAString &currCatName, NAString &currSchName);
  
  void seabaseGrantRevoke(
			  StmtDDLNode                  * stmtDDLNode,
			  NABoolean isGrant,
			  NAString &currCatName, NAString &currSchName);
  
  void dropSeabaseSchema(
			 StmtDDLDropSchema                  * dropSchemaNode,
			 NAString &currCatName, NAString &currSchName);

 void createNativeHbaseTable(
			     StmtDDLCreateHbaseTable                  * createTableNode,
			     NAString &currCatName, NAString &currSchName);

 void dropNativeHbaseTable(
			     StmtDDLDropHbaseTable                  * createTableNode,
			     NAString &currCatName, NAString &currSchName);
  
  void initSeabaseMD();
  void dropSeabaseMD();
  void createSeabaseMDviews();
  void dropSeabaseMDviews();
  void createSeabaseSeqTable();

  short dropSeabaseObjectsFromHbase(const char * pattern);
  short updateSeabaseAuths(ExeCliInterface * cliInterface, const char * sysCat);

  void purgedataHbaseTable(DDLExpr * ddlExpr,
			   NAString &currCatName, NAString &currSchName);

  short updateSeabaseVersions(ExeCliInterface * cliInterface, const char * sysCat,
			      Lng32 majorVersion = -1);

  desc_struct * getSeabaseMDTableDesc(const NAString &catName, 
				      const NAString &schName, 
				      const NAString &objName,
				      const char * objType);

  desc_struct * getSeabaseHistTableDesc(const NAString &catName, 
					const NAString &schName, 
					const NAString &objName);

  desc_struct * getSeabaseSequenceDesc(const NAString &catName, 
				       const NAString &schName, 
				       const NAString &seqName);
    
  Lng32 getSeabaseColumnInfo(ExeCliInterface *cliInterface,
                                   Int64 objUID,
                                   char *direction,
                                   NABoolean *tableIsSalted,
                                   Lng32 *numCols,
                                   ComTdbVirtTableColumnInfo **colInfoArray);
 
  desc_struct * getSeabaseUserTableDesc(const NAString &catName, 
					const NAString &schName, 
					const NAString &objName,
					const char * objType,
					NABoolean includeInvalidDefs);
 
  static NABoolean getMDtableInfo(const NAString &objName,
				  Lng32 &colInfoSize,
				  const ComTdbVirtTableColumnInfo* &colInfo,
				  Lng32 &keyInfoSize,
				  const ComTdbVirtTableKeyInfo* &keyInfo,
				  Lng32 &indexInfoSize,
				  const ComTdbVirtTableIndexInfo* &indexInfo,
				  const char * objType);

  desc_struct * assembleRegionDescs(ByteArrayList* bal, desc_nodetype format);

  void glueQueryFragments(Lng32 queryArraySize,
			  const QString * queryArray,
			  char * &gluedQuery,
			  Lng32 &gluedQuerySize);

  short convertColAndKeyInfoArrays(
				    Lng32 btNumCols, // IN
				    ComTdbVirtTableColumnInfo* btColInfoArray, // IN
				    Lng32 btNumKeys, // IN
				    ComTdbVirtTableKeyInfo* btKeyInfoArray, // IN
				    NAColumnArray *naColArray,
				    NAColumnArray *naKeyArr);

  short processDDLandCreateDescs(
				 Parser &parser,
				 const QString *ddl,
				 Lng32 sizeOfddl,

				 NABoolean isIndexTable,

				 Lng32 btNumCols, // IN
				 ComTdbVirtTableColumnInfo* btColInfoArray, // IN
				 Lng32 btNumKeys, // IN
				 ComTdbVirtTableKeyInfo* btKeyInfoArray, // IN
    
				 Lng32 &numCols,
				 ComTdbVirtTableColumnInfo* &colInfoArray,
				 Lng32 &numKeys,
				 ComTdbVirtTableKeyInfo* &keyInfoArray,
				 ComTdbVirtTableIndexInfo* &indexInfo);

  short createIndexColAndKeyInfoArrays(
				       ElemDDLColRefArray & indexColRefArray,
				       NABoolean isUnique,
				       NABoolean hasSyskey,
				       const NAColumnArray &baseTableNAColArray,
				       const NAColumnArray &baseTableKeyArr,
				       Lng32 &keyColCount,
				       Lng32 &nonKeyColCount,
				       Lng32 &totalColCount,
				       ComTdbVirtTableColumnInfo * &colInfoArray,
				       ComTdbVirtTableKeyInfo * &keyInfoArray,
				       NAList<NAString> &selColList,
				       NAMemory * heap);
 private:
  enum
  {
    NUM_MAX_PARAMS = 20
  };

  NAHeap *heap_;
  ULng32 savedCmpParserFlags_;
  ULng32 savedCliParserFlags_;

  NAString seabaseSysCat_;

  const char * param_[NUM_MAX_PARAMS];

};

#endif // _CMP_SEABASE_DDL_H_