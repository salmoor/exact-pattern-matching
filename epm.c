/**
* by Alemdar Salmoor
**/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

double computeTime(clock_t dif){
    
    double e = (double) dif;
    e = e/CLOCKS_PER_SEC;
    e = e * 1000000.0;

    return e;
}


void bruteForce(char * text, char * pattern, int tLen, int pLen, double * runtime){

    struct timeval t1, t2;
	
	clock_t mTime;
    mTime = clock();

    int i = 0;
    int j = 0;
    int start;
    int location;
    int comparisons = 0;
    bool found = false;


    //for testing
    //printf("Comparison N: %d, Comparing %c from text vs %c from pattern, result of comparison: %d, start is %d and j is %d\n", comparisons, text[start], pattern[j], pattern[j] == text[start], start, j)

    while (i < tLen - pLen + 1)
    {
        start = i;
        while(pattern[j] == text[start]){
            j = j + 1;
            start = start + 1;
            comparisons++;
            if(pLen <= j){
                location = i + 1;
                found = true;
                goto afterloop;
            }
        }
        comparisons++;
        j=0;
        i++;
    }
    afterloop:

    mTime = clock() - mTime;
    *runtime = (int) computeTime(mTime);

    printf("Brute Force Search: ");
    if(found){
        printf("Location of P within T is %d", location);
    }
    else{
        printf("P is not in T\n");
    }
    printf(", Number of character comparisons performed: %d", comparisons);
    printf(", Runtime: %.2lf microseconds\n", *runtime);
    
    
}

void failureFunction(int * F, char * P, int pLen){
    F[0] = 0;
    int i = 1;
    int j = 0;
    
    while (i < pLen)
    {
        if (P[i] == P[j])
        {
            F[i] = j + 1;
            i++;
            j++;
        }
        else if(j > 0){
            j = F[j-1];
        }
        else{
            F[i] = 0;
            i++;
        } 
    }
}

void kmp(char * text, char * pattern, int tLen, int pLen, double * runtime){

    struct timeval t1, t2;
	
	clock_t mTime;
    mTime = clock();

    int i = 0;
    int j = 0;
    int location;
    int comparisons = 0;
    bool found = false;

    int * F = malloc(pLen * sizeof(int));

    failureFunction(F, pattern, pLen);

    // int counter = 0;
    // printf("Failure function: ");
    // while (counter < pLen)
    // {
    //     printf("%d ", F[counter]);
    //     counter++; 
    // }
    // printf("\n");
    

    while(i < tLen){

        comparisons++;
        //printf("Comparison N: %d, Comparing %c from text vs %c from pattern, i is %d, j is %d\n", comparisons, text[i], pattern[j], i, j);
        if(text[i] == pattern[j]){
            if (j == pLen - 1){
                location = i - j + 1;
                found = true;
                break;
            }
            else{
                i++;
                j++;
            }
        }
        else {
            if(j > 0){
                j = F[j-1];
            }
            else{
                i++;
                j = 0;
            }
        }
    }

    mTime = clock() - mTime;
    *runtime = (int) computeTime(mTime);

    printf("Knuth-Morris-Pratt: ");
    if(found){
        printf("Location of P within T is %d", location);
    }
    else{
        printf("P is not in T\n");
    }
    printf(", Number of character comparisons performed: %d", comparisons);
    printf(", Runtime: %.2lf microseconds\n", *runtime);

    free(F);
}

void computeB(int ** B, char * P, int pLen){

    int cnt = 0;

    int lastA = -1;
    int lastC = -1;
    int lastG = -1;
    int lastT = -1;

    while (cnt < pLen)
    {
        if(P[cnt] == 'A'){ lastA = cnt;}
        else if(P[cnt] == 'C'){ lastC = cnt;}
        else if(P[cnt] == 'G'){ lastG = cnt;}
        else{ lastT = cnt;}

        B[cnt][0] = lastA; B[cnt][1] = lastC; B[cnt][2] = lastG;  B[cnt][3] = lastT;

        cnt++;
    }
    
}

int NuctoDec(char c){
    if (c == 'A'){ return 0; }
    else if(c == 'C'){ return 1; }
    else if(c == 'G'){ return 2; }
    else { return 3; }
}

void computeGs(int * Gs, char * P, int pLen){
    int * gs1 = malloc(pLen * sizeof(int));
    int * gs2 = malloc(pLen * sizeof(int));

    gs1[pLen - 1] = 0; gs2[pLen - 1] = 0;

    //Compute gs1
    int j = 0;
    int k, i, z;
    bool found = false;
    while (j < pLen - 1)
    {
        k = pLen - 2;
        while (k >= pLen - j - 2)
        {
            i = k;
            z = pLen - 1;

            while (P[i] == P[z])
            {
                if (z == j + 1){
                    if((k - pLen + j + 1) < 0){
                        found = true;
                        goto foundGs1;
                    }
                    else if (P[j] != (P[k - pLen + j + 1])){
                        found = true;
                        goto foundGs1;
                    }
                    break;
                }
                i--;
                z--;
            }
            k--;
        }
        foundGs1:
        if(found){
            gs1[j] = k + 1;
        }
        else {
            gs1[j] = 0;
        }
        found = false;
        j++;
        
    }

    //Compute gs2
    j = 0;
    k = 0; i = 0; z = 0;
    found = false;
    while (j < pLen - 1)
    {
        k = pLen - j - 2;
        while (k >= 0)
        {
            i = k;
            z = pLen - 1;

            while (P[i] == P[z])
            {
                if (i == 0){
                    found = true;
                    goto foundGs2;
                    break;
                }
                i--;
                z--;
            }
            k--;
        }
        foundGs2:
        if(found){
            gs2[j] = k + 1;
        }
        else {
            gs2[j] = 0;
        }
        found = false;
        j++;
        
    }

    int counter = 0;
    int max = 0;

    Gs[pLen - 1] = 1; //j = m

    while (counter < pLen - 1)
    {
        if (gs1[counter] > gs2[counter]){ max = gs1[counter]; }
        else { max = gs2[counter]; }

        Gs[counter] = pLen - max;

        counter++;
        
    }

    // //Print gs1
    // int counter = 0;
    // printf("gs1: ");
    // while (counter < pLen)
    // {
    //     printf(" %d ", gs1[counter]);   
    //     counter++;
    // }
    // printf("\n");

    // //Print gs2
    // counter = 0;
    // printf("gs2: ");
    // while (counter < pLen)
    // {
    //     printf(" %d ", gs2[counter]);   
    //     counter++;
    // }
    // printf("\n");

    // //Print Gs
    // counter = 0;
    // printf("GS: ");
    // while (counter < pLen)
    // {
    //     printf(" %d ", Gs[counter]);   
    //     counter++;
    // }
    // printf("\n");

    free(gs1);
    free(gs2);

}

void BoyerMoore(char * text, char * pattern, int tLen, int pLen, double * runtime){

    struct timeval t1, t2;
	
	clock_t mTime;
    mTime = clock();

    int i = 0;
    int j = 0;
    int k = 0;//starting position of target
    int z = 0;
    int comparisons = 0;

    char nuc;
    int shift, shiftB, shiftG;

    int location;
    bool found = false;
    
    int ** B = malloc(pLen * sizeof(int *));
    int cnt = 0;
    while (cnt < pLen)
    {
        B[cnt] = malloc(4 * sizeof(int));
        cnt++;        
    }

    int * Gs = malloc(pLen * sizeof(int));

    
    
    computeB(B, pattern, pLen);
    computeGs(Gs, pattern, pLen);

    while(i < tLen - pLen + 1){
        
        k = i;
        z = k + pLen -1;
        j = pLen - 1;
        while (text[z] == pattern[j])
        {
            comparisons++;
            if (j == 0){
                location = i + 1;
                found = true;
                goto BoyerMooreEnd;
            }
            else{
                z--;
                j--;
            }
        }
        comparisons++;
        nuc = NuctoDec(text[z]);
        shiftB = j - B[j][nuc];
        shiftG = Gs[j];

        if(shiftB > shiftG){ shift = shiftB; }
        else { shift = shiftG;}
        
        i = i + shift;
    }
    BoyerMooreEnd:

    mTime = clock() - mTime;
    *runtime = (int) computeTime(mTime);

    printf("Boyer-Moore: ");
    if(found){
        printf("Location of P within T is %d", location);
    }
    else{
        printf("P is not in T\n");
    }
    printf(", Number of character comparisons performed: %d", comparisons);
    printf(", Runtime: %.2lf microseconds\n", *runtime);


    // cnt = 0;
    // while (cnt < pLen)
    // {
    //     int i = 0;
    //     while(i < 4){
    //         printf(" %d ", B[cnt][i]);
    //         i++;
    //     }
    //     printf("\n");
    //     cnt++;        
    // }

    free(Gs);
    
    cnt = 0;
    while (cnt < pLen)
    {
        free(B[cnt]);
        cnt++;
    }

    free(B);
    
}

int largerPrime(int x){

    int i, n;
    if(x%2 == 0){ n = x + 1; }
    else{ n = x + 2; }

    bool prime = true;

    while (true)
    {
        i = 2;
        while (i < n/2)
        {
            if (n%i == 0) { prime = false; break; }

            i++;
        }
        if (prime) { break; }
        n = n + 2;
        prime = true;
        
    }

    return n;
    
}

void RabinKarp(char * text, char * pattern, int tLen, int pLen, double * runtime){

    struct timeval t1, t2;
	
	clock_t mTime;
    mTime = clock();

    int q = largerPrime(pLen);
    int exp = pLen - 1;
    
    int cq = 1;
    int cnt = 0;
    
    while (cnt < exp)
    {
        cq = cq * 4;
        cq = cq % q;
        cnt++; 
    }
    
    bool found;
    int location;
    int comparisons = 0;

    //printf("q is %d and exp is %d and c is %ld\n", q, exp, c);

    int fp = 0;
    int ft = 0;

    int i = 0;

    //preprocessing
    while (i < pLen)
    {
        fp = (4 * fp + NuctoDec(pattern[i])) % q;
        ft = (4 * ft + NuctoDec(text[i])) % q;

        i++;
    }

    //printf("fp is %d and ft is %d\n", fp, ft);


    int s = 0;
    int j;

    while (s < tLen - pLen)
    {
        //printf("at s = %d,fp is %d and ft is %d\n",s, fp, ft);
        if (fp == ft){
            j = 0;
            while (pattern[j] == text[s+j])
            {
                comparisons++;
                if(j == (pLen - 1)){
                    found = true;
                    location = s + 1;
                    goto RKend;
                }
                j++;
            }
            comparisons++;  
        }
        ft = ((ft - (NuctoDec(text[s])* cq))*4 + NuctoDec(text[s+pLen])) % q;
        
        if(ft < 0) { ft = ft + q;}
        s++;
    }
    RKend:

    mTime = clock() - mTime;
    *runtime = (int) computeTime(mTime);

    printf("Rabin-Karp: ");
    if(found){
        printf("Location of P within T is %d", location);
    }
    else{
        printf("P is not in T\n");
    }
    printf(", Number of character comparisons performed: %d", comparisons);
    printf(", Runtime: %.2lf microseconds\n", *runtime);

}

void computeBest(double bf, double kmp, double bm, double rk){
    int bestIndex = 0;
    double bestTime = bf;
    if(kmp <= bf) { bestIndex = 1; bestTime = kmp; }
    if(bm <= bestTime) { bestIndex = 2; bestTime = bm; }
    if(rk <= bestTime) { bestIndex = 3; bestTime = rk; }

    if (bestIndex == 0) { printf("Brute force search ");}
    else if (bestIndex == 1) { printf("Knuth-Morris-Pratt "); }
    else if (bestIndex == 2) { printf("Boyer-Moore "); }
    else { printf("Rabin-Karp "); }

    printf("performed the best with %.2lf microseconds\n", bestTime);
}

int main(int argc, char** argv){

    char * text = argv[1];
    char * pattern = argv[2];
    char nucleotide = '0';

    FILE * ftext = fopen(text, "r");
    int textSize = 0;
    int textCapacity = 1;
    char * textS = malloc(sizeof(char));

    //timing vars
    double bft = 0;
    double kmpt = 0;
    double bmt = 0;
    double rkt = 0;

    while (fscanf(ftext, "%c", &nucleotide) > 0)
    {
        //Remove the line with <
        if (nucleotide == '>'){
            while (fscanf(ftext, "%c", &nucleotide) > 0){
                if (nucleotide == '\n'){
                    break;
                }
            }
        }

        if(nucleotide != '\n'){
            if(textSize >= textCapacity)
            {
                textS = realloc(textS, textCapacity * 2);
                textCapacity = textCapacity * 2;
            }
            textS[textSize] = nucleotide;
            textSize++;
        }
    }



    FILE * fpattern = fopen(pattern, "r");
    int patternSize = 0;
    int patternCapacity = 1;
    char * patternS = malloc(sizeof(char));

    while (fscanf(fpattern, "%c", &nucleotide) > 0)
    {
        //Remove the line with <
        if (nucleotide == '>'){
            while (fscanf(fpattern, "%c", &nucleotide) > 0){
                if (nucleotide == '\n'){
                    break;
                }
            }
        }

        if(nucleotide != '\n'){
            if (patternSize >= patternCapacity)
            {
                patternS = realloc(patternS, patternCapacity * 2);
                patternCapacity = patternCapacity * 2;
            }
            patternS[patternSize] = nucleotide;
            patternSize++;
        }

    }

    fclose(fpattern);

    //Uncomment the following lines of code to display T and P
    //int cnt = 0;
    //printf("Text is: ");

    // printf("The text is: ");
    // while (cnt < textSize)
    // {
    //     printf("%c",textS[cnt]);
    //     cnt++;
    // }
    // printf("\n");
    // fclose(ftext);
    
    // cnt = 0;

    // printf("The pattern is: ");
    // while (cnt < patternSize)
    // {
    //     printf("%c", patternS[cnt]);
    //     cnt++;
    // }
    // printf("\n");


    bruteForce(textS, patternS, textSize, patternSize, &bft);
    kmp(textS, patternS, textSize, patternSize, &kmpt);
    BoyerMoore(textS, patternS, textSize, patternSize, &bmt);
    RabinKarp(textS, patternS, textSize, patternSize, &rkt);

    computeBest(bft, kmpt, bmt, rkt);


    //Free memory
    free(textS);
    free(patternS);

    return 0;
    
}

