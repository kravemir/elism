
rm -rf build || exit 1
mkdir build || exit 1
cd build || exit 1

cmake .. -DCMAKE_BUILD_TYPE=Debug || exit 1
make -j4 || exit 1

cd .. || exit 1
bash scripts/tests/test_lexer.sh || exit 1
bash scripts/tests/test_parser.sh || exit 1
