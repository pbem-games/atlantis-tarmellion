#!/bin/bash

grep -A 1 -B 1 city], report.1 | cut -f2 -d"(" | cut -f1 -d")" > cities
grep -A 1 -B 1 town], report.1 | cut -f2 -d"(" | cut -f1 -d")" > towns
grep -A 1 -B 1 village], report.1 | cut -f2 -d"(" | cut -f1 -d")" > villages

./clean-list.pl  cities
./clean-list.pl  towns
./clean-list.pl  villages

cat cities > all_settlements
cat towns >> all_settlements
cat villages >> all_settlements
