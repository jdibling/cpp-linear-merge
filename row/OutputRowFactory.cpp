//
// Created by john on 4/23/15.
//

#include "OutputRowFactory.hpp"
#include "../std/memory.hpp"

OutputRowPtr OutputRowFactory::CreateOutputRow (boost::optional<InputRow> leftInput,
                                                boost::optional<InputRow> rightInput) {
  OutputRowPtr retval = pstd::make_unique<OutputRow> ();
  OutputRow &outputRow = *retval;

  if (leftInput) {
    const InputRow &inputRow = *leftInput;
    std::copy (inputRow.pbegin (), inputRow.pend (), std::back_inserter (outputRow));
  }
  else {
    std::fill_n (std::back_inserter (outputRow),
                 mInputColumns.size (),
                 boost::none);
  }

  if (rightInput) {
    const InputRow &inputRow = *rightInput;
    std::copy (inputRow.pbegin (), inputRow.pend (), std::back_inserter (outputRow));
  }
  else {
    std::fill_n (std::back_inserter (outputRow),
                 mInputColumns.size (),
                 boost::none);
  }

  for (size_t idx = 0; idx < mOutputColumns.size (); ++idx) {
    if (rightInput && leftInput) {
      const CellFactory &cf = mOutputColumns[idx].mCellFactory;
      CellPtr outCell = cf.CreateMergeCell ((*leftInput)[0], (*rightInput)[0]);
      outputRow.push_back (outCell);
    }
    else {
      outputRow.push_back (boost::none);
    }
  }

/*
  for (OutputRow::const_iterator cellIt = outputRow.begin (); cellIt != outputRow.end (); ++cellIt)
  {
    OptCellPtr optCellPtr = *cellIt;
    if (optCellPtr) {
      CellPtr cellPtr = *optCellPtr;
      const Cell &cell = *cellPtr;
      const std::string repr = cell.Repr ();
      std::cout << repr << std::endl;
    }
  }
*/

  return retval;

}