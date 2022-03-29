//
// Created by tth37 on 2022/3/19.
//

#ifndef CODECRAFT_2021_BENCHMARK_H
#define CODECRAFT_2021_BENCHMARK_H

/*
 * @Param const int X[][40][200], const int D[][40], const int C[], const int
 * Y[][200], int T, int M, int N, int Q
 */
void runJudger(const int[][40][200], const int[][40], const int[],
               const int[][200], int, int, int, int);

/*
 * @Param int X[][40][200], int T, int M, int N
 */
int runBenchmark(int[][40][200], int, int, int);

#endif // CODECRAFT_2021_BENCHMARK_H