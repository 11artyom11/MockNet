/**
 * @file itc.cpp
 * @author Artyom Grigoryan (grigorianartyom1@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-09-11
 * 
 * @section description
 * This file contains class which enabled 
 * the two of users use safe session of 
 * data transferring via streams
 * @copyright Copyright (c) 2022
 * 
 */
#include <sstream>
#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <future>
#include <type_traits>
#include <functional>
#include "debug_helper/debug_helper.h"

using namespace std;


/**
 * @brief Class for creating pipe between trigger and callback memmber function
 * 
 * Need to specify binding typenames and shared memory typename
 * template <typename StreamType, typename InClassType, typename OutClassType>
 * @tparam StreamType 
 * @tparam InClassType 
 * @tparam OutClassType 
 */
template <typename StreamType, typename InClassType, typename OutClassType>
class ITC
{
    public:
        ITC() 
        {
            sem_init (&read_sem, 0, 1);
            sem_init (&write_sem, 0, 1);
        }

        void sread_from_stream (void)
        {
            sem_wait(&write_sem);
            std::__invoke(callback_ptr, *out, ss);
            sem_post(&write_sem);      
        }

        void swrite_to_stream ()
        {
            sem_wait(&read_sem);
            ss = std::__invoke(in_fptr, *in);
            sem_post(&read_sem);  
            return;
        }

        void set_trigger (StreamType (InClassType::*__in_fptr)(void), InClassType* inptr)
        {
            in_fptr = __in_fptr;
            in = inptr;
        }

        void set_callback (int (OutClassType::*__callback_ptr)(const StreamType&), OutClassType* outptr)
        {
            callback_ptr = __callback_ptr;
            out = outptr;
        }

        void trig_loop (void)
        {
            if (callback_ptr && in_fptr){   
                while (1){
                    swrite_to_stream();
                    sread_from_stream();
                } 
            } else {
                debug_f_console("trigger and/or callback is not set for ITC instance...");   
                return;
            }
        }

    private :
        StreamType ss;
        sem_t write_sem;
        sem_t read_sem;

        StreamType (InClassType::*in_fptr)(void);
        int (OutClassType::*callback_ptr)(const StreamType&);
        InClassType* in;
        OutClassType* out;

};

