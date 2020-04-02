make clean
make all

echo "Generating SHA of dict.txt ..."
./dict2sha dict.txt dict.sha

echo "Generating shadow from dict ..."
./shadowFaker dict.txt shadow.sha 1000

echo "Then run"
echo "time ./multiattack_mono 4 dict.sha shadow.sha"
echo "time ./multiattack_omp 4 10 dict.sha shadow.sha"
