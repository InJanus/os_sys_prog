#include "RW_lock.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

void RW_lock_init(RW_lock_t *lock){
    lock->b = 0; //set b to 0 because there are 0 reading threads right now
    lock->r = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER; //standard mutex locks
    lock->g = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
}

void RW_read_lock(RW_lock_t *lock){
    __asm__ __volatile__ ("" ::: "memory");
    pthread_mutex_lock(&lock->r); //lock b protection thread
    lock->b++; //add one to the read because there is one reading thread active
    if(lock->b == 1){
        pthread_mutex_lock(&lock->g); //lock the reader thread since there is a reading thread active
    }
    pthread_mutex_unlock(&lock->r); //unlock reader thread that protects b
    __asm__ __volatile__ ("" ::: "memory");
}

void RW_read_unlock(RW_lock_t *lock){
    __asm__ __volatile__ ("" ::: "memory");
    pthread_mutex_lock(&lock->r);//protect b lock
    lock->b--; //decrease b since there is a reader leaving the protected section
    if(lock->b == 0){
        pthread_mutex_unlock(&lock->g); //unlock reader thread
    }
    pthread_mutex_unlock(&lock->r);//unlock protect b lock
    __asm__ __volatile__ ("" ::: "memory");
}

void RW_write_lock(RW_lock_t *lock){
    pthread_mutex_lock(&lock->g); //no extra buisness is needed here or the one below just lock and unlock for the write read
}

void RW_write_unlock(RW_lock_t *lock){
    pthread_mutex_unlock(&lock->g);
}