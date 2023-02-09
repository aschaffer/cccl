//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
// SPDX-FileCopyrightText: Copyright (c) 2022 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
//
//===----------------------------------------------------------------------===//

// test forward
// UNSUPPORTED: nvrtc

#include <cuda/std/utility>

#include "test_macros.h"

struct A
{
};

__host__ __device__ A source() {return A();}
__host__ __device__ const A csource() {return A();}

int main(int, char**)
{
    {
        (void)cuda::std::forward<A&>(source());  // expected-note {{requested here}}
        // expected-error-re@*:* 1 {{{{(static_assert|static assertion)}} failed{{.*}}cannot forward an rvalue as an lvalue}}
    }
    {
        const A ca = A();
        cuda::std::forward<A&>(ca); // expected-error {{no matching function for call to 'forward'}}
    }
    {
        cuda::std::forward<A&>(csource());  // expected-error {{no matching function for call to 'forward'}}
    }
    {
        const A ca = A();
        cuda::std::forward<A>(ca); // expected-error {{no matching function for call to 'forward'}}
    }
    {
        cuda::std::forward<A>(csource()); // expected-error {{no matching function for call to 'forward'}}
    }
    {
        A a;
        cuda::std::forward(a); // expected-error {{no matching function for call to 'forward'}}
    }

  return 0;
}
