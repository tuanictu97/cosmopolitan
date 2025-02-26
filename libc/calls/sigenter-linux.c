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
#include "ape/sections.internal.h"
#include "libc/calls/calls.h"
#include "libc/calls/internal.h"
#include "libc/calls/state.internal.h"
#include "libc/calls/struct/sigaction.h"
#include "libc/calls/struct/siginfo.h"
#include "libc/calls/ucontext.h"
#include "libc/intrin/likely.h"
#include "libc/math.h"
#include "libc/str/str.h"
#include "libc/sysv/consts/sa.h"

privileged void __sigenter_wsl(int sig, struct siginfo *info, ucontext_t *ctx) {
  int i, rva, flags;
  rva = __sighandrvas[sig & (NSIG - 1)];
  if (rva >= kSigactionMinRva) {
    flags = __sighandflags[sig & (NSIG - 1)];
    // WSL1 doesn't set the fpregs field.
    // https://github.com/microsoft/WSL/issues/2555
    if ((flags & SA_SIGINFO) && UNLIKELY(!ctx->uc_mcontext.fpregs)) {
      ctx->uc_mcontext.fpregs = &ctx->__fpustate;
      for (i = 0; i < 8; ++i) {
        long double nan = NAN;
        memcpy(ctx->__fpustate.st + i, &nan, 16);
      }
    }
    ((sigaction_f)(_base + rva))(sig, info, ctx);
  }
}
