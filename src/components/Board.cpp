#include "Board.h"

#include "Player.h"
#include "Game.h"
#include "Piece.h"
#include "QtBoardView.h"
#include "Utils.h"

Board::Board()
    : _winner(NULL)
{
  for (int i = 0; i < 3; ++i)
  {
    std::vector<Cell> c;
    for (int j = 0; j < 3; ++j)
    {
      c.push_back( Cell(this, i, j) );
    }
    _cells.push_back(c);
  }

  populateWinningPatterns();

  std::cerr << "New Board created: " << this << std::endl;
}

void
Board::populateWinningPatterns()
{
  // 0-based indexing! - 1-based everywhere else, careful!!
  winningPatterns.insert( make_pair( 1, std::make_tuple( std::make_pair(0, 0)
      , std::make_pair(0, 1), std::make_pair(0, 2) ) ) );
  winningPatterns.insert( make_pair( 2, std::make_tuple( std::make_pair(1, 0)
      , std::make_pair(1, 1), std::make_pair(1, 2) ) ) );
  winningPatterns.insert( make_pair( 3, std::make_tuple( std::make_pair(2, 0)
      , std::make_pair(2, 1), std::make_pair(2, 2) ) ) );
  winningPatterns.insert( make_pair( 4, std::make_tuple( std::make_pair(0, 0)
      , std::make_pair(1, 0), std::make_pair(2, 0) ) ) );
  winningPatterns.insert( make_pair( 5, std::make_tuple( std::make_pair(0, 1)
      , std::make_pair(1, 1), std::make_pair(2, 1) ) ) );
  winningPatterns.insert( make_pair( 6, std::make_tuple( std::make_pair(0, 2)
      , std::make_pair(1, 2), std::make_pair(2, 2) ) ) );
  winningPatterns.insert( make_pair( 7, std::make_tuple( std::make_pair(0, 0)
      , std::make_pair(1, 1), std::make_pair(2, 2) ) ) );
  winningPatterns.insert( make_pair( 8, std::make_tuple( std::make_pair(0, 2)
      , std::make_pair(1, 1), std::make_pair(2, 0) ) ) );
}

Cell &
Board::cell(int r, int c)
{
  --r; --c;

  return _cells[r][c];
}

void
Board::setCell(Cell cell, int r, int c)
{
  --r; --c;

  _cells[r][c].setPiece( cell.piece() );
}

Player *
Board::winner()
{
  return _winner;
}

bool
Board::hasWinner()
{
  return isWinner( Game::instance()->firstPlayer() ) 
      || isWinner( Game::instance()->secondPlayer() );
}

bool
Board::isWinner(Player * player)
{
  std::cerr << "checking winner for: ";
  debugPrintPiece(player->piece());
  std::cerr << std::endl;

  for (auto itr = winningPatterns.begin(); itr != winningPatterns.end(); ++itr)
  {
    bool winner = true;
    int r, c;

    r = 1 + std::get<0>(itr->second).first;
    c = 1 + std::get<0>(itr->second).second;
    winner &= (player->piece() == cell(r, c).piece());

    r = 1 + std::get<1>(itr->second).first;
    c = 1 + std::get<1>(itr->second).second;
    winner &= (player->piece() == cell(r, c).piece());

    r = 1 + std::get<2>(itr->second).first;
    c = 1 + std::get<2>(itr->second).second;
    winner &= (player->piece() == cell(r, c).piece());

    if (winner)
    {
      _winner = player;

      return true;
    }
  }

  return false;
}

void
Board::onStateChanged(int r, int c)
{
  std::cerr << std::endl;
  for (int r = 0; r < 3; ++r)
  {
    for (int c = 0; c < 3; c++)
    {
      debugPrintPiece( cell(1 + r, 1 + c).piece() );
    }
    std::cerr << std::endl;
  }

  std::cerr << "content of [" << r << ", " << c << "] changed" << std::endl;

  if (hasWinner())
  {
    std::cout << "We have a winner!" << std::endl;
    debugPrintPiece(winner()->piece());
    std::cerr << "is the winner!!" << std::endl;

    setAllCellsEnabled( false, QtBoardView::board() );
  }
  else
  {
    std::cerr << "No winner yet" << std::endl;
  }
}

#ifdef O
IBoardIterator
Board::begin(IBoardIterator::Type type)
{
  static IBoardIterator rItr = IBoardIterator::rowMajorBegin( *this );
  static IBoardIterator cItr = IBoardIterator::columnMajorBegin( *this );
  
  if (IBoardIterator::Type::ROW == type)
    return rItr;

  if (IBoardIterator::Type::COL == type)
    return cItr;

  return rItr;  // row iter by default, todo throw exception
}

IBoardIterator
Board::end(IBoardIterator::Type type)
{
  static IBoardIterator rItr = IBoardIterator::rowMajorEnd( *this );
  static IBoardIterator cItr = IBoardIterator::columnMajorEnd( *this );
  
  if (IBoardIterator::Type::ROW == type)
    return rItr;

  if (IBoardIterator::Type::COL == type)
    return cItr;

  return rItr;  // row iter by default, todo throw exception
}
#endif

bool operator==(Board lhs, Board rhs)
{
  return &lhs == &rhs;
}

void
Board::debugPrintPiece(Piece p)
{
  const std::string sym[] {"X", "O", "-"};
  std::cerr << sym[(int) p] << " ";
}
