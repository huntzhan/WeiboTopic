/**
 * @file    persistence.cpp
 * @author  Guangyi Zhang <guangyizhang.jan@gmail.com>
 * @date    08/27/2014 10:46:28 PM
 * @version 1.0
 *
 */
#include "persistence.h"

void InsertDataToTable(string tablename, const std::vector<INSERT_DATA> &insert_datas) {
  insert.SetTableName(tablename);
  insert.CreateTable();
  insert.DB_insertData(insert_datas);
}

