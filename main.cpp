//
//
//
// CMPE 142 Lab4
// Author: Yongming Li  李涌铭
//
//
//
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <atomic>
#include <pthread.h>
#include <climits>
#include <cstdlib>
#include <ctime>

using namespace std;

pthread_mutex_t lock0     	= PTHREAD_MUTEX_INITIALIZER; // main lock
pthread_mutex_t lock1     	= PTHREAD_MUTEX_INITIALIZER; // customer 1 lock
pthread_mutex_t lock2     	= PTHREAD_MUTEX_INITIALIZER; // customer 2 lock
pthread_mutex_t lock3     	= PTHREAD_MUTEX_INITIALIZER; // customer 3 lock
pthread_cond_t foodOut1   	= PTHREAD_COND_INITIALIZER;
pthread_cond_t foodOut2		= PTHREAD_COND_INITIALIZER;
pthread_cond_t foodOut3		= PTHREAD_COND_INITIALIZER;

int food[3] = {0,0,0};
//  food[0] = Hamburger
//  food[1] = Fries
//  food[2] = Soda

void *customer1(void * A);
//Customer #1 brings their own infinite supply of hamburgers, and are only waiting for fries and soda.

void *customer2(void * A);
//Customer #2 brings their own infinite supply of fries, and are only waiting for hamburgers and soda.

void *customer3(void * A);
//Customer #3 brings their own infinite supply of soda, and are only waiting for hamburgers and fries.

void *chef(void * A);
//chef producing two random meal components.

atomic <bool> done1(false);
atomic <bool> done2(false);
atomic <bool> done3(false);
volatile bool done = false; // done after 100 production

int mealcount1 = 0;
int mealcount2 = 0;
int mealcount3 = 0;

int main(int argc, char *argv[]) {

    cout << endl << "Program start: "
    << endl << "There are 100 productions from chef in total, and here are the results:"<< endl;

    pthread_t c1, c2, c3, chef1;

    int rc;

    rc = pthread_create(&c1, NULL, &customer1, NULL);
    rc = pthread_create(&c2, NULL, &customer2, NULL);
    rc = pthread_create(&c3, NULL, &customer3, NULL);
    rc = pthread_create(&chef1, NULL, &chef, NULL);

    pthread_join(c1,NULL);
    pthread_join(c2,NULL);
    pthread_join(c3,NULL);
    pthread_join(chef1,NULL);

    cout << "\nCustomer 1 eats: " << mealcount1 << " meals" << endl;
    cout << "Customer 2 eats: " << mealcount2 << " meals" << endl;
    cout << "Customer 3 eats: " << mealcount3 << " meals" << endl;
    cout  << "\nProgram END!\n"<< endl;
}

void *customer1(void * A)
{
    bool hasHamburger = true;
    bool hasFries = false;
    bool hasSoda = false;
    while (!done)
    {
        pthread_cond_wait(&foodOut1, &lock1);
        pthread_mutex_unlock(&lock1);

        (food[1] == 1) ? hasFries = true : hasFries = false;
        (food[2] == 1) ? hasSoda = true : hasSoda = false;

        if(hasHamburger && hasFries && hasSoda) mealcount1++;

        hasFries = false;
        hasSoda = false;
        pthread_mutex_lock(&lock0);
        done1 = true;
        pthread_mutex_unlock(&lock0);
    }
    pthread_exit(NULL);
}

void *customer2(void * A)
{
    bool hasHamburger = false;
    bool hasFries = true;
    bool hasSoda = false;
    while (!done)
    {
        pthread_cond_wait(&foodOut2, &lock2);
        pthread_mutex_unlock(&lock2);

        (food[0] == 1) ? hasHamburger = true : hasHamburger = false;
        (food[2] == 1) ? hasSoda = true : hasSoda = false;

        if(hasHamburger && hasFries && hasSoda) mealcount2++;

        hasHamburger = false;
        hasSoda = false;
        pthread_mutex_lock(&lock0);
        done2 = true;
        pthread_mutex_unlock(&lock0);
    }
    pthread_exit(NULL);
}
void *customer3(void * A)
{
    bool hasHamburger = true;
    bool hasFries = false;
    bool hasSoda = true;
    while (!done)
    {
        pthread_cond_wait(&foodOut3, &lock3);
        pthread_mutex_unlock(&lock3);

        (food[0] == 1) ? hasHamburger = true : hasHamburger = false;
        (food[1] == 1) ? hasFries = true : hasFries = false;

        if(hasHamburger && hasFries && hasSoda) mealcount3++;

        hasHamburger = false;
        hasFries = false;
        pthread_mutex_lock(&lock0);
        done3 = true;
        pthread_mutex_unlock(&lock0);

    }
    pthread_exit(NULL);
}

void *chef(void * A)
{
    srand(time(NULL));
    int f1; // first food produce by chef
    int f2; // second food produce by chef
    int count = 0;
    while (count < 99) // need 100
    {
        pthread_mutex_lock(&lock0);
        food[0] = 0;
        food[1] = 0;
        food[2] = 0;
        pthread_mutex_unlock(&lock0);

        f1 = rand() % 3; // generate first food
        do {f2 = rand() % 3;} while (f2 == f1); // generate second food
        // make sure two foods are different

        pthread_mutex_lock(&lock0);
        food[f1] = 1;
        food[f2] = 1;

        pthread_cond_signal(&foodOut1);
        pthread_cond_signal(&foodOut2);
        pthread_cond_signal(&foodOut3);
        pthread_mutex_unlock(&lock0);

        while(!done1.load() || !done2.load() || !done3.load()) usleep(1000);

        pthread_mutex_lock(&lock0);
        done1 = false;
        done2 = false;
        done3 = false;
        pthread_mutex_unlock(&lock0);
        count++;
    }
    pthread_mutex_lock(&lock0);
    done = true;
    pthread_mutex_unlock(&lock0);
    pthread_cond_signal(&foodOut1);
    pthread_cond_signal(&foodOut2);
    pthread_cond_signal(&foodOut3);

    pthread_exit(NULL);
}