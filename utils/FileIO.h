//
// Created by tth37 on 2022/3/18.
//

#ifndef CODECRAFT_2021_FILEIO_H
#define CODECRAFT_2021_FILEIO_H

#include <string>

using namespace std;

const bool LOCAL_DEBUG = true;

/*
 * @Param int D[][40], string clientName[], int &M, int &T
 */
void readDemand(int[][40], string[], int &, int &);

/*
 * @Param int C[], string siteName[], int &N
 */
void readCap(int[], string[], int &);

/*
 * @Param int Y[][200], int M, int N
 */
void readY(int[][200], int, int);

/*
 * @Param int &Q
 */
void readQ(int &);

/*
 * @Param string clientName[], string siteName[], int Xans[][40][200], int T,
 * int M, int N
 */
void printAns(string[], string[], int[][40][200], int, int, int);

#endif // CODECRAFT_2021_FILEIO_H
