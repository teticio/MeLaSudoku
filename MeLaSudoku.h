#ifndef _ME_LA_SUDOKU_
#define _ME_LA_SUDOKU_

#include <stdio.h>
#include <time.h>

//------------------------------------------------------------------------------------//

class MeLaSudoku
{
public:
	void InitGrid(int **ppiProblem = NULL)
	{
		int i, j;
        
		m_bIsImpossible = 0;
		m_bIsNotUnique = 0;
		m_iNumSolved = 0;
		for (i = 0; i < m_iNumRows; i++) {
			for (j = 0; j < m_iNumColumns; j++) {
				m_ppiSolution[i][j] = 0;
				SetSolution(i, j, ppiProblem[i][j]);
			}
		}
	}
    
	MeLaSudoku(int iSize, int **ppiProblem)
	{
		int i, j, k;

		m_iNumSolved = 0;
		m_bDump = 0;
		m_iSize = iSize;
		m_iNumRows = m_iNumColumns = m_iNumNums = m_iNumBlocks = m_iSize*m_iSize;
		m_pppiGrid = new int**[m_iNumRows];
		m_ppiSolution = new int*[m_iNumRows];
		for (i = 0; i < m_iNumRows; i++) {
			m_pppiGrid[i] = new int*[m_iNumColumns];
			m_ppiSolution[i] = new int[m_iNumColumns];
			for (j = 0; j < m_iNumColumns; j++) {
				m_pppiGrid[i][j] = new int[m_iNumNums];
				for (k = 0; k < m_iNumNums; k++) {
					m_pppiGrid[i][j][k] = 1;
				}
			}
		}
		if (ppiProblem) {
			InitGrid(ppiProblem);
		}
	}

	MeLaSudoku(const MeLaSudoku &b)
	{
		int i, j, k;

		m_bIsImpossible = b.m_bIsImpossible;
		m_bIsNotUnique = b.m_bIsNotUnique;
		m_iNumSolved = b.m_iNumSolved;
		m_bDump = b.m_bDump;
		m_iSize = b.m_iSize;
		m_iNumRows = b.m_iNumRows;
		m_iNumColumns = b.m_iNumColumns;
		m_iNumNums = b.m_iNumNums;
		m_iNumBlocks = b.m_iNumBlocks;
		m_pppiGrid = new int**[m_iNumRows];
		m_ppiSolution = new int*[m_iNumRows];
		for (i = 0; i < m_iNumRows; i++) {
			m_pppiGrid[i] = new int*[m_iNumColumns];
			m_ppiSolution[i] = new int[m_iNumColumns];
			for (j = 0; j < m_iNumColumns; j++) {
				m_ppiSolution[i][j] = b.m_ppiSolution[i][j];
				m_pppiGrid[i][j] = new int[m_iNumNums];
				for (k = 0; k < m_iNumNums; k++) {
					m_pppiGrid[i][j][k] = b.m_pppiGrid[i][j][k];
				}
			}
		}
	}

	virtual ~MeLaSudoku()
	{
		int i, j;

		for (i = 0; i < m_iNumRows; i++) {
			for (j = 0; j < m_iNumColumns; j++) {
				delete [] m_pppiGrid[i][j];
			}
			delete [] m_pppiGrid[i];
			delete [] m_ppiSolution[i];
		}
		delete [] m_pppiGrid;
		delete [] m_ppiSolution;
	}

	void ResetGrid()
	{
		int i, j, k;

		m_bIsImpossible = 0;
		m_bIsNotUnique = 0;
		m_iNumSolved = 0;
		for (i = 0; i < m_iNumRows; i++) {
			for (j = 0; j < m_iNumColumns; j++) {
				if (m_ppiSolution[i][j] == 0) {
					for (k = 0; k < m_iNumNums; k++) {
						m_pppiGrid[i][j][k] = 1;
					}
				} else {
					m_iNumSolved++;
					for (k = 0; k < m_iNumNums; k++) {
						m_pppiGrid[i][j][k] = 0;
					}
					m_pppiGrid[i][j][m_ppiSolution[i][j]-1] = 1;
				}
			}
		}
	}

	void SetSolution(int i, int j, int iNumber)
	{
		int k;

		if (m_ppiSolution[i][j] != 0) {
			return;
		}
		if (iNumber != 0) {
			m_iNumSolved++;
		} else {
			if (m_ppiSolution[i][j] != 0) {
				m_iNumSolved--;
			}
		}
		m_ppiSolution[i][j] = iNumber;
		if (m_ppiSolution[i][j] == 0) {
			for (k = 0; k < m_iNumNums; k++) {
				m_pppiGrid[i][j][k] = 1;
			}
		} else {
			for (k = 0; k < m_iNumNums; k++) {
				m_pppiGrid[i][j][k] = 0;
			}
			m_pppiGrid[i][j][m_ppiSolution[i][j]-1] = 1;
		}
	}

	void Dump()
	{
		int i, j, k;

		for (i = 0; i < m_iNumRows; i++) {
			if (i % m_iSize == 0) {
				for (j = 0; j < m_iSize; j++) {
					printf("+-");
					for (k = 0; k < m_iSize; k++) {
						printf("--");
					}
				}
				printf("+\n");
			}
			for (j = 0; j < m_iNumColumns; j++) {
				if (j % m_iSize == 0) {
					printf("| ");
				}
				if (m_ppiSolution[i][j] == 0) {
					printf("  ");
				} else {
					if (m_iSize <= 3) {
						printf("%d ", m_ppiSolution[i][j]);
					} else {
						printf("%c ", (char)m_ppiSolution[i][j]+'A'-1);
					}
				}
			}
			printf("|\n");
		}
		for (j = 0; j < m_iSize; j++) {
			printf("+-");
			for (k = 0; k < m_iSize; k++) {
				printf("--");
			}
		}
		printf("+\n");
	}

	void SetDump(int bDump) { m_bDump = bDump; }
	void SetIsImpossible(int bIsImpossible) { m_bIsImpossible = bIsImpossible; }
	void SetIsNotUnique(int bIsNotUnique) { m_bIsNotUnique = bIsNotUnique; }
	void SetIsPossible(int i, int j, int iNumber, int bYes) { m_pppiGrid[i][j][iNumber-1] = bYes; }
	int GetDump() { return m_bDump; }
	int IsPossible(int i, int j, int iNumber) { return m_pppiGrid[i][j][iNumber-1]; }
	int GetSolution(int i, int j) { return m_ppiSolution[i][j]; }
	int GetSize() { return m_iSize; }
	int GetNumRows() { return m_iNumRows; }
	int GetNumColumns() { return m_iNumColumns; }
	int GetNumNums() { return m_iNumNums; }
	int GetNumBlocks() { return m_iNumBlocks; }
	int GetNumSolved() { return m_iNumSolved; }
	int IsSolved() { return m_iNumSolved == m_iNumRows*m_iNumColumns; }
	int IsImpossible() { return m_bIsImpossible; }
	int IsNotUnique() { return m_bIsNotUnique; }

protected:
	int m_iSize;
	int m_iNumRows;
	int m_iNumColumns;
	int m_iNumNums;
	int m_iNumBlocks;
	int ***m_pppiGrid;
	int **m_ppiSolution;
	int m_iNumSolved;
	int m_bIsImpossible;
	int m_bIsNotUnique;
	int m_bDump;
};

//------------------------------------------------------------------------------------//

class Iterator {
public:
	Iterator(MeLaSudoku *pMeLaSudoku)
	{
		m_pMeLaSudoku = pMeLaSudoku;
		m_iColumn = 0;
		m_iRow = 0;
	}

	virtual ~Iterator()
	{
	}

	virtual void Reset() = 0;
	virtual void Next() = 0;
	virtual int IsEnd() = 0;

	void SetIsPossible(int iNumber, int bYes) { m_pMeLaSudoku->SetIsPossible(m_iRow, m_iColumn, iNumber, bYes); }
	void SetSolution(int iNumber) { m_pMeLaSudoku->SetSolution(m_iRow, m_iColumn, iNumber); }

	int IsPossible(int iNumber) { return m_pMeLaSudoku->IsPossible(m_iRow, m_iColumn, iNumber); }
	int GetSolution() { return m_pMeLaSudoku->GetSolution(m_iRow, m_iColumn); }
	int GetRow() { return m_iRow; }
	int GetColumn() { return m_iColumn; }
	int GetBlock() { return m_pMeLaSudoku->GetSize() * (m_iRow / m_pMeLaSudoku->GetSize()) + m_iColumn / m_pMeLaSudoku->GetSize(); } 
	int InSameColumn(Iterator *pIterator) { return pIterator->GetColumn() == GetColumn(); }
	int InSameRow(Iterator *pIterator) { return pIterator->GetRow() == GetRow(); }
	int InSameBlock(Iterator *pIterator) { return pIterator->GetBlock() == GetBlock(); }
	MeLaSudoku *GetMeLaSudoku() { return m_pMeLaSudoku; }

	virtual void Dump() = 0;
	virtual Iterator *Clone() = 0;

protected:
	int m_iColumn;
	int m_iRow;
	MeLaSudoku *m_pMeLaSudoku;
};

//------------------------------------------------------------------------------------//

class ColumnIterator : public Iterator {
public:
	ColumnIterator(MeLaSudoku *pMeLaSudoku, int iRow)
		: Iterator(pMeLaSudoku)
	{
		m_iRow = iRow;
	}

	virtual ~ColumnIterator()
	{
	}

	virtual void Reset() { m_iColumn = 0; }
	virtual void Next() { m_iColumn++; }
	virtual int IsEnd() { return m_iColumn == m_pMeLaSudoku->GetNumColumns(); }

	virtual void Dump()
	{
		printf("row %d", m_iRow+1);
	}

	virtual Iterator *Clone() { return (Iterator*)new ColumnIterator(*this); }
};

//------------------------------------------------------------------------------------//

class RowIterator : public Iterator {
public:
	RowIterator(MeLaSudoku *pMeLaSudoku, int iColumn)
		: Iterator(pMeLaSudoku)
	{
		m_iColumn = iColumn;
	}

	virtual ~RowIterator()
	{
	}

	virtual void Reset() { m_iRow = 0; }
	virtual void Next() { m_iRow++; }
	virtual int IsEnd() { return m_iRow == m_pMeLaSudoku->GetNumRows(); }

	virtual void Dump()
	{
		printf("column %d", m_iColumn+1);
	}

	virtual Iterator *Clone() { return (Iterator*)new RowIterator(*this); }
};

//------------------------------------------------------------------------------------//

class BlockIterator : public Iterator {
public:
	BlockIterator(MeLaSudoku *pMeLaSudoku, int iBlock)
		: Iterator(pMeLaSudoku)
	{
		m_iBlock = iBlock;
		m_iStartColumn = m_pMeLaSudoku->GetSize() * (iBlock % m_pMeLaSudoku->GetSize());
		m_iStartRow = m_pMeLaSudoku->GetSize() * (iBlock / m_pMeLaSudoku->GetSize());
		m_iColumn = m_iStartColumn;
		m_iRow = m_iStartRow;
		m_bEnd = 0;
	}

	virtual ~BlockIterator()
	{
	}

	virtual void Reset() { m_iColumn = m_iStartColumn; m_iRow = m_iStartRow; m_bEnd = 0; }

	virtual void Next()
	{
		m_bEnd = 0;
		m_iColumn++;
		if (m_iColumn >= m_iStartColumn+m_pMeLaSudoku->GetSize()) {
			m_iColumn = m_iStartColumn;
			m_iRow++;
			if (m_iRow >= m_iStartRow+m_pMeLaSudoku->GetSize()) {
				m_bEnd = 1;
			}
		}
	}

	virtual int IsEnd() { return m_bEnd; }

	virtual void Dump()
	{
		printf("block %d", m_iBlock+1);
	}

	virtual Iterator *Clone() { return (Iterator*)new BlockIterator(*this); }

private:
	int m_iBlock;
	int m_iStartColumn;
	int m_iStartRow;
	int m_bEnd;
};

//------------------------------------------------------------------------------------//

class Rule {
public:
	Rule()
	{
		m_bDidSomething = 0;
		m_bIsImpossible = 0;
		m_bIsNotUnique = 0;
	}

	virtual ~Rule()
	{
	}

	virtual void Apply(Iterator *pIterator) = 0;
	virtual int AppliesToColumns() = 0;
	virtual int AppliesToRows() = 0;
	virtual int AppliesToBlocks() = 0;
	int DidSomething() { if (m_bDidSomething) { m_bDidSomething = 0; return 1; } else return 0; }
	int IsImpossible() { if (m_bIsImpossible) { m_bIsImpossible = 0; return 1; } else return 0; }
	int IsNotUnique() { if (m_bIsNotUnique) { m_bIsNotUnique = 0; return 1; } else return 0; }

protected:
	int m_bDidSomething;
	int m_bIsImpossible;
	int m_bIsNotUnique;
};

//------------------------------------------------------------------------------------//

class IfNotPossibleElsewhereRule : public Rule
{
public:
	IfNotPossibleElsewhereRule()
	{
	}

	virtual ~IfNotPossibleElsewhereRule()
	{
	}

	virtual void Apply(Iterator *pIterator)
	{
		int iNumber;
		int bFound;
		Iterator *pCopyOfIterator = pIterator->Clone();
		
		for (pCopyOfIterator->Reset(); !pCopyOfIterator->IsEnd(); pCopyOfIterator->Next()) {
			if (pCopyOfIterator->GetSolution() != 0) {
				continue;
			}
			for (iNumber = 1; iNumber < pIterator->GetMeLaSudoku()->GetNumNums()+1; iNumber++) {
				if (!pCopyOfIterator->IsPossible(iNumber)) {
					continue;
				}
				bFound = 0;
				for (pIterator->Reset(); !pIterator->IsEnd(); pIterator->Next()) {
					if ((!pIterator->InSameColumn(pCopyOfIterator) || !pIterator->InSameRow(pCopyOfIterator)) &&
						pIterator->IsPossible(iNumber)) {
						bFound = 1;
						break;
					}
				}
				if (!bFound) {
					if (pIterator->GetMeLaSudoku()->GetDump()) {
						printf("cell (%d, %d) must be %d because %d is not possible anywhere else in ", pCopyOfIterator->GetRow()+1, pCopyOfIterator->GetColumn()+1, iNumber, iNumber);
						pIterator->Dump();
						printf("\n");
					}
					pCopyOfIterator->SetSolution(iNumber);
					m_bDidSomething = 1;
				}
			}
		}

		delete pCopyOfIterator;
	}

	virtual int AppliesToColumns() { return 1; }
	virtual int AppliesToRows() { return 1; }
	virtual int AppliesToBlocks() { return 1; }
};

//------------------------------------------------------------------------------------//

class IfAppearsElsewhereRule : public Rule
{
public:
	IfAppearsElsewhereRule()
	{
	}

	virtual ~IfAppearsElsewhereRule()
	{
	}

	virtual void Apply(Iterator *pIterator)
	{
		int iNumber;
		Iterator *pCopyOfIterator = pIterator->Clone();
		
		for (pCopyOfIterator->Reset(); !pCopyOfIterator->IsEnd(); pCopyOfIterator->Next()) {
			if (pCopyOfIterator->GetSolution() != 0) {
				continue;
			}
			for (iNumber = 1; iNumber < pIterator->GetMeLaSudoku()->GetNumNums()+1; iNumber++) {
				if (!pCopyOfIterator->IsPossible(iNumber)) {
					continue;
				}
				for (pIterator->Reset(); !pIterator->IsEnd(); pIterator->Next()) {
					if ((!pIterator->InSameColumn(pCopyOfIterator) || !pIterator->InSameRow(pCopyOfIterator)) &&
						pIterator->GetSolution() == iNumber) {
						if (pIterator->GetMeLaSudoku()->GetDump()) {
							printf("%d is not possible in cell (%d, %d) because it appears elsewhere in ", iNumber, pCopyOfIterator->GetRow()+1, pCopyOfIterator->GetColumn()+1);
							pIterator->Dump();
							printf("\n");
						}
						pCopyOfIterator->SetIsPossible(iNumber, 0);
						m_bDidSomething = 1;
					}
				}
			}
		}

		delete pCopyOfIterator;
	}

	virtual int AppliesToColumns() { return 1; }
	virtual int AppliesToRows() { return 1; }
	virtual int AppliesToBlocks() { return 1; }
};

//------------------------------------------------------------------------------------//

class IfOnlyPossibleInRowOrColumnInBlockRule : public Rule
{
public:
	IfOnlyPossibleInRowOrColumnInBlockRule()
	{
	}

	virtual ~IfOnlyPossibleInRowOrColumnInBlockRule()
	{
	}

	virtual void Apply(Iterator *pIterator)
	{
		int iNumber;
		int bColumn, bRow;
		Iterator *pCopyOfIterator = pIterator->Clone();
		
		for (pCopyOfIterator->Reset(); !pCopyOfIterator->IsEnd(); pCopyOfIterator->Next()) {
			if (pCopyOfIterator->GetSolution() != 0) {
				continue;
			}
			for (iNumber = 1; iNumber < pIterator->GetMeLaSudoku()->GetNumNums()+1; iNumber++) {
				if (!pCopyOfIterator->IsPossible(iNumber)) {
					continue;
				}
				bColumn = bRow = 1;
				for (pIterator->Reset(); !pIterator->IsEnd(); pIterator->Next()) {
					if (!pIterator->IsPossible(iNumber)) {
						continue;
					}
					if (!pIterator->InSameColumn(pCopyOfIterator)) {
						bColumn = 0;
					}
					if (!pIterator->InSameRow(pCopyOfIterator)) {
						bRow = 0;
					}
					if (bColumn == 0 && bRow == 0) {
						break;
					}
				}
				if (bRow) {
					ColumnIterator columnIterator(pCopyOfIterator->GetMeLaSudoku(), pCopyOfIterator->GetRow());

					for (columnIterator.Reset(); !columnIterator.IsEnd(); columnIterator.Next()) {
						if (columnIterator.IsPossible(iNumber) && !columnIterator.InSameBlock(pCopyOfIterator)) {
							if (pIterator->GetMeLaSudoku()->GetDump()) {
								printf("%d is not possible in cell (%d, %d) because it can only be in the same row in ", iNumber, columnIterator.GetRow()+1, columnIterator.GetColumn()+1);
								pIterator->Dump();
								printf("\n");
							}
							columnIterator.SetIsPossible(iNumber, 0);
							m_bDidSomething = 1;
						}
					}
				}
				if (bColumn) {
					RowIterator rowIterator(pCopyOfIterator->GetMeLaSudoku(), pCopyOfIterator->GetColumn());

					for (rowIterator.Reset(); !rowIterator.IsEnd(); rowIterator.Next()) {
						if (rowIterator.IsPossible(iNumber) && !rowIterator.InSameBlock(pCopyOfIterator)) {
							if (pIterator->GetMeLaSudoku()->GetDump()) {
								printf("%d is not possible in cell (%d, %d) because it can only be in the same column in ", iNumber, rowIterator.GetRow()+1, rowIterator.GetColumn()+1);
								pIterator->Dump();
								printf("\n");
							}
							rowIterator.SetIsPossible(iNumber, 0);
							m_bDidSomething = 1;
						}
					}
				}
			}
		}

		delete pCopyOfIterator;
	}

	virtual int AppliesToColumns() { return 0; }
	virtual int AppliesToRows() { return 0; }
	virtual int AppliesToBlocks() { return 1; }
};

//------------------------------------------------------------------------------------//

class IfOnlyPossibleInBlockInRowOrColumnRule : public Rule
{
public:
	IfOnlyPossibleInBlockInRowOrColumnRule()
	{
	}

	virtual ~IfOnlyPossibleInBlockInRowOrColumnRule()
	{
	}

	virtual void Apply(Iterator *pIterator)
	{
		int iNumber;
		int bFound;
		Iterator *pCopyOfIterator = pIterator->Clone();
		
		for (pCopyOfIterator->Reset(); !pCopyOfIterator->IsEnd(); pCopyOfIterator->Next()) {
			if (pCopyOfIterator->GetSolution() != 0) {
				continue;
			}
			for (iNumber = 1; iNumber < pIterator->GetMeLaSudoku()->GetNumNums()+1; iNumber++) {
				if (!pCopyOfIterator->IsPossible(iNumber)) {
					continue;
				}
				bFound = 0;
				for (pIterator->Reset(); !pIterator->IsEnd(); pIterator->Next()) {
					if (pIterator->IsPossible(iNumber) && !pIterator->InSameBlock(pCopyOfIterator)) {
						bFound = 1;
						break;
					}
				}
				if (!bFound) {
					// all the possibles are in the same block in this row / column
					// so we may eliminate them from elsewhere in the block
					BlockIterator blockIterator(pCopyOfIterator->GetMeLaSudoku(), pCopyOfIterator->GetBlock());

					for (blockIterator.Reset(); !blockIterator.IsEnd(); blockIterator.Next()) {
						if (!blockIterator.IsPossible(iNumber)) {
							continue;
						}
						if ((dynamic_cast<RowIterator*>(pCopyOfIterator) && !pCopyOfIterator->InSameColumn(&blockIterator)) ||
							(dynamic_cast<ColumnIterator*>(pCopyOfIterator) && !pCopyOfIterator->InSameRow(&blockIterator))) {
							blockIterator.SetIsPossible(iNumber, 0);
							m_bDidSomething = 1;
							if (pIterator->GetMeLaSudoku()->GetDump()) {
								printf("%d is not possible in cell (%d, %d) because it must be in ", iNumber, blockIterator.GetRow()+1, blockIterator.GetColumn()+1);
								pCopyOfIterator->Dump();
								printf(" in ");
								blockIterator.Dump();
								printf("\n");
							}
						}
					}
				}
			}
		}

		delete pCopyOfIterator;
	}

	virtual int AppliesToColumns() { return 1; }
	virtual int AppliesToRows() { return 1; }
	virtual int AppliesToBlocks() { return 0; }
};

//------------------------------------------------------------------------------------//

class PairsTriplesEtcRule : public Rule
{
public:
	PairsTriplesEtcRule(int iLevel = -1)
	{
		m_iLevel = iLevel;
	}

	virtual ~PairsTriplesEtcRule()
	{
	}

	virtual void Apply(Iterator *pIterator)
	{
		int iNumber;
		int iCount, iCount2, bFound;
		Iterator *pCopyOfIterator = pIterator->Clone();
		
		for (pCopyOfIterator->Reset(); !pCopyOfIterator->IsEnd(); pCopyOfIterator->Next()) {
			if (pCopyOfIterator->GetSolution() != 0) {
				continue;
			}
			// count how many possibles
			iCount = 0;
			for (iNumber = 1; iNumber < pIterator->GetMeLaSudoku()->GetNumNums()+1; iNumber++) {
				if (pCopyOfIterator->IsPossible(iNumber)) {
					iCount++;
				}
			}
			if (iCount == 1) {
				continue;
			}
			// count how many cells only have the same (or fewer) possibles
			iCount2 = 0;
			for (pIterator->Reset(); !pIterator->IsEnd(); pIterator->Next()) {
				if (pIterator->InSameColumn(pCopyOfIterator) && pIterator->InSameRow(pCopyOfIterator)) {
					continue;
				}
				bFound = 1;
				for (iNumber = 1; iNumber < pIterator->GetMeLaSudoku()->GetNumNums()+1; iNumber++) {
					if (!pCopyOfIterator->IsPossible(iNumber) && pIterator->IsPossible(iNumber)) {
						bFound = 0; // this cell has a possible not in the set
						break;
					}
				}
				if (bFound) {
					iCount2++;
				}
			}
			// if there are the same number of cells as possibles, then they are "self sufficient"
			// and we can eliminate them from any cells in which other possibles appear
			if ((m_iLevel < 0 || iCount <= m_iLevel) /*&& iCount > 1*/ && iCount2 == iCount-1) { // and less than limit
				for (pIterator->Reset(); !pIterator->IsEnd(); pIterator->Next()) {
					bFound = 1;
					for (iNumber = 1; iNumber < pIterator->GetMeLaSudoku()->GetNumNums()+1; iNumber++) {
						if (!pCopyOfIterator->IsPossible(iNumber) && pIterator->IsPossible(iNumber)) {
							bFound = 0; // this cell has a possible not in the set
							break;
						}
					}
					if (!bFound) {
						for (iNumber = 1; iNumber < pIterator->GetMeLaSudoku()->GetNumNums()+1; iNumber++) {
							if (pCopyOfIterator->IsPossible(iNumber) && pIterator->IsPossible(iNumber)) {
								pIterator->SetIsPossible(iNumber, 0);
								m_bDidSomething = 1;
								if (pIterator->GetMeLaSudoku()->GetDump()) {
									int iNumber2;
									printf("%d is not possible in cell (%d, %d) because the set ", iNumber, pIterator->GetRow()+1, pIterator->GetColumn()+1);
									printf("[");
									for (iNumber2 = 1; iNumber2 < pIterator->GetMeLaSudoku()->GetNumNums()+1; iNumber2++) {
										if (pCopyOfIterator->IsPossible(iNumber2)) {
											printf("%d ", iNumber2);
										}
									}
									printf("] is self sufficient in ");
									pIterator->Dump();
									printf("\n");
								}
							}
						}
					}
				}
			}
		}

		delete pCopyOfIterator;
	}

	virtual int AppliesToColumns() { return 1; }
	virtual int AppliesToRows() { return 1; }
	virtual int AppliesToBlocks() { return 1; }

protected:
	int m_iLevel;
};

//------------------------------------------------------------------------------------//

class PairsTriplesEtcRule2 : public PairsTriplesEtcRule
{
public:
	PairsTriplesEtcRule2(int iLevel = -1)
		: PairsTriplesEtcRule(iLevel)
	{
	}

	virtual ~PairsTriplesEtcRule2()
	{
	}

	virtual void Apply(Iterator *pIterator)
	{
		int iNumber, iNumber2;
		int iCount, iCount2, bFound;
		Iterator *pCopyOfIterator = pIterator->Clone();
		
		for (pCopyOfIterator->Reset(); !pCopyOfIterator->IsEnd(); pCopyOfIterator->Next()) {
			if (pCopyOfIterator->GetSolution() != 0) {
				continue;
			}
			for (iNumber = 1; iNumber < pIterator->GetMeLaSudoku()->GetNumNums()+1; iNumber++) {
				if (!pCopyOfIterator->IsPossible(iNumber)) {
					continue;
				}
				// count in how many other cells possible
				iCount = 0;
				for (pIterator->Reset(); !pIterator->IsEnd(); pIterator->Next()) {
					if (pIterator->InSameColumn(pCopyOfIterator) && pIterator->InSameRow(pCopyOfIterator)) {
						continue;
					}
					if (pIterator->IsPossible(iNumber)) {
						iCount++;
					}
				}
				if (iCount == 1) {
					continue;
				}
				// count how many numbers only possible in same cells (or subset of)
				iCount2 = 0;
				for (iNumber2 = 1; iNumber2 < pIterator->GetMeLaSudoku()->GetNumNums()+1; iNumber2++) {
					if (iNumber2 == iNumber || !pCopyOfIterator->IsPossible(iNumber2)) {
						continue;
					}
					bFound = 1;
					for (pIterator->Reset(); !pIterator->IsEnd(); pIterator->Next()) {
						if (!pIterator->IsPossible(iNumber2) || pIterator->IsPossible(iNumber) ||
							(pIterator->InSameColumn(pCopyOfIterator) && pIterator->InSameRow(pCopyOfIterator))) {
							continue;
						}
						if (pIterator->IsPossible(iNumber2)) {
							bFound = 0;
							break;
						}
					}
					if (bFound) {
						iCount2++;
					}
				}
				if ((m_iLevel < 0 || iCount+1 <= m_iLevel) && iCount2 == iCount+1)
				{
					// if there are the same number of cells as possibles, then they are "self sufficient"
					// and we can eliminate other possiblities from these cells
					iCount2 = 0;
					for (iNumber2 = 1; iNumber2 < pIterator->GetMeLaSudoku()->GetNumNums()+1; iNumber2++) {
						if (iNumber2 == iNumber || !pCopyOfIterator->IsPossible(iNumber2)) {
							continue;
						}
						bFound = 1;
						for (pIterator->Reset(); !pIterator->IsEnd(); pIterator->Next()) {
							if (!pIterator->IsPossible(iNumber2) || pIterator->IsPossible(iNumber) ||
								(pIterator->InSameColumn(pCopyOfIterator) && pIterator->InSameRow(pCopyOfIterator))) {
								continue;
							}
							if (pIterator->IsPossible(iNumber2)) {
								bFound = 0;
								break;
							}
						}
						if (!bFound) {
							pCopyOfIterator->SetIsPossible(iNumber2, 0);
							m_bDidSomething = 1;
							if (pIterator->GetMeLaSudoku()->GetDump()) {
								int iNumber3;
								printf("%d is not possible in cell (%d, %d) because the set ", iNumber2, pCopyOfIterator->GetRow()+1, pCopyOfIterator->GetColumn()+1);
								printf("[");
								printf("%d ", iNumber);
								for (iNumber3 = 1; iNumber3 < pIterator->GetMeLaSudoku()->GetNumNums()+1; iNumber3++) {
									if (iNumber3 == iNumber || !pCopyOfIterator->IsPossible(iNumber3)) {
										continue;
									}
									bFound = 1;
									for (pIterator->Reset(); !pIterator->IsEnd(); pIterator->Next()) {
										if (!pIterator->IsPossible(iNumber3) || pIterator->IsPossible(iNumber) ||
											(pIterator->InSameColumn(pCopyOfIterator) && pIterator->InSameRow(pCopyOfIterator))) {
											continue;
										}
										if (pIterator->IsPossible(iNumber3)) {
											bFound = 0;
											break;
										}
									}
									if (bFound) {
										printf("%d ", iNumber3);
									}
								}
								printf("] is self sufficient in ");
								pIterator->Dump();
								printf("\n");
							}
						}
					}
				}
			}
		}

		delete pCopyOfIterator;
	}
};

//------------------------------------------------------------------------------------//

class OnlyOnePossibilityRule : public Rule
{
public:
	OnlyOnePossibilityRule()
	{
		m_bIsImpossible = 0;
	}

	virtual ~OnlyOnePossibilityRule()
	{
	}

	virtual void Apply(Iterator *pIterator)
	{
		int i;
		int iCount, iNumber;
		
		for (pIterator->Reset(); !pIterator->IsEnd(); pIterator->Next()) {
			if (pIterator->GetSolution() != 0) {
				continue;
			}
			iCount = 0;
			for (i = 1; i < pIterator->GetMeLaSudoku()->GetNumNums()+1; i++) {
				if (pIterator->IsPossible(i)) {
					iCount++;
					if (iCount > 1) {
						break;
					}
					iNumber = i;
				}
			}
			if (iCount == 0) {
				m_bIsImpossible = 1;
				break;
			}
			if (iCount == 1) {
				if (pIterator->GetMeLaSudoku()->GetDump()) {
					printf("cell (%d, %d) must be %d because there are no other possibilities", pIterator->GetRow()+1, pIterator->GetColumn()+1, iNumber);
					printf("\n");
				}
				pIterator->SetSolution(iNumber);
				m_bDidSomething = 1;
			}
		}
	}

	virtual int AppliesToColumns() { return 1; }
	virtual int AppliesToRows() { return 0; }
	virtual int AppliesToBlocks() { return 0; }
};

//------------------------------------------------------------------------------------//

class MeLaSudokuSolver : public MeLaSudoku
{
public:
	MeLaSudokuSolver(int iSize, int **ppiProblem = NULL)
		: MeLaSudoku(iSize, ppiProblem)
	{
	}

	MeLaSudokuSolver(const MeLaSudoku &b)
		: MeLaSudoku((MeLaSudoku)b)
	{
	}

	virtual ~MeLaSudokuSolver()
	{
	}

	int Reduce(int iNumRules, Rule **ppRules)
	{
		int i, j;
		ColumnIterator **ppColumnIterators = new ColumnIterator*[m_iNumRows];
		RowIterator **ppRowIterators = new RowIterator*[m_iNumColumns];
		BlockIterator **ppBlockIterators = new BlockIterator*[m_iNumBlocks];

		for (i = 0; i < m_iNumRows; i++) {
			ppColumnIterators[i] = new ColumnIterator(this, i);
		}
		for (i = 0; i <  m_iNumColumns; i++) {
			ppRowIterators[i] = new RowIterator(this, i);
		}
		for (i = 0; i < m_iNumBlocks; i++) {
			ppBlockIterators[i] = new BlockIterator(this, i);
		}
		for (j = 0; j < iNumRules; j++) {
			if (ppRules[j]->AppliesToColumns()) {
				for (i = 0; i < m_iNumRows; i++) {
					ppRules[j]->Apply(ppColumnIterators[i]);
					if (ppRules[j]->IsImpossible()) {
						m_bIsImpossible = 1;
						goto impossible;
					}
					if (ppRules[j]->IsNotUnique()) {
						m_bIsNotUnique = 1;
					}
				}
			}
			if (ppRules[j]->AppliesToRows()) {
				for (i = 0; i < m_iNumColumns; i++) {
					ppRules[j]->Apply(ppRowIterators[i]);
					if (ppRules[j]->IsImpossible()) {
						m_bIsImpossible = 1;
						goto impossible;
					}
					if (ppRules[j]->IsNotUnique()) {
						m_bIsNotUnique = 1;
					}
				}
			}
			if (ppRules[j]->AppliesToBlocks()) {
				for (i = 0; i < m_iNumBlocks; i++) {
					ppRules[j]->Apply(ppBlockIterators[i]);
					if (ppRules[j]->IsImpossible()) {
						m_bIsImpossible = 1;
						goto impossible;
					}
					if (ppRules[j]->IsNotUnique()) {
						m_bIsNotUnique = 1;
					}
				}
			}
		}
impossible:
		for (i = 0; i < m_iNumRows; i++) {
			delete ppColumnIterators[i];
		}
		for (i = 0; i < m_iNumColumns; i++) {
			delete ppRowIterators[i];
		}
		for (i = 0; i < m_iNumBlocks; i++) {
			delete ppBlockIterators[i];
		}
		delete [] ppColumnIterators;
		delete [] ppRowIterators;
		delete [] ppBlockIterators;
		if (m_bIsImpossible) {
			return 0;
		}
		for (j = 0; j < iNumRules; j++) {
			if (ppRules[j]->DidSomething()) {
				return 1;
			}
		}
		return 0;
	}

	int Check()
	{
		int i, j;
		int bOK = 1;
		int iCount;
		ColumnIterator **ppColumnIterators = new ColumnIterator*[m_iNumRows];
		RowIterator **ppRowIterators = new RowIterator*[m_iNumColumns];
		BlockIterator **ppBlockIterators = new BlockIterator*[m_iNumBlocks];

		for (i = 0; i < m_iNumRows; i++) {
			ppColumnIterators[i] = new ColumnIterator(this, i);
		}
		for (i = 0; i <  m_iNumColumns; i++) {
			ppRowIterators[i] = new RowIterator(this, i);
		}
		for (i = 0; i < m_iNumBlocks; i++) {
			ppBlockIterators[i] = new BlockIterator(this, i);
		}
		for (j = 1; j < m_iNumNums+1; j++) {
			for (i = 0; i < m_iNumRows; i++) {
				iCount = 0;
				for (ppColumnIterators[i]->Reset(); !ppColumnIterators[i]->IsEnd(); ppColumnIterators[i]->Next()) {
					if (ppColumnIterators[i]->GetSolution() == j) {
						iCount++;
						if (iCount > 1) {
							bOK = 0;
							goto not_ok;
						}
					}
				}
			}
			for (i = 0; i < m_iNumColumns; i++) {
				iCount = 0;
				for (ppRowIterators[i]->Reset(); !ppRowIterators[i]->IsEnd(); ppRowIterators[i]->Next()) {
					if (ppRowIterators[i]->GetSolution() == j) {
						iCount++;
						if (iCount > 1) {
							bOK = 0;
							goto not_ok;
						}
					}
				}
			}
			for (i = 0; i < m_iNumBlocks; i++) {
				iCount = 0;
				for (ppBlockIterators[i]->Reset(); !ppBlockIterators[i]->IsEnd(); ppBlockIterators[i]->Next()) {
					if (ppBlockIterators[i]->GetSolution() == j) {
						iCount++;
						if (iCount > 1) {
							bOK = 0;
							goto not_ok;
						}
					}
				}
			}
		}
not_ok:
		for (i = 0; i < m_iNumRows; i++) {
			delete ppColumnIterators[i];
		}
		for (i = 0; i < m_iNumColumns; i++) {
			delete ppRowIterators[i];
		}
		for (i = 0; i < m_iNumBlocks; i++) {
			delete ppBlockIterators[i];
		}
		delete [] ppColumnIterators;
		delete [] ppRowIterators;
		delete [] ppBlockIterators;
		return bOK;
	}

	int Solve(int iNumRules, Rule **ppRules)
	{
		int j;
		int bDidSomething = 0;

		do {
			while(Reduce(iNumRules, ppRules))
				;
			bDidSomething = 0;
			if (!IsSolved() && !IsImpossible() && !IsNotUnique()) {
				ColumnIterator DummyIterator(this, 0);
				for (j = 0; j < iNumRules; j++) {
					if (!ppRules[j]->AppliesToColumns() && !ppRules[j]->AppliesToRows() && !ppRules[j]->AppliesToBlocks()) {
						ppRules[j]->Apply(&DummyIterator);
						if (ppRules[j]->DidSomething()) {
							bDidSomething = 1;
						}
						if (ppRules[j]->IsImpossible()) {
							SetIsImpossible(1);
							break;
						}
						if (ppRules[j]->IsNotUnique()) {
							SetIsNotUnique(1);
							break;
						}
					}
				}
			}
		} while (bDidSomething && !IsImpossible() && !IsNotUnique());
		return IsSolved();
	}
};

//------------------------------------------------------------------------------------//

class TrialAndErrorRule : public Rule
{
public:
	TrialAndErrorRule()
	{
	}

	virtual ~TrialAndErrorRule()
	{
	}

	void SetRules(int iNumRules, Rule **ppRules) { m_iNumRules = iNumRules; m_ppRules = ppRules; }

	virtual void Apply(Iterator *pIterator)
	{
		int i, j, k;
		int bFound, iCount;
	
		if (pIterator->GetMeLaSudoku()->IsSolved()) {
			return;
		}
		bFound = 0;
		for (i = 0; i < pIterator->GetMeLaSudoku()->GetNumRows() && !bFound && !m_bIsImpossible && !m_bIsNotUnique; i++) {
			for (j = 0; j < pIterator->GetMeLaSudoku()->GetNumRows() && !bFound && !m_bIsImpossible && !m_bIsNotUnique; j++) {
				if (pIterator->GetMeLaSudoku()->GetSolution(i, j) != 0) {
					continue;
				}
				iCount = 0;
				for (k = 1; k < pIterator->GetMeLaSudoku()->GetNumNums()+1 && !bFound; k++) {
					if (!pIterator->GetMeLaSudoku()->IsPossible(i, j, k)) {
						continue;
					}
					int bCheck;
					MeLaSudokuSolver CopyOfMeLaSudoku(*pIterator->GetMeLaSudoku());

					CopyOfMeLaSudoku.SetDump(0);
					CopyOfMeLaSudoku.SetSolution(i, j, k);
					CopyOfMeLaSudoku.ResetGrid();
					CopyOfMeLaSudoku.Solve(m_iNumRules, m_ppRules);
					if (CopyOfMeLaSudoku.IsNotUnique()) {
						m_bIsNotUnique = 1;
						break;
					}
					if (CopyOfMeLaSudoku.IsImpossible() || (CopyOfMeLaSudoku.IsSolved() && !(bCheck = CopyOfMeLaSudoku.Check()))) {
						pIterator->GetMeLaSudoku()->SetIsPossible(i, j, k, 0);
						m_bDidSomething = 1;
						bFound = 1;
						if (pIterator->GetMeLaSudoku()->GetDump()) {
							printf("by trial and error %d is not possible in cell (%d, %d)", k, i+1, j+1);
							printf("\n");
						}
					}
					if (CopyOfMeLaSudoku.IsSolved() && bCheck) {
						iCount++;
					}
				}
				if (iCount > 1) { // more than one solution exists!!
					m_bIsNotUnique = 1;
				}
			}
		}
	}

	virtual int AppliesToColumns() { return 0; }
	virtual int AppliesToRows() { return 0; }
	virtual int AppliesToBlocks() { return 0; }

private:
	int m_iNumRules;
	Rule **m_ppRules;
};

//------------------------------------------------------------------------------------//

// ran1 - numerical receipes in C

#define IA 16807
#define IM 2147483647
#define AM (1.0/IM)
#define IQ 127773
#define IR 2836
#define NTAB 32
#define NDIV (1+(IM-1)/NTAB)
#define EPS 1.2e-7
#define RNMX (1.0-EPS)

double ran1(long *idum)
{
	int j;
	long k;
	static long iy = 0;
	static long iv[NTAB];
	double temp;

	if (*idum <= 0 || !iy) {
		if (-(*idum) < 1) *idum = 1;
		else *idum = -(*idum);
		for (j = NTAB+7; j >= 0; j--) {
			k = (*idum)/IQ;
			*idum = IA * (*idum-k*IQ)-IR*k;
			if (*idum < 0) *idum += IM;
			if (j < NTAB) iv[j] = *idum;
		}
		iy = iv[0];
	}
	k = (*idum)/IQ;
	*idum = IA*(*idum-k*IQ)-IR*k;
	if (*idum < 0) *idum += IM;
	j = iy/NDIV;
	iy = iv[j];
	iv[j] = *idum;
	if ((temp = AM*iy) > RNMX) return RNMX;
	else return temp;
}

//------------------------------------------------------------------------------------//

class MeLaSudokuGenerator
{
public:

	MeLaSudokuGenerator(int iSize, int iNumRules, Rule **ppRules,
		int iNumEasyRules, Rule **ppEasyRules)
		: meLaSudoku(iSize)
	{
		int i, j, k;
		int iCount;
		int iNumber, iNumber2;
		static long seed = -(long)time(NULL);

		m_iSize = iSize;
		m_iNumRows = m_iNumColumns = m_iNumBlocks = m_iNumNums = m_iSize*m_iSize;
		m_ppiProblem = new int*[m_iNumRows];
		for (i = 0; i < m_iNumRows; i++) {
			m_ppiProblem[i] = new int[m_iNumColumns];
		}
		do {
			do {
				for (i = 0; i < m_iNumRows; i++) {
					for (j = 0; j < m_iNumColumns; j++) {
						m_ppiProblem[i][j] = 0;
					}
				}
				meLaSudoku.InitGrid(m_ppiProblem);
				while(meLaSudoku.Reduce(iNumRules, ppRules))
					;
				// works out faster not to use trial and error to
				// determine possibilities
				meLaSudoku.SetIsNotUnique(1);
				do {
					do {
						i = (int)(ran1(&seed) * (double)m_iNumRows);
						j = (int)(ran1(&seed) * (double)m_iNumColumns);
					} while (m_ppiProblem[i][j] != 0);
					iCount = 0;
					for (k = 1; k < m_iNumNums+1; k++) {
						if (meLaSudoku.IsPossible(i, j, k)) {
							iCount++;
						}
					}
					if (iCount == 0) {
						meLaSudoku.SetIsImpossible(1);
						break;
					}
					iCount = (int)(ran1(&seed) * (double)iCount);
					for (k = 1; k < m_iNumNums+1; k++) {
						if (meLaSudoku.IsPossible(i, j, k)) {
							iCount--;
							if (iCount < 0) {
								iNumber = k;
								break;
							}
						}
					}
					m_ppiProblem[i][j] = iNumber;
					meLaSudoku.SetSolution(i, j, iNumber);
					meLaSudoku.Solve(iNumRules, ppRules);
					if (meLaSudoku.IsImpossible()) {
						break;
					}
					iCount = 0;
					for (k = 1; k < m_iNumNums+1; k++) {
						if (meLaSudoku.IsPossible(m_iNumRows-i-1, m_iNumColumns-j-1, k)) {
							iCount++;
						}
					}
					if (iCount == 0) {
						meLaSudoku.SetIsImpossible(1);
						break;
					}
					iCount = (int)(ran1(&seed) * (double)iCount);
					for (k = 1; k < m_iNumNums+1; k++) {
						if (meLaSudoku.IsPossible(m_iNumRows-i-1, m_iNumColumns-j-1, k)) {
							iCount--;
							if (iCount < 0) {
								iNumber = k;
								break;
							}
						}
					}
					m_ppiProblem[m_iNumRows-i-1][m_iNumColumns-j-1] = iNumber; // symmetric
					meLaSudoku.SetSolution(m_iNumRows-i-1, m_iNumRows-j-1, iNumber);
					meLaSudoku.Solve(iNumRules, ppRules);
					if (meLaSudoku.IsImpossible()) {
						break;
					}
				} while (!meLaSudoku.IsSolved());
				printf("."); fflush(stdout);
			} while (meLaSudoku.IsImpossible() || !meLaSudoku.Check());
			// try to remove some solutions
			for (i = 0; i < m_iNumRows / 2 + 1; i++) { // because symmetric
				for (j = 0; j < m_iNumColumns; j++) {
					if (m_ppiProblem[i][j] == 0) {
						continue;
					}
					iNumber = m_ppiProblem[i][j];
					iNumber2 = m_ppiProblem[m_iNumRows-i-1][m_iNumColumns-j-1];
					m_ppiProblem[i][j] = 0;
					m_ppiProblem[m_iNumRows-i-1][m_iNumColumns-j-1] = 0;
					meLaSudoku.InitGrid(m_ppiProblem);
					meLaSudoku.Solve(iNumRules, ppRules);
					if (meLaSudoku.IsImpossible() || meLaSudoku.IsNotUnique() || !meLaSudoku.IsSolved() || !meLaSudoku.Check()) {
						m_ppiProblem[i][j] = iNumber;
						m_ppiProblem[m_iNumRows-i-1][m_iNumColumns-j-1] = iNumber2;
						continue;
					}
				}
			}
			meLaSudoku.InitGrid(m_ppiProblem);
			meLaSudoku.Solve(iNumEasyRules, ppEasyRules);
			printf("."); fflush(stdout);
		} while (meLaSudoku.IsSolved()); // too easy... try again!
		printf("\n");
		meLaSudoku.InitGrid(m_ppiProblem);
	}

	virtual ~MeLaSudokuGenerator()
	{
		int i;

		for (i = 0; i < m_iNumRows; i++) {
			delete [] m_ppiProblem[i];
		}
		delete [] m_ppiProblem;
	}

	MeLaSudokuSolver &GetMeLaSudokuSolver() { return meLaSudoku; }

	void Dump()
	{
		int i, j, k;

		for (i = 0; i < m_iNumRows; i++) {
			if (i % m_iSize == 0) {
				for (j = 0; j < m_iSize; j++) {
					printf("+-");
					for (k = 0; k < m_iSize; k++) {
						printf("--");
					}
				}
				printf("+\n");
			}
			for (j = 0; j < m_iNumColumns; j++) {
				if (j % m_iSize == 0) {
					printf("| ");
				}
				if (m_ppiProblem[i][j] == 0) {
					printf("  ");
				} else {
					if (m_iSize <= 3) {
						printf("%d ", m_ppiProblem[i][j]);
					} else {
						printf("%c ", (char)m_ppiProblem[i][j]+'A'-1);
					}
				}
			}
			printf("|\n");
		}
		for (j = 0; j < m_iSize; j++) {
			printf("+-");
			for (k = 0; k < m_iSize; k++) {
				printf("--");
			}
		}
		printf("+\n");
	}

private:
	int m_iSize;
	int m_iNumRows;
	int m_iNumColumns;
	int m_iNumNums;
	int m_iNumBlocks;
	int **m_ppiProblem;
	MeLaSudokuSolver meLaSudoku;
};

//------------------------------------------------------------------------------------//

#endif