# bruteforce-openmp

A tool to test bruteforce attack using OpenMP


## Run

`
sh run.sh
`

## Measures

cat mono_timeline.csv | awk -F ', ' '{print $2}' | uniq -c > mono_pwd_p_sec.csv

### Speed Test
shadow 100
OMP
time ./multiattack_omp 4 10 dict.sha shadow.sha
real	0m0,063s
user	0m0,243s
sys	0m0,134s

MONO
time ./multiattack_mono 10 dict.sha shadow.sha
real	0m0,127s
user	0m0,346s
sys	0m0,129s

### Equivalence Test
shadow 1000
OMP
time ./multiattack_omp 10 dict.sha shadow.sha
real	0m0,620s
user	0m3,099s
sys	0m0,992s

MONO
time ./multiattack_mono 10 dict.sha shadow.sha
real	0m0,646s
user	0m3,129s
sys	0m0,860s
