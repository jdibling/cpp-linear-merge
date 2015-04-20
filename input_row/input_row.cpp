//
// Created by john on 4/17/15.
//

#include "../std/memory.hpp"

#include "../input_row.hpp"

#include "../column/text_column.hpp"
#include "../column/uint_column.hpp"

bool Equ (const Row &lhs, const Row &rhs, const Columns &cols) {
//    const auto &leftSeq = lhs[1]->Repr ();
//    const auto &rightSeq = rhs[1]->Repr ();

  for (size_t idx = 0; idx < cols.size (); ++idx) {
    const Column &col = *cols[idx];
    if (col.mImportance == Column::Importance::Key) {
      const Cell &lhc = lhs[idx];
      const Cell &rhc = rhs[idx];

      if (lhc != rhc) {
        return false;
      }
    }
  }

  return true;
}

Columns GetInputFileHeader () {
  Columns cols;
  cols.push_back (pstd::make_unique<UIntColumn> ("RecvTime"));
  cols.push_back (pstd::make_unique<TextColumn> ("SeqNum"));
  cols.push_back (pstd::make_unique<TextColumn> ("Ticker"));
  cols.push_back (pstd::make_unique<TextColumn> ("BidSide"));
  cols.push_back (pstd::make_unique<TextColumn> ("BidPrice", Column::Importance::Key));
  cols.push_back (pstd::make_unique<TextColumn> ("BidSizeRaw"));
  cols.push_back (pstd::make_unique<TextColumn> ("BidSize", Column::Importance::Key));
  cols.push_back (pstd::make_unique<TextColumn> ("BidTime"));
  cols.push_back (pstd::make_unique<TextColumn> ("BidMarket"));
  cols.push_back (pstd::make_unique<TextColumn> ("AskSide"));
  cols.push_back (pstd::make_unique<TextColumn> ("AskPrice", Column::Importance::Key));
  cols.push_back (pstd::make_unique<TextColumn> ("AskSizeRaw"));
  cols.push_back (pstd::make_unique<TextColumn> ("AskSize", Column::Importance::Key));
  cols.push_back (pstd::make_unique<TextColumn> ("AskTime"));
  cols.push_back (pstd::make_unique<TextColumn> ("AskMarket"));

  return cols;

}
