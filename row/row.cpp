//
// Created by john on 4/17/15.
//

#include "../std/memory.hpp"

#include "../row.hpp"

bool Equ (const Row &lhs, const Row &rhs, const InputColumns &cols) {
//    const auto &leftSeq = lhs[1]->Repr ();
//    const auto &rightSeq = rhs[1]->Repr ();

  for (size_t idx = 0; idx < cols.size (); ++idx) {
    const InputColumn &col = *cols[idx];
    if (col.mImportance == InputColumn::Importance::Key) {
      const Cell &lhc = lhs[idx];
      const Cell &rhc = rhs[idx];

      if (lhc != rhc) {
        return false;
      }
    }
  }

  return true;
}

InputColumns GetInputFileHeader () {
  InputColumns cols;
  cols.push_back (InputColumn (InputCellFactory::Type::UInt, "RecvTime");
/*
  cols.push_back (pstd::make_unique<TextColumn> ("SeqNum"));
  cols.push_back (pstd::make_unique<TextColumn> ("Ticker"));
  cols.push_back (pstd::make_unique<TextColumn> ("BidSide"));
  cols.push_back (pstd::make_unique<TextColumn> ("BidPrice", InputColumn::Importance::Key));
  cols.push_back (pstd::make_unique<TextColumn> ("BidSizeRaw"));
  cols.push_back (pstd::make_unique<TextColumn> ("BidSize", InputColumn::Importance::Key));
  cols.push_back (pstd::make_unique<TextColumn> ("BidTime"));
  cols.push_back (pstd::make_unique<TextColumn> ("BidMarket"));
  cols.push_back (pstd::make_unique<TextColumn> ("AskSide"));
  cols.push_back (pstd::make_unique<TextColumn> ("AskPrice", InputColumn::Importance::Key));
  cols.push_back (pstd::make_unique<TextColumn> ("AskSizeRaw"));
  cols.push_back (pstd::make_unique<TextColumn> ("AskSize", InputColumn::Importance::Key));
  cols.push_back (pstd::make_unique<TextColumn> ("AskTime"));
  cols.push_back (pstd::make_unique<TextColumn> ("AskMarket"));
*/

  return cols;

}
