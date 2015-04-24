//
// Created by john on 4/23/15.
//

#ifndef LMERGE_OUTPUTROWFACTORY_H
#define LMERGE_OUTPUTROWFACTORY_H

#include <boost/optional.hpp>

#include "../column/Column.hpp"
#include "InputRow.hpp"

#include "OutputRow.hpp"

class OutputRowFactory {
  const Columns &mInputColumns;
  const Columns &mOutputColumns;

public:
  OutputRowFactory (const Columns &inputColumns,
                    const Columns &outputColumns)
    :
    mInputColumns (inputColumns),
    mOutputColumns (outputColumns) {

  }

  OutputRowPtr CreateOutputRow (boost::optional<InputRow> leftInput,
                               boost::optional<InputRow> rightInput);
};


#endif //LMERGE_OUTPUTROWFACTORY_H
