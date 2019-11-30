//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// UNSUPPORTED: libcpp-has-no-threads, pre-sm-60

// <cuda/std/atomic>

// struct atomic_flag

// void atomic_flag_clear(volatile atomic_flag*);
// void atomic_flag_clear(atomic_flag*);

#include <cuda/std/atomic>
#include <cuda/std/cassert>

#include "test_macros.h"
#include "cuda_space_selector.h"

template<template<typename, typename> class Selector>
__host__ __device__
void test()
{
    {
        Selector<cuda::std::atomic_flag, default_initializer> sel;
        cuda::std::atomic_flag & f = *sel.construct();
        f.clear();
        f.test_and_set();
        atomic_flag_clear(&f);
        assert(f.test_and_set() == 0);
    }
    {
        Selector<volatile cuda::std::atomic_flag, default_initializer> sel;
        volatile cuda::std::atomic_flag & f = *sel.construct();
        f.clear();
        f.test_and_set();
        atomic_flag_clear(&f);
        assert(f.test_and_set() == 0);
    }
}

int main(int, char**)
{
#if !defined(__CUDA_ARCH__) || __CUDA_ARCH__ >= 700
    test<local_memory_selector>();
#endif
#ifdef __CUDA_ARCH__
    test<shared_memory_selector>();
    test<global_memory_selector>();
#endif

  return 0;
}
