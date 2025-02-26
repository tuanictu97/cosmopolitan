/*-*- mode:c;indent-tabs-mode:nil;c-basic-offset:2;tab-width:8;coding:utf-8 -*-│
│vi: set net ft=c ts=2 sts=2 sw=2 fenc=utf-8                                :vi│
╞══════════════════════════════════════════════════════════════════════════════╡
│ Copyright 2022 Justine Alexandra Roberts Tunney                              │
│                                                                              │
│ Permission to use, copy, modify, and/or distribute this software for         │
│ any purpose with or without fee is hereby granted, provided that the         │
│ above copyright notice and this permission notice appear in all copies.      │
│                                                                              │
│ THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL                │
│ WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED                │
│ WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE             │
│ AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL         │
│ DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR        │
│ PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER               │
│ TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR             │
│ PERFORMANCE OF THIS SOFTWARE.                                                │
╚─────────────────────────────────────────────────────────────────────────────*/
#include "libc/calls/struct/timespec.internal.h"
#include "libc/calls/struct/timeval.h"
#include "libc/calls/struct/timeval.internal.h"
#include "libc/fmt/conv.h"
#include "libc/sock/internal.h"
#include "libc/sysv/consts/clock.h"
#include "libc/sysv/consts/timer.h"
#include "libc/sysv/errfuns.h"

int sys_clock_nanosleep_xnu(int clock, int flags, const struct timespec *req,
                            struct timespec *rem) {
  int res;
  struct timeval now, abs, rel;
  if (clock == CLOCK_REALTIME) {
    if (flags & TIMER_ABSTIME) {
      abs = timespec_totimeval(*req);
      sys_gettimeofday_xnu(&now, 0, 0);
      if (timeval_cmp(abs, now) > 0) {
        rel = timeval_sub(abs, now);
        res = sys_select(0, 0, 0, 0, &rel);
      } else {
        res = 0;
      }
    } else {
      res = sys_nanosleep_xnu(req, rem);
    }
  } else {
    res = enotsup();
  }
  return res;
}
