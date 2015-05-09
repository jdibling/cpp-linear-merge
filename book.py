import csv

fileName = 'C:\\bbo\\SPY-Direct-Nasdaq-order.txt'

with open(fileName, 'r') as file:
    reader = csv.reader(file, delimiter=',')
    for row in reader:
        cells = {}
        for cell in row:
            if len(cell) < 1:
                continue
            cellItems = cell.split('=')
            if len(cellItems) < 2:
                continue;
            cellName = cellItems[0]
            cellValue = cellItems[1]
            cells[cellName] = cellValue

        if cells['TransactionType'] is None:
            continue

        transType = cells['TransactionType']
        if transType.find('Order') != -1:
            sku = cells.get('OrderSku', "")
            ssc = cells.get("SourceContent", "")
            ticker = cells.get("Ticker", "")
            seqNum = cells.get("TransactionSequence", "0")
            ordTime = cells.get("OrderTime", "")
            revType = cells.get("ReviseType", "")
            ordSize = cells.get("OrderSize", "")
            lastSize = cells.get("LastSize", "")
            price = cells.get("OrderPrice", "")
            orderType = cells.get("OrderType", "")

            print('\t'.join([seqNum, ticker, transType, orderType, revType, price, lastSize, ordSize, ordTime]))

