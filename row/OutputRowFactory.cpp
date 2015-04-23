//
// Created by john on 4/23/15.
//

#include "OutputRowFactory.hpp"

OutputRow &&OutputRowFactory::CreateOutputRow (boost::optional<InputRow> leftInput,
                                               boost::optional<InputRow> rightInput) {
  OutputRow retval;

  if (leftInput) {
    const InputRow &inputRow = *leftInput;
    std::copy (inputRow.pbegin (), inputRow.pend (), std::back_inserter (retval));
  }
  else {
    std::fill_n (std::back_inserter (retval),
                 mInputColumns.size (),
                 boost::none);
  }

  if (rightInput) {
    const InputRow &inputRow = *rightInput;
    std::copy (inputRow.pbegin (), inputRow.pend (), std::back_inserter (retval));
  }
  else {
    std::fill_n (std::back_inserter (retval),
                 mInputColumns.size (),
                 boost::none);
  }

  for (size_t idx = 0; idx < mOutputColumns.size (); ++idx) {
    if (rightInput && leftInput) {
      const CellFactory &cf = mInputColumns[idx].mCellFactory;
      CellPtr outCell = cf.CreateMergeCell ((*leftInput)[0], (*rightInput)[0]);
      retval.push_back (outCell);
    }
    else {
      retval.push_back (boost::none);
    }
  }

  return std::move (retval);

}