
rm -rf build || exit 1
mkdir build || exit 1
cd build || exit 1

cmake .. || exit 1
make || exit 1

cd .. || exit 1
bash tests/test_lexer.sh || exit 1
bash tests/test_parser.sh || exit 1
