#ifndef TABLEIO_H_
#define TABLEIO_H_
#include "table.h"

Table read(std::string filename);
std::ostream &operator<<(std::ostream &stream, const Table &table);

#endif // TABLEIO_H_
