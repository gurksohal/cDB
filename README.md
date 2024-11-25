# CDB (C++ version of SimpleDB)

C++ implementation of the SimpleDB system, based on the Database Design and Implementation book.

## Major features

- parser
- query planning and optimizing
- transactions
- recovery
- storage engine
- hash index

## Build

```shell
mkdir build && cd build
cmake ..
make
```

## Run

```shell
# create table
./createDB

# start db and run queries against the created table
./cdb
```

## Example

```shell
wsl/cDB/build$ ./createDB 
Create new database
wsl/cDB/build$ ./cdb
recovering existing database
> select SId, SName, GradYear, DName from STUDENT, DEPT where DId = MajorId
1 | joe | 2021 | compsci
2 | amy | 2020 | math
3 | max | 2022 | compsci
4 | sue | 2022 | math
5 | bob | 2020 | drama
6 | kim | 2020 | math
7 | art | 2021 | drama
8 | pat | 2019 | math
9 | lee | 2021 | compsci

```


